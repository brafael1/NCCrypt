#ifndef CAESAR_H
#define CAESAR_H

void caesar_encrypt(const char *input, char *output, int shift);
void caesar_decrypt(const char *input, char *output, int shift);
int is_valid_caesar_shift(int shift);

#endif