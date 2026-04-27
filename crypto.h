#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>

void caesar_encrypt(const char *input, char *output, int shift);
void caesar_decrypt(const char *input, char *output, int shift);

void vigenere_encrypt(const char *input, char *output, const char *key);
void vigenere_decrypt(const char *input, char *output, const char *key);

void substitution_encrypt(const char *input, char *output, const char *key);
void substitution_decrypt(const char *input, char *output, const char *key);

void atbash_transform(const char *input, char *output);

void base64_encode(const char *input, char *output);
void base64_decode(const char *input, char *output);

int is_valid_caesar_shift(int shift);
int is_valid_vigenere_key(const char *key);
int is_valid_substitution_key(const char *key);

#endif