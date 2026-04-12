// ============================================================================
// BARE-METAL MINI-FACTORIO
// Target: 32-bit x86 Custom OS (No stdlib)
// ============================================================================

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#define GRID_W 80
#define GRID_H 25
#define VGA_MEM ((volatile uint16_t *)0xB8000)
#define COM1 0x3F8

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

typedef enum {
  TERR_EMPTY = 0,
  TERR_IRON,
  TERR_COPPER,
  TERR_COAL,
  TERR_STONE
} TerrainType;

typedef enum {
  EMPTY = 0,
  BELT,
  SPLITTER,
  MINER,
  FURNACE,
  CRAFTER,
  LAB
} EntityType;

typedef enum {
  NONE = 0,
  IRON_ORE,
  COPPER_ORE,
  COAL,
  STONE,
  IRON_PLATE,
  COPPER_PLATE,
  GEAR,
  COPPER_WIRE,
  CIRCUIT,
  MACHINE_PART,
  ITEM_BELT,
  ITEM_SPLITTER,
  ITEM_MINER,
  ITEM_FURNACE,
  ITEM_CRAFTER,
  ITEM_LAB
} ItemType;

typedef enum { DIR_UP = 0, DIR_RIGHT = 1, DIR_DOWN = 2, DIR_LEFT = 3 } Dir;

struct Cell {
  EntityType building;
  ItemType item;
  TerrainType terrain;
  uint8_t resource_amount;
  uint8_t handled;

  // Generic per-cell orientation for rotatable entities.
  uint8_t rotation;

  // Crafter inventory.
  uint8_t inv_iron;
  uint8_t inv_copper;
  uint8_t inv_gear;

  // Furnace manual inventory.
  uint8_t furnace_ore_type;
  uint8_t furnace_ore_count;
  uint8_t furnace_fuel_count;

