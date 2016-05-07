#include <pebble.h>
#include "ui.h"
#include "BudgetLine.h"

static Window *me;
static ActionBarLayer *action_bar;
static GBitmap *icon_up, *icon_down, *icon_refresh;
static TextLayer *textl_title, *textl_amount;
static char title[20], amount[10];
// $A0000.00 + \0 is 10
static BudgetLine *current_line;
static char *status_message;

void display_BudgetLine(BudgetLine *line) {
  current_line = line;
  
  text_layer_set_text(textl_title, line->name);
  text_layer_set_text(textl_amount, line->amount);
}

void set_status_message(char *message) {
  status_message = (char*)malloc(strlen(message)+1);
  
  strncpy(status_message, message, strlen(message));
  text_layer_set_text(textl_title, status_message);
}

static void set_loading_message() {
  snprintf(title, sizeof(title), "Loading...");
  snprintf(amount, sizeof(amount), "0.00");
  text_layer_set_text(textl_title, title);
  text_layer_set_text(textl_amount, amount);
}

// Main window button handlers
static void select_click_handler(ClickRecognizerRef recog, void *context) {
  Window *w = create_add_window();
  window_stack_push(w, true);
}

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

static void long_click_up_handler(ClickRecognizerRef recog, void *context) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "long_click_up_handler");    
}

static void long_click_handler(ClickRecognizerRef recog, void *context) {
  // TODO make this into a refresh
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT, 0, long_click_handler, long_click_up_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void main_window_load(Window *window) {
  // ready message to Pebblekit JS will trigger 
  // first update of text, no need to call explicitly
  
  // setup action bar
  action_bar = action_bar_layer_create();
  action_bar_layer_set_background_color(action_bar, GColorWhite);
  
  icon_up = gbitmap_create_with_resource(RESOURCE_ID_ACTION_BAR_ARROW_UP);
  icon_refresh = gbitmap_create_with_resource(RESOURCE_ID_ACTION_BAR_PLUS);
  icon_down = gbitmap_create_with_resource(RESOURCE_ID_ACTION_BAR_ARROW_DOWN);
  
  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, icon_up);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, icon_refresh);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, icon_down);
  
  action_bar_layer_set_click_config_provider(action_bar, click_config_provider);

  // setup text layers
  textl_title = text_layer_create(GRect(5, 35, 144, 30));
  text_layer_set_font(textl_title, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  
  textl_amount = text_layer_create(GRect(5, 65, 144, 100));
  //text_layer_set_font(textl_amount, fonts_get_system_font(FONT_KEY_LECO_36_BOLD_NUMBERS));
  // text_layer_set_font(textl_amount, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_font(textl_amount, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  
  layer_add_child(window_get_root_layer(me), text_layer_get_layer(textl_title));
  layer_add_child(window_get_root_layer(me), text_layer_get_layer(textl_amount));
  
  // add action bar last so it overlaps the text	
  action_bar_layer_add_to_window(action_bar, me);

  set_loading_message();
}

static void main_window_unload(Window *window) {
  text_layer_destroy(textl_title);  
  text_layer_destroy(textl_amount);
  
  action_bar_layer_destroy(action_bar);
  gbitmap_destroy(icon_up);
  gbitmap_destroy(icon_down); 
  gbitmap_destroy(icon_refresh);
}

Window *create_main_window() {
  me = window_create();
  window_set_window_handlers(me, (WindowHandlers) { .load = main_window_load, .unload = main_window_unload });
  window_stack_push(me, false);

  return me;
}

void destroy_main_window() {
  if (status_message != NULL)
    free(status_message);
  window_destroy(me);
}
