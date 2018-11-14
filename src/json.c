#include <string.h>

#include "utils.h"
#include "config.h"
#include "json.h"

#ifdef DEBUG
#define TYPE2STR(t) (((t) == JSMN_OBJECT)   ? "Object":    \
                     ((t) == JSMN_ARRAY)    ? "Array":     \
                     ((t) == JSMN_STRING)   ? "String":    \
                     ((t) == JSMN_PRIMITIVE)? "Primitive": \
                                              "Undefined")
#endif


#define KEYEQ(json, t, s) ((int)strlen(s) == ((t)->end - (t)->start)) && strncmp((json) + (t)->start, s, (t)->end - (t)->start) == 0

int
parse_json(const char* json, int jsonlen,
	   char** username, char** pwd, char** pbk, char** gecos, int* uid)
{
  return 0;
}
