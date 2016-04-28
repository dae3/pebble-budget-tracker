#include <pebble.h>
#include "appmsgkeys.h"
#include "ui.h"
#include "BudgetLine.h"

// AppMessage functions
static void inbox_received_callback(DictionaryIterator *iter, void *context) {  
  Tuple *t, *tuple_name, *tuple_amount;
  BudgetLine *line;
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox_received_callback in");
  
  t = dict_find(iter, MSGKEY_TIMEOUT);
  if (t != NULL) {
    // XMLHTTPRequest to GAS application has timed out; data is the timeout period in ms
    APP_LOG(APP_LOG_LEVEL_ERROR, "timeout requesting data from GAS application after %d ms", t->value->int8);
    
  } else {
    tuple_name = dict_find(iter, MSGKEY_NAME);
    tuple_amount = dict_find(iter, MSGKEY_AMOUNT);
    line = storeBudgetLine(tuple_name->value->cstring, tuple_amount->value->cstring);
    display_BudgetLine(line);
  }
}  

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped: %d!", reason);
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "outbox sent callback");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "outbox failed callback %d", reason);
}

void handle_init(void) {
  ui_setup();
  
  // setup comms with Pebblekit JS code
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  if (app_message_open(APP_MESSAGE_INBOX_SIZE_MINIMUM , APP_MESSAGE_INBOX_SIZE_MINIMUM ) != APP_MSG_OK) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "appmessage open failed");
  }
}

void handle_deinit(void) {
  ui_destroy();
  freeBudgetData();
}

int main(void) {  
  handle_init();
  app_event_loop();
  handle_deinit();
}
