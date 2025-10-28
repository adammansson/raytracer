CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wno-unused-function -O3 -fopenmp
LDFLAGS = -lm

SRC = $(wildcard *.c)
TARGET = raytracer

all: $(TARGET)

$(TARGET): $(SRC) Makefile
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)
	feh image.ppm

clean:
	rm -f $(TARGET) image.ppm
