
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGET = shell
DEMO_TARGET = scheduler-demo

SHELL_SRCS = src/shell.c src/executor.c src/scheduler.c src/priority.c
DEMO_SRCS = src/main.c src/scheduler.c src/priority.c

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(SHELL_SRCS)
	$(CC) $(CFLAGS) -o $@ $(SHELL_SRCS)

$(DEMO_TARGET): $(DEMO_SRCS)
	$(CC) $(CFLAGS) -o $@ $(DEMO_SRCS)

test:
	$(CC) $(CFLAGS) -o /tmp/test_executor tests/test_executor.c src/executor.c
	/tmp/test_executor
	$(CC) $(CFLAGS) -o /tmp/test_executor_error tests/test_executor_error.c src/executor.c
	/tmp/test_executor_error

clean:
	rm -f $(TARGET) $(DEMO_TARGET)
