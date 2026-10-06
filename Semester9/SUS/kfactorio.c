#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/string.h>

#define GRID_W 22
#define GRID_H 10
#define WIN_POINTS 100

/* Terrain types */
#define TERR_EMPTY  0
#define TERR_IRON   1
#define TERR_COPPER 2

/* Building types */
#define BLD_EMPTY     0
#define BLD_BELT      1
#define BLD_MINER     2
#define BLD_FURNACE   3
#define BLD_ASSEMBLER 4
#define BLD_LAB       5

/* Direction orientations */
#define DIR_UP    0
#define DIR_RIGHT 1
#define DIR_DOWN  2
#define DIR_LEFT  3

/* Transported items and internal products */
#define ITEM_NONE         0
#define ITEM_IRON_ORE     1
#define ITEM_COPPER_ORE   2
#define ITEM_IRON_PLATE   3
#define ITEM_COPPER_PLATE 4
#define ITEM_GEAR         5
#define ITEM_CABLE        6
#define ITEM_CIRCUIT      7
#define ITEM_ADV_CIRCUIT  8
#define ITEM_SCI_1        9
#define ITEM_SCI_2        10

/* Assembler recipe IDs */
#define RECIPE_NONE        0
#define RECIPE_GEAR        1  /* 2x Iron Plate */
#define RECIPE_CABLE       2  /* 3x Copper Plate */
#define RECIPE_CIRCUIT     3  /* 3x Copper Cable + 2x Iron Plate */
#define RECIPE_SCI_1       4  /* 5x Circuit + 3x Gear */
#define RECIPE_ADV_CIRCUIT 5  /* 2x Circuit + 2x Gear + 4x Cable */
#define RECIPE_SCI_2       6  /* 1x Science Pack I + 2x Adv Circuit */

struct Cell {
    int terrain;
    int building;
    int dir;
    int item;
    bool handled;

    /* Internal storage for Furnaces */
    int furnace_ore;

    /* Internal sub-inventory for Assemblers */
    int recipe;
    int inv_iron;
    int inv_copper;
    int inv_gear;
    int inv_cable;
    int inv_circuit;
    int inv_adv_circuit;
    int inv_sci1;
};

struct GameState {
    struct Cell grid[GRID_H][GRID_W];
    int cursor_x;
    int cursor_y;
    int selected_dir;

    int points;
    bool game_won;
    bool menu_open; /* Assembler recipe selection modal */
    char status_msg[80];
};

static struct GameState game;
static DEFINE_MUTEX(game_lock);

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_board;
static struct proc_dir_entry *proc_cmd;

static const int dx[] = { 0, 1, 0, -1 };
static const int dy[] = { -1, 0, 1, 0 };

static inline bool in_bounds(int x, int y)
{
    return (x >= 0 && x < GRID_W && y >= 0 && y < GRID_H);
}

/* Initialize map terrain, starting cursor and defaults */
static void init_game(void)
{
    int x, y;
    memset(&game, 0, sizeof(game));

    /* Iron deposit patch (top-left) */
    for (y = 1; y <= 4; y++) {
        for (x = 1; x <= 4; x++) {
            game.grid[y][x].terrain = TERR_IRON;
        }
    }

    /* Copper deposit patch (bottom-left) */
    for (y = 5; y <= 8; y++) {
        for (x = 1; x <= 4; x++) {
            game.grid[y][x].terrain = TERR_COPPER;
        }
    }

    game.cursor_x = 7;
    game.cursor_y = 4;
    game.selected_dir = DIR_RIGHT;
    game.points = 0;
    game.game_won = false;
    game.menu_open = false;

    strscpy(game.status_msg, "Factory online. Infinite items: 1:Belt 2:Miner 3:Furnace 4:Assembler 5:Lab",
            sizeof(game.status_msg));
}

