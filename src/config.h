
#ifndef __C4GH_CONFIG_H_INCLUDED__
#define __C4GH_CONFIG_H_INCLUDED__

#include <stdbool.h>
#include <sys/types.h> 

struct options_s {
  char* cfgfile;
  char* buffer;
};

typedef struct options_s options_t;

extern options_t* options;

bool loadconfig(void);
void cleanconfig(void);

#endif /* !__C4GH_CONFIG_H_INCLUDED__ */