  // Splitter bookkeeping.
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

void clear_entity_at(int x, int y);
int place_splitter(int ax, int ay, uint8_t rot);
char hex_digit(uint8_t v);
static void sort_main_inventory(void);
static void reset_cell_runtime(int x, int y);

enum { MODE_TITLE = 0, MODE_GAME = 1, MODE_INVENTORY = 2, MODE_MACHINE = 3 };

enum { INVENTORY_PANEL_MAIN = 0, INVENTORY_PANEL_CRAFT = 1 };

static const Recipe g_recipes[] = {
    {COPPER_WIRE, 2, 1, {{COPPER_PLATE, 1}}, "Copper Wire"},
    {CIRCUIT, 1, 2, {{COPPER_WIRE, 3}, {IRON_PLATE, 2}}, "Circuit"},
    {ITEM_FURNACE, 1, 2, {{STONE, 10}, {COAL, 1}}, "Furnace"},
    {ITEM_BELT, 1, 2, {{IRON_PLATE, 1}, {GEAR, 1}}, "Belt"},
    {ITEM_MINER, 1, 3, {{CIRCUIT, 2}, {IRON_PLATE, 4}, {GEAR, 2}}, "Miner"},
    {ITEM_CRAFTER, 1, 2, {{CIRCUIT, 4}, {MACHINE_PART, 2}}, "Crafter"},
    {ITEM_LAB, 1, 3, {{CIRCUIT, 8}, {MACHINE_PART, 4}, {IRON_PLATE, 8}}, "Lab"},
    {ITEM_SPLITTER, 1, 1, {{IRON_PLATE, 2}}, "Splitter"},
    {MACHINE_PART, 1, 2, {{GEAR, 1}, {COPPER_PLATE, 1}}, "Machine Part"},
    {GEAR, 1, 1, {{IRON_PLATE, 2}}, "Gear"},
};

static const int g_recipe_count = sizeof(g_recipes) / sizeof(g_recipes[0]);

// --- Global State ---
struct Cell grid[GRID_H][GRID_W];
PlayerInventory player;
int cursor_x = 40;
int cursor_y = 12;
uint8_t selected_rotation = DIR_RIGHT;
uint32_t tick_counter = 0;
int current_state = 0;
uint32_t score = 0;
uint8_t ctrl_held = 0;
char io_status[22] = "IO: idle             ";
uint8_t should_exit_app = 0;
int machine_x = -1;
int machine_y = -1;
uint8_t machine_panel = 0;
uint8_t machine_focus = 0;

static inline int item_is_placeable(ItemType item) {
  return item == ITEM_BELT || item == ITEM_SPLITTER || item == ITEM_MINER ||
         item == ITEM_FURNACE || item == ITEM_CRAFTER || item == ITEM_LAB;
}

static inline EntityType item_to_building(ItemType item) {
  if (item == ITEM_BELT)
    return BELT;
  if (item == ITEM_SPLITTER)
    return SPLITTER;
  if (item == ITEM_MINER)
    return MINER;
  if (item == ITEM_FURNACE)
    return FURNACE;
  if (item == ITEM_CRAFTER)
    return CRAFTER;
  if (item == ITEM_LAB)
    return LAB;
  return EMPTY;
}

static inline ItemType building_to_item(EntityType building) {
  if (building == BELT)
    return ITEM_BELT;
  if (building == SPLITTER)
    return ITEM_SPLITTER;
  if (building == MINER)
    return ITEM_MINER;
  if (building == FURNACE)
    return ITEM_FURNACE;
  if (building == CRAFTER)
    return ITEM_CRAFTER;
  if (building == LAB)
    return ITEM_LAB;
  return NONE;
}

static inline ItemType terrain_to_item(TerrainType terrain) {
  if (terrain == TERR_IRON)
    return IRON_ORE;
  if (terrain == TERR_COPPER)
    return COPPER_ORE;
  if (terrain == TERR_COAL)
    return COAL;
  if (terrain == TERR_STONE)
    return STONE;
  return NONE;
}

static inline char item_glyph(ItemType item) {
  if (item == IRON_ORE || item == COPPER_ORE)
    return '*';
  if (item == COAL)
    return 'o';
  if (item == STONE)
    return 's';
  if (item == IRON_PLATE)
    return 'i';
  if (item == COPPER_PLATE)
    return 'c';
  if (item == GEAR)
    return 'G';
  if (item == COPPER_WIRE)
    return 'w';
  if (item == CIRCUIT)
    return 'X';
  if (item == MACHINE_PART)
    return 'P';
  if (item == ITEM_BELT)
    return 'B';
  if (item == ITEM_SPLITTER)
    return 'T';
  if (item == ITEM_MINER)
    return 'M';
  if (item == ITEM_FURNACE)
    return 'F';
  if (item == ITEM_CRAFTER)
    return 'C';
  if (item == ITEM_LAB)
    return 'L';
  return '?';
}

static inline uint8_t item_color(ItemType item) {
  if (item == IRON_ORE || item == IRON_PLATE)
    return COL_LIGHT_GREY;
  if (item == COPPER_ORE || item == COPPER_PLATE || item == COPPER_WIRE)
    return COL_BROWN;
  if (item == COAL)
    return COL_DARK_GREY;
  if (item == STONE)
    return COL_DARK_GREY;
  if (item == GEAR)
    return COL_WHITE;
  if (item == CIRCUIT)
    return COL_GREEN;
  if (item == MACHINE_PART)
    return COL_YELLOW;
  if (item == ITEM_BELT)
    return COL_CYAN;
  if (item == ITEM_SPLITTER)
    return COL_GREEN;
  if (item == ITEM_MINER)
    return COL_MAGENTA;
  if (item == ITEM_FURNACE)
    return COL_RED;
  if (item == ITEM_CRAFTER)
    return COL_YELLOW;
  if (item == ITEM_LAB)
    return COL_GREEN;
  return COL_WHITE;
}

static inline const char *item_name(ItemType item) {
  if (item == IRON_ORE)
    return "Iron Ore";
  if (item == COPPER_ORE)
    return "Copper Ore";
  if (item == COAL)
    return "Coal";
  if (item == STONE)
    return "Stone";
  if (item == IRON_PLATE)
    return "Iron Plate";
  if (item == COPPER_PLATE)
    return "Copper Plate";
  if (item == GEAR)
    return "Gear";
  if (item == COPPER_WIRE)
    return "Copper Wire";
  if (item == CIRCUIT)
    return "Circuit";
  if (item == MACHINE_PART)
    return "Machine Part";
  if (item == ITEM_BELT)
    return "Belt";
  if (item == ITEM_SPLITTER)
    return "Splitter";
  if (item == ITEM_MINER)
    return "Miner";
  if (item == ITEM_FURNACE)
    return "Furnace";
  if (item == ITEM_CRAFTER)
    return "Crafter";
  if (item == ITEM_LAB)
    return "Lab";
  return "Empty";
}

static inline void stack_clear(struct ItemStack *stack) {
  stack->item = NONE;
  stack->count = 0;
}

static inline int stack_is_empty(const struct ItemStack *stack) {
  return stack->item == NONE || stack->count == 0;
}

static inline void inventory_clear(void) {
  for (int i = 0; i < HOTBAR_SLOTS; i++)
    stack_clear(&player.hotbar[i]);
  for (int i = 0; i < MAIN_INV_SLOTS; i++)
    stack_clear(&player.main[i]);
  player.selected_hotbar = 0;
  player.main_focus = 0;
  player.recipe_focus = 0;
  player.active_panel = INVENTORY_PANEL_MAIN;
}

static inline int inventory_stack_can_merge(const struct ItemStack *stack,
                                            ItemType item) {
  return !stack_is_empty(stack) && stack->item == item && stack->count < 255;
}

static int add_to_stack(struct ItemStack *stack, ItemType item, uint8_t count) {
  if (count == 0)
    return 1;
  if (stack_is_empty(stack)) {
    stack->item = item;
    stack->count = count;
    return 1;
  }
  if (stack->item != item || stack->count > 255 - count)
    return 0;
  stack->count += count;
  return 1;
}

static int add_item_to_main_inventory(ItemType item, uint8_t count) {
  if (item == NONE || count == 0)
    return 1;

  for (int i = 0; i < MAIN_INV_SLOTS && count > 0; i++) {
    if (inventory_stack_can_merge(&player.main[i], item)) {
      uint8_t space = (uint8_t)(255 - player.main[i].count);
      uint8_t add = count < space ? count : space;
      player.main[i].count += add;
      count -= add;
    }
  }

  for (int i = 0; i < MAIN_INV_SLOTS && count > 0; i++) {
    if (stack_is_empty(&player.main[i])) {
      player.main[i].item = item;
      player.main[i].count = count;
      count = 0;
      break;
    }
  }

  if (count != 0)
    return 0;

  sort_main_inventory();
  return 1;
}

static int add_item_to_hotbar(uint8_t slot, ItemType item, uint8_t count) {
  if (slot >= HOTBAR_SLOTS || item == NONE || count == 0)
    return 0;
  if (stack_is_empty(&player.hotbar[slot])) {
    player.hotbar[slot].item = item;
    player.hotbar[slot].count = count;
    return 1;
  }
  if (player.hotbar[slot].item != item ||
      player.hotbar[slot].count > 255 - count)
    return 0;
  player.hotbar[slot].count += count;
  return 1;
}

static void sort_main_inventory(void) {
  for (int i = 0; i < MAIN_INV_SLOTS; i++) {
    for (int j = i + 1; j < MAIN_INV_SLOTS; j++) {
      struct ItemStack *a = &player.main[i];
      struct ItemStack *b = &player.main[j];
      int a_empty = stack_is_empty(a);
      int b_empty = stack_is_empty(b);
      if (a_empty && !b_empty) {
        struct ItemStack tmp = *a;
        *a = *b;
        *b = tmp;
        continue;
      }
      if (!a_empty && !b_empty) {
        if (b->count > a->count ||
            (b->count == a->count && b->item < a->item)) {
          struct ItemStack tmp = *a;
          *a = *b;
          *b = tmp;
        }
      }
    }
  }
}

static int inventory_total_count(ItemType item) {
  int total = 0;
  for (int i = 0; i < MAIN_INV_SLOTS; i++)
    if (!stack_is_empty(&player.main[i]) && player.main[i].item == item)
      total += player.main[i].count;
  for (int i = 0; i < HOTBAR_SLOTS; i++)
    if (!stack_is_empty(&player.hotbar[i]) && player.hotbar[i].item == item)
      total += player.hotbar[i].count;
  return total;
}

static int remove_item_from_inventory(ItemType item, uint8_t count) {
  if (item == NONE || count == 0)
    return 1;

  for (int i = 0; i < MAIN_INV_SLOTS && count > 0; i++) {
    struct ItemStack *stack = &player.main[i];
    if (stack_is_empty(stack) || stack->item != item)
      continue;
    uint8_t take = stack->count < count ? stack->count : count;
    stack->count -= take;
    count -= take;
    if (stack->count == 0)
      stack_clear(stack);
  }

  for (int i = 0; i < HOTBAR_SLOTS && count > 0; i++) {
    struct ItemStack *stack = &player.hotbar[i];
    if (stack_is_empty(stack) || stack->item != item)
      continue;
    uint8_t take = stack->count < count ? stack->count : count;
    stack->count -= take;
    count -= take;
    if (stack->count == 0)
      stack_clear(stack);
  }

  if (count != 0)
    return 0;

  sort_main_inventory();
  return 1;
}

static int recipe_can_craft(int recipe_index) {
  if (recipe_index < 0 || recipe_index >= g_recipe_count)
    return 0;
  const Recipe *recipe = &g_recipes[recipe_index];
  for (int i = 0; i < recipe->ingredient_count; i++) {
    if (inventory_total_count(recipe->ingredients[i].item) <
        recipe->ingredients[i].count)
      return 0;
  }
  return 1;
}

static int craft_recipe(int recipe_index) {
  if (!recipe_can_craft(recipe_index))
    return 0;

  const Recipe *recipe = &g_recipes[recipe_index];
  for (int i = 0; i < recipe->ingredient_count; i++) {
    if (!remove_item_from_inventory(recipe->ingredients[i].item,
                                    recipe->ingredients[i].count))
      return 0;
  }

  if (!add_item_to_main_inventory(recipe->result, recipe->result_count)) {
    for (int i = 0; i < recipe->ingredient_count; i++)
      add_item_to_main_inventory(recipe->ingredients[i].item,
                                 recipe->ingredients[i].count);
    return 0;
  }

  return 1;
}

static int get_selected_hotbar_item(ItemType *item_out) {
  if (player.selected_hotbar >= HOTBAR_SLOTS)
    return 0;
  if (stack_is_empty(&player.hotbar[player.selected_hotbar]))
    return 0;
  *item_out = player.hotbar[player.selected_hotbar].item;
  return 1;
}

static int hotbar_slot_from_scancode(uint8_t scancode) {
  if (scancode >= 0x02 && scancode <= 0x0A)
    return (int)(scancode - 0x02);
  return -1;
}

static int placeable_slot_count(void) {
  int count = 0;
  for (int i = 0; i < HOTBAR_SLOTS; i++)
    if (!stack_is_empty(&player.hotbar[i]))
      count++;
  return count;
}

static int is_furnace_ore(ItemType item) {
  return item == IRON_ORE || item == COPPER_ORE;
}

static ItemType furnace_output_for(ItemType ore) {
  if (ore == IRON_ORE)
    return IRON_PLATE;
  if (ore == COPPER_ORE)
    return COPPER_PLATE;
  return NONE;
}

static int transfer_main_and_hotbar(int main_index, int hotbar_index) {
  if (main_index < 0 || main_index >= MAIN_INV_SLOTS || hotbar_index < 0 ||
      hotbar_index >= HOTBAR_SLOTS)
    return 0;

  if (!stack_is_empty(&player.main[main_index])) {
    struct ItemStack tmp = player.hotbar[hotbar_index];
    player.hotbar[hotbar_index] = player.main[main_index];
    player.main[main_index] = tmp;
    sort_main_inventory();
    return 1;
  }

  if (stack_is_empty(&player.hotbar[hotbar_index]))
    return 0;

  player.main[main_index] = player.hotbar[hotbar_index];
  stack_clear(&player.hotbar[hotbar_index]);
  return 1;
}

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static inline void outb(uint16_t port, uint8_t value) {
  __asm__ volatile("outb %b0, %w1" : : "a"(value), "Nd"(port));
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

int in_bounds(int x, int y) {
  return x >= 0 && x < GRID_W && y >= 0 && y < GRID_H;
}

void status_set(const char *msg) {
  int i = 0;
  while (i < 21) {
    if (msg[i] == '\0')
      break;
    io_status[i] = msg[i];
    i++;
  }
  while (i < 21) {
    io_status[i] = ' ';
    i++;
  }
  io_status[21] = '\0';
}

int serial_can_read() { return (inb(COM1 + 5) & 0x01) != 0; }

int serial_can_write() { return (inb(COM1 + 5) & 0x20) != 0; }

void serial_putc(char c) {
  while (!serial_can_write())
    ;
  outb(COM1, (uint8_t)c);
}

void serial_print(const char *s) {
  while (*s) {
    serial_putc(*s++);
  }
}

void serial_print_hex8(uint8_t v) {
  serial_putc(hex_digit((uint8_t)((v >> 4) & 0x0F)));
  serial_putc(hex_digit((uint8_t)(v & 0x0F)));
}

char serial_read_blocking() {
  while (!serial_can_read())
    ;
  return (char)inb(COM1);
}

int read_serial_sized(char *buf, int read_len) {
  int i = 0;
  if (read_len <= 0) {
    buf[0] = '\0';
    return 0;
  }

  while (i < read_len) {
    char c = serial_read_blocking();
    if (c == '\r')
      continue;
    if (c == '\n')
      continue;
    buf[i++] = c;
  }

  buf[i] = '\0';
  return i;
}

char hex_digit(uint8_t v) {
  if (v < 10)
    return (char)('0' + v);
  return (char)('A' + (v - 10));
}

int hex_value(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  return -1;
}

static void print_hex_byte(uint8_t value) { serial_print_hex8(value); }

static void print_hex_u32(uint32_t value) {
  print_hex_byte((uint8_t)((value >> 24) & 0xFF));
  print_hex_byte((uint8_t)((value >> 16) & 0xFF));
  print_hex_byte((uint8_t)((value >> 8) & 0xFF));
  print_hex_byte((uint8_t)(value & 0xFF));
}

static uint8_t parse_hex_byte_at(const char *line, int offset) {
  int hi = hex_value(line[offset]);
  int lo = hex_value(line[offset + 1]);
  if (hi < 0 || lo < 0)
    return 0;
  return (uint8_t)((hi << 4) | lo);
}

void save_map_to_serial() {
  serial_print("MF2\n");

  serial_putc('P');
  print_hex_byte((uint8_t)cursor_x);
  print_hex_byte((uint8_t)cursor_y);
  print_hex_byte(player.selected_hotbar);
  print_hex_byte(player.main_focus);
  print_hex_byte(player.recipe_focus);
  print_hex_byte(player.active_panel);
  print_hex_u32(score);
  serial_putc('\n');

  serial_putc('H');
  for (int i = 0; i < HOTBAR_SLOTS; i++) {
    print_hex_byte((uint8_t)player.hotbar[i].item);
    print_hex_byte(player.hotbar[i].count);
  }
  serial_putc('\n');

  serial_putc('I');
  for (int i = 0; i < MAIN_INV_SLOTS; i++) {
    print_hex_byte((uint8_t)player.main[i].item);
    print_hex_byte(player.main[i].count);
  }
  serial_putc('\n');

  for (int y = 0; y < GRID_H; y++) {
    serial_putc('W');
    for (int x = 0; x < GRID_W; x++) {
      struct Cell *cell = &grid[y][x];
      print_hex_byte((uint8_t)cell->building);
      print_hex_byte(cell->rotation);
      print_hex_byte((uint8_t)cell->item);
      print_hex_byte((uint8_t)cell->terrain);
      print_hex_byte(cell->resource_amount);
      print_hex_byte(cell->inv_iron);
      print_hex_byte(cell->inv_copper);
      print_hex_byte(cell->inv_gear);
      print_hex_byte(cell->split_anchor);
      print_hex_byte(cell->furnace_ore_type);
      print_hex_byte(cell->furnace_ore_count);
      print_hex_byte(cell->furnace_fuel_count);
    }
    serial_putc('\n');
  }

  serial_print("END\n");
  status_set("IO: map saved");
}

static int load_stack_line(const char *line, struct ItemStack *slots,
                           int slot_count, int payload_offset) {
  for (int i = 0; i < slot_count; i++) {
    int offset = payload_offset + i * 4;
    uint8_t item = parse_hex_byte_at(line, offset);
    uint8_t count = parse_hex_byte_at(line, offset + 2);
    if (item == NONE || count == 0) {
      stack_clear(&slots[i]);
    } else {
      slots[i].item = (ItemType)item;
      slots[i].count = count;
    }
  }
  return 1;
}

int load_map_from_serial() {
  char line[2048];

  read_serial_sized(line, 3);
  if (!(line[0] == 'M' && line[1] == 'F' && line[2] == '2' &&
        line[3] == '\0')) {
    status_set("IO: bad header");
    return 0;
  }

  int len = read_serial_sized(line, 1 + 2 + 2 + 2 + 2 + 2 + 2 + 8);
  if (len != 21 || line[0] != 'P') {
    status_set("IO: bad player");
    return 0;
  }

  cursor_x = parse_hex_byte_at(line, 1);
  cursor_y = parse_hex_byte_at(line, 3);
  player.selected_hotbar = parse_hex_byte_at(line, 5) % HOTBAR_SLOTS;
  player.main_focus = parse_hex_byte_at(line, 7) % MAIN_INV_SLOTS;
  player.recipe_focus = parse_hex_byte_at(line, 9) % (uint8_t)g_recipe_count;
  player.active_panel = parse_hex_byte_at(line, 11) & 1;
  score = ((uint32_t)parse_hex_byte_at(line, 13) << 24) |
          ((uint32_t)parse_hex_byte_at(line, 15) << 16) |
          ((uint32_t)parse_hex_byte_at(line, 17) << 8) |
          (uint32_t)parse_hex_byte_at(line, 19);

  len = read_serial_sized(line, 1 + HOTBAR_SLOTS * 4);
  if (len != 37 || line[0] != 'H') {
    status_set("IO: bad hotbar");
    return 0;
  }
  load_stack_line(line, player.hotbar, HOTBAR_SLOTS, 1);

  len = read_serial_sized(line, 1 + MAIN_INV_SLOTS * 4);
  if (len != 101 || line[0] != 'I') {
    status_set("IO: bad inventory");
    return 0;
  }
  load_stack_line(line, player.main, MAIN_INV_SLOTS, 1);

  for (int y = 0; y < GRID_H; y++) {
    len = read_serial_sized(line, 1 + GRID_W * 24);
    if (len != 1921 || line[0] != 'W') {
      status_set("IO: bad world");
      return 0;
    }

    for (int x = 0; x < GRID_W; x++) {
      int base = 1 + x * 24;
      uint8_t b = parse_hex_byte_at(line, base);
      uint8_t r = parse_hex_byte_at(line, base + 2);
      uint8_t it = parse_hex_byte_at(line, base + 4);
      uint8_t terr = parse_hex_byte_at(line, base + 6);
      uint8_t res = parse_hex_byte_at(line, base + 8);
      uint8_t inv_iron = parse_hex_byte_at(line, base + 10);
      uint8_t inv_copper = parse_hex_byte_at(line, base + 12);
      uint8_t inv_gear = parse_hex_byte_at(line, base + 14);
      uint8_t split_anchor = parse_hex_byte_at(line, base + 16);
      uint8_t furnace_ore_type = parse_hex_byte_at(line, base + 18);
      uint8_t furnace_ore_count = parse_hex_byte_at(line, base + 20);
      uint8_t furnace_fuel_count = parse_hex_byte_at(line, base + 22);

      reset_cell_runtime(x, y);
      grid[y][x].terrain = (TerrainType)terr;
      grid[y][x].resource_amount = res;
      grid[y][x].inv_iron = inv_iron;
      grid[y][x].inv_copper = inv_copper;
      grid[y][x].inv_gear = inv_gear;
      grid[y][x].furnace_ore_type = furnace_ore_type;
      grid[y][x].furnace_ore_count = furnace_ore_count;
      grid[y][x].furnace_fuel_count = furnace_fuel_count;

      if (b == EMPTY)
        continue;

      if (b == SPLITTER) {
        if (split_anchor)
          place_splitter(x, y, (uint8_t)(r & 3));
        continue;
      }

      grid[y][x].building = (EntityType)b;
      grid[y][x].rotation = (uint8_t)(r & 3);
      grid[y][x].item = (ItemType)it;
      grid[y][x].split_anchor = 0;
    }
  }

  read_serial_sized(line, 3);
  if (!(line[0] == 'E' && line[1] == 'N' && line[2] == 'D' &&
        line[3] == '\0')) {
    status_set("IO: bad trailer");
    return 0;
  }

  status_set("IO: map loaded");
  return 1;
}

int dir_dx(uint8_t dir) {
  if (dir == DIR_LEFT)
    return -1;
  if (dir == DIR_RIGHT)
    return 1;
  return 0;
}

int dir_dy(uint8_t dir) {
  if (dir == DIR_UP)
    return -1;
  if (dir == DIR_DOWN)
    return 1;
  return 0;
}

uint8_t right_dir(uint8_t dir) { return (uint8_t)((dir + 1) & 3); }

uint8_t left_dir(uint8_t dir) { return (uint8_t)((dir + 3) & 3); }

char dir_char(uint8_t dir) {
  if (dir == DIR_UP)
    return '^';
  if (dir == DIR_RIGHT)
    return '>';
  if (dir == DIR_DOWN)
    return 'v';
  return '<';
}

int is_rotatable(EntityType e) { return e == BELT || e == SPLITTER; }

void reset_cell_runtime(int x, int y) {
  grid[y][x].building = EMPTY;
  grid[y][x].item = NONE;
  grid[y][x].handled = 0;
  grid[y][x].rotation = DIR_RIGHT;
  grid[y][x].resource_amount = 0;
  grid[y][x].inv_iron = 0;
  grid[y][x].inv_copper = 0;
  grid[y][x].inv_gear = 0;
  grid[y][x].furnace_ore_type = NONE;
  grid[y][x].furnace_ore_count = 0;
  grid[y][x].furnace_fuel_count = 0;
  grid[y][x].split_anchor = 0;
  grid[y][x].split_ax = (uint8_t)x;
  grid[y][x].split_ay = (uint8_t)y;
  grid[y][x].split_toggle_out = 0;
  grid[y][x].split_toggle_in = 0;
}

void clear_splitter_at_anchor(int ax, int ay) {
  if (!in_bounds(ax, ay))
    return;
  if (grid[ay][ax].building != SPLITTER)
    return;

  uint8_t rot = grid[ay][ax].rotation;
  int sx = ax + dir_dx(right_dir(rot));
  int sy = ay + dir_dy(right_dir(rot));

  reset_cell_runtime(ax, ay);
  if (in_bounds(sx, sy) && grid[sy][sx].building == SPLITTER &&
      grid[sy][sx].split_ax == (uint8_t)ax &&
      grid[sy][sx].split_ay == (uint8_t)ay) {
    reset_cell_runtime(sx, sy);
  }
}

void clear_entity_at(int x, int y) {
  if (!in_bounds(x, y))
    return;
  if (grid[y][x].building == SPLITTER) {
    clear_splitter_at_anchor(grid[y][x].split_ax, grid[y][x].split_ay);
  } else {
    reset_cell_runtime(x, y);
  }
}

void splitter_secondary(int ax, int ay, uint8_t rot, int *sx, int *sy) {
  uint8_t rd = right_dir(rot);
  *sx = ax + dir_dx(rd);
  *sy = ay + dir_dy(rd);
}

int place_splitter(int ax, int ay, uint8_t rot) {
  int bx, by;
  splitter_secondary(ax, ay, rot, &bx, &by);
  if (!in_bounds(ax, ay) || !in_bounds(bx, by))
    return 0;

  clear_entity_at(ax, ay);
  clear_entity_at(bx, by);

  grid[ay][ax].building = SPLITTER;
  grid[ay][ax].rotation = rot;
  grid[ay][ax].split_anchor = 1;
  grid[ay][ax].split_ax = (uint8_t)ax;
  grid[ay][ax].split_ay = (uint8_t)ay;
  grid[ay][ax].item = NONE;
  grid[ay][ax].split_toggle_out = 0;
  grid[ay][ax].split_toggle_in = 0;

  grid[by][bx].building = SPLITTER;
  grid[by][bx].rotation = rot;
  grid[by][bx].split_anchor = 0;
  grid[by][bx].split_ax = (uint8_t)ax;
  grid[by][bx].split_ay = (uint8_t)ay;
  grid[by][bx].item = NONE;

  return 1;
}

int belt_points_to(int bx, int by, int tx, int ty) {
  if (!in_bounds(bx, by) || !in_bounds(tx, ty))
    return 0;
  if (grid[by][bx].building != BELT)
    return 0;
  return bx + dir_dx(grid[by][bx].rotation) == tx &&
         by + dir_dy(grid[by][bx].rotation) == ty;
}

int belt_points_away_from(int bx, int by, int sx, int sy) {
  if (!in_bounds(bx, by) || !in_bounds(sx, sy))
    return 0;
  if (grid[by][bx].building != BELT)
    return 0;
  return bx - sx == dir_dx(grid[by][bx].rotation) &&
         by - sy == dir_dy(grid[by][bx].rotation);
}

static int resource_item_at(int x, int y, ItemType *item_out) {
  if (!in_bounds(x, y))
    return 0;
  if (grid[y][x].resource_amount == 0)
    return 0;
  *item_out = terrain_to_item(grid[y][x].terrain);
  return *item_out != NONE;
}

static int mine_resource_at(int x, int y) {
  ItemType item = NONE;
  if (!resource_item_at(x, y, &item))
    return 0;
  if (grid[y][x].building != EMPTY)
    return 0;
  if (!add_item_to_main_inventory(item, 1))
    return 0;

  if (grid[y][x].resource_amount > 0)
    grid[y][x].resource_amount--;
  if (grid[y][x].resource_amount == 0)
    grid[y][x].terrain = TERR_EMPTY;
  return 1;
}

static void clear_entity_with_refund(int x, int y) {
  if (!in_bounds(x, y))
    return;

  EntityType building = grid[y][x].building;
  if (building != EMPTY)
    add_item_to_main_inventory(building_to_item(building), 1);
  clear_entity_at(x, y);
}

static int move_main_stack_to_hotbar(int main_index, int hotbar_index) {
  if (main_index < 0 || main_index >= MAIN_INV_SLOTS || hotbar_index < 0 ||
      hotbar_index >= HOTBAR_SLOTS)
    return 0;
  if (stack_is_empty(&player.main[main_index]))
    return 0;

  struct ItemStack tmp = player.hotbar[hotbar_index];
  player.hotbar[hotbar_index] = player.main[main_index];
  player.main[main_index] = tmp;
  sort_main_inventory();
  return 1;
}

static int place_from_hotbar(void) {
  if (player.selected_hotbar >= HOTBAR_SLOTS)
    return 0;

  struct ItemStack *slot = &player.hotbar[player.selected_hotbar];
  if (stack_is_empty(slot))
    return 0;
  if (!item_is_placeable(slot->item))
    return 0;

  ItemType item = slot->item;
  EntityType building = item_to_building(item);

  if (!in_bounds(cursor_x, cursor_y))
    return 0;

  if (item == ITEM_SPLITTER) {
    int sx = cursor_x + dir_dx(right_dir(selected_rotation));
    int sy = cursor_y + dir_dy(right_dir(selected_rotation));
    if (!in_bounds(sx, sy))
      return 0;
    clear_entity_with_refund(cursor_x, cursor_y);
    clear_entity_with_refund(sx, sy);
    if (!place_splitter(cursor_x, cursor_y, selected_rotation))
      return 0;
  } else {
    clear_entity_with_refund(cursor_x, cursor_y);
    grid[cursor_y][cursor_x].building = building;
    grid[cursor_y][cursor_x].rotation =
        item == ITEM_BELT || item == ITEM_SPLITTER ? selected_rotation
                                                   : DIR_RIGHT;
    grid[cursor_y][cursor_x].item = NONE;
  }

  if (slot->count > 0)
    slot->count--;
  if (slot->count == 0)
    stack_clear(slot);
  return 1;
}

static void start_player_inventory(void) {
  inventory_clear();
  add_item_to_main_inventory(ITEM_FURNACE, 1);
  add_item_to_main_inventory(COAL, 10);
  player.selected_hotbar = 0;
  player.main_focus = 0;
  player.recipe_focus = 0;
  player.active_panel = INVENTORY_PANEL_MAIN;
}

// --- Initialization ---
void init_grid() {
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      grid[y][x].terrain = TERR_EMPTY;
      reset_cell_runtime(x, y);

      // Draw map patches
      if (x >= 10 && x <= 18 && y >= 5 && y <= 12)
        grid[y][x].terrain = TERR_IRON;
      if (x >= 60 && x <= 68 && y >= 14 && y <= 20)
        grid[y][x].terrain = TERR_COPPER;
      if (x >= 24 && x <= 32 && y >= 3 && y <= 9)
        grid[y][x].terrain = TERR_COAL;
      if (x >= 40 && x <= 48 && y >= 14 && y <= 21)
        grid[y][x].terrain = TERR_STONE;

      if (grid[y][x].terrain != TERR_EMPTY)
        grid[y][x].resource_amount = 32;
    }
  }
}

