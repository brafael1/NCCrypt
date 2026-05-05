CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = -lncurses

TARGET = nccrypt

SRCS = src/main.c src/crypto.c src/ui.c
OBJS = main.o crypto.o ui.o

CIPHER_SRCS = \
	src/ciphers/caesar/caesar.c \
	src/ciphers/vigenere/vigenere.c \
	src/ciphers/substitution/substitution.c \
	src/ciphers/atbash/atbash.c \
	src/ciphers/base64/base64.c

UI_SRCS = \
	src/ui/ui_core.c \
	src/ui/ui_menus.c \
	src/ui/ui_input.c \
	src/ui/ui_process.c

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

main.o: src/main.c src/ui/ui.h
	$(CC) $(CFLAGS) -c src/main.c

crypto.o: src/crypto.c src/crypto.h $(CIPHER_SRCS)
	$(CC) $(CFLAGS) -c src/crypto.c

ui.o: src/ui.c src/ui.h src/crypto.h $(UI_SRCS)
	$(CC) $(CFLAGS) -c src/ui.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run