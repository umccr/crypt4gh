#include <ctype.h>
#include <errno.h>
#include <grp.h>
#include <strings.h>

#include "utils.h"
#include "config.h"

#define CFGFILE "/etc/ega/auth.conf"

options_t* options = NULL;
char* syslog_name = "C4GH";

void
cleanconfig(void)
{
  if(!options) return;
  D2("Cleaning configuration [%p]", options);

  if(options->buffer){ free((char*)options->buffer); }
  free(options);
  return;
}


bool
valid_options(void)
{
  bool valid = true;
  if(!options) { D3("No config struct"); return false; }

  D2("Checking the config struct");
  if(!valid){ D3("Invalid config struct from %s", options->cfgfile); }
  return valid;
}

#define INJECT_OPTION(key,ckey,val,loc) do { if(!strcmp(key, ckey) && copy2buffer(val, &(loc), &buffer, &buflen) < 0 ){ return -1; } } while(0)
#define COPYVAL(val,dest) do { if( copy2buffer(val, &(dest), &buffer, &buflen) < 0 ){ return -1; } } while(0)

static inline int
readconfig(FILE* fp, char* buffer, size_t buflen)
{
  D3("Reading configuration file");
  _cleanup_str_ char* line = NULL;
  size_t len = 0;
  char *key,*eq,*val,*end;

  COPYVAL(CFGFILE   , options->cfgfile          );

  /* Parse line by line */
  while (getline(&line, &len, fp) > 0) {
	
    key=line;
    /* remove leading whitespace */
    while(isspace(*key)) key++;
      
    if((eq = strchr(line, '='))) {
      end = eq - 1; /* left of = */
      val = eq + 1; /* right of = */
	  
      /* find the end of the left operand */
      while(end > key && isspace(*end)) end--;
      *(end+1) = '\0';
	  
      /* find where the right operand starts */
      while(*val && isspace(*val)) val++;
	  
      /* find the end of the right operand */
      eq = val;
      while(*eq != '\0') eq++;
      eq--;
      if(*eq == '\n') { *eq = '\0'; } /* remove new line */
	  
    } else val = NULL; /* could not find the '=' sign */
	
  }
  return 0;
}

bool
loadconfig(void)
{
  D1("Loading configuration %s", CFGFILE);
  if(options){ D2("Already loaded [@ %p]", options); return true; }

  _cleanup_file_ FILE* fp = NULL;
  size_t size = 1024;
  
  /* read or re-read */
  fp = fopen(CFGFILE, "r");
  if (fp == NULL || errno == EACCES) { D2("Error accessing the config file: %s", strerror(errno)); return false; }

  options = (options_t*)malloc(sizeof(options_t));
  if(!options){ D3("Could not allocate options data structure"); return false; };
  options->buffer = NULL;

REALLOC:
  D3("Allocating buffer of size %zd", size);
  if(options->buffer)free(options->buffer);
  options->buffer = malloc(sizeof(char) * size);
  /* memset(options->buffer, '\0', size); */
  *(options->buffer) = '\0';
  if(!options->buffer){ D3("Could not allocate buffer of size %zd", size); return false; };

  if( readconfig(fp, options->buffer, size) < 0 ){
    size = size << 1; // double it
    goto REALLOC;
  }

  D2("Conf loaded [@ %p]", options);

#ifdef DEBUG
  return valid_options();
#else
  return true;
#endif
}
