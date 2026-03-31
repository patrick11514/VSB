// ============================================================================
// BARE-METAL MINI-FACTORIO
// Target: 32-bit x86 Custom OS (No stdlib)
// ============================================================================

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#define GRID_W 80
#define GRID_H 25
#define VGA_MEM ((volatile uint16_t *)0xB8000)

// VGA Colors
#define COL_BLACK 0x00
#define COL_BLUE 0x01
#define COL_GREEN 0x02
#define COL_CYAN 0x03
#define COL_RED 0x04
#define COL_MAGENTA 0x05
#define COL_BROWN 0x06
#define COL_LIGHT_GREY 0x07
#define COL_DARK_GREY 0x08
#define COL_YELLOW 0x0E
#define COL_WHITE 0x0F

typedef enum { TERR_EMPTY = 0, TERR_IRON, TERR_COPPER } TerrainType;

typedef enum {
  EMPTY = 0,
  BELT_UP,
  BELT_DOWN,
  BELT_LEFT,
  BELT_RIGHT,
  MINER,
  FURNACE,
  CRAFTER,
  LAB
} EntityType;

typedef enum {
  NONE = 0,
  IRON_ORE,
  COPPER_ORE,
  IRON_PLATE,
  COPPER_PLATE,
  GEAR
} ItemType;

struct Cell {
  EntityType building;
  ItemType item;
  TerrainType terrain;
  uint8_t handled;
  uint8_t inv_iron;   // Crafter stored iron plates
  uint8_t inv_copper; // Crafter stored copper plates
};

// --- Global State ---
struct Cell grid[GRID_H][GRID_W];
int cursor_x = 40;
int cursor_y = 12;
EntityType selected_building = BELT_RIGHT;
uint32_t tick_counter = 0;
int current_state = 0;
uint32_t score = 0;

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port));
  return ret;
}

uint8_t get_scancode() {
  if (inb(0x64) & 1)
    return inb(0x60);
  return 0;
}

void print_text(int x, int y, const char *str, uint8_t fg, uint8_t bg) {
  int i = 0;
  while (str[i] != '\0') {
    if (x + i >= GRID_W)
      break;
    VGA_MEM[y * GRID_W + (x + i)] = (bg << 12) | (fg << 8) | str[i];
    i++;
  }
}

// --- Initialization ---
void init_grid() {
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      grid[y][x].building = EMPTY;
      grid[y][x].item = NONE;
      grid[y][x].terrain = TERR_EMPTY;
      grid[y][x].handled = 0;
      grid[y][x].inv_iron = 0;
      grid[y][x].inv_copper = 0;

      // Draw map patches
      if (x >= 10 && x <= 18 && y >= 5 && y <= 12)
        grid[y][x].terrain = TERR_IRON;
      if (x >= 60 && x <= 68 && y >= 14 && y <= 20)
        grid[y][x].terrain = TERR_COPPER;
    }
  }
}

// --- Game Logic ---
void push_item_to_belt(int x, int y, ItemType item) {
  int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  for (int d = 0; d < 4; d++) {
    int dx = x + dirs[d][0];
    int dy = y + dirs[d][1];
    if (dx >= 0 && dx < GRID_W && dy >= 0 && dy < GRID_H) {
      struct Cell *target = &grid[dy][dx];
      if (target->building >= BELT_UP && target->building <= BELT_RIGHT &&
          target->item == NONE) {
        // Output must push to a belt facing AWAY from this location
        int valid = 0;
        if (dy == y - 1 && target->building == BELT_UP)
          valid = 1;
        if (dy == y + 1 && target->building == BELT_DOWN)
          valid = 1;
        if (dx == x - 1 && target->building == BELT_LEFT)
          valid = 1;
        if (dx == x + 1 && target->building == BELT_RIGHT)
          valid = 1;

        if (valid) {
          target->item = item;
          target->handled = 1;
          return;
        }
      }
    }
  }
}