// --- Game Logic ---
int push_item_to_belt(int x, int y, ItemType item) {
  int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  for (int d = 0; d < 4; d++) {
    int bx = x + dirs[d][0];
    int by = y + dirs[d][1];
    if (!in_bounds(bx, by))
      continue;

    struct Cell *target = &grid[by][bx];
    if (target->building == BELT && target->item == NONE &&
        belt_points_away_from(bx, by, x, y)) {
      target->item = item;
      target->handled = 1;
      return 1;
    }
  }
  return 0;
}

void move_splitter_item(struct Cell *anchor, int srcx, int srcy, int dstx,
                        int dsty, int use_out_toggle, int use_in_toggle) {
  if (!in_bounds(srcx, srcy) || !in_bounds(dstx, dsty))
    return;
  if (grid[srcy][srcx].item == NONE || grid[dsty][dstx].item != NONE)
    return;

  grid[dsty][dstx].item = grid[srcy][srcx].item;
  grid[dsty][dstx].handled = 1;
  grid[srcy][srcx].item = NONE;

  if (use_out_toggle)
    anchor->split_toggle_out ^= 1;
  if (use_in_toggle)
    anchor->split_toggle_in ^= 1;
}

void process_splitter_anchor(int ax, int ay) {
  struct Cell *anchor = &grid[ay][ax];
  uint8_t dir = anchor->rotation;
  int bx, by;
  splitter_secondary(ax, ay, dir, &bx, &by);
  if (!in_bounds(bx, by) || grid[by][bx].building != SPLITTER)
    return;

  int in0x = ax - dir_dx(dir);
  int in0y = ay - dir_dy(dir);
  int in1x = bx - dir_dx(dir);
  int in1y = by - dir_dy(dir);
  int out0x = ax + dir_dx(dir);
  int out0y = ay + dir_dy(dir);
  int out1x = bx + dir_dx(dir);
  int out1y = by + dir_dy(dir);

  int src_ok[2] = {0, 0};
  int dst_ok[2] = {0, 0};

  if (in_bounds(in0x, in0y) && belt_points_to(in0x, in0y, ax, ay) &&
      grid[in0y][in0x].item != NONE)
    src_ok[0] = 1;
  if (in_bounds(in1x, in1y) && belt_points_to(in1x, in1y, bx, by) &&
      grid[in1y][in1x].item != NONE)
    src_ok[1] = 1;

  if (in_bounds(out0x, out0y) && belt_points_away_from(out0x, out0y, ax, ay) &&
      grid[out0y][out0x].item == NONE)
    dst_ok[0] = 1;
  if (in_bounds(out1x, out1y) && belt_points_away_from(out1x, out1y, bx, by) &&
      grid[out1y][out1x].item == NONE)
    dst_ok[1] = 1;

  int src_count = src_ok[0] + src_ok[1];
  int dst_count = dst_ok[0] + dst_ok[1];
  if (src_count == 0 || dst_count == 0)
    return;

  // 1 -> 2 split mode, alternating output side.
  if (src_count == 1 && dst_count >= 1) {
    int s = src_ok[0] ? 0 : 1;
    int d = 0;
    if (dst_count == 1) {
      d = dst_ok[0] ? 0 : 1;
      move_splitter_item(anchor, s == 0 ? in0x : in1x, s == 0 ? in0y : in1y,
                         d == 0 ? out0x : out1x, d == 0 ? out0y : out1y, 0, 0);
    } else {
      d = anchor->split_toggle_out ? 1 : 0;
      if (!dst_ok[d])
        d ^= 1;
      move_splitter_item(anchor, s == 0 ? in0x : in1x, s == 0 ? in0y : in1y,
                         d == 0 ? out0x : out1x, d == 0 ? out0y : out1y, 1, 0);
    }
    return;
  }

  // 2 -> 1 merge mode, alternating input side.
  if (src_count >= 1 && dst_count == 1) {
    int d = dst_ok[0] ? 0 : 1;
    int s = 0;
    if (src_count == 1) {
      s = src_ok[0] ? 0 : 1;
      move_splitter_item(anchor, s == 0 ? in0x : in1x, s == 0 ? in0y : in1y,
                         d == 0 ? out0x : out1x, d == 0 ? out0y : out1y, 0, 0);
    } else {
      s = anchor->split_toggle_in ? 1 : 0;
      if (!src_ok[s])
        s ^= 1;
      move_splitter_item(anchor, s == 0 ? in0x : in1x, s == 0 ? in0y : in1y,
                         d == 0 ? out0x : out1x, d == 0 ? out0y : out1y, 0, 1);
    }
    return;
  }

  // Fallback passthrough when multiple paths are valid.
  if (src_ok[0] && dst_ok[0]) {
    move_splitter_item(anchor, in0x, in0y, out0x, out0y, 0, 0);
  } else if (src_ok[1] && dst_ok[1]) {
    move_splitter_item(anchor, in1x, in1y, out1x, out1y, 0, 0);
  }
}

