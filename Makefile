# compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -I src
LDFLAGS = -lncurses

# directories
SRC_DIR = src
BUILD_DIR = build

# object files
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/tui.o

# target executable
TARGET = debugger

# default rule: build everything
all: $(TARGET)

# link final executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# compile files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ensure /build exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# clean up object and executable files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
