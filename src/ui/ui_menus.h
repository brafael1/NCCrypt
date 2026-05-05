#ifndef UI_MENUS_H
#define UI_MENUS_H

#include "ui_types.h"

void ui_draw_main_menu(CryptAction selected);
void ui_draw_cipher_menu(CipherType selected);
int ui_main_menu_loop(void);
int ui_cipher_menu_loop(CryptAction action);

#endif