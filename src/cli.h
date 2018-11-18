#ifndef __C4GH_ARGS_H_INCLUDED__
#define __C4GH_ARGS_H_INCLUDED__

#include <stdbool.h>
#include <sys/types.h>

#define C4GH_VERSION "GA4GH cryptographic utilities (version 1)"

#define C4GH_USAGE "\n\
Cryptographic utility for the GA4GH encryption format.\n\
Reads from stdin and outputs to stdout\n\
\n\
Usage: crypt4gh <command> [options...]\n\
\n\
Commands:\n\
   * help, usage, version          \n\
   * encrypt, decrypt, reencrypt   \n\
   * generate                      \n\
\n\
Options:\n\
   -h, --help               Prints the help and exits\n\
   -V, --version            Prints the version and exits\n\
   -l <file>,               \n\
   --log <file>             Path to the logger file (in YML format)\n\
   -p <path>,               \n\
   --public_key <path>      Public Curve25519 key to be used for encryption [default: ~/.c4gh/key.pub]\n\
   -s <path>                \n\
   --secret_key <path>      Private Curve25519 key to be used for decryption [default: ~/.c4gh/key]\n\
   -S <path>,               \n\
   --signing_key <path>     Ed25519 Signing key for the header\n\
   -o <path>,               \n\
   --output <path>          Private Curve25519 key (.pub is appended for the Public one) [default: ~/.c4gh/sign]\n\
   -P <secret>,             \n\
   --passphrase <secret>    Passphrase to lock the secret key [default: None]\n\
   -k, --signing            Generate an ed25519 signing/verifying keypair\n\
   -f <fmt>,                \n\
   --format <fmt>           Key format: PKCS8, SSH2, or none [default: none]\n\
\n\
Environment variables:\n\
   C4GH_LOG         If defined, it will be used as the default logger\n\
   C4GH_PUBLIC_KEY  If defined, it will be used as the default public key (ie --pk ${{C4GH_PUBLIC_KEY}})\n\
   C4GH_SECRET_KEY  If defined, it will be used as the default secret key (ie --sk ${{C4GH_SECRET_KEY}})\n\
   C4GH_SIGNING_KEY If defined, it will be used as the default signing key (ie --signing_key ${{C4GH_SIGNING_KEY}})\n\
"

typedef enum cmd {
  oUNKNOWN,
  oUSAGE,
  oVERSION,
  oENCRYPT,
  oDECRYPT,
  oREENCRYPT,
  oGENERATE
} command;

typedef enum format {
  oNONE,
  oPKCS8,
  oSSH2
} format;

typedef struct {
  char* buffer;

  /* commands */
  command command;

  /* arguments */
  char* logfile;
  char* public_key;
  char* secret_key;
  char* signing_key;
  bool do_sign;
  char* output;
  format output_format;
  char* passphrase;
} c4gh_args_t;

extern c4gh_args_t* c4gh_args;

void clean_args(void);
int parse_args(const int argc, char *const argv[]); /* 0: all good, -1: bad arg, -2: error */
void print_args(void);

#endif /* !__C4GH_ARGS_H_INCLUDED__ */
