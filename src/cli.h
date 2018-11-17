#ifndef __C4GH_ARGS_H_INCLUDED__
#define __C4GH_ARGS_H_INCLUDED__

#include <stdbool.h>
#include <sys/types.h>

#define C4GH_VERSION "GA4GH cryptographic utilities (version 1)"

/* DB schema */
#define C4GH_USAGE "\
\n\
LocalEGA utilities for the cryptographic GA4GH standard.\n\
Reads from stdin and Outputs to stdout\n\
\n\
Usage:\n\
   {PROG} [-hv] [--log <file>] encrypt [--signing_key <file>] [--pk <path>]\n\
   {PROG} [-hv] [--log <file>] decrypt [--sk <path>]\n\
   {PROG} [-hv] [--log <file>] reencrypt [--signing_key <file>] [--sk <path>] [--pk <path>]\n\
   {PROG} [-hv] [--log <file>] generate [-o <path>] [-P <passphrase>] [--signing] [-f PKCS8|SSH2|none]\n\
\n\
Options:\n\
   -h, --help             Prints this help and exit\n\
   -v, --version          Prints the version and exits\n\
   --log <file>           Path to the logger file (in YML format)\n\
   --pk <keyfile>         Public Curve25519 key to be used for encryption [default: ~/.c4gh/key.pub]\n\
   --sk <keyfile>         Private Curve25519 key to be used for decryption [default: ~/.c4gh/key]\n\
   --signing_key <file>   Ed25519 Signing key for the header\n\
   -o <path>              Private Curve25519 key (.pub is appended for the Public one) [default: ~/.c4gh/sign]\n\
   -P <passphrase>        Passphrase to lock the secret key [default: None]\n\
   --signing              Generate an ed25519 signing/verifying keypair\n\
   -f <fmt>               Key format: PKCS8, SSH2, or none [default: none]\n\
\n\
Environment variables:\n\
   C4GH_LOG         If defined, it will be used as the default logger\n\
   C4GH_PUBLIC_KEY  If defined, it will be used as the default public key (ie --pk ${{C4GH_PUBLIC_KEY}})\n\
   C4GH_SECRET_KEY  If defined, it will be used as the default secret key (ie --sk ${{C4GH_SECRET_KEY}})\n\
   C4GH_SIGNING_KEY If defined, it will be used as the default signing key (ie --signing_key ${{C4GH_SIGNING_KEY}})\n\
"

typedef struct {
  char* buffer;

  /* commands */
  enum cmd {
    oUSAGE,
    oVERSION,
    oENCRYPT,
    oDECRYPT,
    oREENCRYPT,
    oGENERATE
  } command;

  /* arguments */
  char* logfile;
  char* public_key;
  char* secret_key;
  char* signing_key;
  bool do_sign;
  char* output;
  enum format {
    oNONE,
    oPKCS8,
    oSSH2
  } output_format;
  char* passphrase;
} c4gh_args;

extern c4gh_args* args;

void clean_args(void);
int parse_args(const int argc, const char *argv[]); /* 0: all good, 1: help, 2: version, -1: error */
void print_args(void);

#endif /* !__C4GH_ARGS_H_INCLUDED__ */
