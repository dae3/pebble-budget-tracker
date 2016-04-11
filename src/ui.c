#include <pebble.h>
#include "ui.h"
#include "BudgetLine.h"


void ui_setup(void) {    
  Window *main_window = create_main_window();
  Window *add_window = create_add_window();
}

void ui_destroy(void) {
  destroy_main_window();
  destroy_add_window();
}