/* Primary simulation step executed on every action/tick */
static void tick_simulation(void)
{
    int x, y;

    /* Reset processed flags for current tick */
    for (y = 0; y < GRID_H; y++) {
        for (x = 0; x < GRID_W; x++) {
            game.grid[y][x].handled = false;
        }
    }

    /* 1. MINERS: extract raw ore from resource patch to forward belt */
    for (y = 0; y < GRID_H; y++) {
        for (x = 0; x < GRID_W; x++) {
            struct Cell *c = &game.grid[y][x];
            if (c->building == BLD_MINER && c->terrain != TERR_EMPTY) {
                int tx = x + dx[c->dir];
                int ty = y + dy[c->dir];
                if (in_bounds(tx, ty)) {
                    struct Cell *target = &game.grid[ty][tx];
                    if (target->building == BLD_BELT && target->item == ITEM_NONE && !target->handled) {
                        if (c->terrain == TERR_IRON)
                            target->item = ITEM_IRON_ORE;
                        else if (c->terrain == TERR_COPPER)
                            target->item = ITEM_COPPER_ORE;
                        target->handled = true;
                    }
                }
            }
        }
    }

    /* 2. FURNACES: smelt ores into metal plates */
    for (y = 0; y < GRID_H; y++) {
        for (x = 0; x < GRID_W; x++) {
            struct Cell *c = &game.grid[y][x];
            if (c->building == BLD_FURNACE) {
                /* Smelt raw ore into plate */
                if (c->furnace_ore == ITEM_IRON_ORE && c->item == ITEM_NONE) {
                    c->item = ITEM_IRON_PLATE;
                    c->furnace_ore = ITEM_NONE;
                } else if (c->furnace_ore == ITEM_COPPER_ORE && c->item == ITEM_NONE) {
                    c->item = ITEM_COPPER_PLATE;
                    c->furnace_ore = ITEM_NONE;
                }

                /* Output finished plate forward */
                if (c->item != ITEM_NONE) {
                    int tx = x + dx[c->dir];
                    int ty = y + dy[c->dir];
                    if (in_bounds(tx, ty)) {
                        struct Cell *target = &game.grid[ty][tx];
                        if (target->building == BLD_BELT && target->item == ITEM_NONE && !target->handled) {
                            target->item = c->item;
                            target->handled = true;
                            c->item = ITEM_NONE;
                        }
                    }
                }
            }
        }
    }

    /* 3. ASSEMBLERS: craft products according to selected recipe */
    for (y = 0; y < GRID_H; y++) {
        for (x = 0; x < GRID_W; x++) {
            struct Cell *c = &game.grid[y][x];
            if (c->building == BLD_ASSEMBLER && c->item == ITEM_NONE) {
                if (c->recipe == RECIPE_GEAR && c->inv_iron >= 2) {
                    c->inv_iron -= 2;
                    c->item = ITEM_GEAR;
                } else if (c->recipe == RECIPE_CABLE && c->inv_copper >= 3) {
                    c->inv_copper -= 3;
                    c->item = ITEM_CABLE;
                } else if (c->recipe == RECIPE_CIRCUIT && c->inv_cable >= 3 && c->inv_iron >= 2) {
                    c->inv_cable -= 3;
                    c->inv_iron -= 2;
                    c->item = ITEM_CIRCUIT;
                } else if (c->recipe == RECIPE_SCI_1 && c->inv_circuit >= 5 && c->inv_gear >= 3) {
                    c->inv_circuit -= 5;
                    c->inv_gear -= 3;
                    c->item = ITEM_SCI_1;
                } else if (c->recipe == RECIPE_ADV_CIRCUIT && c->inv_circuit >= 2 && c->inv_gear >= 2 && c->inv_cable >= 4) {
                    c->inv_circuit -= 2;
                    c->inv_gear -= 2;
                    c->inv_cable -= 4;
                    c->item = ITEM_ADV_CIRCUIT;
                } else if (c->recipe == RECIPE_SCI_2 && c->inv_sci1 >= 1 && c->inv_adv_circuit >= 2) {
                    c->inv_sci1 -= 1;
                    c->inv_adv_circuit -= 2;
                    c->item = ITEM_SCI_2;
                }
            }

            /* Output finished craft forward */
            if (c->building == BLD_ASSEMBLER && c->item != ITEM_NONE) {
                int tx = x + dx[c->dir];
                int ty = y + dy[c->dir];
                if (in_bounds(tx, ty)) {
                    struct Cell *target = &game.grid[ty][tx];
                    if (target->building == BLD_BELT && target->item == ITEM_NONE && !target->handled) {
                        target->item = c->item;
                        target->handled = true;
                        c->item = ITEM_NONE;
                    }
                }
            }
        }
    }

    /* 4. BELTS: move items along conveyor network and feed machines */
    for (y = 0; y < GRID_H; y++) {
        for (x = 0; x < GRID_W; x++) {
            struct Cell *c = &game.grid[y][x];
            if (c->building != BLD_BELT || c->item == ITEM_NONE || c->handled)
                continue;

            int tx = x + dx[c->dir];
            int ty = y + dy[c->dir];
            if (!in_bounds(tx, ty))
                continue;

            struct Cell *target = &game.grid[ty][tx];

            /* Feed into Furnace */
            if (target->building == BLD_FURNACE) {
                if ((c->item == ITEM_IRON_ORE || c->item == ITEM_COPPER_ORE) && target->furnace_ore == ITEM_NONE) {
                    target->furnace_ore = c->item;
                    c->item = ITEM_NONE;
                    c->handled = true;
                    continue;
                }
            }

            /* Feed into Assembler buffer */
            if (target->building == BLD_ASSEMBLER) {
                bool absorbed = false;
                if (c->item == ITEM_IRON_PLATE && target->inv_iron < 50) {
                    target->inv_iron++; absorbed = true;
                } else if (c->item == ITEM_COPPER_PLATE && target->inv_copper < 50) {
                    target->inv_copper++; absorbed = true;
                } else if (c->item == ITEM_GEAR && target->inv_gear < 50) {
                    target->inv_gear++; absorbed = true;
                } else if (c->item == ITEM_CABLE && target->inv_cable < 50) {
                    target->inv_cable++; absorbed = true;
                } else if (c->item == ITEM_CIRCUIT && target->inv_circuit < 50) {
                    target->inv_circuit++; absorbed = true;
                } else if (c->item == ITEM_ADV_CIRCUIT && target->inv_adv_circuit < 50) {
                    target->inv_adv_circuit++; absorbed = true;
                } else if (c->item == ITEM_SCI_1 && target->inv_sci1 < 50) {
                    target->inv_sci1++; absorbed = true;
                }

                if (absorbed) {
                    c->item = ITEM_NONE;
                    c->handled = true;
                    continue;
                }
            }

            /* Feed into Laboratory (research points toward victory) */
            if (target->building == BLD_LAB) {
                if (c->item == ITEM_SCI_1) {
                    game.points += 1;
                    c->item = ITEM_NONE;
                    c->handled = true;
                    if (game.points >= WIN_POINTS) game.game_won = true;
                    continue;
                } else if (c->item == ITEM_SCI_2) {
                    game.points += 10;
                    c->item = ITEM_NONE;
                    c->handled = true;
                    if (game.points >= WIN_POINTS) game.game_won = true;
                    continue;
                }
            }

            /* Shift to next connected empty belt */
            if (target->building == BLD_BELT && target->item == ITEM_NONE && !target->handled) {
                target->item = c->item;
                target->handled = true;
                c->item = ITEM_NONE;
                c->handled = true;
            }
        }
    }
}

