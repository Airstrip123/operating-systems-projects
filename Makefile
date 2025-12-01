# ============================================================================
# Makefile for CMPUT 379 Assignment 3 - File System Simulator
# ============================================================================
# 
# TARGETS:
#   fs       - Build the executable (default target)
#   compile  - Compile source files to object files
#   clean    - Remove object files and executable
# 
# COMPILER FLAGS:
#   -Wall    - Enable all warnings
#   -Werror  - Treat warnings as errors
#   -g       - Include debugging symbols
#   -std=c11 - Use C11 standard
# ============================================================================

CC = gcc
CFLAGS = -Wall -Werror -g -std=c11
TARGET = fs
OBJS = fs-sim.o

# Default target: build the executable
fs: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile target: produce object files
compile: $(OBJS)

# Rule to compile .c files to .o files
fs-sim.o: fs-sim.c fs-sim.h
	$(CC) $(CFLAGS) -c fs-sim.c

# Clean target: remove generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets (not actual files)
.PHONY: clean compile