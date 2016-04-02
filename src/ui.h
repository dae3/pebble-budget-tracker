#pragma once
#include "BudgetLine.h"

void ui_setup(void);
void ui_destroy(void);
void display_BudgetLine(BudgetLine *line);
void ui_display_error(char *errormsg);
BudgetLine *current_line;