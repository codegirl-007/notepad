CC = gcc
TARGET = notepad
SRC = notepad.c
GTK_CFLAGS = $(shell pkg-config --cflags gtk4 gtksourceview-5)
GTK_LIBS = $(shell pkg-config --libs gtk4 gtksourceview-5)
CFLAGS = -Wall -Wextra -g $(GTK_CFLAGS)
LDFLAGS = $(GTK_LIBS)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)
