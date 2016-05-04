#include <pebble.h>

static Window *me;
static TextLayer *result_text;
static AppTimer *close_timer;

// fwd decl.
void destroy_addresult_window(void);

void close_timer_callback(void *data) {
  text_layer_set_text(result_text, "bye!");
  psleep(2000);
  window_stack_remove(me, true);
}

static void window_handler_load(Window *w) {
  result_text = text_layer_create(GRect(0,0,150,150));
  text_layer_set_text(result_text, "result");
  layer_add_child(window_get_root_layer(me), text_layer_get_layer(result_text));

  close_timer = app_timer_register(2000, close_timer_callback, NULL);
}

static void window_handler_unload(Window *w) {
  text_layer_destroy(result_text);
  destroy_addresult_window();
}


Window *create_addresult_window(void)
{
  me = window_create();
  window_set_window_handlers(me, (WindowHandlers){ .load = window_handler_load, .unload = window_handler_unload });
  return me;
}

void destroy_addresult_window(void) {
  if (me != NULL)
    window_destroy(me);
}
