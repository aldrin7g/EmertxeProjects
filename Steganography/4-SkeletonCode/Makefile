CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/common.c src/decode.c src/encode.c src/steganography.c
TARGET = bin/stego.out

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)