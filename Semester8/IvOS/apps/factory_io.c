#include "factory.h"

int serial_can_read() { return (inb(COM1 + 5) & 0x01) != 0; }
int serial_can_write() { return (inb(COM1 + 5) & 0x20) != 0; }

void serial_putc(char c) {
  while (!serial_can_write());
  outb(COM1, (uint8_t)c);
}

void serial_print(const char *s) {
  while (*s) serial_putc(*s++);
}

char hex_digit(uint8_t v) {
  if (v < 10) return (char)('0' + v);
  return (char)('A' + (v - 10));
}

void serial_print_hex8(uint8_t v) {
  serial_putc(hex_digit((uint8_t)((v >> 4) & 0x0F)));
  serial_putc(hex_digit((uint8_t)(v & 0x0F)));
}

char serial_read_blocking() {
  while (!serial_can_read());
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
    if (c == '\r') continue;
    if (c == '\n') continue;
    buf[i++] = c;
  }
  buf[i] = '\0';
  return i;
}

int hex_value(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
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
  if (hi < 0 || lo < 0) return 0;
  return (uint8_t)((hi << 4) | lo);
}

static int cell_equals(struct Cell *a, struct Cell *b) {
  return a->building == b->building &&
         a->rotation == b->rotation &&
         a->item == b->item &&
         a->terrain == b->terrain &&
         a->resource_amount == b->resource_amount &&
         a->inv_iron == b->inv_iron &&
         a->inv_copper == b->inv_copper &&
         a->inv_gear == b->inv_gear &&
         a->split_anchor == b->split_anchor &&
         a->furnace_ore_type == b->furnace_ore_type &&
         a->furnace_ore_count == b->furnace_ore_count &&
         a->furnace_fuel_count == b->furnace_fuel_count;
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

  serial_print("W2\n");
  int count = 0;
  struct Cell *prev = 0;

  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      struct Cell *cell = &grid[y][x];
      if (prev != 0 && cell_equals(cell, prev) && count < 255) {
        count++;
      } else {
        if (prev != 0) {
          print_hex_byte(count);
          serial_putc('x');
          print_hex_byte((uint8_t)prev->building);
          print_hex_byte(prev->rotation);
          print_hex_byte((uint8_t)prev->item);
          print_hex_byte((uint8_t)prev->terrain);
          print_hex_byte(prev->resource_amount);
          print_hex_byte(prev->inv_iron);
          print_hex_byte(prev->inv_copper);
          print_hex_byte(prev->inv_gear);
          print_hex_byte(prev->split_anchor);
          print_hex_byte(prev->furnace_ore_type);
          print_hex_byte(prev->furnace_ore_count);
          print_hex_byte(prev->furnace_fuel_count);
          serial_putc('\n');
        }
        prev = cell;
        count = 1;
      }
    }
  }
  if (prev != 0) {
    print_hex_byte(count);
    serial_putc('x');
    print_hex_byte((uint8_t)prev->building);
    print_hex_byte(prev->rotation);
    print_hex_byte((uint8_t)prev->item);
    print_hex_byte((uint8_t)prev->terrain);
    print_hex_byte(prev->resource_amount);
    print_hex_byte(prev->inv_iron);
    print_hex_byte(prev->inv_copper);
    print_hex_byte(prev->inv_gear);
    print_hex_byte(prev->split_anchor);
    print_hex_byte(prev->furnace_ore_type);
    print_hex_byte(prev->furnace_ore_count);
    print_hex_byte(prev->furnace_fuel_count);
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
  if (!(line[0] == 'M' && line[1] == 'F' && line[2] == '2' && line[3] == '\0')) {
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

  read_serial_sized(line, 2);
  if (line[0] == 'W' && line[1] == '2') {
    int total_cells = 0;
    while (total_cells < GRID_W * GRID_H) {
      len = read_serial_sized(line, 27);
      if (len != 27 || line[2] != 'x') {
        status_set("IO: bad RLE world");
        return 0;
      }
      uint8_t count = parse_hex_byte_at(line, 0);
      uint8_t b = parse_hex_byte_at(line, 3);
      uint8_t r = parse_hex_byte_at(line, 5);
      uint8_t it = parse_hex_byte_at(line, 7);
      uint8_t terr = parse_hex_byte_at(line, 9);
      uint8_t res = parse_hex_byte_at(line, 11);
      uint8_t inv_iron = parse_hex_byte_at(line, 13);
      uint8_t inv_copper = parse_hex_byte_at(line, 15);
      uint8_t inv_gear = parse_hex_byte_at(line, 17);
      uint8_t split_anchor = parse_hex_byte_at(line, 19);
      uint8_t furnace_ore_type = parse_hex_byte_at(line, 21);
      uint8_t furnace_ore_count = parse_hex_byte_at(line, 23);
      uint8_t furnace_fuel_count = parse_hex_byte_at(line, 25);

      for (int i = 0; i < count; i++) {
        if (total_cells >= GRID_W * GRID_H) break;
        int cx = total_cells % GRID_W;
        int cy = total_cells / GRID_W;
        reset_cell_runtime(cx, cy);
        
        grid[cy][cx].terrain = (TerrainType)terr;
        grid[cy][cx].resource_amount = res;
        grid[cy][cx].inv_iron = inv_iron;
        grid[cy][cx].inv_copper = inv_copper;
        grid[cy][cx].inv_gear = inv_gear;
        grid[cy][cx].furnace_ore_type = furnace_ore_type;
        grid[cy][cx].furnace_ore_count = furnace_ore_count;
        grid[cy][cx].furnace_fuel_count = furnace_fuel_count;
        
        if (b != EMPTY) {
          if (b == SPLITTER) {
            if (split_anchor)
              place_splitter(cx, cy, (uint8_t)(r & 3));
          } else {
            grid[cy][cx].building = (EntityType)b;
            grid[cy][cx].rotation = (uint8_t)(r & 3);
            grid[cy][cx].item = (ItemType)it;
            grid[cy][cx].split_anchor = 0;
          }
        }
        total_cells++;
      }
    }
  } else if (line[0] == 'W') {
    // Old format fallback support for W (one W per line with 80 cells)
    // The previous loader read 1 + GRID_W * 24 bytes per line.
    // The first line was already read partially (we read 2 chars "W\n" or "W" ? wait, previous was serial_putc('W') then 80 cells, so 1921 chars).
    // Let's assume backward compatibility is dropped since the save format drastically changes.
    status_set("IO: W format drop");
    return 0;
  }

  read_serial_sized(line, 3);
  if (!(line[0] == 'E' && line[1] == 'N' && line[2] == 'D' && line[3] == '\0')) {
    status_set("IO: bad trailer");
    return 0;
  }

  status_set("IO: map loaded");
  return 1;
}
