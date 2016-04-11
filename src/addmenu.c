#include <pebble.h>

static Window *me;

// menu data
SimpleMenuLayer *menu;
SimpleMenuItem menu_items[] = {
  { "title1", NULL, NULL, NULL },
  { "title2", NULL, NULL, NULL }
};
static SimpleMenuSection section = { "sectiontitle", menu_items, 2 };


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
