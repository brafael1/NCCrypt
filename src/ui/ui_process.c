#include "ui_process.h"
#include "ui_input.h"
#include "ui_menus.h"
#include "ui_strings.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#include "../ciphers/caesar/caesar.h"
#include "../ciphers/vigenere/vigenere.h"
#include "../ciphers/substitution/substitution.h"
#include "../ciphers/atbash/atbash.h"
#include "../ciphers/base64/base64.h"

void ui_process_encrypt_decrypt(CryptAction action, CipherType cipher) {
    char input[MAX_INPUT_LENGTH] = {0};
    char key_buffer[256] = {0};
    char output[MAX_OUTPUT_LENGTH] = {0};
    const char *result_name = (action == CRYPT_ACTION_ENCRYPT) ? "Mensagem Criptografada" : "Mensagem Descriptografar";

    clear();
    int box_height = LINES * 3 / 4;
    int box_width = COLS * 3 / 4;
    int box_y = (LINES - box_height) / 2;
    int box_x = (COLS - box_width) / 2;

    WINDOW *input_win = newwin(box_height, box_width, box_y, box_x);
    wbkgd(input_win, COLOR_PAIR(1));
    box(input_win, 0, 0);
    mvwaddstr(input_win, 0, (box_width - 20) / 2, "ENTRADA DE DADOS");
    mvwaddstr(input_win, 2, 2, "Algoritmo: ");
    waddstr(input_win, cipher_names[cipher]);
    mvwaddstr(input_win, 3, 2, "Acao: ");
    waddstr(input_win, (action == CRYPT_ACTION_ENCRYPT) ? "Criptografar" : "Descriptografar");

    mvwaddstr(input_win, 5, 2, "Digite a mensagem:");
    wrefresh(input_win);

    echo();
    curs_set(1);
    wgetnstr(input_win, input, MAX_INPUT_LENGTH - 1);
    noecho();
    curs_set(0);

    int need_key = (cipher == CIPHER_CAESAR) || (cipher == CIPHER_VIGENERE) || (cipher == CIPHER_SUBSTITUTION);

    if (need_key) {
        mvwaddstr(input_win, 7, 2, cipher_keys[cipher]);
        wrefresh(input_win);
        echo();
        curs_set(1);
        wgetnstr(input_win, key_buffer, 254);
        noecho();
        curs_set(0);
    }

    int success = 1;

    switch (cipher) {
        case CIPHER_CAESAR: {
            int shift = atoi(key_buffer);
            if (!is_valid_caesar_shift(shift)) {
                success = 0;
            } else if (action == CRYPT_ACTION_ENCRYPT) {
                caesar_encrypt(input, output, shift);
            } else {
                caesar_decrypt(input, output, shift);
            }
            break;
        }
        case CIPHER_VIGENERE:
            if (!is_valid_vigenere_key(key_buffer)) {
                success = 0;
            } else if (action == CRYPT_ACTION_ENCRYPT) {
                vigenere_encrypt(input, output, key_buffer);
            } else {
                vigenere_decrypt(input, output, key_buffer);
            }
            break;
        case CIPHER_SUBSTITUTION:
            if (!is_valid_substitution_key(key_buffer)) {
                success = 0;
            } else if (action == CRYPT_ACTION_ENCRYPT) {
                substitution_encrypt(input, output, key_buffer);
            } else {
                substitution_decrypt(input, output, key_buffer);
            }
            break;
        case CIPHER_ATBASH:
            atbash_transform(input, output);
            break;
        case CIPHER_BASE64:
            if (action == CRYPT_ACTION_ENCRYPT) {
                base64_encode(input, output);
            } else {
                base64_decode(input, output);
            }
            break;
        default:
            success = 0;
    }

    delwin(input_win);
    clear();
    refresh();

    if (!success) {
        ui_message_box("ERRO", "Chave invalida! Verifique os requisitos.", 10, 50);
        return;
    }

    int res_height = LINES * 3 / 4;
    int res_width = COLS * 3 / 4;
    int res_y = (LINES - res_height) / 2;
    int res_x = (COLS - res_width) / 2;

    WINDOW *result_win = newwin(res_height, res_width, res_y, res_x);
    wbkgd(result_win, COLOR_PAIR(1));
    box(result_win, 0, 0);
    mvwaddstr(result_win, 0, (res_width - 18) / 2, "RESULTADO");
    mvwaddstr(result_win, 2, 2, result_name);
    mvwaddstr(result_win, 3, 2, output);

    mvwaddstr(result_win, res_height - 3, res_width / 2 - 10, "[ENTER] para continuar");
    wrefresh(result_win);

    int ch;
    while ((ch = wgetch(result_win)) != KEY_ENTER && ch != '\n' && ch != 27) {
    }

    delwin(result_win);
}