#include "factory.h"

int dir_dx(uint8_t dir) {
  if (dir == DIR_LEFT) return -1;
  if (dir == DIR_RIGHT) return 1;
  return 0;
}

int dir_dy(uint8_t dir) {
  if (dir == DIR_UP) return -1;
  if (dir == DIR_DOWN) return 1;
  return 0;
}

uint8_t right_dir(uint8_t dir) { return (uint8_t)((dir + 1) & 3); }
uint8_t left_dir(uint8_t dir) { return (uint8_t)((dir + 3) & 3); }

char dir_char(uint8_t dir) {
  if (dir == DIR_UP) return '^';
  if (dir == DIR_RIGHT) return '>';
  if (dir == DIR_DOWN) return 'v';
  return '<';
}

int is_rotatable(EntityType e) { return e == BELT || e == SPLITTER; }

void reset_cell_runtime(int x, int y) {
  grid[y][x].building = EMPTY;
  grid[y][x].item = NONE;
  grid[y][x].handled = 0;
  grid[y][x].rotation = DIR_RIGHT;
  // NOTE: terrain and resource_amount are preserved intentionally!
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
  if (!in_bounds(ax, ay)) return;
  if (grid[ay][ax].building != SPLITTER) return;

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
  if (!in_bounds(x, y)) return;
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
  if (!in_bounds(ax, ay) || !in_bounds(bx, by)) return 0;

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
  if (!in_bounds(bx, by) || !in_bounds(tx, ty)) return 0;
  if (grid[by][bx].building != BELT) return 0;
  return bx + dir_dx(grid[by][bx].rotation) == tx &&
         by + dir_dy(grid[by][bx].rotation) == ty;
}

int belt_points_away_from(int bx, int by, int sx, int sy) {
  if (!in_bounds(bx, by) || !in_bounds(sx, sy)) return 0;
  if (grid[by][bx].building != BELT) return 0;
  return bx - sx == dir_dx(grid[by][bx].rotation) &&
         by - sy == dir_dy(grid[by][bx].rotation);
}

int resource_item_at(int x, int y, ItemType *item_out) {
  if (!in_bounds(x, y)) return 0;
  if (grid[y][x].resource_amount == 0) return 0;
  *item_out = terrain_to_item(grid[y][x].terrain);
  return *item_out != NONE;
}

int mine_resource_at(int x, int y) {
  ItemType item = NONE;
  if (!resource_item_at(x, y, &item)) return 0;
  if (grid[y][x].building != EMPTY) return 0;
  if (!add_item_to_main_inventory(item, 1)) return 0;

  if (grid[y][x].resource_amount > 0)
    grid[y][x].resource_amount--;
  if (grid[y][x].resource_amount == 0)
    grid[y][x].terrain = TERR_EMPTY;
  return 1;
}

void clear_entity_with_refund(int x, int y) {
  if (!in_bounds(x, y)) return;

  EntityType building = grid[y][x].building;
  if (building != EMPTY)
    add_item_to_main_inventory(building_to_item(building), 1);
  clear_entity_at(x, y);
}

int place_from_hotbar(void) {
  if (player.selected_hotbar >= HOTBAR_SLOTS) return 0;

  struct ItemStack *slot = &player.hotbar[player.selected_hotbar];
  if (stack_is_empty(slot)) return 0;
  if (!item_is_placeable(slot->item)) return 0;

  ItemType item = slot->item;
  EntityType building = item_to_building(item);

  if (!in_bounds(cursor_x, cursor_y)) return 0;

  if (item == ITEM_SPLITTER) {
    int sx = cursor_x + dir_dx(right_dir(selected_rotation));
    int sy = cursor_y + dir_dy(right_dir(selected_rotation));
    if (!in_bounds(sx, sy)) return 0;
    clear_entity_with_refund(cursor_x, cursor_y);
    clear_entity_with_refund(sx, sy);
    if (!place_splitter(cursor_x, cursor_y, selected_rotation)) return 0;
  } else {
    clear_entity_with_refund(cursor_x, cursor_y);
    grid[cursor_y][cursor_x].building = building;
    grid[cursor_y][cursor_x].rotation =
        item == ITEM_BELT || item == ITEM_SPLITTER ? selected_rotation : DIR_RIGHT;
    grid[cursor_y][cursor_x].item = NONE;
  }

  if (slot->count > 0) slot->count--;
  if (slot->count == 0) stack_clear(slot);
  return 1;
}

void init_grid() {
  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      grid[y][x].terrain = TERR_EMPTY;
      grid[y][x].resource_amount = 0;
      reset_cell_runtime(x, y);

      if (x >= 10 && x <= 18 && y >= 5 && y <= 12) grid[y][x].terrain = TERR_IRON;
      if (x >= 60 && x <= 68 && y >= 14 && y <= 20) grid[y][x].terrain = TERR_COPPER;
      if (x >= 24 && x <= 32 && y >= 3 && y <= 9) grid[y][x].terrain = TERR_COAL;
      if (x >= 40 && x <= 48 && y >= 14 && y <= 21) grid[y][x].terrain = TERR_STONE;

      if (grid[y][x].terrain != TERR_EMPTY)
        grid[y][x].resource_amount = 32;
    }
  }
}

