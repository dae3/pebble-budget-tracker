#include <pebble.h>
#include "ui.h"
#include "BudgetLine.h"

// callbacks for menu layer

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index,
				      void *context) {
  const uint16_t num_rows = 3;
  return num_rows;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer,
			      MenuIndex *cell_index, void *context) {
  static char rowtext[10];
  snprintf(rowtext, sizeof(rowtext), "exp %d", (int)cell_index->row);
  menu_cell_basic_draw(ctx, cell_layer, rowtext, NULL, NULL);
}

static uint16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index,
					 void *context) {
  const uint16_t height = 44;
  return height;
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "menu layer select callback");
}

// menu window and layer setup
