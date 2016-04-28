#include <pebble.h>
#include "appmsgkeys.h"

static Window *me;

// menu data
// callback function forward definitions
void menu_callback(int index, void *context);

SimpleMenuLayer *menu;
SimpleMenuItem menu_items[] = {
  { "Coffee", "$2", NULL, menu_callback },
  { "Coffee", "$3.50", NULL, menu_callback },
  { "Drinks", "$20", NULL, menu_callback },
};
static SimpleMenuSection section = { "Add an expense", menu_items, 3 };


// window load and unload
static void window_handler_load(Window *w) {
  SimpleMenuLayer *m;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "addexp_window_handler_load");
  menu = simple_menu_layer_create(GRect(0,0,150,150), w, &section, 1, NULL);
  layer_add_child(window_get_root_layer(me), simple_menu_layer_get_layer(menu));
}

static void window_handler_unload(Window *w) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "addexp_window_handler_unload");
  simple_menu_layer_destroy(menu);
}

Window *create_add_window(void) {
  me = window_create();
  window_set_window_handlers(me, (WindowHandlers) { .load = window_handler_load, .unload = window_handler_unload });

  return me;

}
				 
void destroy_add_window() {
  window_destroy(me);
}



// app message back to JS app
void addExpense(char *description, char *category, long *value) {
	DictionaryIterator *dict;
	
	if (app_message_outbox_begin(&dict) == APP_MSG_OK) {
		dict_write_cstring(dict, MSGKEY_NEWEXPENSEDESC, description);
		dict_write_uint16(dict, MSGKEY_NEWEXPENSEVALUE, *value);
		dict_write_end(dict);
		app_message_outbox_send();
	} else {
		APP_LOG(APP_LOG_LEVEL_ERROR, "app_message_outbox_begin fail");
	}
}

void menu_callback(int index, void *context) {
	char desc[] = "testdesc";
	long value = 123.45;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "menu callback: %d", index);
	addExpense(desc, NULL, &value);
}