/* Handle keystrokes while the Assembler sub-menu is active */
static void handle_menu_cmd(char cmd)
{
    struct Cell *cur = &game.grid[game.cursor_y][game.cursor_x];

    if (cmd == 'o' || cmd == 'O' || cmd == 'q' || cmd == 'x') {
        game.menu_open = false;
        strscpy(game.status_msg, "Menu closed.", sizeof(game.status_msg));
        return;
    }

    if (cur->building != BLD_ASSEMBLER) {
        game.menu_open = false;
        return;
    }

    switch (cmd) {
    case 'g':
        cur->recipe = RECIPE_GEAR;
        strscpy(game.status_msg, "Recipe selected: Gear", sizeof(game.status_msg));
        game.menu_open = false;
        break;
    case '@':
    case 'c':
        cur->recipe = RECIPE_CABLE;
        strscpy(game.status_msg, "Recipe selected: Copper Cable", sizeof(game.status_msg));
        game.menu_open = false;
        break;
    case 'z':
        cur->recipe = RECIPE_CIRCUIT;
        strscpy(game.status_msg, "Recipe selected: Electronic Circuit", sizeof(game.status_msg));
        game.menu_open = false;
        break;
    case '1':
        cur->recipe = RECIPE_SCI_1;
        strscpy(game.status_msg, "Recipe selected: Science Pack I", sizeof(game.status_msg));
        game.menu_open = false;
        break;
    case 'a':
    case 'A':
        cur->recipe = RECIPE_ADV_CIRCUIT;
        strscpy(game.status_msg, "Recipe selected: Advanced Circuit", sizeof(game.status_msg));
        game.menu_open = false;
        break;
    case '2':
        cur->recipe = RECIPE_SCI_2;
        strscpy(game.status_msg, "Recipe selected: Science Pack II", sizeof(game.status_msg));
        game.menu_open = false;
        break;
    default:
        break;
    }
}

