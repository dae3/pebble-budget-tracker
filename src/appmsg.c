#include <pebble.h>
#include "appmsgkeys.h"
#include "BudgetLine.h"
#include "ui.h"

// from utils.c
extern void debug_dump_dict(DictionaryIterator *iter);

static void handle_add_message(DictionaryIterator *iter) {
  Tuple *data_tuple1 = dict_find(iter, MSGKEY_NAME);
  Tuple *data_tuple2 = dict_find(iter, MSGKEY_AMOUNT);

  if (data_tuple1 == NULL || data_tuple2 == NULL)
    APP_LOG(APP_LOG_LEVEL_ERROR, "Got add appmessage but not name and amount");
  else {
    BudgetLine *line = storeBudgetLine(data_tuple1->value->cstring, data_tuple2->value->cstring);
    display_BudgetLine(line);
  }
}

static void inbox_received_callback(DictionaryIterator *iter, void *context) {
  Tuple *msgtype, *data_tuple1, *data_tuple2;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "AppMessage received");
  debug_dump_dict(iter);

  // determine action
  msgtype = dict_find(iter, MSGKEY_TYPE);
  if (msgtype == NULL)
    APP_LOG(APP_LOG_LEVEL_ERROR, "Received appmessage with no type");
  else {
    switch (msgtype->value->int8) {
    case TYPE_ADD:
      handle_add_message(iter);
      break;

    case TYPE_CANCEL:

      break;

    case TYPE_LOAD:
      
      break;

    case TYPE_STATUS:
      data_tuple1 = dict_find(iter, MSGKEY_STATUSTEXT);
      if (data_tuple1 == NULL)
	APP_LOG(APP_LOG_LEVEL_ERROR, "Received status appmessage without status text");

      display_status_message(data_tuple1->value->cstring);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Appmessage status %s", data_tuple1->value->cstring);
      break;

    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "Received appmessage with unknown type %d", msgtype->value->int8);
      break;
    }
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "appmessage dropped: %d", reason);
}

static void outbox_sent_callback(DictionaryIterator *iter, void *context) {
  Tuple *msgtype;
  
  msgtype = dict_find(iter, MSGKEY_TYPE);
  if (msgtype == NULL)
    APP_LOG(APP_LOG_LEVEL_ERROR, "outbox callback with no type");
  else {
    switch (msgtype->value->uint8) {
    case TYPE_ADD:
      APP_LOG(APP_LOG_LEVEL_DEBUG, "outbox callback for add");
      window_stack_push(create_addresult_window(), true);
      break;

    case TYPE_CANCEL:
      break;

    case TYPE_LOAD:
      break;

    case TYPE_STATUS:
      break;

    default:
      APP_LOG(APP_LOG_LEVEL_ERROR, "outbox callback with unknown type %d", msgtype->value->uint8);
      break;
    }
  }
  
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "outbox failed callback %d", reason);
}

void setup_appmsg(void) {
 
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  if (app_message_open(APP_MESSAGE_INBOX_SIZE_MINIMUM , APP_MESSAGE_INBOX_SIZE_MINIMUM ) != APP_MSG_OK) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "appmessage open failed");
  }
}
