Crypt4GH Key Format
===================

A key is stored in the following PEM format:


::

    -----BEGIN CRYPT4GH <type> KEY-----
    BASE64-ENCODED DATA
    -----END CRYPT4GH <type> KEY-----

where ``<type>`` is either PUBLIC, PRIVATE, or ENCRYPTED PRIVATE

For a non-encrypted key, whether a public key or a private key, the key data is the byte representation of the plaintext key material.

For an encryted key (especially for encrypted private keys), we use the following format.

::

   byte[]  MAGIC_WORD
   string  kdfname
   string  (rounds || salt)
   string  ciphername
   string  nonce (12 bytes) || encrypted data
   string  comment


The ``MAGIC_WORD`` is the byte-representation of the ASCII word "c4gh-v1".

Everything ``string`` consists of a length n (encoded as 2 big-endian bytes) and a sequence of n bytes.
For example, the ``string`` hello, is encoded as ``\x00\x05hello``.

The ``kdfname`` is the name of the Key Derivation Function. We support either ``scrypt``, ``pbkdf2_hmac_sha256`` or ``bcrypt``. The python implementation uses bcrypt when generating keys.

``rounds`` is a 4 big-endian bytes representation of the number of iterations used in the KDF.

The ``ciphername`` describes which symmetric algorithm is used to generate the encrypted data, as follows.
The only supported cipher is ``chacha20_poly1305`` (so far).

The KDF is used to derive a secret from a user-supplied passphrase.
A nonce is randomly generated, and used in conjonction with the secret to encrypt the private key, using Chacha20 and authenticated with Poly1305.
The nonce is prepended to the encrypted data.

Finally, an optional comment can be used at the end of the encoded format.
