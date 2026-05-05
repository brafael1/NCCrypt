#ifndef VIGENERE_H
#define VIGENERE_H

void vigenere_encrypt(const char *input, char *output, const char *key);
void vigenere_decrypt(const char *input, char *output, const char *key);
int is_valid_vigenere_key(const char *key);

#endif