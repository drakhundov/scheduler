CC = clang
CFLAGS = -Wall -Werror -Iinclude

TARGET = build/bin/main
TEST_TARGET = build/bin/test

SRCS = $(wildcard src/*.c)
TEST_SRCS = $(wildcard tests/*.c)

# Exclude main.c from the object files for testing.
OBJS = $(patsubst src/%.c, build/obj/%.o, $(SRCS))
TEST_OBJS = $(filter-out build/obj/main.o, $(OBJS)) $(patsubst tests/%.c, build/obj/%.o, $(TEST_SRCS))

all: $(TARGET)

build/obj/%.o: src/%.c
	@mkdir -p build/obj
	$(CC) $(CFLAGS) -c $< -o $@

build/obj/%.o: tests/%.c
	@mkdir -p build/obj
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@mkdir -p build/bin
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_OBJS)
	@mkdir -p build/bin
	$(CC) $(CFLAGS) -o $@ $^

run:
	./$(TARGET) Jobs.txt

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	@rm -rf build/*

.PHONY: all clean run test