void process_tick() {
  for (int y = 0; y < GRID_H; y++)
    for (int x = 0; x < GRID_W; x++)
      grid[y][x].handled = 0;

  // 1. Miners output ore.
  if (tick_counter % 5 == 0) {
    for (int y = 0; y < GRID_H; y++) {
      for (int x = 0; x < GRID_W; x++) {
        if (grid[y][x].building == MINER && grid[y][x].resource_amount > 0) {
          ItemType resource = terrain_to_item(grid[y][x].terrain);
          if (resource != NONE && push_item_to_belt(x, y, resource)) {
            grid[y][x].resource_amount--;
            if (grid[y][x].resource_amount == 0)
              grid[y][x].terrain = TERR_EMPTY;
          }
        }
      }
    }
  }

  // 2. Furnaces and Crafters push finished outputs.
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      if ((grid[y][x].building == FURNACE || grid[y][x].building == CRAFTER) &&
          grid[y][x].item != NONE) {
        if (push_item_to_belt(x, y, grid[y][x].item))
          grid[y][x].item = NONE;
      }
    }
  }

  // 3a. Belts push into machine inputs / sinks.
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      struct Cell *current = &grid[y][x];
      if (current->building != BELT || current->item == NONE ||
          current->handled)
        continue;

      int tx = x + dir_dx(current->rotation);
      int ty = y + dir_dy(current->rotation);
      if (!in_bounds(tx, ty))
        continue;

      struct Cell *target = &grid[ty][tx];

      if (target->building == CRAFTER) {
        if (current->item == IRON_PLATE && target->inv_iron < 10) {
          target->inv_iron++;
          current->item = NONE;
        } else if (current->item == COPPER_PLATE && target->inv_copper < 10) {
          target->inv_copper++;
          current->item = NONE;
        } else if (current->item == GEAR && target->inv_gear < 10) {
          target->inv_gear++;
          current->item = NONE;
        }
      } else if (target->building == LAB) {
        if (current->item == MACHINE_PART) {
          score++;
          current->item = NONE;
        }
      } else if (target->building == FURNACE && target->item == NONE) {
        if (current->item == IRON_ORE || current->item == COPPER_ORE) {
          target->item = current->item;
          current->item = NONE;
        }
      }
    }
  }

  // 3b. Pull-style belt->belt transfer avoids corner gaps by allowing a belt to
  // refill in the same tick after it empties.
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      struct Cell *target = &grid[y][x];
      if (target->building != BELT || target->item != NONE || target->handled)
        continue;

      int sx = x - dir_dx(target->rotation);
      int sy = y - dir_dy(target->rotation);
      uint8_t incoming_pref[4] = {
          (uint8_t)((target->rotation + 2) & 3), left_dir(target->rotation),
          right_dir(target->rotation), target->rotation};

      for (int k = 0; k < 4; k++) {
        uint8_t in_dir = incoming_pref[k];
        sx = x + dir_dx(in_dir);
        sy = y + dir_dy(in_dir);
        if (!in_bounds(sx, sy))
          continue;

        struct Cell *source = &grid[sy][sx];
        if (source->building != BELT || source->item == NONE || source->handled)
          continue;
        if (!belt_points_to(sx, sy, x, y))
          continue;

        target->item = source->item;
        target->handled = 1;
        source->item = NONE;
        break;
      }
    }
  }

  // 4. Splitter transfers.
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      if (grid[y][x].building == SPLITTER && grid[y][x].split_anchor)
        process_splitter_anchor(x, y);
    }
  }

  // 5. Furnaces smelt.
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      if (grid[y][x].building == FURNACE) {
        if (grid[y][x].item == IRON_ORE)
          grid[y][x].item = IRON_PLATE;
        else if (grid[y][x].item == COPPER_ORE)
          grid[y][x].item = COPPER_PLATE;
        else if (grid[y][x].item == NONE && grid[y][x].furnace_ore_count > 0 &&
                 grid[y][x].furnace_fuel_count > 0) {
          ItemType output =
              furnace_output_for((ItemType)grid[y][x].furnace_ore_type);
          if (output != NONE) {
            grid[y][x].item = output;
            grid[y][x].furnace_ore_count--;
            grid[y][x].furnace_fuel_count--;
            if (grid[y][x].furnace_ore_count == 0)
              grid[y][x].furnace_ore_type = NONE;
          }
        }
      }
    }
  }

  // 6. Crafters craft: Iron -> Gear, Gear + Copper Plate -> Machine Part.
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      if (grid[y][x].building != CRAFTER || grid[y][x].item != NONE)
        continue;

      if (grid[y][x].inv_gear >= 1 && grid[y][x].inv_copper >= 1) {
        grid[y][x].inv_gear--;
        grid[y][x].inv_copper--;
        grid[y][x].item = MACHINE_PART;
      } else if (grid[y][x].inv_iron >= 1) {
        grid[y][x].inv_iron--;
        grid[y][x].item = GEAR;
      }
    }
  }
}

