# Makefile for the UNIX shell project.
# Run `make` to build. Run `make clean` to remove built files.

# The compiler to use.
CC = gcc

# Compiler flags.
# -Wall turns on all common warnings.
# -g adds debug info.
CFLAGS = -Wall -g

# The name of the final program.
TARGET = shell

# All the .c files to compile.
SRCS = src/main.c src/scheduler.c

# Build the program from the source files.
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Remove the built program.
clean:
	rm -f $(TARGET)