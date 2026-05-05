#include "atbash.h"
#include <ctype.h>

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