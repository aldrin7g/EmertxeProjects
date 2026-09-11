CC = gcc
CFLAGS = -Wall -Wextra -Wno-char-subscripts -Wno-sign-compare -Wno-unused-parameter -Iinclude

SRC = src/common.c src/decode.c src/encode.c src/steganography.c
TARGET = bin/stego.out

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)