#include <stdio.h>
#include <sys/types.h>

#include "utils.h"
#include "crypt4gh.h"
#include "cli.h"

int
main(int argc, char *const argv[])
{
  atexit(clean_args);

  int rc = parse_args(argc,argv);

  if(rc){
    D1("Oh oh... %d", rc);
    return rc;
  }

  if(c4gh_args->command == oUSAGE){
    printf("%s\n", C4GH_USAGE);
    return 0; /* exit */
  }

  if(c4gh_args->command == oVERSION){
    printf("%s\n", C4GH_VERSION);
    return 0; /* exit */
  }

  if(c4gh_args->command == oUNKNOWN){
    printf("%s\n", C4GH_USAGE);
    return 1; /* exit */
  }

  print_args();
  return 0;
}