void process_tick() {
  for (int y = 0; y < GRID_H; y++)
    for (int x = 0; x < GRID_W; x++)
      grid[y][x].handled = 0;

  // 1. Miners output ore
  if (tick_counter % 5 == 0) {
    for (int y = 0; y < GRID_H; y++) {
      for (int x = 0; x < GRID_W; x++) {
        if (grid[y][x].building == MINER) {
          if (grid[y][x].terrain == TERR_IRON)
            push_item_to_belt(x, y, IRON_ORE);
          if (grid[y][x].terrain == TERR_COPPER)
            push_item_to_belt(x, y, COPPER_ORE);
        }
      }
    }
  }

  // 2. Furnaces and Crafters push finished outputs
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      if ((grid[y][x].building == FURNACE || grid[y][x].building == CRAFTER) &&
          grid[y][x].item != NONE) {
        // Try pushing completed item to adjacent belt
        push_item_to_belt(x, y, grid[y][x].item);
        if (grid[y + 1][x].handled || grid[y - 1][x].handled ||
            grid[y][x + 1].handled ||
            grid[y][x - 1].handled) { // Rough check if push worked
          grid[y][x].item = NONE;     // Cleared successfully
        }
      }
    }
  }

  // 3. Move items on Belts
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      struct Cell *current = &grid[y][x];

      if (current->item == NONE || current->handled)
        continue;

      int tx = x, ty = y;
      if (current->building == BELT_UP)
        ty--;
      else if (current->building == BELT_DOWN)
        ty++;
      else if (current->building == BELT_LEFT)
        tx--;
      else if (current->building == BELT_RIGHT)
        tx++;
      else
        continue;

      if (tx >= 0 && tx < GRID_W && ty >= 0 && ty < GRID_H) {
        struct Cell *target = &grid[ty][tx];

        // Push into Crafter
        if (target->building == CRAFTER) {
          if (current->item == IRON_PLATE && target->inv_iron < 5) {
            target->inv_iron++;
            current->item = NONE;
          } else if (current->item == COPPER_PLATE && target->inv_copper < 5) {
            target->inv_copper++;
            current->item = NONE;
          }
        }
        // Push into Lab
        else if (target->building == LAB) {
          if (current->item == GEAR) {
            score++;
            current->item = NONE;
          }
        }
        // Push into Furnace
        else if (target->building == FURNACE && target->item == NONE) {
          if (current->item == IRON_ORE || current->item == COPPER_ORE) {
            target->item = current->item;
            current->item = NONE;
          }
        }
        // Move to next Belt
        else if (target->item == NONE && target->building >= BELT_UP &&
                 target->building <= BELT_RIGHT) {
          target->item = current->item;
          target->handled = 1;
          current->item = NONE;
        }
      }
    }
  }

  // 4. Furnaces Smelt
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      if (grid[y][x].building == FURNACE) {
        if (grid[y][x].item == IRON_ORE)
          grid[y][x].item = IRON_PLATE;
        if (grid[y][x].item == COPPER_ORE)
          grid[y][x].item = COPPER_PLATE;
      }
    }
  }

  // 5. Crafters craft (1 Iron Plate + 1 Copper Plate -> Gear)
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      if (grid[y][x].building == CRAFTER && grid[y][x].inv_iron >= 1 &&
          grid[y][x].inv_copper >= 1 && grid[y][x].item == NONE) {
        grid[y][x].inv_iron--;
        grid[y][x].inv_copper--;
        grid[y][x].item = GEAR;
      }
    }
  }
}

