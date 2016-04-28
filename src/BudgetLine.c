#include <pebble.h>
#include "appmsgkeys.h"
#include "BudgetLine.h"

static BudgetList *budget_data;
// static BudgetLine *budget_data;

void freeBudgetData(void) {
  BudgetLine *line, *next;
    
  line = budget_data->first;
  while (line) {
    next = line->next;
    free(line);
    line = next;
  }
  
  budget_data->first = NULL;
  budget_data->last = NULL;

	free(budget_data);
}

BudgetLine *storeBudgetLine(char *name, char *amount) {
  BudgetLine *line = NULL;
  
  if (budget_data == NULL) {
    budget_data = calloc(1, sizeof(*budget_data));
  }  
    
  if (budget_data != NULL) {
    line = calloc(1, sizeof(*line));    
    if (line != NULL) {
      if (budget_data->last == NULL) {   
        budget_data->first = line;
        budget_data->last = line;
      } else {
        budget_data->last->next = line;
        line->prev = budget_data->last;    
        budget_data->last = line;    
      }
      
      // TODO dynamically allocate strings
      strncpy(line->name, name, sizeof(line->name));
      strncpy(line->amount, amount, sizeof(line->amount));
      
    } else {
      APP_LOG(APP_LOG_LEVEL_ERROR, "storeBudgetLine malloc of BudgetLine failed");  
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "storeBudgetLine malloc of BudgetList failed");
  }
  return(line);
}
      
