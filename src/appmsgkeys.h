#include <pebble.h>

void setup_appmsg(void);

#define MSGKEY_TYPE 0
#define MSGKEY_AMOUNT 1
#define MSGKEY_NAME 2
#define MSGKEY_NEWEXPENSEDESC 3
#define MSGKEY_NEWEXPENSECAT 4
#define MSGKEY_NEWEXPENSEVALUE 5
#define MSGKEY_STATUSTEXT 6

typedef enum {
  TYPE_ADD,
  TYPE_CANCEL,
  TYPE_LOAD,
  TYPE_STATUS
} APPMSG_TYPE;