/* Standard movement, building and interaction commands */
static void handle_player_cmd(char cmd)
{
    struct Cell *cur = &game.grid[game.cursor_y][game.cursor_x];

    switch (cmd) {
    case 'w': if (game.cursor_y > 0) game.cursor_y--; break;
    case 's': if (game.cursor_y < GRID_H - 1) game.cursor_y++; break;
    case 'a': if (game.cursor_x > 0) game.cursor_x--; break;
    case 'd': if (game.cursor_x < GRID_W - 1) game.cursor_x++; break;
    case 'r':
        game.selected_dir = (game.selected_dir + 1) & 3;
        break;
    case '1': /* Place Belt */
        cur->building = BLD_BELT;
        cur->dir = game.selected_dir;
        break;
    case '2': /* Place Miner */
        if (cur->terrain == TERR_EMPTY) {
            strscpy(game.status_msg, "Miners must be placed on ore deposits (# / ~)!", sizeof(game.status_msg));
        } else {
            cur->building = BLD_MINER;
            cur->dir = game.selected_dir;
        }
        break;
    case '3': /* Place Furnace */
        cur->building = BLD_FURNACE;
        cur->dir = game.selected_dir;
        break;
    case '4': /* Place Assembler */
        cur->building = BLD_ASSEMBLER;
        cur->dir = game.selected_dir;
        cur->recipe = RECIPE_NONE;
        break;
    case '5': /* Place Laboratory */
        cur->building = BLD_LAB;
        cur->dir = game.selected_dir;
        break;
    case 'x': /* Deconstruct building */
        cur->building = BLD_EMPTY;
        cur->item = ITEM_NONE;
        cur->recipe = RECIPE_NONE;
        cur->furnace_ore = ITEM_NONE;
        break;
    case 'o':
    case 'O':
        if (cur->building == BLD_ASSEMBLER) {
            game.menu_open = true;
        } else {
            strscpy(game.status_msg, "No Assembler at current position.", sizeof(game.status_msg));
        }
        break;
    case 't':
        /* Manual tick advance */
        break;
    default:
        break;
    }

    tick_simulation();
}

