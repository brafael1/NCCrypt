CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = -lncurses

TARGET = nccrypt
OBJS = main.o crypto.o ui.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

main.o: main.c ui.h
	$(CC) $(CFLAGS) -c main.c

crypto.o: crypto.c crypto.h
	$(CC) $(CFLAGS) -c crypto.c

ui.o: ui.c ui.h crypto.h
	$(CC) $(CFLAGS) -c ui.c

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run