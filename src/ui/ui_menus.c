#include "ui_menus.h"
#include "ui_strings.h"
#include <ncurses.h>
#include <string.h>

const char *cipher_names[] = {
    "Cifra de Cesar",
    "Cifra de Vigenere",
    "Substituicao Simples",
    "Atbash",
    "Base64"
};

const char *cipher_keys[] = {
    "Cifra de Cesar (deslocamento 1-25)",
    "Cifra de Vigenere (chave alfabetica)",
    "Substituicao (26 letras unicas)",
    "Atbash (sem chave)",
    "Base64 (sem chave)"
};

void ui_draw_main_menu(CryptAction selected) {
    clear();

    int height = LINES;
    int width = COLS;

    attron(COLOR_PAIR(3));
    for (int i = 0; i < width; i++) {
        mvaddch(0, i, ACS_HLINE);
        mvaddch(height / 4, i, ACS_HLINE);
        mvaddch(height - 1, i, ACS_HLINE);
    }
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(3) | A_BOLD);
    int title_y = 2;
    int title_x = (width - 30) / 2;
    mvaddstr(title_y, title_x, "NCCrypt - FERRAMENTA DE CRIPTOGRAFIA");
    attroff(COLOR_PAIR(3) | A_BOLD);

    int menu_start_y = height / 4 + 2;
    int menu_start_x = width / 2 - 15;

    const char *options[] = {
        " Criptografar Mensagem ",
        " Descriptografar Mensagem ",
        " Sair "
    };

    CryptAction actions[] = {
        CRYPT_ACTION_ENCRYPT,
        CRYPT_ACTION_DECRYPT,
        CRYPT_ACTION_EXIT
    };

    for (int i = 0; i < 3; i++) {
        int y = menu_start_y + (i * 3);
        int x = menu_start_x;

        if (actions[i] == selected) {
            attron(COLOR_PAIR(2));
            mvaddstr(y, x - 1, ">");
            attroff(COLOR_PAIR(2));
            attron(COLOR_PAIR(4) | A_BOLD);
        } else {
            attron(COLOR_PAIR(1));
        }

        mvaddstr(y, x, options[i]);
        attroff(COLOR_PAIR(1) | A_BOLD);
    }

    int help_y = height - 2;
    int help_x = 2;
    attron(COLOR_PAIR(1));
    mvaddstr(help_y, help_x, "Use: Arrow Keys=Navigate  ENTER=Select  ESC=Back");
    attroff(COLOR_PAIR(1));

    refresh();
}

void ui_draw_cipher_menu(CipherType selected) {
    clear();

    int height = LINES;
    int width = COLS;

    attron(COLOR_PAIR(3));
    for (int i = 0; i < width; i++) {
        mvaddch(2, i, ACS_HLINE);
    }
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(3) | A_BOLD);
    mvaddstr(1, (width - 25) / 2, "SELECIONE O ALGORITMO");
    attroff(COLOR_PAIR(3) | A_BOLD);

    int menu_start_y = 5;
    int menu_start_x = width / 2 - 20;

    for (int i = 0; i < CIPHER_COUNT; i++) {
        int y = menu_start_y + (i * 3);
        int x = menu_start_x;

        if ((CipherType)i == selected) {
            attron(COLOR_PAIR(2));
            mvaddstr(y, x - 1, ">");
            attroff(COLOR_PAIR(2));
            attron(COLOR_PAIR(4) | A_BOLD);
        } else {
            attron(COLOR_PAIR(1));
        }

        mvaddstr(y, x, cipher_names[i]);
        attroff(COLOR_PAIR(1) | A_BOLD);
    }

    int help_y = height - 2;
    attron(COLOR_PAIR(1));
    mvaddstr(help_y, 2, "Use: Arrow Keys=Navigate  ENTER=Select  ESC=Back");
    attroff(COLOR_PAIR(1));

    refresh();
}

int ui_main_menu_loop(void) {
    CryptAction selected = CRYPT_ACTION_ENCRYPT;
    CryptAction current = selected;
    int running = 1;
    int key;

    while (running) {
        ui_draw_main_menu(current);
        key = getch();

        switch (key) {
            case KEY_UP:
                if (current > CRYPT_ACTION_ENCRYPT) {
                    current--;
                }
                break;
            case KEY_DOWN:
                if (current < CRYPT_ACTION_EXIT) {
                    current++;
                }
                break;
            case '\n':
            case KEY_ENTER:
                running = 0;
                selected = current;
                break;
            case 27:
                selected = CRYPT_ACTION_EXIT;
                running = 0;
                break;
        }
    }

    return selected;
}

int ui_cipher_menu_loop(CryptAction action) {
    (void)action;
    CipherType selected = CIPHER_CAESAR;
    CipherType current = selected;
    int running = 1;
    int key;

    while (running) {
        ui_draw_cipher_menu(current);
        key = getch();

        switch (key) {
            case KEY_UP:
                if (current > 0) {
                    current--;
                }
                break;
            case KEY_DOWN:
                if (current < CIPHER_COUNT - 1) {
                    current++;
                }
                break;
            case '\n':
            case KEY_ENTER:
                running = 0;
                selected = current;
                break;
            case 27:
                running = 0;
                selected = -1;
                break;
        }
    }

    return selected;
}