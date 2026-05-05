#include "substitution.h"
#include <ctype.h>
#include <string.h>

int is_valid_substitution_key(const char *key) {
    if (!key || strlen(key) != 26) return 0;
    int seen[128] = {0};
    for (size_t i = 0; i < 26; i++) {
        char c = toupper((unsigned char)key[i]);
        if (c < 'A' || c > 'Z' || seen[(unsigned char)c]) return 0;
        seen[(unsigned char)c] = 1;
    }
    return 1;
}

void substitution_encrypt(const char *input, char *output, const char *key) {
    if (!input || !output || !key) return;
    size_t i = 0;
    while (input[i]) {
        if (isupper((unsigned char)input[i])) {
            output[i] = toupper(key[input[i] - 'A']);
        } else if (islower((unsigned char)input[i])) {
            output[i] = tolower(key[input[i] - 'a']);
        } else {
            output[i] = input[i];
        }
        i++;
    }
    output[i] = '\0';
}

void substitution_decrypt(const char *input, char *output, const char *key) {
    if (!input || !output || !key) return;
    char rev_key[26];
    for (int i = 0; i < 26; i++) {
        rev_key[toupper(key[i]) - 'A'] = 'A' + i;
    }
    size_t i = 0;
    while (input[i]) {
        if (isupper((unsigned char)input[i])) {
            output[i] = rev_key[input[i] - 'A'];
        } else if (islower((unsigned char)input[i])) {
            output[i] = tolower(rev_key[input[i] - 'a']);
        } else {
            output[i] = input[i];
        }
        i++;
    }
    output[i] = '\0';
}