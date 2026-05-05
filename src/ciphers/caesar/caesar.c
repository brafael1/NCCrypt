#include "caesar.h"
#include <ctype.h>
#include <string.h>

int is_valid_caesar_shift(int shift) {
    return shift >= 1 && shift <= 25;
}

void caesar_encrypt(const char *input, char *output, int shift) {
    if (!input || !output) return;
    int s = ((shift % 26) + 26) % 26;
    size_t i = 0;
    while (input[i]) {
        if (isalpha((unsigned char)input[i])) {
            char base = isupper((unsigned char)input[i]) ? 'A' : 'a';
            output[i] = (char)(((input[i] - base + s) % 26) + base);
        } else {
            output[i] = input[i];
        }
        i++;
    }
    output[i] = '\0';
}

void caesar_decrypt(const char *input, char *output, int shift) {
    int s = ((-shift) % 26 + 26) % 26;
    caesar_encrypt(input, output, s);
}