static void buffer_put_text(uint16_t *buffer, int x, int y, const char *str,
                            uint8_t fg, uint8_t bg) {
  int i = 0;
  while (str[i] != '\0' && x + i < GRID_W && y >= 0 && y < GRID_H) {
    buffer[y * GRID_W + x + i] = (bg << 12) | (fg << 8) | str[i];
    i++;
  }
}

static void buffer_put_char(uint16_t *buffer, int x, int y, char ch, uint8_t fg,
                            uint8_t bg) {
  if (!in_bounds(x, y))
    return;
  buffer[y * GRID_W + x] = (bg << 12) | (fg << 8) | (uint8_t)ch;
}

static void buffer_put_stack(uint16_t *buffer, int x, int y,
                             const struct ItemStack *stack, int selected) {
  uint8_t bg = selected ? COL_BLUE : COL_BLACK;
  uint8_t fg = selected ? COL_WHITE : COL_LIGHT_GREY;
  buffer_put_char(buffer, x, y,
                  stack_is_empty(stack) ? '.' : item_glyph(stack->item), fg,
                  bg);
  buffer_put_char(buffer, x + 1, y, ' ', fg, bg);
  uint8_t count = stack_is_empty(stack) ? 0 : stack->count;
  if (count > 99)
    count = 99;
  buffer_put_char(buffer, x + 2, y, (char)('0' + (count / 10)), fg, bg);
  buffer_put_char(buffer, x + 3, y, (char)('0' + (count % 10)), fg, bg);
}

