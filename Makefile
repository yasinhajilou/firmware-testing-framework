CC = gcc
CFLAGS = -I./include -Wall -Wextra -g
LDFLAGS = -lm

TEST_SOURCES = $(wildcard tests/*.c)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)
TEST_EXECUTABLES = $(TEST_SOURCES:.c=.test)

.PHONY: all clean test

all: $(TEST_EXECUTABLES)

%.test: %.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: all
	@for test in $(TEST_EXECUTABLES) ; do \
		echo "\nRunning $$test..." ; \
		./$$test ; \
	done

clean:
	rm -f $(TEST_OBJECTS) $(TEST_EXECUTABLES)