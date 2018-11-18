#include <string.h>

#include "utils.h"
#include "cli.h"

command
command_from_string(const char* s){
  if(!strcasecmp(s, "usage") ||
     !strcasecmp(s, "help") || !strcasecmp(s, "--help") || !strcasecmp(s, "-h")) return oUSAGE;
  if(!strcasecmp(s, "version") || !strcasecmp(s, "--version") || !strcmp(s, "-V")) return oVERSION;

  if(!strcasecmp(s, "encrypt")) return oENCRYPT;
  if(!strcasecmp(s, "decrypt")) return oDECRYPT;
  if(!strcasecmp(s, "reencrypt")) return oREENCRYPT;
  if(!strcasecmp(s, "generate")) return oGENERATE;
  return oUNKNOWN;
}

format
format_from_string(const char* s){
  if(!strcasecmp(s, "PKCS8")) return oPKCS8;
  if(!strcasecmp(s, "SSH2")) return oSSH2;
  return oNONE;
}
