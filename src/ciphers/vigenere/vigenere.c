#include "vigenere.h"
#include <ctype.h>
#include <string.h>

int is_valid_vigenere_key(const char *key) {
    if (!key || strlen(key) == 0) return 0;
    for (size_t i = 0; key[i]; i++) {
        if (!isalpha((unsigned char)key[i])) return 0;
    }
    return 1;
}

void vigenere_encrypt(const char *input, char *output, const char *key) {
    if (!input || !output || !key) return;
    size_t key_len = strlen(key);
    if (key_len == 0) {
        strcpy(output, input);
        return;
    }
    size_t i = 0;
    size_t key_idx = 0;
    while (input[i]) {
        if (isalpha((unsigned char)input[i])) {
            char base = isupper((unsigned char)input[i]) ? 'A' : 'a';
            char k = toupper((unsigned char)key[key_idx % key_len]);
            int shift = k - 'A';
            output[i] = (char)(((input[i] - base + shift) % 26) + base);
            key_idx++;
        } else {
            output[i] = input[i];
        }
        i++;
    }
    output[i] = '\0';
}

void vigenere_decrypt(const char *input, char *output, const char *key) {
    if (!input || !output || !key) return;
    size_t key_len = strlen(key);
    if (key_len == 0) {
        strcpy(output, input);
        return;
    }
    size_t i = 0;
    size_t key_idx = 0;
    while (input[i]) {
        if (isalpha((unsigned char)input[i])) {
            char base = isupper((unsigned char)input[i]) ? 'A' : 'a';
            char k = toupper((unsigned char)key[key_idx % key_len]);
            int shift = k - 'A';
            int new_shift = (26 - shift) % 26;
            output[i] = (char)(((input[i] - base + new_shift) % 26) + base);
            key_idx++;
        } else {
            output[i] = input[i];
        }
        i++;
    }
    output[i] = '\0';
}