#include <pebble.h>
#include <math.h>
#include <limits.h>

int float_to_int_dpart(float f) {
  return (int)trunc(f);
}

int float_to_int_fpart(float f) {
  int dpart = float_to_int_dpart(f);
  float fpart = (f - trunc(f)) * 100;
  int r = round(fpart);
  return (int)fpart;
}

void format_float_as_string(char **formatted, float f) {
  int slen, dollars, cents;
  const int INITSLEN = 6;  /* most expenses are no more than $10 ie X.XX\0" */
  
  dollars = float_to_int_dpart(f);
  cents = float_to_int_fpart(f);

  *formatted = (char*)malloc(INITSLEN);
  if (*formatted == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "format_float_as_string: malloc failed");
  } else {
    slen = snprintf(*formatted, INITSLEN, "%d.%02d", dollars, cents);
    if (slen > INITSLEN) {
      *formatted = (char*)realloc(*formatted, slen+1);
      if (*formatted == NULL) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "format_float_as_string: malloc failed");
      }
    }
  }
}
