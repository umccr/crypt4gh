#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"
#include "cli.h"

c4gh_args* args = NULL;

static int _parse_args(const int argc, const char** argv, char* buf, int buflen);

/**
   0: all good
   1: help
   2: version
   -1: bad args
   -2: worse error
*/
int parse_args(const int argc, const char *argv[]) {
  size_t size = 1024;

  args = (c4gh_args*)malloc(sizeof(c4gh_args));
  if(!args){ D3("Could not allocate args data structure"); return -2; };
  args->buffer = NULL;

REALLOC:
  D3("Allocating buffer of size %zd", size);
  if(args->buffer)free(args->buffer);
  args->buffer = malloc(sizeof(char) * size);
  /* memset(args->buffer, '\0', size); */
  *(args->buffer) = '\0';
  if(!args->buffer){ D3("Could not allocate buffer of size %zd", size); return -2; };

  if( _parse_args(argc, argv, args->buffer, size) < 0 ){
    size = size << 1; // double it
    goto REALLOC;
  }


  args->command = oUSAGE;
  args->logfile = NULL;
  args->public_key = NULL;
  args->secret_key = NULL;
  args->signing_key = NULL;
  args->do_sign = false;
  args->output = NULL;
  args->output_format = oNONE;
  args->passphrase = NULL;

  return 0;
}

void
clean_args(void){
  if(!args) return;
  D2("Delete args struct [%p]", args);
  if(args->buffer){ free((char*)args->buffer); }
  free(args);
  return;
}

static int
_parse_args(const int argc, const char** argv, char* buf, int buflen){
  return 0;
}

void
print_args(void){
  if(!args){ D1("No args"); return; }

  switch(args->command){
  case oUSAGE:
    D1("Command: help/usage");
    printf("%s\n", C4GH_USAGE);
    break;
  case oVERSION:
    D1("Command: version");
    printf("%s\n", C4GH_VERSION);
    break;
  case oENCRYPT:
    D1("Command: encrypt");
    break;
  case oDECRYPT:
    D1("Command: decrypt");
    break;
  case oREENCRYPT:
    D1("Command: reencrypt");
    break;
  case oGENERATE:
    D1("Command: generate");
    break;
  default:
    D1("Command: unknown");
    break;
  }

  D1("logfile: %s", args->logfile);
  D1("pk: %s", args->public_key);
  D1("sk: %s", args->secret_key);
  D1("signing key: %s", args->signing_key);
  D1("Do sign: %s", (args->do_sign)?"true":"false");

  D1("output: %s", args->output);
  D1("passphrase: %s", args->passphrase);

  switch(args->output_format){
  case oPKCS8:
    D1("Format: PKCS#8");
    break;
  case oSSH2:
    D1("Format: SSH2");
    break;
  case oNONE:
    D1("Format: none");
    break;
  default:
    D1("Format: unknown");
    break;
  }
  return;
}
