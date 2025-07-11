# Snake Game Makefile for NixOS
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 -O2
LDFLAGS = -lncurses

# Directories
SRCDIR = .
OBJDIR = obj
BINDIR = bin

# Files
TARGET = snake
SOURCES = snake.c
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)

# Default target
all: $(BINDIR)/$(TARGET)

# Create directories if they don't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Build the executable
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Snake game compiled successfully!"
	@echo "Run with: ./$(BINDIR)/$(TARGET)"

# Build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Installation prefix
PREFIX ?= /usr/local

# Install system-wide (optional)
install: $(BINDIR)/$(TARGET)
	@echo "Installing snake game..."
	install -d $(PREFIX)/bin
	install -m 755 $(BINDIR)/$(TARGET) $(PREFIX)/bin/
	@echo "Snake game installed to $(PREFIX)/bin/snake"

# Uninstall
uninstall:
	@echo "Uninstalling snake game..."
	rm -f $(PREFIX)/bin/$(TARGET)
	@echo "Snake game uninstalled"

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Cleaned build artifacts"

# Run the game
run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(BINDIR)/$(TARGET)

# Check for ncurses library
check-deps:
	@echo "Checking for ncurses library..."
	@pkg-config --exists ncurses && echo "✓ ncurses found" || echo "✗ ncurses not found"
	@echo "If ncurses is missing, install with: nix-env -iA nixpkgs.ncurses"

# Help
help:
	@echo "Snake Game Makefile"
	@echo "Available targets:"
	@echo "  all         - Build the game (default)"
	@echo "  clean       - Remove build artifacts"
	@echo "  run         - Build and run the game"
	@echo "  debug       - Build with debug symbols"
	@echo "  install     - Install system-wide"
	@echo "  uninstall   - Remove system installation"
	@echo "  check-deps  - Check for required dependencies"
	@echo "  help        - Show this help message"

# Phony targets
.PHONY: all clean run debug install uninstall check-deps help