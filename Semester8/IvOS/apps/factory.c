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

typedef enum { TERR_EMPTY = 0, TERR_IRON, TERR_COPPER } TerrainType;

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
  IRON_PLATE,
  COPPER_PLATE,
  GEAR,
  MACHINE_PART
} ItemType;

typedef enum { DIR_UP = 0, DIR_RIGHT = 1, DIR_DOWN = 2, DIR_LEFT = 3 } Dir;

struct Cell {
  EntityType building;
  ItemType item;
  TerrainType terrain;
  uint8_t handled;

  // Generic per-cell orientation for rotatable entities.
  uint8_t rotation;

  // Crafter inventory.
  uint8_t inv_iron;
  uint8_t inv_copper;
  uint8_t inv_gear;

  // Splitter bookkeeping.
  uint8_t split_anchor;
  uint8_t split_ax;
  uint8_t split_ay;
  uint8_t split_toggle_out;
  uint8_t split_toggle_in;
};

void clear_entity_at(int x, int y);
int place_splitter(int ax, int ay, uint8_t rot);
char hex_digit(uint8_t v);

// --- Global State ---
struct Cell grid[GRID_H][GRID_W];
int cursor_x = 40;
int cursor_y = 12;
EntityType selected_building = BELT;
uint8_t selected_rotation = DIR_RIGHT;
uint32_t tick_counter = 0;
int current_state = 0;
uint32_t score = 0;
uint8_t ctrl_held = 0;
char io_status[22] = "IO: idle             ";
uint8_t should_exit_app = 0;

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

void save_map_to_serial() {
  serial_print("MF1\n");
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      uint8_t b = (uint8_t)grid[y][x].building;
      uint8_t r = grid[y][x].rotation & 0x0F;
      uint8_t f = 0;
      if (grid[y][x].building == SPLITTER && grid[y][x].split_anchor)
        f = 1;

      serial_putc(hex_digit(b & 0x0F));
      serial_putc(hex_digit(r));
      serial_putc(hex_digit(f));
    }
    serial_putc('\n');
  }
  serial_print("END\n");
  status_set("IO: map saved");
}

