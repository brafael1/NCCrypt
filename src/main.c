#include "ui/ui.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    CryptAction action;
    CipherType cipher;

    ui_init();

    while (1) {
        action = ui_main_menu_loop();

        if (action == CRYPT_ACTION_EXIT) {
            break;
        }

        cipher = ui_cipher_menu_loop(action);

        if (cipher < 0) {
            continue;
        }

        ui_process_encrypt_decrypt(action, cipher);
    }

    ui_cleanup();

    return 0;
}