// --- Input Handling ---
void handle_input() {
  uint8_t scancode = get_scancode();

  if (scancode == 0 || (scancode & 0x80))
    return;

  if (scancode == 0x11 && cursor_y > 1)
    cursor_y--; // W
  if (scancode == 0x1F && cursor_y < GRID_H - 1)
    cursor_y++; // S
  if (scancode == 0x1E && cursor_x > 0)
    cursor_x--; // A
  if (scancode == 0x20 && cursor_x < GRID_W - 1)
    cursor_x++; // D

  if (scancode == 0x02)
    selected_building = BELT_UP; // 1
  if (scancode == 0x03)
    selected_building = BELT_DOWN; // 2
  if (scancode == 0x04)
    selected_building = BELT_LEFT; // 3
  if (scancode == 0x05)
    selected_building = BELT_RIGHT; // 4
  if (scancode == 0x06)
    selected_building = MINER; // 5
  if (scancode == 0x07)
    selected_building = CRAFTER; // 6
  if (scancode == 0x08)
    selected_building = FURNACE; // 7
  if (scancode == 0x09)
    selected_building = LAB; // 8
  if (scancode == 0x0B)
    selected_building = EMPTY; // 0

  if (scancode == 0x39) {
    grid[cursor_y][cursor_x].building = selected_building;
    if (selected_building == EMPTY) {
      grid[cursor_y][cursor_x].item = NONE;
      grid[cursor_y][cursor_x].inv_iron = 0;
      grid[cursor_y][cursor_x].inv_copper = 0;
    }
  }
}

// --- Rendering ---
void render() {
  uint16_t buffer[GRID_H * GRID_W];

  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      struct Cell *cell = &grid[y][x];
      uint8_t ch = ' ';
      uint8_t fg = COL_WHITE;
      uint8_t bg = COL_BLACK;

      // Draw Terrain layer if empty
      if (cell->building == EMPTY && cell->item == NONE) {
        if (cell->terrain == TERR_IRON) {
          ch = '.';
          fg = COL_DARK_GREY;
        }
        if (cell->terrain == TERR_COPPER) {
          ch = '.';
          fg = COL_BROWN;
        }
      }

      // Draw Building
      switch (cell->building) {
      case BELT_UP:
        ch = '^';
        fg = COL_CYAN;
        break;
      case BELT_DOWN:
        ch = 'v';
        fg = COL_CYAN;
        break;
      case BELT_LEFT:
        ch = '<';
        fg = COL_CYAN;
        break;
      case BELT_RIGHT:
        ch = '>';
        fg = COL_CYAN;
        break;
      case MINER:
        ch = 'M';
        fg = COL_MAGENTA;
        break;
      case FURNACE:
        ch = 'F';
        fg = COL_RED;
        break;
      case CRAFTER:
        ch = 'C';
        fg = COL_YELLOW;
        break;
      case LAB:
        ch = 'L';
        fg = COL_GREEN;
        break;
      default:
        break;
      }

      // Draw Item
      if (cell->building < MINER) {
        if (cell->item == IRON_ORE) {
          ch = '*';
          fg = COL_LIGHT_GREY;
        } else if (cell->item == COPPER_ORE) {
          ch = '*';
          fg = COL_BROWN;
        } else if (cell->item == IRON_PLATE) {
          ch = 'i';
          fg = COL_LIGHT_GREY;
        } else if (cell->item == COPPER_PLATE) {
          ch = 'c';
          fg = COL_BROWN;
        } else if (cell->item == GEAR) {
          ch = 'G';
          fg = COL_WHITE;
        }
      }

      // Draw Cursor
      if (x == cursor_x && y == cursor_y) {
        bg = COL_MAGENTA;
        if (ch == ' ') {
          ch = '+';
          fg = COL_WHITE;
        }
      }

      buffer[y * GRID_W + x] = (bg << 12) | (fg << 8) | ch;
    }
  }

  // Draw UI overlay
  const char *sel = "EMPTY      ";
  if (selected_building == BELT_UP)
    sel = "BELT UP    ";
  if (selected_building == BELT_DOWN)
    sel = "BELT DOWN  ";
  if (selected_building == BELT_LEFT)
    sel = "BELT LEFT  ";
  if (selected_building == BELT_RIGHT)
    sel = "BELT RIGHT ";
  if (selected_building == MINER)
    sel = "MINER     ";
  if (selected_building == CRAFTER)
    sel = "CRAFTER   ";
  if (selected_building == FURNACE)
    sel = "FURNACE   ";
  if (selected_building == LAB)
    sel = "LAB       ";

  const char *txt1 = " Selected: ";
  for (int i = 0; txt1[i] != '\0'; i++) {
    buffer[i] = (COL_BLUE << 12) | (COL_YELLOW << 8) | txt1[i];
  }
  for (int i = 0; sel[i] != '\0'; i++) {
    buffer[11 + i] = (COL_BLUE << 12) | (COL_WHITE << 8) | sel[i];
  }

  // Draw Score
  const char *s_txt = " Score: ";
  int sx = 60;
  for (int i = 0; s_txt[i] != '\0'; i++) {
    buffer[sx + i] = (COL_BLUE << 12) | (COL_YELLOW << 8) | s_txt[i];
  }
  sx += 8;

  char score_buf[16];
  int v = score, dig = 0;
  if (v == 0)
    score_buf[dig++] = '0';
  while (v > 0) {
    score_buf[dig++] = '0' + (v % 10);
    v /= 10;
  }
  for (int j = 0; j < dig / 2; j++) {
    char t = score_buf[j];
    score_buf[j] = score_buf[dig - 1 - j];
    score_buf[dig - 1 - j] = t;
  }
  score_buf[dig] = '\0';

  for (int i = 0; score_buf[i] != '\0'; i++) {
    buffer[sx + i] = (COL_BLUE << 12) | (COL_WHITE << 8) | score_buf[i];
  }

  for (int i = 0; i < GRID_W * GRID_H; i++) {
    VGA_MEM[i] = buffer[i];
  }
}

