#pragma once
#include <pebble.h>

// TODO make name and amount pointers and dynamically allocate
typedef struct BudgetLine {
  char name[20];
  char amount[10];
  struct BudgetLine *prev;
  struct BudgetLine *next;
} BudgetLine;

typedef struct BudgetList {
  BudgetLine *first;
  BudgetLine *last;
} BudgetList;

BudgetLine *storeBudgetLine(char *name, char *amount);
void freeBudgetData(void);