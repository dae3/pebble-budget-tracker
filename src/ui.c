#include <pebble.h>
#include "ui.h"
#include "BudgetLine.h"

static Window *main_window;
static Window *add_window;

extern void set_status_message(char *message);

void display_status_message(char *message) {
  Window *top =  window_stack_get_top_window();
  if (top == main_window) {
    set_status_message(message);
  } else if (top == add_window) {

  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "display_status_message, unknown window on top of stack");
  }
}

void setup_ui(void) {    
  main_window = create_main_window();
  add_window = create_add_window();
}

void destroy_ui(void) {
  destroy_main_window();
  destroy_add_window();
  destroy_addresult_window();
}
