#include "factory.h"

void inventory_clear(void) {
  for (int i = 0; i < HOTBAR_SLOTS; i++)
    stack_clear(&player.hotbar[i]);
  for (int i = 0; i < MAIN_INV_SLOTS; i++)
    stack_clear(&player.main[i]);
  player.selected_hotbar = 0;
  player.main_focus = 0;
  player.recipe_focus = 0;
  player.active_panel = INVENTORY_PANEL_MAIN;
}

int add_to_stack(struct ItemStack *stack, ItemType item, uint8_t count) {
  if (count == 0) return 1;
  if (stack_is_empty(stack)) {
    stack->item = item;
    stack->count = count;
    return 1;
  }
  if (stack->item != item || stack->count > 255 - count) return 0;
  stack->count += count;
  return 1;
}

void sort_main_inventory(void) {
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
        if (b->count > a->count || (b->count == a->count && b->item < a->item)) {
          struct ItemStack tmp = *a;
          *a = *b;
          *b = tmp;
        }
      }
    }
  }
}

int add_item_to_main_inventory(ItemType item, uint8_t count) {
  if (item == NONE || count == 0) return 1;
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
  if (count != 0) return 0;
  sort_main_inventory();
  return 1;
}

int add_item_to_hotbar(uint8_t slot, ItemType item, uint8_t count) {
  if (slot >= HOTBAR_SLOTS || item == NONE || count == 0) return 0;
  if (stack_is_empty(&player.hotbar[slot])) {
    player.hotbar[slot].item = item;
    player.hotbar[slot].count = count;
    return 1;
  }
  if (player.hotbar[slot].item != item || player.hotbar[slot].count > 255 - count) return 0;
  player.hotbar[slot].count += count;
  return 1;
}

int inventory_total_count(ItemType item) {
  int total = 0;
  for (int i = 0; i < MAIN_INV_SLOTS; i++)
    if (!stack_is_empty(&player.main[i]) && player.main[i].item == item)
      total += player.main[i].count;
  for (int i = 0; i < HOTBAR_SLOTS; i++)
    if (!stack_is_empty(&player.hotbar[i]) && player.hotbar[i].item == item)
      total += player.hotbar[i].count;
  return total;
}

int remove_item_from_inventory(ItemType item, uint8_t count) {
  if (item == NONE || count == 0) return 1;
  for (int i = 0; i < MAIN_INV_SLOTS && count > 0; i++) {
    struct ItemStack *stack = &player.main[i];
    if (stack_is_empty(stack) || stack->item != item) continue;
    uint8_t take = stack->count < count ? stack->count : count;
    stack->count -= take;
    count -= take;
    if (stack->count == 0) stack_clear(stack);
  }
  for (int i = 0; i < HOTBAR_SLOTS && count > 0; i++) {
    struct ItemStack *stack = &player.hotbar[i];
    if (stack_is_empty(stack) || stack->item != item) continue;
    uint8_t take = stack->count < count ? stack->count : count;
    stack->count -= take;
    count -= take;
    if (stack->count == 0) stack_clear(stack);
  }
  if (count != 0) return 0;
  sort_main_inventory();
  return 1;
}

int recipe_can_craft(int recipe_index) {
  if (recipe_index < 0 || recipe_index >= g_recipe_count) return 0;
  const Recipe *recipe = &g_recipes[recipe_index];
  for (int i = 0; i < recipe->ingredient_count; i++) {
    if (inventory_total_count(recipe->ingredients[i].item) < recipe->ingredients[i].count) return 0;
  }
  return 1;
}

int craft_recipe(int recipe_index) {
  if (!recipe_can_craft(recipe_index)) return 0;
  const Recipe *recipe = &g_recipes[recipe_index];
  for (int i = 0; i < recipe->ingredient_count; i++) {
    if (!remove_item_from_inventory(recipe->ingredients[i].item, recipe->ingredients[i].count)) return 0;
  }
  if (!add_item_to_main_inventory(recipe->result, recipe->result_count)) {
    for (int i = 0; i < recipe->ingredient_count; i++)
      add_item_to_main_inventory(recipe->ingredients[i].item, recipe->ingredients[i].count);
    return 0;
  }
  return 1;
}

int get_selected_hotbar_item(ItemType *item_out) {
  if (player.selected_hotbar >= HOTBAR_SLOTS) return 0;
  if (stack_is_empty(&player.hotbar[player.selected_hotbar])) return 0;
  *item_out = player.hotbar[player.selected_hotbar].item;
  return 1;
}

int hotbar_slot_from_scancode(uint8_t scancode) {
  if (scancode >= 0x02 && scancode <= 0x0A)
    return (int)(scancode - 0x02);
  return -1;
}

int placeable_slot_count(void) {
  int count = 0;
  for (int i = 0; i < HOTBAR_SLOTS; i++)
    if (!stack_is_empty(&player.hotbar[i])) count++;
  return count;
}

int transfer_main_and_hotbar(int main_index, int hotbar_index) {
  if (main_index < 0 || main_index >= MAIN_INV_SLOTS || hotbar_index < 0 || hotbar_index >= HOTBAR_SLOTS) return 0;
  if (!stack_is_empty(&player.main[main_index])) {
    struct ItemStack tmp = player.hotbar[hotbar_index];
    player.hotbar[hotbar_index] = player.main[main_index];
    player.main[main_index] = tmp;
    sort_main_inventory();
    return 1;
  }
  if (stack_is_empty(&player.hotbar[hotbar_index])) return 0;
  player.main[main_index] = player.hotbar[hotbar_index];
  stack_clear(&player.hotbar[hotbar_index]);
  return 1;
}

int move_main_stack_to_hotbar(int main_index, int hotbar_index) {
  if (main_index < 0 || main_index >= MAIN_INV_SLOTS || hotbar_index < 0 || hotbar_index >= HOTBAR_SLOTS) return 0;
  if (stack_is_empty(&player.main[main_index])) return 0;
  struct ItemStack tmp = player.hotbar[hotbar_index];
  player.hotbar[hotbar_index] = player.main[main_index];
  player.main[main_index] = tmp;
  sort_main_inventory();
  return 1;
}

void start_player_inventory(void) {
  inventory_clear();
  add_item_to_main_inventory(ITEM_FURNACE, 1);
  add_item_to_main_inventory(COAL, 10);
  player.selected_hotbar = 0;
  player.main_focus = 0;
  player.recipe_focus = 0;
  player.active_panel = INVENTORY_PANEL_MAIN;
}
