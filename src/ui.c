#include <pebble.h>
#include "ui.h"
#include "BudgetLine.h"

static Window *my_window;
static ActionBarLayer *action_bar;
GBitmap *icon_up, *icon_down, *icon_refresh;
static TextLayer *textl_title, *textl_amount;
static char title[20], amount[10];
// $A0000.00 + \0 is 10

// UI general
void display_BudgetLine(BudgetLine *line) {
  current_line = line;
  
  text_layer_set_text(textl_title, line->name);
  text_layer_set_text(textl_amount, line->amount);
}

// Window stuff
static void set_loading_message() {
  snprintf(title, sizeof(title), "loading...");
  snprintf(amount, sizeof(amount), "$0.00");
  text_layer_set_text(textl_title, title);
  text_layer_set_text(textl_amount, amount);
}

static void my_window_load(Window *window) {
  // ready message to Pebblekit JS will trigger 
  // first update of text, no need to call explicitly
  
  // setup UI - text layers
  textl_title = text_layer_create(GRect(0, 25, 144, 30));
  text_layer_set_font(textl_title, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  
  textl_amount = text_layer_create(GRect(0, 55, 144, 100));
  text_layer_set_font(textl_amount, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(textl_title));
  layer_add_child(window_get_root_layer(my_window), text_layer_get_layer(textl_amount));
  
  // setup UI - action bar
  //action_bar = Action
  
  set_loading_message();
}

static void my_window_unload(Window *window) {
  text_layer_destroy(textl_title);  
  text_layer_destroy(textl_amount);
}

// Button handlers
static void down_click_handler(ClickRecognizerRef recog, void *context) {
  if (current_line->next != NULL) {
    display_BudgetLine(current_line->next);
  }
}

static void up_click_handler(ClickRecognizerRef recog, void *context) {
  if (current_line->prev != NULL) {
    display_BudgetLine(current_line->prev);
  }
}

static void select_click_handler(ClickRecognizerRef recog, void *context) {
  // TODO move the non UI stuff out of here
  DictionaryIterator *d;
    
  APP_LOG(APP_LOG_LEVEL_DEBUG, "select_click_handler top");
  
  set_loading_message();
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "select_click_handler post slm");
  
  freeBudgetData();
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "select_click_handler post fbd");

  APP_LOG(APP_LOG_LEVEL_DEBUG, "about to peek PebbleKit connection");
  if (connection_service_peek_pebble_app_connection()) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "about to send reload appmsg");
    if (app_message_outbox_begin(&d) == APP_MSG_OK) {
      if (dict_write_int8(d, 0, 0) == DICT_OK) {
        dict_write_end(d);
        app_message_outbox_send();
        APP_LOG(APP_LOG_LEVEL_DEBUG, "reload appmsg sent");
      }
    } else {
      APP_LOG(APP_LOG_LEVEL_ERROR, "AppMessage outbox init fail");
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "no connection to PebbleKit");    
  }
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void ui_display_error(char *errormsg) {
  /* TODO: setup another text layer better suited to an error,
  and display it here, hiding the normal 2 */
  snprintf(amount, sizeof(amount), "0.00");
  snprintf(title, sizeof(title), "%s", errormsg);
}


void ui_setup(void) {    
  my_window = window_create();  
  window_set_window_handlers(my_window, (WindowHandlers) { .load = my_window_load, .unload = my_window_unload });
  // window_set_click_config_provider(my_window, click_config_provider);
  window_stack_push(my_window, false);
  
  action_bar = action_bar_layer_create();
  action_bar_layer_set_background_color(action_bar, GColorWhite);
  
  icon_up = gbitmap_create_with_resource(RESOURCE_ID_ACTION_BAR_ARROW_UP);
  icon_refresh = gbitmap_create_with_resource(RESOURCE_ID_ACTION_BAR_REFRESH);
  icon_down = gbitmap_create_with_resource(RESOURCE_ID_ACTION_BAR_ARROW_DOWN);
  
  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, icon_up);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, icon_refresh);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, icon_down);
  
  action_bar_layer_set_click_config_provider(action_bar, click_config_provider);
  
  action_bar_layer_add_to_window(action_bar, my_window);
}

void ui_destroy(void) {
  action_bar_layer_destroy(action_bar);
  gbitmap_destroy(icon_up);
  gbitmap_destroy(icon_refresh);
  gbitmap_destroy(icon_down);
  my_window_unload(my_window);
}