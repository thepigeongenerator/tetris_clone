# dependencies:
#  - make
#  - clang
#  - bear (debug)
#  - sdl2
#  - git bash (windows)
#
# project name = the workspace directory name
NAME := $(shell basename $(PWD))
DEBUG ?= 0
ARCH ?= 0

# compiler settings
CC := clang
STD := c17
CFLAGS := -m32 -Wall -Wextra -Wpedantic -Wno-pointer-arith
LDFLAGS := -m32 -lm

ifneq ($(DEBUG),0)
CFLAGS  += -g -Og -fsanitize=address,undefined
LDFLAGS += -fsanitize=address,undefined
PROF    := dbg
else
CFLAGS  += -DNDEBUG -O2 -Werror
PROF    := rel
endif

ifneq ($(MAKECMDGOALS),clean)
ifeq      ($(ARCH),linux-x86)
CFLAGS  += -target x86_64-pc-linux-gnu $(shell pkg-config --cflags sdl2 SDL2_ttf)
LDFLAGS += -target x86_64-pc-linux-gnu $(shell pkg-config --libs sdl2 SDL2_ttf)
else ifeq ($(ARCH),win-x86)
CFLAGS  += -target x86_64-pc-windows-gnu
LDFLAGS += -target x86_64-pc-windows-gnu -fuse-ld=lld
EXT     := .exe
else
$(error you must set the ARCH environment variable to one of these: 'linux-x86' 'win-x86')
endif
endif

ifneq ($(ARCH),0)
# dirs
DIR_BIN := bin/$(ARCH)/$(PROF)
DIR_OBJ := obj/$(ARCH)/$(PROF)
TARGET := $(DIR_BIN)/$(NAME)$(EXT)

# source files
SRC := $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) $(wildcard src/**/**/**/**/*.c)
OBJ := $(patsubst src/%,$(DIR_OBJ)/%,$(SRC:.c=.o))
DEP := $(OBJ:.o=.d)
SRC_ASSETS := $(wildcard assets/*)
ASSETS := $(patsubst assets/%,$(DIR_BIN)/%,$(SRC_ASSETS))

COMPILE_COMMANDS := $(DIR_OBJ)/compile_commands.json
endif

define wr_colour
	@printf '\033[%sm%s\033[0m\n' $(2) $(1)
endef

# compiles and execute the binary
run: compile
	cd $(dir $(TARGET)) && ./$(notdir $(TARGET))
compile: compile_commands $(TARGET) $(ASSETS)

.NOTPARALLEL:
clean:
	rm -rf obj/ bin/ compile_commands.json

# create the binary (linking step)
$(TARGET): $(OBJ)
	@$(call wr_colour,"CC: '$(CC)'",94)
	@$(call wr_colour,"CFLAGS: '$(CFLAGS)'",94)
	@$(call wr_colour,"LDFLAGS: '$(LDFLAGS)'",94)
	@$(call wr_colour,"linking to: '$@'",92)

	@mkdir -p ${@D}
	@$(CC) -o $(TARGET) $^ $(LDFLAGS)
	@$(call wr_colour,"current profile: '$(PROF)'",93)

# create .o and .d files
$(DIR_OBJ)/%.o: src/%.c
	@$(call wr_colour,"compiling $(notdir $@) from $(notdir $<)",92)
	@mkdir -p ${@D}
	@$(CC) $(CFLAGS) -c -MD -MP -std=$(STD) -x c -o $@ $<

# copy assets
$(DIR_BIN)/%: assets/%
	@mkdir -p ${@D}
	@cp -v $< $@

# update compile commands if the makefile has been updated (for linting)
compile_commands: # default, empty rule
ifneq ($(shell which bear),)
ifneq ($(COMPILE_COMMANDS),)
ifeq ($(NOCMDS),)
.NOTPARALLEL .PHONY:
compile_commands: $(COMPILE_COMMANDS)
	@[ "$(readlink compile_commands.json)" != "$<" ] && ln -sf $< compile_commands.json

.NOTPARALLEL:
$(COMPILE_COMMANDS): makefile
	@$(call wr_colour,"regenerating compile_commands.json thus recompiling.",93)
	@mkdir -p ${@D} # ensure the target directory exists
	@touch $@       # create the file so it isn't retriggered (will just change modification time if already exists)
	@bear --output $@ -- make -B compile NOCMDS=1 # rebuild the current target using bear, to create the compile commands
endif
endif
endif

# disable implicit rules
.SUFFIXES:

# include the dependencies
-include $(DEP)
