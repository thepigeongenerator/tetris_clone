NAME = sdl_template

# compiler settings
CC := clang
STD := c17
LANG = c
CFLAGS := $(shell pkg-config --cflags sdl2) -Wall -Wall -Wextra -Wpedantic -Wno-pointer-arith
LDFLAGS := $(shell pkg-config --libs sdl2) -lm

ifeq ($(DEBUG),1)
CFLAGS += -Og -g
else
REL_FLAGS += -O3
endif

# file locations
DIR_BIN := bin
DIR_OBJ := obj
DIR := $(DIR_BIN)/$(ARCH) $(DIR_OBJ)/$(ARCH)

SRC := $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) $(wildcard src/**/**/**/**/*.c)
OBJ := $(patsubst src/%,$(DIR_OBJ)/$(ARCH)/%,$(SRC:.c=.o))
DEP := $(OBJ:.o=.d)
TARGET := $(DIR_BIN)/$(ARCH)/$(NAME)$(EXT)

# sets the variables for the different targets
linux-x86_64:
	$(MAKE) build ARCH=linux-x86_64 CFLAGS="$(CFLAGS) -target x86_64-pc-linux-gnu"
win-x86_64:
	$(MAKE) build ARCH=win-x86-64 CFLAGS="$(CFLAGS) -target x86_64-pc-windows-gnu" EXT=".exe"
web:
	$(MAKE) build ARCH=web CC=emcc EXT=".html"

all: linux-x86_64 win-x86_64 web
build: $(DIR) $(TARGET) compile_commands.json
clean:
	rm -rf $(DIR_BIN) $(DIR_OBJ)

# creates the binary
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $^ $(CFLAGS) $(LDFLAGS)

# creates .o and .d files, include a flag for no unused command line arguments, because in this context it's unneeded
$(DIR_OBJ)/$(ARCH)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -o $@ -MD -MP -c $< $(CFLAGS) -std=$(STD) -x $(LANG) -Wno-unused-command-line-argument

$(DIR):
	mkdir -p $@

# update compile commands if the makefile has been updated (for linting)
compile_commands.json: makefile
	touch compile_commands.json
	$(MAKE) clean
	bear -- make

-include $(DEP)
