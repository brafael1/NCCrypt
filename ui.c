#include "ui.h"
#include "crypto.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static const char *cipher_names[] = {
    "Cifra de Cesar",
    "Cifra de Vigenere",
    "Substituicao Simples",
    "Atbash",
    "Base64"
};

static const char *cipher_keys[] = {
    "Cifra de Cesar (deslocamento 1-25)",
    "Cifra de Vigenere (chave alfabetica)",
    "Substituicao (26 letras unicas)",
    "Atbash (sem chave)",
    "Base64 (sem chave)"
};

static int ui_message_box(const char *title, const char *message, int height, int width) {
    int box_height = height;
    int box_width = width;
    int box_y = (LINES - box_height) / 2;
    int box_x = (COLS - box_width) / 2;

    WINDOW *win = newwin(box_height, box_width, box_y, box_x);
    if (!win) return 0;

    wbkgd(win, COLOR_PAIR(1));
    box(win, 0, 0);

    int title_len = strlen(title);
    mvwaddch(win, 0, box_width / 2 - title_len / 2 - 1, ACS_RTEE);
    waddstr(win, title);
    waddch(win, ACS_LTEE);

    int msg_y = 2;
    const char *ptr = message;
    while (*ptr && msg_y < box_height - 2) {
        char line[256] = {0};
        int line_len = 0;
        while (*ptr && line_len < box_width - 4) {
            line[line_len++] = *ptr++;
        }
        mvwaddstr(win, msg_y++, 2, line);
    }

    mvwaddstr(win, box_height - 2, box_width / 2 - 6, "[ENTER]");
    wrefresh(win);

    int ch;
    while ((ch = wgetch(win)) != KEY_ENTER && ch != '\n' && ch != 27) {
    }

    delwin(win);
    return 1;
}

void ui_init(void) {
    initscr();
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    init_pair(4, COLOR_CYAN, COLOR_BLUE);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void ui_cleanup(void) {
    endwin();
}

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

        if (i == selected) {
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

int ui_get_input(const char *prompt, char *buffer, int max_len) {
    echo();
    curs_set(1);

    int y = LINES / 2 + 2;
    mvaddstr(y, 2, prompt);
    clrtoeol();
    refresh();

    getnstr(buffer, max_len - 1);

    noecho();
    curs_set(0);

    return strlen(buffer) > 0;
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
    CipherType selected = CIPHER_CAESAR;
    CipherType current = selected;
    int running = 1;
    int key;

    const char *action_name = (action == CRYPT_ACTION_ENCRYPT) ? "CIPTOGRAFAR" : "DESCRIPTOGRAFAR";

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

void ui_process_encrypt_decrypt(CryptAction action, CipherType cipher) {
    char input[MAX_INPUT_LENGTH] = {0};
    char key_buffer[256] = {0};
    char output[MAX_OUTPUT_LENGTH] = {0};
    const char *action_name = (action == CRYPT_ACTION_ENCRYPT) ? "Mensagem a Criptografar" : "Mensagem a Descriptografar";
    const char *result_name = (action == CRYPT_ACTION_ENCRYPT) ? "Mensagem Criptografada" : "Mensagem Descriptografada";

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