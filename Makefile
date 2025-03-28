CC = clang
CFLAGS = -Wall -Werror -Iinclude

TARGET = build/bin/main

SRCS = $(wildcard src/*.c)

OBJS = $(patsubst src/%.c, build/obj/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p build/bin
	$(CC) $(CFLAGS) -o $@ $^

build/obj/%.o: src/%.c
	@mkdir -p build/obj
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(TARGET)

clean:
	@rm -rf build/*

.PHONY: all clean