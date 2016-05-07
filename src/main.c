#include <pebble.h>
#include "appmsgkeys.h"
#include "ui.h"
#include "BudgetLine.h"


void handle_init(void) {
  setup_ui();
  setup_appmsg();
}

void handle_deinit(void) {
  destroy_ui();
  freeBudgetData();
}

int main(void) {  
  handle_init();
  app_event_loop();
  handle_deinit();
}