static void draw_hotbar(uint16_t *buffer) {
  const int start_x = 2;
  const int y = 0;
  for (int i = 0; i < HOTBAR_SLOTS; i++) {
    int x = start_x + i * 8;
    uint8_t bg = i == player.selected_hotbar ? COL_BLUE : COL_BLACK;
    uint8_t fg = i == player.selected_hotbar ? COL_WHITE : COL_YELLOW;
    buffer_put_char(buffer, x, y, (char)('1' + i), fg, bg);
    buffer_put_char(buffer, x + 1, y, ':', fg, bg);
    if (stack_is_empty(&player.hotbar[i])) {
      buffer_put_char(buffer, x + 2, y, '.', COL_LIGHT_GREY, bg);
      buffer_put_char(buffer, x + 3, y, '.', COL_LIGHT_GREY, bg);
      buffer_put_char(buffer, x + 4, y, '.', COL_LIGHT_GREY, bg);
      buffer_put_char(buffer, x + 5, y, '.', COL_LIGHT_GREY, bg);
    } else {
      buffer_put_char(buffer, x + 2, y, item_glyph(player.hotbar[i].item),
                      item_color(player.hotbar[i].item), bg);
      uint8_t count = player.hotbar[i].count;
      if (count > 99)
        count = 99;
      buffer_put_char(buffer, x + 3, y, (char)('0' + (count / 10)), fg, bg);
      buffer_put_char(buffer, x + 4, y, (char)('0' + (count % 10)), fg, bg);
      buffer_put_char(buffer, x + 5, y, ' ', fg, bg);
    }
  }
}

void place_selected() { place_from_hotbar(); }

static void move_inventory_focus(int dx, int dy) {
  int row = player.main_focus / 5;
  int col = player.main_focus % 5;
  row = (row + dy + 5) % 5;
  col = (col + dx + 5) % 5;
  player.main_focus = (uint8_t)(row * 5 + col);
}

static void handle_inventory_input(uint8_t scancode) {
  if (scancode == 0x0F) {
    current_state = MODE_GAME;
    return;
  }

  if (scancode == 0x12) {
    player.active_panel = (player.active_panel == INVENTORY_PANEL_MAIN)
                              ? INVENTORY_PANEL_CRAFT
                              : INVENTORY_PANEL_MAIN;
    return;
  }

  if (player.active_panel == INVENTORY_PANEL_MAIN) {
    if (scancode == 0x11)
      move_inventory_focus(0, -1);
    else if (scancode == 0x1F)
      move_inventory_focus(0, 1);
    else if (scancode == 0x1E)
      move_inventory_focus(-1, 0);
    else if (scancode == 0x20)
      move_inventory_focus(1, 0);
  }

  if (player.active_panel == INVENTORY_PANEL_CRAFT) {
    if (scancode == 0x11 && player.recipe_focus > 0) {
      player.recipe_focus--;
      return;
    }
    if (scancode == 0x1F && player.recipe_focus + 1 < g_recipe_count) {
      player.recipe_focus++;
      return;
    }
    if (scancode == 0x1C) {
      if (craft_recipe(player.recipe_focus))
        status_set("IO: crafted");
      else
        status_set("IO: missing items");
      return;
    }
  }

  int hotbar_slot = hotbar_slot_from_scancode(scancode);
  if (hotbar_slot >= 0) {
    if (player.active_panel == INVENTORY_PANEL_MAIN) {
      transfer_main_and_hotbar(player.main_focus, hotbar_slot);
      player.selected_hotbar = (uint8_t)hotbar_slot;
    }
    return;
  }
}

static void handle_machine_input(uint8_t scancode) {
  if (scancode == 0x0F) {
    current_state = MODE_GAME;
    return;
  }

  if (!in_bounds(machine_x, machine_y) ||
      grid[machine_y][machine_x].building != FURNACE) {
    current_state = MODE_GAME;
    return;
  }

  struct Cell *machine = &grid[machine_y][machine_x];

  if (scancode == 0x12) {
    machine_panel ^= 1;
    return;
  }

  if (machine_panel == 0) {
    if (scancode == 0x11)
      move_inventory_focus(0, -1);
    else if (scancode == 0x1F)
      move_inventory_focus(0, 1);
    else if (scancode == 0x1E)
      move_inventory_focus(-1, 0);
    else if (scancode == 0x20)
      move_inventory_focus(1, 0);
  } else {
    if (scancode == 0x11 && machine_focus > 0)
      machine_focus--;
    if (scancode == 0x1F && machine_focus < 2)
      machine_focus++;
  }

  int hotbar_slot = hotbar_slot_from_scancode(scancode);
  if (hotbar_slot >= 0 && machine_panel == 0) {
    transfer_main_and_hotbar(player.main_focus, hotbar_slot);
    player.selected_hotbar = (uint8_t)hotbar_slot;
    return;
  }

  if (scancode != 0x1C)
    return;

  if (machine_panel == 0) {
    struct ItemStack *focused = &player.main[player.main_focus];
    if (stack_is_empty(focused))
      return;

    if (is_furnace_ore(focused->item)) {
      if (machine->furnace_ore_count == 0 ||
          machine->furnace_ore_type == focused->item) {
        if (machine->furnace_ore_count < 255) {
          machine->furnace_ore_type = focused->item;
          machine->furnace_ore_count++;
          focused->count--;
          if (focused->count == 0)
            stack_clear(focused);
        }
      }
      return;
    }

    if (focused->item == COAL) {
      if (machine->furnace_fuel_count < 255) {
        machine->furnace_fuel_count++;
        focused->count--;
        if (focused->count == 0)
          stack_clear(focused);
      }
      return;
    }
    return;
  }

  if (machine_focus == 0) {
    if (machine->furnace_ore_count > 0 &&
        add_item_to_main_inventory((ItemType)machine->furnace_ore_type, 1)) {
      machine->furnace_ore_count--;
      if (machine->furnace_ore_count == 0)
        machine->furnace_ore_type = NONE;
    }
    return;
  }

  if (machine_focus == 1) {
    if (machine->furnace_fuel_count > 0 && add_item_to_main_inventory(COAL, 1))
      machine->furnace_fuel_count--;
    return;
  }

  if (machine_focus == 2) {
    if (machine->item != NONE && add_item_to_main_inventory(machine->item, 1))
      machine->item = NONE;
  }
}

static void handle_game_input(uint8_t scancode) {
  if (scancode == 0x0F) {
    current_state = MODE_INVENTORY;
    player.active_panel = INVENTORY_PANEL_MAIN;
    return;
  }

  if (scancode == 0x1D) {
    ctrl_held = 1;
    return;
  }
  if (scancode == 0x9D) {
    ctrl_held = 0;
    return;
  }

  if (scancode == 0x11 && cursor_y > 1)
    cursor_y--;
  if (scancode == 0x1F && !ctrl_held && cursor_y < GRID_H - 1)
    cursor_y++;
  if (scancode == 0x1E && cursor_x > 0)
    cursor_x--;
  if (scancode == 0x20 && cursor_x < GRID_W - 1)
    cursor_x++;

  int hotbar_slot = hotbar_slot_from_scancode(scancode);
  if (hotbar_slot >= 0) {
    player.selected_hotbar = (uint8_t)hotbar_slot;
    return;
  }

  if (scancode == 0x13) {
    selected_rotation = (uint8_t)((selected_rotation + 1) & 3);
    return;
  }

  if (scancode == 0x12) {
    if (mine_resource_at(cursor_x, cursor_y))
      status_set("IO: mined");
    else
      status_set("IO: nothing to mine");
    return;
  }

  if (scancode == 0x39) {
    if (place_from_hotbar())
      status_set("IO: placed");
    else
      status_set("IO: cannot place");
    return;
  }

  if (scancode == 0x1C) {
    if (grid[cursor_y][cursor_x].building == FURNACE) {
      machine_x = cursor_x;
      machine_y = cursor_y;
      machine_panel = 0;
      machine_focus = 0;
      current_state = MODE_MACHINE;
    }
    return;
  }

  if (scancode == 0x1F && ctrl_held) {
    save_map_to_serial();
    return;
  }

  if (scancode == 0x26) {
    load_map_from_serial();
    return;
  }
}

