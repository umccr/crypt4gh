
`crypt4gh` is a tool to encrypt, decrypt or re-encrypt files
according to the [GA4GH cryptographic standard](https://github.com/daviesrob/hts-specs/tree/crypt4gh_improved).

# Installation

```
git clone https://github.com/EGA-archive/crypt4gh
cd crypt4gh/src
make
make install
```

# Usage

```bash
$ crypt4gh --help

Cryptographic utility for the GA4GH encryption format.
Reads from stdin and outputs to stdout

Usage: crypt4gh <command> [options...]

Commands:
   * help, usage, version
   * encrypt, decrypt, reencrypt
   * generate

Options:
   -h, --help               Prints the help and exits
   -V, --version            Prints the version and exits
   -l <file>,
   --log <file>             Path to the logger file (in YML format)
   -p <path>,
   --public_key <path>      Public Curve25519 key to be used for encryption [default: ~/.c4gh/key.pub]
   -s <path>,
   --secret_key <path>      Private Curve25519 key to be used for decryption [default: ~/.c4gh/key]
   -S <path>,
   --signing_key <path>     Ed25519 Signing key for the header
   -o <path>,
   --output <path>          Private Curve25519 key (.pub is appended for the Public one) [default: ~/.c4gh/sign]
   -P <secret>,
   --passphrase <secret>    Passphrase to lock the secret key [default: None]
   -k, --signing            Generate an ed25519 signing/verifying keypair
   -f <fmt>,
   --format <fmt>           Key format: PKCS8, SSH2, or none [default: none]

Environment variables:
   C4GH_LOG         If defined, it will be used as the default logger
   C4GH_PUBLIC_KEY  If defined, it will be used as the default public key (ie --pk ${{C4GH_PUBLIC_KEY}})
   C4GH_SECRET_KEY  If defined, it will be used as the default secret key (ie --sk ${{C4GH_SECRET_KEY}})
   C4GH_SIGNING_KEY If defined, it will be used as the default signing key (ie --signing_key ${{C4GH_SIGNING_KEY}})
```

# Examples

If you want to encrypt a file:

```bash
$ crypt4gh encrypt --pk path_to_pubkey < file > file.c4gh
```

If you want to decrypt a file:

```bash
$ crypt4gh decrypt --sk path_to_seckey < file.c4gh > file
```
