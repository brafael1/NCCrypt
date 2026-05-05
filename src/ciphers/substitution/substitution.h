#ifndef SUBSTITUTION_H
#define SUBSTITUTION_H

void substitution_encrypt(const char *input, char *output, const char *key);
void substitution_decrypt(const char *input, char *output, const char *key);
int is_valid_substitution_key(const char *key);

#endif