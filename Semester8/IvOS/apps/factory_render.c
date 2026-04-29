#include "factory.h"

void print_text(int x, int y, const char *str, uint8_t fg, uint8_t bg) {
  int i = 0;
  while (str[i] != '\0') {
    if (x + i >= GRID_W) break;
    VGA_MEM[y * GRID_W + (x + i)] = (bg << 12) | (fg << 8) | str[i];
    i++;
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
  if (!in_bounds(x, y)) return;
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
  if (count > 99) count = 99;
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
      if (count > 99) count = 99;
      buffer_put_char(buffer, x + 3, y, (char)('0' + (count / 10)), fg, bg);
      buffer_put_char(buffer, x + 4, y, (char)('0' + (count % 10)), fg, bg);
      buffer_put_char(buffer, x + 5, y, ' ', fg, bg);
    }
  }
}

void render_gameplay(uint16_t *buffer) {
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      struct Cell *cell = &grid[y][x];
      uint8_t ch = ' ';
      uint8_t fg = COL_WHITE;
      uint8_t bg = COL_BLACK;

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

      switch (cell->building) {
      case BELT: ch = (uint8_t)dir_char(cell->rotation); fg = COL_CYAN; break;
      case SPLITTER: ch = (uint8_t)dir_char(cell->rotation); fg = COL_GREEN; break;
      case MINER: ch = 'M'; fg = COL_MAGENTA; break;
      case FURNACE: ch = 'F'; fg = COL_RED; break;
      case CRAFTER: ch = 'C'; fg = COL_YELLOW; break;
      case LAB: ch = 'L'; fg = COL_GREEN; break;
      default: break;
      }

      if (cell->building == BELT || cell->building == EMPTY) {
        if (cell->item != NONE) {
          ch = item_glyph(cell->item);
          fg = item_color(cell->item);
        }
      }

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

  const char *s_txt = " Score: ";
  int sx = 50;
  buffer_put_text(buffer, sx, GRID_H - 1, s_txt, COL_YELLOW, COL_BLUE);
  sx += 8;

  char score_buf[16];
  int v = (int)score, dig = 0;
  if (v == 0) score_buf[dig++] = '0';
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

void render_inventory_screen(uint16_t *buffer) {
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
    if (count > 99) count = 99;
    buffer_put_char(buffer, x + 2, y, (char)('0' + (count / 10)), COL_WHITE, bg);
    buffer_put_char(buffer, x + 3, y, (char)('0' + (count % 10)), COL_WHITE, bg);
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
      if (cnt > 99) cnt = 99;
      buffer_put_char(buffer, req_x, 22, (char)('0' + (cnt / 10)), COL_WHITE,
                      COL_BLACK);
      buffer_put_char(buffer, req_x + 1, 22, (char)('0' + (cnt % 10)),
                      COL_WHITE, COL_BLACK);
      buffer_put_char(buffer, req_x + 2, 22, 'x', COL_LIGHT_GREY, COL_BLACK);
      buffer_put_text(buffer, req_x + 4, 22,
                      item_name(recipe->ingredients[i].item),
                      item_color(recipe->ingredients[i].item), COL_BLACK);
      req_x += 18;
      if (req_x > 74) break;
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

void render_machine_screen(uint16_t *buffer) {
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
    if (count > 99) count = 99;
    buffer_put_char(buffer, x + 2, y, (char)('0' + (count / 10)), COL_WHITE, bg);
    buffer_put_char(buffer, x + 3, y, (char)('0' + (count % 10)), COL_WHITE, bg);
  }

  struct Cell *m = (in_bounds(machine_x, machine_y) &&
                    grid[machine_y][machine_x].building == FURNACE)
                       ? &grid[machine_y][machine_x]
                       : 0;

  buffer_put_text(buffer, 44, 1, "FURNACE", COL_RED, COL_BLACK);
  buffer_put_text(buffer, 44, 2, "Enter: move in/out", COL_LIGHT_GREY, COL_BLACK);

  uint8_t bg0 = (machine_panel == 1 && machine_focus == 0) ? COL_BLUE : COL_BLACK;
  uint8_t bg1 = (machine_panel == 1 && machine_focus == 1) ? COL_BLUE : COL_BLACK;
  uint8_t bg2 = (machine_panel == 1 && machine_focus == 2) ? COL_BLUE : COL_BLACK;

  buffer_put_text(buffer, 44, 5, "Ore :", COL_WHITE, bg0);
  if (m && m->furnace_ore_count > 0) {
    buffer_put_char(buffer, 50, 5, item_glyph((ItemType)m->furnace_ore_type),
                    item_color((ItemType)m->furnace_ore_type), bg0);
    buffer_put_char(buffer, 52, 5,
                    (char)('0' + ((m->furnace_ore_count / 10) % 10)), COL_WHITE, bg0);
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
    buffer_put_char(buffer, 50, 9, item_glyph(m->item), item_color(m->item), bg2);
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
  buffer_put_text(buffer, 2, 23, "TAB closes machine view.", COL_WHITE, COL_BLACK);
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
  print_text(10, 10, "E    - Mine resource / pickup item", COL_WHITE, COL_BLACK);
  print_text(10, 11, "R    - Rotate placeable items", COL_WHITE, COL_BLACK);
  print_text(10, 12, "TAB  - Open inventory / crafting", COL_WHITE, COL_BLACK);
  print_text(10, 13, "CTRL+S - Save map", COL_WHITE, COL_BLACK);
  print_text(10, 14, "L    - Load map", COL_WHITE, COL_BLACK);
  print_text(10, 16, "Inventory:", COL_GREEN, COL_BLACK);
  print_text(10, 17, "Left 5x5 grid auto-sorts by stack size", COL_WHITE, COL_BLACK);
  print_text(10, 18, "1-9 moves focused stack to the hotbar", COL_WHITE, COL_BLACK);
  print_text(10, 19, "Right panel lists craftable recipes", COL_WHITE, COL_BLACK);
  print_text(10, 20, "Enter crafts one item at a time", COL_WHITE, COL_BLACK);
  print_text(22, 23, "Press SPACE to start", COL_GREEN, COL_BLACK);
}