/* Format readable recipe name */
static const char *get_recipe_name(int r)
{
    switch (r) {
    case RECIPE_GEAR: return "Gear [g]";
    case RECIPE_CABLE: return "Cable [@/c]";
    case RECIPE_CIRCUIT: return "Circuit [z]";
    case RECIPE_SCI_1: return "Science Pack I [1]";
    case RECIPE_ADV_CIRCUIT: return "Adv Circuit [A]";
    case RECIPE_SCI_2: return "Science Pack II [2]";
    default: return "NONE (Press O to choose)";
    }
}

/* Map items to single terminal glyphs */
static char get_item_char(int item)
{
    switch (item) {
    case ITEM_IRON_ORE:     return 'i';
    case ITEM_COPPER_ORE:   return 'c';
    case ITEM_IRON_PLATE:   return 'I';
    case ITEM_COPPER_PLATE: return 'C';
    case ITEM_GEAR:         return 'g';
    case ITEM_CABLE:        return '@';
    case ITEM_CIRCUIT:      return 'z';
    case ITEM_ADV_CIRCUIT:  return 'A';
    case ITEM_SCI_1:        return '1';
    case ITEM_SCI_2:        return '2';
    default: return '?';
    }
}

static int board_show(struct seq_file *m, void *v)
{
    int x, y;
    static const char belt_arrows[] = { '^', '>', 'v', '<' };

    mutex_lock(&game_lock);

    seq_puts(m, "\033[H\033[J");
    seq_puts(m, "=================== LINUX KERNEL FACTORIO ===================\n");

    if (game.game_won) {
        seq_puts(m, "*************************************************************\n");
        seq_puts(m, "***       CONGRATULATIONS! 100 RESEARCH POINTS WON!       ***\n");
        seq_puts(m, "***                 YOU HAVE WON THE GAME!                ***\n");
        seq_puts(m, "*************************************************************\n");
    }

    seq_printf(m, "Research Points: [%3d / %3d] | Facing: %s\n",
               game.points, WIN_POINTS,
               game.selected_dir == DIR_UP ? "UP (^)" :
               game.selected_dir == DIR_RIGHT ? "RIGHT (>)" :
               game.selected_dir == DIR_DOWN ? "DOWN (v)" : "LEFT (<)");

    seq_puts(m, "+");
    for (x = 0; x < GRID_W; x++) seq_putc(m, '-');
    seq_puts(m, "+\n");

    for (y = 0; y < GRID_H; y++) {
        seq_putc(m, '|');
        for (x = 0; x < GRID_W; x++) {
            if (x == game.cursor_x && y == game.cursor_y) {
                seq_putc(m, 'X'); /* Player cursor */
                continue;
            }

            struct Cell *c = &game.grid[y][x];

            if (c->building == BLD_BELT) {
                if (c->item != ITEM_NONE)
                    seq_putc(m, get_item_char(c->item));
                else
                    seq_putc(m, belt_arrows[c->dir]);
            } else if (c->building == BLD_MINER) {
                seq_putc(m, 'M');
            } else if (c->building == BLD_FURNACE) {
                seq_putc(m, 'F');
            } else if (c->building == BLD_ASSEMBLER) {
                seq_putc(m, 'A');
            } else if (c->building == BLD_LAB) {
                seq_putc(m, 'L');
            } else {
                if (c->terrain == TERR_IRON)
                    seq_putc(m, '#'); /* Iron ore patch */
                else if (c->terrain == TERR_COPPER)
                    seq_putc(m, '~'); /* Copper ore patch */
                else
                    seq_putc(m, '.');
            }
        }
        seq_puts(m, "|\n");
    }

    seq_puts(m, "+");
    for (x = 0; x < GRID_W; x++) seq_putc(m, '-');
    seq_puts(m, "+\n");

    /* ASSEMBLER SUB-MENU */
    if (game.menu_open) {
        struct Cell *cur = &game.grid[game.cursor_y][game.cursor_x];
        seq_puts(m, "\n>>> ASSEMBLER CONFIGURATION (Press key to choose recipe) <<<\n");
        seq_printf(m, "Active Recipe: %s\n", get_recipe_name(cur->recipe));
        seq_printf(m, "Storage: [Iron: %d] [Copper: %d] [Gears: %d] [Cables: %d] [Circuits: %d] [Adv: %d] [Sci1: %d]\n",
                   cur->inv_iron, cur->inv_copper, cur->inv_gear, cur->inv_cable,
                   cur->inv_circuit, cur->inv_adv_circuit, cur->inv_sci1);
        seq_puts(m, "Recipes:\n");
        seq_puts(m, "  [g] Gear            - 2x Iron Plate\n");
        seq_puts(m, "  [@] Cable           - 3x Copper Plate\n");
        seq_puts(m, "  [z] Circuit         - 3x Copper Cable + 2x Iron Plate\n");
        seq_puts(m, "  [1] Science Pack I  - 5x Circuit + 3x Gear (+1 Pt)\n");
        seq_puts(m, "  [A] Adv Circuit     - 2x Circuit + 2x Gear + 4x Cable\n");
        seq_puts(m, "  [2] Science Pack II - 1x Sci I + 2x Adv Circuit (+10 Pts)\n");
        seq_puts(m, "Press [o] to close menu.\n");
    } else {
        seq_puts(m, "Controls: WASD: Move | R: Rotate | 1: Belt | 2: Miner | 3: Furnace | 4: Assembler | 5: Lab\n");
        seq_puts(m, "          X: Remove | O: Open Assembler | T: Step Tick\n");
        seq_printf(m, "Status:   %s\n", game.status_msg);
    }

    mutex_unlock(&game_lock);
    return 0;
}

