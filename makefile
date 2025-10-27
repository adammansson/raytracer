CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O0 -g
LDFLAGS = -lm

SRC = $(wildcard *.c)
TARGET = raytracer

all: $(TARGET)

$(TARGET): $(SRC) makefile
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)
	feh image.ppm

clean:
	rm -f $(TARGET) image.ppm
