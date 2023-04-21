# Makefile for Linux targeting Linux

CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -pipe -g -Wno-unused-parameter -Werror
DEPFLAGS := -MMD -MP
LDFLAGS := -lm -lSDL2 -lSDL2_image -lSDL2_mixer

SRC_DIR := ./src
BUILD_DIR := ./build/linux

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

BINNAME := soupdl
BINPATH := ./$(BINNAME)

all: $(BINPATH)

$(BINPATH): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

.DELETE_ON_ERROR:
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm $(BINNAME)

-include $(DEPS)
