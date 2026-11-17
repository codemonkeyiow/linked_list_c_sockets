CC = gcc
CFLAGS = -Wall -Wextra -g
BIN = bin

.PHONY: all clean

all: $(BIN)/server $(BIN)/client

$(BIN):
	mkdir -p $(BIN)

$(BIN)/linkedlist.o: linkedlist.c linkedlist.h colours.h | $(BIN)
	$(CC) $(CFLAGS) -c linkedlist.c -o $@

$(BIN)/server: server.c $(BIN)/linkedlist.o | $(BIN)
	$(CC) $(CFLAGS) server.c $(BIN)/linkedlist.o -o $@

$(BIN)/client: client.c | $(BIN)
	$(CC) $(CFLAGS) client.c -o $@

clean:
	rm -rf $(BIN)