int load_map_from_serial() {
  char line[256];

  read_serial_sized(line, 3);
  if (!(line[0] == 'M' && line[1] == 'F' && line[2] == '1' &&
        line[3] == '\0')) {
    status_set("IO: bad header");
    return 0;
  }

  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      clear_entity_at(x, y);
    }
  }

  for (int y = 0; y < GRID_H; y++) {
    int len = read_serial_sized(line, GRID_W * 3);
    if (len != GRID_W * 3) {
      status_set("IO: short row");
      return 0;
    }

    for (int x = 0; x < GRID_W; x++) {
      int bi = hex_value(line[x * 3]);
      int ri = hex_value(line[x * 3 + 1]);
      int fi = hex_value(line[x * 3 + 2]);
      if (bi < 0 || ri < 0 || fi < 0) {
        status_set("IO: bad data");
        return 0;
      }

      EntityType b = (EntityType)bi;
      uint8_t r = (uint8_t)(ri & 3);
      if (b == EMPTY)
        continue;

      if (b == SPLITTER) {
        if ((fi & 1) != 0)
          place_splitter(x, y, r);
      } else {
        grid[y][x].building = b;
        grid[y][x].rotation = r;
        grid[y][x].item = NONE;
        grid[y][x].inv_iron = 0;
        grid[y][x].inv_copper = 0;
        grid[y][x].inv_gear = 0;
      }
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
  grid[y][x].inv_iron = 0;
  grid[y][x].inv_copper = 0;
  grid[y][x].inv_gear = 0;
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
        if (grid[y][x].building == MINER) {
          if (grid[y][x].terrain == TERR_IRON)
            push_item_to_belt(x, y, IRON_ORE);
          if (grid[y][x].terrain == TERR_COPPER)
            push_item_to_belt(x, y, COPPER_ORE);
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

// --- Input Handling ---
void place_selected() {
  if (!in_bounds(cursor_x, cursor_y))
    return;

  if (selected_building == EMPTY) {
    clear_entity_at(cursor_x, cursor_y);
    return;
  }

  if (selected_building == SPLITTER) {
    place_splitter(cursor_x, cursor_y, selected_rotation);
    return;
  }

  clear_entity_at(cursor_x, cursor_y);
  grid[cursor_y][cursor_x].building = selected_building;
  grid[cursor_y][cursor_x].item = NONE;
  grid[cursor_y][cursor_x].rotation =
      is_rotatable(selected_building) ? selected_rotation : DIR_RIGHT;
  grid[cursor_y][cursor_x].inv_iron = 0;
  grid[cursor_y][cursor_x].inv_copper = 0;
  grid[cursor_y][cursor_x].inv_gear = 0;
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

  if (scancode == 0x11 && cursor_y > 1)
    cursor_y--; // W
  if (scancode == 0x1F && !ctrl_held && cursor_y < GRID_H - 1)
    cursor_y++; // S
  if (scancode == 0x1E && cursor_x > 0)
    cursor_x--; // A
  if (scancode == 0x20 && cursor_x < GRID_W - 1)
    cursor_x++; // D

  if (scancode == 0x02)
    selected_building = BELT; // 1
  if (scancode == 0x03)
    selected_building = SPLITTER; // 2
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

  if (scancode == 0x1F && ctrl_held) {
    save_map_to_serial(); // Ctrl+S
  }
  if (scancode == 0x26) {
    load_map_from_serial(); // L
  }

  if (scancode == 0x13 && is_rotatable(selected_building))
    selected_rotation = (uint8_t)((selected_rotation + 1) & 3); // R

  if (scancode == 0x39)
    place_selected(); // SPACE
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

      // Draw Terrain layer if empty.
      if (cell->building == EMPTY && cell->item == NONE) {
        if (cell->terrain == TERR_IRON) {
          ch = '.';
          fg = COL_DARK_GREY;
        } else if (cell->terrain == TERR_COPPER) {
          ch = '.';
          fg = COL_BROWN;
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
        } else if (cell->item == MACHINE_PART) {
          ch = 'P';
          fg = COL_YELLOW;
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

  // Draw UI overlay.
  const char *sel = "EMPTY         ";
  if (selected_building == BELT) {
    if (selected_rotation == DIR_UP)
      sel = "BELT (^)      ";
    else if (selected_rotation == DIR_RIGHT)
      sel = "BELT (>)      ";
    else if (selected_rotation == DIR_DOWN)
      sel = "BELT (v)      ";
    else
      sel = "BELT (<)      ";
  } else if (selected_building == SPLITTER) {
    if (selected_rotation == DIR_UP)
      sel = "SPLITTER (^^) ";
    else if (selected_rotation == DIR_RIGHT)
      sel = "SPLITTER (>>) ";
    else if (selected_rotation == DIR_DOWN)
      sel = "SPLITTER (vv) ";
    else
      sel = "SPLITTER (<<) ";
  } else if (selected_building == MINER) {
    sel = "MINER         ";
  } else if (selected_building == CRAFTER) {
    sel = "CRAFTER       ";
  } else if (selected_building == FURNACE) {
    sel = "FURNACE       ";
  } else if (selected_building == LAB) {
    sel = "LAB           ";
  }

  const char *txt1 = " Selected: ";
  for (int i = 0; txt1[i] != '\0'; i++) {
    buffer[i] = (COL_BLUE << 12) | (COL_YELLOW << 8) | txt1[i];
  }
  for (int i = 0; sel[i] != '\0'; i++) {
    buffer[11 + i] = (COL_BLUE << 12) | (COL_WHITE << 8) | sel[i];
  }

  // Draw Score.
  const char *s_txt = " Score: ";
  int sx = 60;
  for (int i = 0; s_txt[i] != '\0'; i++) {
    buffer[sx + i] = (COL_BLUE << 12) | (COL_YELLOW << 8) | s_txt[i];
  }
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

  for (int i = 0; score_buf[i] != '\0'; i++) {
    buffer[sx + i] = (COL_BLUE << 12) | (COL_WHITE << 8) | score_buf[i];
  }

  for (int i = 0; io_status[i] != '\0'; i++) {
    buffer[37 + i] = (COL_BLUE << 12) | (COL_LIGHT_GREY << 8) | io_status[i];
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
  print_text(10, 11, "1       - Select Belt (use R to rotate)", COL_WHITE,
             COL_BLACK);
  print_text(10, 12, "2       - Select Splitter 2-wide (use R to rotate)",
             COL_WHITE, COL_BLACK);
  print_text(10, 13, "5       - Select Miner (M) on Ore patches", COL_WHITE,
             COL_BLACK);
  print_text(10, 14, "7       - Select Furnace (F). Smelts Ore -> Plates",
             COL_WHITE, COL_BLACK);
  print_text(10, 15, "6       - Select Crafter (C). Iron -> Gear", COL_WHITE,
             COL_BLACK);
  print_text(10, 16, "          Gear + Copper Plate -> Machine Part", COL_WHITE,
             COL_BLACK);
  print_text(10, 17, "8       - Select Lab (L). Machine Part gives 1 point",
             COL_WHITE, COL_BLACK);
  print_text(10, 18, "0       - Select Empty (Eraser)", COL_WHITE, COL_BLACK);
  print_text(10, 19, "SPACE   - Place building", COL_WHITE, COL_BLACK);
  print_text(10, 20, "R       - Rotate Belt / Splitter", COL_WHITE, COL_BLACK);
  print_text(10, 21, "CTRL+S  - Save map to serial (no items)", COL_WHITE,
             COL_BLACK);
  print_text(10, 22, "L       - Load map from serial", COL_WHITE, COL_BLACK);
  print_text(28, 24, "Press SPACE to start...", COL_GREEN, COL_BLACK);
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
    current_state = 1;
    for (int i = 0; i < GRID_W * GRID_H; i++)
      VGA_MEM[i] = 0;
  }
}

__attribute__((section(".text.entry"))) void entry() {
  current_state = 0;
  cursor_x = 40;
  cursor_y = 12;
  selected_building = BELT;
  selected_rotation = DIR_RIGHT;
  tick_counter = 0;
  score = 0;
  ctrl_held = 0;
  should_exit_app = 0;
  status_set("IO: idle");

  init_grid();
  while (inb(0x64) & 1)
    inb(0x60);

  uint32_t loop_counter = 0;
  render_title();

  while (1) {
    if (current_state == 0) {
      handle_title_input();
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