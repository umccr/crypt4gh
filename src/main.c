#include <stdio.h>
#include <sys/types.h>

#include "utils.h"
#include "crypt4gh.h"
#include "cli.h"

int
main(int argc, const char **argv)
{

  parse_args(argc,argv);

  print_args();

  clean_args();

  return 0;
  //return (sftp_server_main(argc, argv));
}
