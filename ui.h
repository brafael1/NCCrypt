#ifndef UI_H
#define UI_H

#define MAX_INPUT_LENGTH 4096
#define MAX_OUTPUT_LENGTH 8192

typedef enum {
    CRYPT_ACTION_ENCRYPT,
    CRYPT_ACTION_DECRYPT,
    CRYPT_ACTION_EXIT
} CryptAction;

typedef enum {
    CIPHER_CAESAR,
    CIPHER_VIGENERE,
    CIPHER_SUBSTITUTION,
    CIPHER_ATBASH,
    CIPHER_BASE64,
    CIPHER_COUNT
} CipherType;

typedef enum {
    STATE_MAIN_MENU,
    STATE_CIPHER_MENU,
    STATE_INPUT,
    STATE_KEY,
    STATE_RESULT,
    STATE_EXIT
} AppState;

void ui_init(void);
void ui_cleanup(void);
void ui_draw_main_menu(CryptAction selected);
void ui_draw_cipher_menu(CipherType selected);
void ui_draw_input_window(const char *title, char *buffer, int max_len);
void ui_draw_key_window(const char *title, const char *cipher_name, char *buffer, int max_len);
void ui_draw_result_window(const char *title, const char *result);
void ui_show_error(const char *message);
int ui_main_menu_loop(void);
int ui_cipher_menu_loop(CryptAction action);
void ui_process_encrypt_decrypt(CryptAction action, CipherType cipher);

int ui_get_input(const char *prompt, char *buffer, int max_len);
void ui_display_result(const char *result);

#endif