void handle_input() {
  uint8_t scancode = get_scancode();

  if (scancode == 0)
    return;

  if (scancode == 0x01) {
    should_exit_app = 1;
    return;
  }

  if (scancode == 0x1D) {
    ctrl_held = 1;
    return;
  }
  if (scancode == 0x9D) {
    ctrl_held = 0;
    return;
  }

  if (scancode & 0x80)
    return;

  if (current_state == MODE_INVENTORY) {
    handle_inventory_input(scancode);
  } else if (current_state == MODE_MACHINE) {
    handle_machine_input(scancode);
  } else if (current_state == MODE_GAME) {
    handle_game_input(scancode);
  }
}

// --- Rendering ---
static void render_gameplay(uint16_t *buffer) {
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      struct Cell *cell = &grid[y][x];
      uint8_t ch = ' ';
      uint8_t fg = COL_WHITE;
      uint8_t bg = COL_BLACK;

      // Draw Terrain layer if empty.
      if (cell->building == EMPTY && cell->item == NONE) {
        if (cell->terrain == TERR_IRON) {
          ch = '^';
          fg = COL_DARK_GREY;
        } else if (cell->terrain == TERR_COPPER) {
          ch = '~';
          fg = COL_BROWN;
        } else if (cell->terrain == TERR_COAL) {
          ch = '#';
          fg = COL_DARK_GREY;
        } else if (cell->terrain == TERR_STONE) {
          ch = ':';
          fg = COL_DARK_GREY;
        }
      }

      // Draw Building.
      switch (cell->building) {
      case BELT:
        ch = (uint8_t)dir_char(cell->rotation);
        fg = COL_CYAN;
        break;
      case SPLITTER:
        ch = (uint8_t)dir_char(cell->rotation);
        fg = COL_GREEN;
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

      // Draw Item on transport entities.
      if (cell->building == BELT || cell->building == EMPTY) {
        if (cell->item != NONE) {
          ch = item_glyph(cell->item);
          fg = item_color(cell->item);
        }
      }

      // Draw Cursor.
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

  draw_hotbar(buffer);

  const char *sel = "EMPTY";
  if (!stack_is_empty(&player.hotbar[player.selected_hotbar])) {
    sel = item_name(player.hotbar[player.selected_hotbar].item);
  }
  buffer_put_text(buffer, 2, GRID_H - 1, "Selected:", COL_YELLOW, COL_BLUE);
  buffer_put_text(buffer, 12, GRID_H - 1, sel, COL_WHITE, COL_BLUE);

  // Draw Score.
  const char *s_txt = " Score: ";
  int sx = 50;
  buffer_put_text(buffer, sx, GRID_H - 1, s_txt, COL_YELLOW, COL_BLUE);
  sx += 8;

  char score_buf[16];
  int v = (int)score, dig = 0;
  if (v == 0)
    score_buf[dig++] = '0';
  while (v > 0) {
    score_buf[dig++] = (char)('0' + (v % 10));
    v /= 10;
  }
  for (int j = 0; j < dig / 2; j++) {
    char t = score_buf[j];
    score_buf[j] = score_buf[dig - 1 - j];
    score_buf[dig - 1 - j] = t;
  }
  score_buf[dig] = '\0';

  buffer_put_text(buffer, sx, GRID_H - 1, score_buf, COL_WHITE, COL_BLUE);
  buffer_put_text(buffer, 37, GRID_H - 1, io_status, COL_LIGHT_GREY, COL_BLUE);
}

static void render_inventory_screen(uint16_t *buffer) {
  for (int i = 0; i < GRID_W * GRID_H; i++)
    buffer[i] = (COL_BLACK << 12) | (COL_WHITE << 8) | ' ';

  buffer_put_text(buffer, 2, 1, "INVENTORY", COL_YELLOW, COL_BLACK);
  buffer_put_text(buffer, 2, 2,
                  "WASD move  E swap panel  1-9 send to hotbar  TAB close",
                  COL_LIGHT_GREY, COL_BLACK);
  buffer_put_text(buffer, 44, 1, "CRAFTING", COL_GREEN, COL_BLACK);
  buffer_put_text(buffer, 44, 2, "W/S select  ENTER craft  E back",
                  COL_LIGHT_GREY, COL_BLACK);

  for (int i = 0; i < MAIN_INV_SLOTS; i++) {
    int row = i / 5;
    int col = i % 5;
    int x = 2 + col * 8;
    int y = 4 + row * 2;
    uint8_t bg =
        (player.active_panel == INVENTORY_PANEL_MAIN && player.main_focus == i)
            ? COL_BLUE
            : COL_BLACK;
    buffer_put_char(
        buffer, x, y,
        stack_is_empty(&player.main[i]) ? '.' : item_glyph(player.main[i].item),
        stack_is_empty(&player.main[i]) ? COL_DARK_GREY
                                        : item_color(player.main[i].item),
        bg);
    buffer_put_char(buffer, x + 1, y, ' ', COL_WHITE, bg);
    uint8_t count = stack_is_empty(&player.main[i]) ? 0 : player.main[i].count;
    if (count > 99)
      count = 99;
    buffer_put_char(buffer, x + 2, y, (char)('0' + (count / 10)), COL_WHITE,
                    bg);
    buffer_put_char(buffer, x + 3, y, (char)('0' + (count % 10)), COL_WHITE,
                    bg);
    buffer_put_text(buffer, x, y + 1, "    ", COL_LIGHT_GREY, bg);
  }

  for (int i = 0; i < g_recipe_count; i++) {
    int y = 4 + i;
    uint8_t bg = (player.active_panel == INVENTORY_PANEL_CRAFT &&
                  player.recipe_focus == i)
                     ? COL_BLUE
                     : COL_BLACK;
    uint8_t fg = recipe_can_craft(i) ? COL_WHITE : COL_DARK_GREY;
    buffer_put_char(buffer, 44, y,
                    (player.active_panel == INVENTORY_PANEL_CRAFT &&
                     player.recipe_focus == i)
                        ? '>'
                        : ' ',
                    COL_YELLOW, bg);
    buffer_put_text(buffer, 46, y, g_recipes[i].name, fg, bg);
  }

  buffer_put_text(buffer, 2, 16, "Hotbar:", COL_CYAN, COL_BLACK);
  for (int i = 0; i < HOTBAR_SLOTS; i++) {
    int x = 2 + i * 8;
    buffer_put_stack(buffer, x, 18, &player.hotbar[i],
                     i == player.selected_hotbar);
  }
  if (player.active_panel == INVENTORY_PANEL_CRAFT) {
    const Recipe *recipe = &g_recipes[player.recipe_focus];
    buffer_put_text(buffer, 2, 21, "Recipe:", COL_YELLOW, COL_BLACK);
    buffer_put_text(buffer, 10, 21, recipe->name, COL_WHITE, COL_BLACK);
    buffer_put_text(buffer, 2, 22, "Need:", COL_YELLOW, COL_BLACK);
    int req_x = 8;
    for (int i = 0; i < recipe->ingredient_count; i++) {
      uint8_t cnt = recipe->ingredients[i].count;
      if (cnt > 99)
        cnt = 99;
      buffer_put_char(buffer, req_x, 22, (char)('0' + (cnt / 10)), COL_WHITE,
                      COL_BLACK);
      buffer_put_char(buffer, req_x + 1, 22, (char)('0' + (cnt % 10)),
                      COL_WHITE, COL_BLACK);
      buffer_put_char(buffer, req_x + 2, 22, 'x', COL_LIGHT_GREY, COL_BLACK);
      buffer_put_text(buffer, req_x + 4, 22,
                      item_name(recipe->ingredients[i].item),
                      item_color(recipe->ingredients[i].item), COL_BLACK);
      req_x += 18;
      if (req_x > 74)
        break;
    }
    buffer_put_text(buffer, 2, 23, "Enter crafts selected recipe.", COL_WHITE,
                    COL_BLACK);
  } else {
    buffer_put_text(buffer, 2, 21, "Selected item:", COL_YELLOW, COL_BLACK);
    if (!stack_is_empty(&player.main[player.main_focus])) {
      buffer_put_text(
          buffer, 17, 21, item_name(player.main[player.main_focus].item),
          item_color(player.main[player.main_focus].item), COL_BLACK);
    } else if (!stack_is_empty(&player.hotbar[player.selected_hotbar])) {
      buffer_put_text(
          buffer, 17, 21, item_name(player.hotbar[player.selected_hotbar].item),
          item_color(player.hotbar[player.selected_hotbar].item), COL_BLACK);
    } else {
      buffer_put_text(buffer, 17, 21, "Empty", COL_DARK_GREY, COL_BLACK);
    }
    buffer_put_text(buffer, 2, 22, "Enter crafts the selected recipe.",
                    COL_WHITE, COL_BLACK);
    buffer_put_text(buffer, 2, 23, "Tab returns to the game.", COL_WHITE,
                    COL_BLACK);
  }
}

static void render_machine_screen(uint16_t *buffer) {
  for (int i = 0; i < GRID_W * GRID_H; i++)
    buffer[i] = (COL_BLACK << 12) | (COL_WHITE << 8) | ' ';

  buffer_put_text(buffer, 2, 1, "MACHINE - FURNACE", COL_YELLOW, COL_BLACK);
  buffer_put_text(buffer, 2, 2,
                  "WASD move inv  1-9 swap hotbar  E swap panel  TAB close",
                  COL_LIGHT_GREY, COL_BLACK);

  for (int i = 0; i < MAIN_INV_SLOTS; i++) {
    int row = i / 5;
    int col = i % 5;
    int x = 2 + col * 8;
    int y = 4 + row * 2;
    uint8_t bg =
        (machine_panel == 0 && player.main_focus == i) ? COL_BLUE : COL_BLACK;
    buffer_put_char(
        buffer, x, y,
        stack_is_empty(&player.main[i]) ? '.' : item_glyph(player.main[i].item),
        stack_is_empty(&player.main[i]) ? COL_DARK_GREY
                                        : item_color(player.main[i].item),
        bg);
    buffer_put_char(buffer, x + 1, y, ' ', COL_WHITE, bg);
    uint8_t count = stack_is_empty(&player.main[i]) ? 0 : player.main[i].count;
    if (count > 99)
      count = 99;
    buffer_put_char(buffer, x + 2, y, (char)('0' + (count / 10)), COL_WHITE,
                    bg);
    buffer_put_char(buffer, x + 3, y, (char)('0' + (count % 10)), COL_WHITE,
                    bg);
  }

  struct Cell *m = (in_bounds(machine_x, machine_y) &&
                    grid[machine_y][machine_x].building == FURNACE)
                       ? &grid[machine_y][machine_x]
                       : 0;

  buffer_put_text(buffer, 44, 1, "FURNACE", COL_RED, COL_BLACK);
  buffer_put_text(buffer, 44, 2, "Enter: move in/out", COL_LIGHT_GREY,
                  COL_BLACK);

  uint8_t bg0 =
      (machine_panel == 1 && machine_focus == 0) ? COL_BLUE : COL_BLACK;
  uint8_t bg1 =
      (machine_panel == 1 && machine_focus == 1) ? COL_BLUE : COL_BLACK;
  uint8_t bg2 =
      (machine_panel == 1 && machine_focus == 2) ? COL_BLUE : COL_BLACK;

  buffer_put_text(buffer, 44, 5, "Ore :", COL_WHITE, bg0);
  if (m && m->furnace_ore_count > 0) {
    buffer_put_char(buffer, 50, 5, item_glyph((ItemType)m->furnace_ore_type),
                    item_color((ItemType)m->furnace_ore_type), bg0);
    buffer_put_char(buffer, 52, 5,
                    (char)('0' + ((m->furnace_ore_count / 10) % 10)), COL_WHITE,
                    bg0);
    buffer_put_char(buffer, 53, 5, (char)('0' + (m->furnace_ore_count % 10)),
                    COL_WHITE, bg0);
  } else {
    buffer_put_text(buffer, 50, 5, "--", COL_DARK_GREY, bg0);
  }

  buffer_put_text(buffer, 44, 7, "Fuel:", COL_WHITE, bg1);
  if (m && m->furnace_fuel_count > 0) {
    buffer_put_char(buffer, 50, 7, item_glyph(COAL), item_color(COAL), bg1);
    buffer_put_char(buffer, 52, 7,
                    (char)('0' + ((m->furnace_fuel_count / 10) % 10)),
                    COL_WHITE, bg1);
    buffer_put_char(buffer, 53, 7, (char)('0' + (m->furnace_fuel_count % 10)),
                    COL_WHITE, bg1);
  } else {
    buffer_put_text(buffer, 50, 7, "--", COL_DARK_GREY, bg1);
  }

  buffer_put_text(buffer, 44, 9, "Out :", COL_WHITE, bg2);
  if (m && m->item != NONE) {
    buffer_put_char(buffer, 50, 9, item_glyph(m->item), item_color(m->item),
                    bg2);
  } else {
    buffer_put_text(buffer, 50, 9, "--", COL_DARK_GREY, bg2);
  }

  buffer_put_text(buffer, 2, 21, "Selected item:", COL_YELLOW, COL_BLACK);
  if (!stack_is_empty(&player.main[player.main_focus])) {
    buffer_put_text(buffer, 17, 21,
                    item_name(player.main[player.main_focus].item),
                    item_color(player.main[player.main_focus].item), COL_BLACK);
  } else {
    buffer_put_text(buffer, 17, 21, "Empty", COL_DARK_GREY, COL_BLACK);
  }
  buffer_put_text(buffer, 2, 23, "TAB closes machine view.", COL_WHITE,
                  COL_BLACK);
}

void render() {
  uint16_t buffer[GRID_H * GRID_W];
  if (current_state == MODE_INVENTORY) {
    render_inventory_screen(buffer);
  } else if (current_state == MODE_MACHINE) {
    render_machine_screen(buffer);
  } else {
    render_gameplay(buffer);
  }

  for (int i = 0; i < GRID_W * GRID_H; i++)
    VGA_MEM[i] = buffer[i];
}

void render_title() {
  for (int i = 0; i < GRID_W * GRID_H; i++)
    VGA_MEM[i] = (COL_BLACK << 12) | (COL_WHITE << 8) | ' ';

  print_text(27, 3, "=== MINI-FACTORIO ===", COL_YELLOW, COL_BLACK);
  print_text(10, 6, "Gameplay:", COL_CYAN, COL_BLACK);
  print_text(10, 7, "WASD - Move cursor", COL_WHITE, COL_BLACK);
  print_text(10, 8, "1-9  - Select hotbar slot", COL_WHITE, COL_BLACK);
  print_text(10, 9, "SPACE - Place selected hotbar item", COL_WHITE, COL_BLACK);
  print_text(10, 10, "E    - Manual mine resource under cursor", COL_WHITE,
             COL_BLACK);
  print_text(10, 11, "R    - Rotate placeable items", COL_WHITE, COL_BLACK);
  print_text(10, 12, "TAB  - Open inventory / crafting", COL_WHITE, COL_BLACK);
  print_text(10, 13, "CTRL+S - Save map", COL_WHITE, COL_BLACK);
  print_text(10, 14, "L    - Load map", COL_WHITE, COL_BLACK);
  print_text(10, 16, "Inventory:", COL_GREEN, COL_BLACK);
  print_text(10, 17, "Left 5x5 grid auto-sorts by stack size", COL_WHITE,
             COL_BLACK);
  print_text(10, 18, "1-9 moves focused stack to the hotbar", COL_WHITE,
             COL_BLACK);
  print_text(10, 19, "Right panel lists craftable recipes", COL_WHITE,
             COL_BLACK);
  print_text(10, 20, "Enter crafts one item at a time", COL_WHITE, COL_BLACK);
  print_text(22, 23, "Press SPACE to start", COL_GREEN, COL_BLACK);
}

void handle_title_input() {
  uint8_t scancode = get_scancode();
  if (scancode == 0 || (scancode & 0x80))
    return;
  if (scancode == 0x01) {
    should_exit_app = 1;
    return;
  }
  if (scancode == 0x39) {
    current_state = MODE_GAME;
    start_player_inventory();
    for (int i = 0; i < GRID_W * GRID_H; i++)
      VGA_MEM[i] = 0;
  }
}

__attribute__((section(".text.entry"))) void entry() {
  current_state = MODE_TITLE;
  cursor_x = 40;
  cursor_y = 12;
  selected_rotation = DIR_RIGHT;
  tick_counter = 0;
  score = 0;
  ctrl_held = 0;
  should_exit_app = 0;
  status_set("IO: idle");

  init_grid();
  inventory_clear();
  while (inb(0x64) & 1)
    inb(0x60);

  uint32_t loop_counter = 0;
  render_title();

  while (1) {
    if (current_state == MODE_TITLE) {
      handle_title_input();
    } else if (current_state == MODE_INVENTORY) {
      handle_input();
      render();
    } else {
      handle_input();
      if (loop_counter++ > 500) {
        process_tick();
        tick_counter++;
        loop_counter = 0;
      }
      render();
    }

    if (should_exit_app) {
      return;
    }
  }
}