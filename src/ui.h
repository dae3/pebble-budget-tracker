#pragma once
#include <pebble.h>
#include "BudgetLine.h"

// UI interface to main
void ui_setup(void);
void ui_destroy(void);
void display_BudgetLine(BudgetLine *line);
void ui_display_error(char *errormsg);

// main window
Window *create_main_window(void);
void destroy_main_window();

// add window
Window *create_add_window(void);
void destroy_add_window(void);
