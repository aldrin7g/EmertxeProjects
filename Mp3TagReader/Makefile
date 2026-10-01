CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c src/common.c src/edit.c src/help.c src/view.c

a.out: $(SRC)
	$(CC) $(CFLAGS) $(SRC)

clean:
	rm -f a.out