#pragma once
#include <pebble.h>
#include "BudgetLine.h"

// UI interface to main
void setup_ui(void);
void destroy_ui(void);
void display_BudgetLine(BudgetLine *line);
void ui_display_error(char *errormsg);
void display_status_message(char *message);

// main window
Window *create_main_window(void);
void destroy_main_window();

// add window
Window *create_add_window(void);
void destroy_add_window(void);

// add result window
Window *create_addresult_window(void);
void destroy_addresult_window(void);