static int board_open(struct inode *inode, struct file *file)
{
    return single_open(file, board_show, NULL);
}

static const struct proc_ops board_proc_ops = {
    .proc_open    = board_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static ssize_t cmd_write(struct file *file, const char __user *ubuf,
                         size_t count, loff_t *ppos)
{
    char kbuf[16];
    size_t copy_len;
    size_t i;

    if (count == 0)
        return 0;

    copy_len = min(count, sizeof(kbuf) - 1);
    if (copy_from_user(kbuf, ubuf, copy_len))
        return -EFAULT;

    kbuf[copy_len] = '\0';

    mutex_lock(&game_lock);
    for (i = 0; i < copy_len; i++) {
        char ch = kbuf[i];
        if (ch == '\n' || ch == '\r' || ch == ' ')
            continue;

        if (game.menu_open)
            handle_menu_cmd(ch);
        else
            handle_player_cmd(ch);
    }
    mutex_unlock(&game_lock);

    return count;
}

static const struct proc_ops cmd_proc_ops = {
    .proc_write = cmd_write,
};

static int __init kfactorio_init(void)
{
    proc_dir = proc_mkdir("factorio", NULL);
    if (!proc_dir)
        return -ENOMEM;

    proc_board = proc_create("board", 0444, proc_dir, &board_proc_ops);
    if (!proc_board) {
        proc_remove(proc_dir);
        return -ENOMEM;
    }

    proc_cmd = proc_create("cmd", 0222, proc_dir, &cmd_proc_ops);
    if (!proc_cmd) {
        proc_remove(proc_board);
        proc_remove(proc_dir);
        return -ENOMEM;
    }

    init_game();

    pr_info("kfactorio: loaded. /proc/factorio/board ready.\n");
    return 0;
}

static void __exit kfactorio_exit(void)
{
    proc_remove(proc_cmd);
    proc_remove(proc_board);
    proc_remove(proc_dir);

    pr_info("kfactorio: unloaded.\n");
}

module_init(kfactorio_init);
module_exit(kfactorio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Patrik Mintel");
MODULE_DESCRIPTION("SUS Kernel Factorio Game with Assembler and Recipes");
MODULE_VERSION("1.0");