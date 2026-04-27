#include "crypto.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int char_to_base64_idx(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return 26 + (c - 'a');
    if (c >= '0' && c <= '9') return 52 + (c - '0');
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

int is_valid_caesar_shift(int shift) {
    return shift >= 1 && shift <= 25;
}

int is_valid_vigenere_key(const char *key) {
    if (!key || strlen(key) == 0) return 0;
    for (size_t i = 0; key[i]; i++) {
        if (!isalpha((unsigned char)key[i])) return 0;
    }
    return 1;
}

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

void atbash_transform(const char *input, char *output) {
    if (!input || !output) return;
    size_t i = 0;
    while (input[i]) {
        if (isupper((unsigned char)input[i])) {
            output[i] = (char)('Z' - (input[i] - 'A'));
        } else if (islower((unsigned char)input[i])) {
            output[i] = (char)('z' - (input[i] - 'a'));
        } else {
            output[i] = input[i];
        }
        i++;
    }
    output[i] = '\0';
}

void base64_encode(const char *input, char *output) {
    if (!input || !output) return;
    size_t len = strlen(input);
    size_t i = 0;
    size_t j = 0;
    while (i < len) {
        unsigned char b1 = i < len ? (unsigned char)input[i++] : 0;
        unsigned char b2 = i < len ? (unsigned char)input[i++] : 0;
        unsigned char b3 = i < len ? (unsigned char)input[i++] : 0;
        output[j++] = base64_table[b1 >> 2];
        output[j++] = base64_table[((b1 & 0x03) << 4) | (b2 >> 4)];
        output[j++] = (i > len - 1) ? '=' : base64_table[((b2 & 0x0F) << 2) | (b3 >> 6)];
        output[j++] = (i > len) ? '=' : base64_table[b3 & 0x3F];
    }
    output[j] = '\0';
}

void base64_decode(const char *input, char *output) {
    if (!input || !output) return;
    size_t len = strlen(input);
    size_t i = 0;
    size_t j = 0;
    while (i < len) {
        int idx1 = char_to_base64_idx(input[i++]);
        int idx2 = (i < len) ? char_to_base64_idx(input[i++]) : 0;
        int idx3 = (i < len) ? char_to_base64_idx(input[i++]) : 0;
        int idx4 = (i < len) ? char_to_base64_idx(input[i++]) : 0;
        if (idx1 < 0 || idx2 < 0) break;
        unsigned char b1 = (unsigned char)((idx1 << 2) | (idx2 >> 4));
        unsigned char b2 = (unsigned char)((idx3 >= 0) ? ((idx2 & 0x0F) << 4) | (idx3 >> 2) : 0);
        unsigned char b3 = (unsigned char)((idx4 >= 0) ? ((idx3 & 0x03) << 6) | idx4 : 0);
        output[j++] = b1;
        if (idx3 >= 0 && idx3 != 64) output[j++] = b2;
        if (idx4 >= 0 && idx4 != 64) output[j++] = b3;
    }
    output[j] = '\0';
}