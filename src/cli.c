#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "utils.h"
#include "cli.h"
#include "misc.h"

c4gh_args_t*  c4gh_args = NULL;

static int _parse_args(int argc, char *const *argv, char* buffer, size_t buflen);

/**
   0: all good
   1: bad args
   2: too many args
   -2: allocation error
*/
int parse_args(const int argc, char *const argv[]) {
  int rc = 0;
  size_t size = 1024;

  c4gh_args = (c4gh_args_t*)malloc(sizeof(c4gh_args_t));
  if(!c4gh_args){ D3("Could not allocate c4gh_args data structure"); return -2; };
  c4gh_args->buffer = NULL;

REALLOC:
  D3("Allocating buffer of size %zd", size);
  if(c4gh_args->buffer)free(c4gh_args->buffer);
  c4gh_args->buffer = malloc(sizeof(char) * size);
  /* memset(c4gh_args->buffer, '\0', size); */
  *(c4gh_args->buffer) = '\0';
  if(!c4gh_args->buffer){ D3("Could not allocate buffer of size %zd", size); return -2; };

  if( (rc = _parse_args(argc, argv, c4gh_args->buffer, size)) == -1 ){
    size = size << 1; // double it
    goto REALLOC;
  }
  return rc;
}

void
clean_args(void){
  if(!c4gh_args) return;
  D2("Delete c4gh_args struct [%p]", c4gh_args);
  if(c4gh_args->buffer){ free((char*)c4gh_args->buffer); }
  free(c4gh_args);
  return;
}


static struct option long_options[] = {
  {"help"       , no_argument      , NULL, 'h' },
  {"version"    , no_argument      , NULL, 'v' },
  {"log"        , required_argument, NULL, 'l' },
  {"pk"         , required_argument, NULL, 'p' },
  {"sk"         , required_argument, NULL, 's' },
  {"signing_key", required_argument, NULL, 'S' },
  {"output"     , required_argument, NULL, 'o' },
  {"passphrase" , required_argument, NULL, 'P' },
  {"signing"    , no_argument      , NULL, 'k' },
  {"format"     , required_argument, NULL, 'f' },
  {NULL,0,NULL,0}
};

static char short_options[] = "hvl:p:s:So:P:kf:";

/**
   0: all good
   -1: buffer too small
   -2: worse error
   1: bad arg
   2: too many args
*/
static int
_parse_args(int argc, char *const *argv, char* buffer, size_t buflen){

  int option_index = 0;
  D1("Internal c4gh_args parsing");

  /* Defaults */
  c4gh_args->logfile = NULL;
  char* logfile = getenv("C4GH_LOG");
  D3("env log file: %s", logfile);
  if(logfile && copy2buffer(logfile, &(c4gh_args->logfile), &buffer, &buflen) < 0){ return -1; }

  c4gh_args->public_key = NULL;
  char* public_key = getenv("C4GH_PUBLIC_KEY");
  D3("env public key: %s", public_key);
  if(public_key && copy2buffer(public_key, &(c4gh_args->public_key), &buffer, &buflen) < 0){ return -1; }

  c4gh_args->secret_key = NULL;
  char* secret_key = getenv("C4GH_SECRET_KEY");
  D3("env secret key: %s", secret_key);
  if(secret_key && copy2buffer(secret_key, &(c4gh_args->secret_key), &buffer, &buflen) < 0){ return -1; }

  c4gh_args->signing_key = NULL;
  char* signing_key = getenv("C4GH_SIGNING_KEY");
  D3("env signing key: %s", signing_key);
  if(signing_key && copy2buffer(signing_key, &(c4gh_args->signing_key), &buffer, &buflen) < 0){ return -1; }
	      
  c4gh_args->do_sign = false;
  c4gh_args->output = NULL;
  c4gh_args->output_format = oNONE;
  c4gh_args->passphrase = NULL;

  /* command */
  c4gh_args->command = oUSAGE; /* if no args */
  if(argc>1) c4gh_args->command = command_from_string(argv[1]);

  D3("command: %d", c4gh_args->command);

  if(c4gh_args->command == oUSAGE ||
     c4gh_args->command == oVERSION) return 0; /* early exit */

  argc--; argv++; /* parse what's after the command */
  
  char c;
  while( (c = getopt_long (argc, argv, short_options, long_options, &option_index)) >= 0){
    D1("------ optind: %d | argc: %d", optind, argc);
    switch (c){
    case 'h':
      c4gh_args->command = oUSAGE; return 0;
      break;
    case 'v':
      c4gh_args->command = oVERSION; return 0;
      break;
    case 'l':
      if( copy2buffer(optarg, &(c4gh_args->logfile), &buffer, &buflen) < 0 ) return -1;
      break;
    case 'p':
      D1("got a pk: %s", optarg);
      if( copy2buffer(optarg, &(c4gh_args->public_key), &buffer, &buflen) < 0 ) return -1;
      break;
    case 's':
      if( copy2buffer(optarg, &(c4gh_args->secret_key), &buffer, &buflen) < 0 ) return -1;
      break;
    case 'S':
      if( copy2buffer(optarg, &(c4gh_args->signing_key), &buffer, &buflen) < 0 ) return -1;
      break;
    case 'k':
      c4gh_args->do_sign = true;
      break;
    case 'o':
      if( copy2buffer(optarg, &(c4gh_args->output), &buffer, &buflen) < 0) return -1;
      break;
    case 'f':
      c4gh_args->output_format = format_from_string(optarg);
      break;
    case 'P':
      if( copy2buffer(optarg, &(c4gh_args->passphrase), &buffer, &buflen) < 0 ) return -1;
      break;
    default:
      D1("unknown arg: %s at position %d", optarg, optind);
      return 1; /* bad arg */
    }
  }

  D1("optind: %d | argc: %d", optind, argc);
  if(optind<argc){
#if DEBUG
    D1("remaining args:");
    while (optind < argc) D2("- %s", argv[optind++]);
#endif
    return 2; /* too many args */
  }
  return 0;
}


void
print_args(void){
  if(!c4gh_args){ D1("No c4gh_args"); return; }

  switch(c4gh_args->command){
  case oUSAGE:
    printf("Command: help/usage\n");
    break;
  case oVERSION:
    printf("Command: version\n");
    break;
  case oENCRYPT:
    printf("Command: encrypt\n");
    break;
  case oDECRYPT:
    printf("Command: decrypt\n");
    break;
  case oREENCRYPT:
    printf("Command: reencrypt\n");
    break;
  case oGENERATE:
    printf("Command: generate\n");
    break;
  default:
    printf("Command: unknown\n");
    break;
  }

  printf("logfile: %s\n", c4gh_args->logfile);
  printf("pk: %s\n", c4gh_args->public_key);
  printf("sk: %s\n", c4gh_args->secret_key);
  printf("signing key: %s\n", c4gh_args->signing_key);
  printf("Do sign: %s\n", (c4gh_args->do_sign)?"true":"false");

  printf("output: %s\n", c4gh_args->output);
  printf("passphrase: %s\n", c4gh_args->passphrase);

  switch(c4gh_args->output_format){
  case oPKCS8:
    printf("Format: PKCS#8\n");
    break;
  case oSSH2:
    printf("Format: SSH2\n");
    break;
  case oNONE:
    printf("Format: none\n");
    break;
  default:
    printf("Format: unknown\n");
    break;
  }
  return;
}
