CC := gcc
CFLAGS := -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -O2
DEPFLAGS := -MMD -MP
LDFLAGS := -lm -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

SRC_DIR := ./src
BUILD_DIR := ./build

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

BINNAME := soupdl
#BINPATH := $(BUILD_DIR)/$(BINNAME)
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
