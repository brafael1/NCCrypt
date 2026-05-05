#include "base64.h"
#include <string.h>

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int char_to_base64_idx(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return 26 + (c - 'a');
    if (c >= '0' && c <= '9') return 52 + (c - '0');
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
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