#ifndef UI_INPUT_H
#define UI_INPUT_H

#include <stddef.h>

int ui_message_box(const char *title, const char *message, int height, int width);
int ui_get_input(const char *prompt, char *buffer, int max_len);

#endif