int push_item_to_belt(int x, int y, ItemType item) {
  int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  for (int d = 0; d < 4; d++) {
    int bx = x + dirs[d][0];
    int by = y + dirs[d][1];
    if (!in_bounds(bx, by)) continue;

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
  if (!in_bounds(srcx, srcy) || !in_bounds(dstx, dsty)) return;
  if (grid[srcy][srcx].item == NONE || grid[dsty][dstx].item != NONE) return;

  grid[dsty][dstx].item = grid[srcy][srcx].item;
  grid[dsty][dstx].handled = 1;
  grid[srcy][srcx].item = NONE;

  if (use_out_toggle) anchor->split_toggle_out ^= 1;
  if (use_in_toggle) anchor->split_toggle_in ^= 1;
}

void process_splitter_anchor(int ax, int ay) {
  struct Cell *anchor = &grid[ay][ax];
  uint8_t dir = anchor->rotation;
  int bx, by;
  splitter_secondary(ax, ay, dir, &bx, &by);
  if (!in_bounds(bx, by) || grid[by][bx].building != SPLITTER) return;

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
  if (src_count == 0 || dst_count == 0) return;

  if (src_count == 1 && dst_count >= 1) {
    int s = src_ok[0] ? 0 : 1;
    int d = 0;
    if (dst_count == 1) {
      d = dst_ok[0] ? 0 : 1;
      move_splitter_item(anchor, s == 0 ? in0x : in1x, s == 0 ? in0y : in1y,
                         d == 0 ? out0x : out1x, d == 0 ? out0y : out1y, 0, 0);
    } else {
      d = anchor->split_toggle_out ? 1 : 0;
      if (!dst_ok[d]) d ^= 1;
      move_splitter_item(anchor, s == 0 ? in0x : in1x, s == 0 ? in0y : in1y,
                         d == 0 ? out0x : out1x, d == 0 ? out0y : out1y, 1, 0);
    }
    return;
  }

  if (src_count >= 1 && dst_count == 1) {
    int d = dst_ok[0] ? 0 : 1;
    int s = 0;
    if (src_count == 1) {
      s = src_ok[0] ? 0 : 1;
      move_splitter_item(anchor, s == 0 ? in0x : in1x, s == 0 ? in0y : in1y,
                         d == 0 ? out0x : out1x, d == 0 ? out0y : out1y, 0, 0);
    } else {
      s = anchor->split_toggle_in ? 1 : 0;
      if (!src_ok[s]) s ^= 1;
      move_splitter_item(anchor, s == 0 ? in0x : in1x, s == 0 ? in0y : in1y,
                         d == 0 ? out0x : out1x, d == 0 ? out0y : out1y, 0, 1);
    }
    return;
  }

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

  if (tick_counter % 5 == 0) {
    for (int y = 0; y < GRID_H; y++) {
      for (int x = 0; x < GRID_W; x++) {
        // MINER IS INFINITE! (No resource_amount check)
        if (grid[y][x].building == MINER) {
          ItemType resource = terrain_to_item(grid[y][x].terrain);
          if (resource != NONE) {
            push_item_to_belt(x, y, resource);
          }
        }
      }
    }
  }

  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      if ((grid[y][x].building == FURNACE || grid[y][x].building == CRAFTER) &&
          grid[y][x].item != NONE) {
        if (push_item_to_belt(x, y, grid[y][x].item))
          grid[y][x].item = NONE;
      }
    }
  }

  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      struct Cell *current = &grid[y][x];
      if (current->building != BELT || current->item == NONE || current->handled) continue;

      int tx = x + dir_dx(current->rotation);
      int ty = y + dir_dy(current->rotation);
      if (!in_bounds(tx, ty)) continue;

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
      } else if (target->building == FURNACE) {
        if ((current->item == IRON_ORE || current->item == COPPER_ORE) && target->furnace_ore_count < 255) {
          if (target->furnace_ore_count == 0 || target->furnace_ore_type == current->item) {
            target->furnace_ore_type = current->item;
            target->furnace_ore_count++;
            current->item = NONE;
          }
        } else if (current->item == COAL && target->furnace_fuel_count < 255) {
          target->furnace_fuel_count++;
          current->item = NONE;
        }
      }
    }
  }

  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      struct Cell *target = &grid[y][x];
      if (target->building != BELT || target->item != NONE || target->handled) continue;

      int sx = x - dir_dx(target->rotation);
      int sy = y - dir_dy(target->rotation);
      uint8_t incoming_pref[4] = {
          (uint8_t)((target->rotation + 2) & 3), left_dir(target->rotation),
          right_dir(target->rotation), target->rotation};

      for (int k = 0; k < 4; k++) {
        uint8_t in_dir = incoming_pref[k];
        sx = x + dir_dx(in_dir);
        sy = y + dir_dy(in_dir);
        if (!in_bounds(sx, sy)) continue;

        struct Cell *source = &grid[sy][sx];
        if (source->building != BELT || source->item == NONE || source->handled) continue;
        if (!belt_points_to(sx, sy, x, y)) continue;

        target->item = source->item;
        target->handled = 1;
        source->item = NONE;
        break;
      }
    }
  }

  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      if (grid[y][x].building == SPLITTER && grid[y][x].split_anchor)
        process_splitter_anchor(x, y);
    }
  }

  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      if (grid[y][x].building == FURNACE) {
        if (grid[y][x].item == NONE && grid[y][x].furnace_ore_count > 0 &&
            grid[y][x].furnace_fuel_count > 0) {
          ItemType output = furnace_output_for((ItemType)grid[y][x].furnace_ore_type);
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

  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      if (grid[y][x].building != CRAFTER || grid[y][x].item != NONE) continue;

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
