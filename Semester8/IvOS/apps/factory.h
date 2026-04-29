#ifndef FACTORY_H
#define FACTORY_H

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#define GRID_W 80
#define GRID_H 25
#define VGA_MEM ((volatile uint16_t *)0xB8000)
#define COM1 0x3F8

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

typedef enum { TERR_EMPTY = 0, TERR_IRON, TERR_COPPER, TERR_COAL, TERR_STONE } TerrainType;
typedef enum { EMPTY = 0, BELT, SPLITTER, MINER, FURNACE, CRAFTER, LAB } EntityType;
typedef enum { NONE = 0, IRON_ORE, COPPER_ORE, COAL, STONE, IRON_PLATE, COPPER_PLATE, GEAR, COPPER_WIRE, CIRCUIT, MACHINE_PART, ITEM_BELT, ITEM_SPLITTER, ITEM_MINER, ITEM_FURNACE, ITEM_CRAFTER, ITEM_LAB } ItemType;
typedef enum { DIR_UP = 0, DIR_RIGHT = 1, DIR_DOWN = 2, DIR_LEFT = 3 } Dir;

struct Cell {
  EntityType building;
  ItemType item;
  TerrainType terrain;
  uint8_t resource_amount;
  uint8_t handled;
  uint8_t rotation;
  uint8_t inv_iron;
  uint8_t inv_copper;
  uint8_t inv_gear;
  uint8_t furnace_ore_type;
  uint8_t furnace_ore_count;
  uint8_t furnace_fuel_count;
  uint8_t split_anchor;
  uint8_t split_ax;
  uint8_t split_ay;
  uint8_t split_toggle_out;
  uint8_t split_toggle_in;
};

#define HOTBAR_SLOTS 9
#define MAIN_INV_SLOTS 25

struct ItemStack {
  ItemType item;
  uint8_t count;
};

typedef struct {
  struct ItemStack hotbar[HOTBAR_SLOTS];
  struct ItemStack main[MAIN_INV_SLOTS];
  uint8_t selected_hotbar;
  uint8_t main_focus;
  uint8_t recipe_focus;
  uint8_t active_panel;
} PlayerInventory;

typedef struct {
  ItemType result;
  uint8_t result_count;
  uint8_t ingredient_count;
  struct ItemStack ingredients[4];
  const char *name;
} Recipe;

enum { MODE_TITLE = 0, MODE_GAME = 1, MODE_INVENTORY = 2, MODE_MACHINE = 3 };
enum { INVENTORY_PANEL_MAIN = 0, INVENTORY_PANEL_CRAFT = 1 };

extern const Recipe g_recipes[];
extern const int g_recipe_count;

extern struct Cell grid[GRID_H][GRID_W];
extern PlayerInventory player;
extern int cursor_x;
extern int cursor_y;
extern uint8_t selected_rotation;
extern uint32_t tick_counter;
extern int current_state;
extern uint32_t score;
extern uint8_t ctrl_held;
extern char io_status[22];
extern uint8_t should_exit_app;
extern int machine_x;
extern int machine_y;
extern uint8_t machine_panel;
extern uint8_t machine_focus;

static inline int in_bounds(int x, int y) { return x >= 0 && x < GRID_W && y >= 0 && y < GRID_H; }
static inline uint8_t inb(uint16_t port) { uint8_t ret; __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port)); return ret; }
static inline void outb(uint16_t port, uint8_t value) { __asm__ volatile("outb %b0, %w1" : : "a"(value), "Nd"(port)); }