void render_title() {
  for (int i = 0; i < GRID_W * GRID_H; i++)
    VGA_MEM[i] = (COL_BLACK << 12) | (COL_WHITE << 8) | ' ';
  print_text(30, 4, "=== MINI-FACTORIO ===", COL_YELLOW, COL_BLACK);
  print_text(25, 8, "CONTROLS:", COL_CYAN, COL_BLACK);
  print_text(10, 10, "WASD    - Move Cursor", COL_WHITE, COL_BLACK);
  print_text(10, 11, "1,2,3,4 - Select Belts (^, v, <, >)", COL_WHITE,
             COL_BLACK);
  print_text(10, 12, "5       - Select Miner (M) on Ore patches to mine",
             COL_WHITE, COL_BLACK);
  print_text(10, 13,
             "7       - Select Furnace (F). Smelts Ore to Plates (i, c)",
             COL_WHITE, COL_BLACK);
  print_text(
      10, 14,
      "6       - Select Crafter (C). 1x Fe Plate + 1x Cu Plate -> Gear(G)",
      COL_WHITE, COL_BLACK);
  print_text(10, 15,
             "8       - Select Lab (L). Consumes Gears (G) to increase Score!",
             COL_WHITE, COL_BLACK);
  print_text(10, 16, "0       - Select Empty (Eraser)", COL_WHITE, COL_BLACK);
  print_text(10, 17, "SPACE   - Place building", COL_WHITE, COL_BLACK);
  print_text(28, 20, "Press SPACE to start...", COL_GREEN, COL_BLACK);
}

void handle_title_input() {
  uint8_t scancode = get_scancode();
  if (scancode == 0 || (scancode & 0x80))
    return;
  if (scancode == 0x39) {
    current_state = 1;
    for (int i = 0; i < GRID_W * GRID_H; i++)
      VGA_MEM[i] = 0;
  }
}

__attribute__((section(".text.entry"))) void entry() {
  current_state = 0;
  cursor_x = 40;
  cursor_y = 12;
  selected_building = BELT_RIGHT;
  tick_counter = 0;
  score = 0;
  init_grid();
  while (inb(0x64) & 1)
    inb(0x60);
  uint32_t loop_counter = 0;
  render_title();
  while (1) {
    if (current_state == 0)
      handle_title_input();
    else {
      handle_input();
      if (loop_counter++ > 500) {
        process_tick();
        tick_counter++;
        loop_counter = 0;
      }
      render();
    }
  }
}