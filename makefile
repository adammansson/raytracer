CC = gcc
CFLAGS = -Wall -Wextra -O3
LDFLAGS = -lm

SRC = $(wildcard *.c)
TARGET = raytracer

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET) > image.ppm
	feh image.ppm

clean:
	rm -f $(TARGET) image.ppm