static inline int item_is_placeable(ItemType item) {
  return item == ITEM_BELT || item == ITEM_SPLITTER || item == ITEM_MINER || item == ITEM_FURNACE || item == ITEM_CRAFTER || item == ITEM_LAB;
}
static inline EntityType item_to_building(ItemType item) {
  if (item == ITEM_BELT) return BELT;
  if (item == ITEM_SPLITTER) return SPLITTER;
  if (item == ITEM_MINER) return MINER;
  if (item == ITEM_FURNACE) return FURNACE;
  if (item == ITEM_CRAFTER) return CRAFTER;
  if (item == ITEM_LAB) return LAB;
  return EMPTY;
}
static inline ItemType building_to_item(EntityType building) {
  if (building == BELT) return ITEM_BELT;
  if (building == SPLITTER) return ITEM_SPLITTER;
  if (building == MINER) return ITEM_MINER;
  if (building == FURNACE) return ITEM_FURNACE;
  if (building == CRAFTER) return ITEM_CRAFTER;
  if (building == LAB) return ITEM_LAB;
  return NONE;
}
static inline ItemType terrain_to_item(TerrainType terrain) {
  if (terrain == TERR_IRON) return IRON_ORE;
  if (terrain == TERR_COPPER) return COPPER_ORE;
  if (terrain == TERR_COAL) return COAL;
  if (terrain == TERR_STONE) return STONE;
  return NONE;
}
static inline char item_glyph(ItemType item) {
  if (item == IRON_ORE || item == COPPER_ORE) return '*';
  if (item == COAL) return 'o';
  if (item == STONE) return 's';
  if (item == IRON_PLATE) return 'i';
  if (item == COPPER_PLATE) return 'c';
  if (item == GEAR) return 'G';
  if (item == COPPER_WIRE) return 'w';
  if (item == CIRCUIT) return 'X';
  if (item == MACHINE_PART) return 'P';
  if (item == ITEM_BELT) return 'B';
  if (item == ITEM_SPLITTER) return 'T';
  if (item == ITEM_MINER) return 'M';
  if (item == ITEM_FURNACE) return 'F';
  if (item == ITEM_CRAFTER) return 'C';
  if (item == ITEM_LAB) return 'L';
  return '?';
}
static inline uint8_t item_color(ItemType item) {
  if (item == IRON_ORE || item == IRON_PLATE) return COL_LIGHT_GREY;
  if (item == COPPER_ORE || item == COPPER_PLATE || item == COPPER_WIRE) return COL_BROWN;
  if (item == COAL) return COL_DARK_GREY;
  if (item == STONE) return COL_DARK_GREY;
  if (item == GEAR) return COL_WHITE;
  if (item == CIRCUIT) return COL_GREEN;
  if (item == MACHINE_PART) return COL_YELLOW;
  if (item == ITEM_BELT) return COL_CYAN;
  if (item == ITEM_SPLITTER) return COL_GREEN;
  if (item == ITEM_MINER) return COL_MAGENTA;
  if (item == ITEM_FURNACE) return COL_RED;
  if (item == ITEM_CRAFTER) return COL_YELLOW;
  if (item == ITEM_LAB) return COL_GREEN;
  return COL_WHITE;
}
static inline const char *item_name(ItemType item) {
  if (item == IRON_ORE) return "Iron Ore";
  if (item == COPPER_ORE) return "Copper Ore";
  if (item == COAL) return "Coal";
  if (item == STONE) return "Stone";
  if (item == IRON_PLATE) return "Iron Plate";
  if (item == COPPER_PLATE) return "Copper Plate";
  if (item == GEAR) return "Gear";
  if (item == COPPER_WIRE) return "Copper Wire";
  if (item == CIRCUIT) return "Circuit";
  if (item == MACHINE_PART) return "Machine Part";
  if (item == ITEM_BELT) return "Belt";
  if (item == ITEM_SPLITTER) return "Splitter";
  if (item == ITEM_MINER) return "Miner";
  if (item == ITEM_FURNACE) return "Furnace";
  if (item == ITEM_CRAFTER) return "Crafter";
  if (item == ITEM_LAB) return "Lab";
  return "Empty";
}
static inline int is_furnace_ore(ItemType item) { return item == IRON_ORE || item == COPPER_ORE; }
static inline ItemType furnace_output_for(ItemType ore) {
  if (ore == IRON_ORE) return IRON_PLATE;
  if (ore == COPPER_ORE) return COPPER_PLATE;
  return NONE;
}
static inline void stack_clear(struct ItemStack *stack) { stack->item = NONE; stack->count = 0; }
static inline int stack_is_empty(const struct ItemStack *stack) { return stack->item == NONE || stack->count == 0; }
static inline int inventory_stack_can_merge(const struct ItemStack *stack, ItemType item) {
  return !stack_is_empty(stack) && stack->item == item && stack->count < 255;
}

// Subsystem functions
void status_set(const char *msg);

// IO
void save_map_to_serial();
int load_map_from_serial();
char hex_digit(uint8_t v);

// Inventory
void inventory_clear(void);
int add_to_stack(struct ItemStack *stack, ItemType item, uint8_t count);
int add_item_to_main_inventory(ItemType item, uint8_t count);
int add_item_to_hotbar(uint8_t slot, ItemType item, uint8_t count);
void sort_main_inventory(void);
int inventory_total_count(ItemType item);
int remove_item_from_inventory(ItemType item, uint8_t count);
int recipe_can_craft(int recipe_index);
int craft_recipe(int recipe_index);
int get_selected_hotbar_item(ItemType *item_out);
int hotbar_slot_from_scancode(uint8_t scancode);
int placeable_slot_count(void);
int transfer_main_and_hotbar(int main_index, int hotbar_index);
int move_main_stack_to_hotbar(int main_index, int hotbar_index);
void start_player_inventory(void);

// Logic
int dir_dx(uint8_t dir);
int dir_dy(uint8_t dir);
uint8_t right_dir(uint8_t dir);
uint8_t left_dir(uint8_t dir);
char dir_char(uint8_t dir);
int is_rotatable(EntityType e);
void reset_cell_runtime(int x, int y);
void clear_splitter_at_anchor(int ax, int ay);
void clear_entity_at(int x, int y);
void splitter_secondary(int ax, int ay, uint8_t rot, int *sx, int *sy);
int place_splitter(int ax, int ay, uint8_t rot);
int belt_points_to(int bx, int by, int tx, int ty);
int belt_points_away_from(int bx, int by, int sx, int sy);
int resource_item_at(int x, int y, ItemType *item_out);
int mine_resource_at(int x, int y);
void clear_entity_with_refund(int x, int y);
int place_from_hotbar(void);
void init_grid();
int push_item_to_belt(int x, int y, ItemType item);
void move_splitter_item(struct Cell *anchor, int srcx, int srcy, int dstx, int dsty, int use_out_toggle, int use_in_toggle);
void process_splitter_anchor(int ax, int ay);
void process_tick();

// Render
void print_text(int x, int y, const char *str, uint8_t fg, uint8_t bg);
void render_gameplay(uint16_t *buffer);
void render_inventory_screen(uint16_t *buffer);
void render_machine_screen(uint16_t *buffer);
void render();
void render_title();

#endif
