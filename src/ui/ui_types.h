#ifndef UI_TYPES_H
#define UI_TYPES_H

#include <stddef.h>

#define MAX_INPUT_LENGTH 4096
#define MAX_OUTPUT_LENGTH 8192

typedef enum {
    CRYPT_ACTION_ENCRYPT,
    CRYPT_ACTION_DECRYPT,
    CRYPT_ACTION_EXIT
} CryptAction;

typedef enum {
    CIPHER_CAESAR,
    CIPHER_VIGENERE,
    CIPHER_SUBSTITUTION,
    CIPHER_ATBASH,
    CIPHER_BASE64,
    CIPHER_COUNT
} CipherType;

typedef enum {
    STATE_MAIN_MENU,
    STATE_CIPHER_MENU,
    STATE_INPUT,
    STATE_KEY,
    STATE_RESULT,
    STATE_EXIT
} AppState;

#endif