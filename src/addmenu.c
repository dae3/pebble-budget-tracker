#include <pebble.h>
#include <math.h>
#include "appmsgkeys.h"
#include "float_to_int.h"

struct expdata { float value; char *description; };

static const struct expdata initdata[] = {
  { 0.01, "Test" },
  { 3.50, "Coffee" },
  { 7.00, "Coffee" },
  { 15.00, "Lunch" }
};
static const int ninitdata = 4;

static Window *me;

// menu data
// callback function forward definitions
void menu_callback(int index, void *context);

static const char section_title[] = "Add expense";

static SimpleMenuLayer *menu;
static SimpleMenuItem *menu_items;
static SimpleMenuSection menu_section;

struct expense_item  {
  char *description;
  char *category;
  char *value_as_string;
  float value;
};

static struct expense_item *expenses;
	
/* Read common expense data from preferences, allocate and load into SimpleMenulayer and internal 
   data structures */
static void unload_expenses() {
  int i;

  for (i = 0; i < ninitdata; i++) {
    free(expenses[i].description);
    free(expenses[i].value_as_string);
  }

  free(menu_items);
  free(expenses);
}

static void load_expenses() {
  int i;

  expenses = calloc(sizeof(struct expense_item), ninitdata);
  menu_items = calloc(sizeof(SimpleMenuItem), ninitdata);
	
  if (expenses == NULL || menu_items == NULL) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "couldn't calloc for expenses");
  } else {
    for (i = 0; i < ninitdata; i++) {
      //TODO malloc fail check
      expenses[i].description = (char*)malloc(strlen(initdata[i].description)+1);
      strncpy(expenses[i].description, initdata[i].description, strlen(initdata[i].description));
      format_float_as_string(&expenses[i].value_as_string, initdata[i].value);
      
      expenses[i].value = initdata[i].value;
      menu_items[i].title = (const char*)expenses[i].description;
      menu_items[i].subtitle = (const char*)expenses[i].value_as_string;
      menu_items[i].callback = menu_callback;

      /* APP_LOG(APP_LOG_LEVEL_DEBUG, "loade %s %s", initdata[i].description, expenses[i].value_as_string); */
    }
  }
}


// window load and unload
static void window_handler_load(Window *w) {
  load_expenses();

  menu_section.title = section_title;
  menu_section.items = menu_items;
  menu_section.num_items = ninitdata;

  menu = simple_menu_layer_create(GRect(0,0,150,150), w, &menu_section, 1, NULL);
  layer_add_child(window_get_root_layer(me), simple_menu_layer_get_layer(menu));
}

static void window_handler_unload(Window *w) {
  unload_expenses();
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


void menu_callback(int index, void *context) {
  DictionaryIterator *dict;

  if (app_message_outbox_begin(&dict) == APP_MSG_OK) {

    dict_write_uint8(dict, MSGKEY_TYPE, TYPE_ADD);
    dict_write_cstring(dict, MSGKEY_NEWEXPENSEDESC, expenses[index].description);
    dict_write_cstring(dict, MSGKEY_NEWEXPENSEVALUE, expenses[index].value_as_string);
    dict_write_end(dict);
    app_message_outbox_send();
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "app_message_outbox_begin fail");
  }
}

