CC = gcc
CFLAGS = -Wall -Wextra -Wno-char-subscripts -Iinclude

SRC = src/main.c src/contact.c src/file.c src/modules.c
TARGET = bin/AddBk.out

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean