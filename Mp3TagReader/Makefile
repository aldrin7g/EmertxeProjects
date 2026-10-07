CC = gcc
CFLAGS = -Wall -Wextra -Wno-char-subscripts -Iinclude

SRC = src/main.c src/common.c src/edit.c src/help.c src/view.c
TARGET = Mp3Tag

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean