#include "factory.h"

const Recipe g_recipes[] = {
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

const int g_recipe_count = sizeof(g_recipes) / sizeof(g_recipes[0]);

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

uint8_t get_scancode() {
  if (inb(0x64) & 1) return inb(0x60);
  return 0;
}

void status_set(const char *msg) {
  int i = 0;
  while (i < 21) {
    if (msg[i] == '\0') break;
    io_status[i] = msg[i];
    i++;
  }
  while (i < 21) {
    io_status[i] = ' ';
    i++;
  }
  io_status[21] = '\0';
}

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
    player.active_panel = (player.active_panel == INVENTORY_PANEL_MAIN) ? INVENTORY_PANEL_CRAFT : INVENTORY_PANEL_MAIN;
    return;
  }
  if (player.active_panel == INVENTORY_PANEL_MAIN) {
    if (scancode == 0x11) move_inventory_focus(0, -1);
    else if (scancode == 0x1F) move_inventory_focus(0, 1);
    else if (scancode == 0x1E) move_inventory_focus(-1, 0);
    else if (scancode == 0x20) move_inventory_focus(1, 0);
  }
  if (player.active_panel == INVENTORY_PANEL_CRAFT) {
    if (scancode == 0x11 && player.recipe_focus > 0) { player.recipe_focus--; return; }
    if (scancode == 0x1F && player.recipe_focus + 1 < g_recipe_count) { player.recipe_focus++; return; }
    if (scancode == 0x1C) {
      if (craft_recipe(player.recipe_focus)) status_set("IO: crafted");
      else status_set("IO: missing items");
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
  if (!in_bounds(machine_x, machine_y) || grid[machine_y][machine_x].building != FURNACE) {
    current_state = MODE_GAME;
    return;
  }
  struct Cell *machine = &grid[machine_y][machine_x];
  if (scancode == 0x12) {
    machine_panel ^= 1;
    return;
  }
  if (machine_panel == 0) {
    if (scancode == 0x11) move_inventory_focus(0, -1);
    else if (scancode == 0x1F) move_inventory_focus(0, 1);
    else if (scancode == 0x1E) move_inventory_focus(-1, 0);
    else if (scancode == 0x20) move_inventory_focus(1, 0);
  } else {
    if (scancode == 0x11 && machine_focus > 0) machine_focus--;
    if (scancode == 0x1F && machine_focus < 2) machine_focus++;
  }
  int hotbar_slot = hotbar_slot_from_scancode(scancode);
  if (hotbar_slot >= 0 && machine_panel == 0) {
    transfer_main_and_hotbar(player.main_focus, hotbar_slot);
    player.selected_hotbar = (uint8_t)hotbar_slot;
    return;
  }
  if (scancode != 0x1C) return;
  if (machine_panel == 0) {
    struct ItemStack *focused = &player.main[player.main_focus];
    if (stack_is_empty(focused)) return;
    if (is_furnace_ore(focused->item)) {
      if (machine->furnace_ore_count == 0 || machine->furnace_ore_type == focused->item) {
        if (machine->furnace_ore_count < 255) {
          machine->furnace_ore_type = focused->item;
          machine->furnace_ore_count++;
          focused->count--;
          if (focused->count == 0) stack_clear(focused);
        }
      }
      return;
    }
    if (focused->item == COAL) {
      if (machine->furnace_fuel_count < 255) {
        machine->furnace_fuel_count++;
        focused->count--;
        if (focused->count == 0) stack_clear(focused);
      }
      return;
    }
    return;
  }
  if (machine_focus == 0) {
    if (machine->furnace_ore_count > 0 && add_item_to_main_inventory((ItemType)machine->furnace_ore_type, 1)) {
      machine->furnace_ore_count--;
      if (machine->furnace_ore_count == 0) machine->furnace_ore_type = NONE;
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
  if (scancode == 0x1D) { ctrl_held = 1; return; }
  if (scancode == 0x9D) { ctrl_held = 0; return; }
  if (scancode == 0x11 && cursor_y > 1) cursor_y--;
  if (scancode == 0x1F && !ctrl_held && cursor_y < GRID_H - 1) cursor_y++;
  if (scancode == 0x1E && cursor_x > 0) cursor_x--;
  if (scancode == 0x20 && cursor_x < GRID_W - 1) cursor_x++;

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
    if (grid[cursor_y][cursor_x].item != NONE) {
      if (add_item_to_main_inventory(grid[cursor_y][cursor_x].item, 1)) {
        grid[cursor_y][cursor_x].item = NONE;
        status_set("IO: picked up item");
      } else {
        status_set("IO: inventory full");
      }
    } else if (mine_resource_at(cursor_x, cursor_y)) {
      status_set("IO: mined");
    } else {
      status_set("IO: nothing to mine");
    }
    return;
  }

  if (scancode == 0x39) {
    if (place_from_hotbar()) status_set("IO: placed");
    else status_set("IO: cannot place");
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
  if (scancode == 0) return;
  if (scancode == 0x01) { should_exit_app = 1; return; }
  if (scancode == 0x1D) { ctrl_held = 1; return; }
  if (scancode == 0x9D) { ctrl_held = 0; return; }
  if (scancode & 0x80) return;

  if (current_state == MODE_INVENTORY) handle_inventory_input(scancode);
  else if (current_state == MODE_MACHINE) handle_machine_input(scancode);
  else if (current_state == MODE_GAME) handle_game_input(scancode);
}

void handle_title_input() {
  uint8_t scancode = get_scancode();
  if (scancode == 0 || (scancode & 0x80)) return;
  if (scancode == 0x01) { should_exit_app = 1; return; }
  if (scancode == 0x39) {
    current_state = MODE_GAME;
    start_player_inventory();
    for (int i = 0; i < GRID_W * GRID_H; i++) VGA_MEM[i] = 0;
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
  while (inb(0x64) & 1) inb(0x60);

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