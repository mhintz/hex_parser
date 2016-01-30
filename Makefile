COMPILER := gcc-5
FLAGS := -g -Wall -Wextra

HEADER_DIRS := \
	bstrlib

HEADER_SEARCH = $(addprefix -I, $(HEADER_DIRS))

DEP_FILES := \
	hex_decode.c

OUT := out/

APP_NAME := out/hex_decode

.PHONY: all strict

all: clean $(APP_NAME)

strict: clean memcheck_run

# Tasks

clean:
	rm -rf $(APP_NAME) $(APP_NAME).dSYM

$(APP_NAME): $(DEP_FILES)
	mkdir -p $(dir $@)
	$(COMPILER) -o $@ $(FLAGS) $(HEADER_SEARCH) $^

run: $(APP_NAME)
	./$(APP_NAME)

memcheck_run: $(APP_NAME)
	valgrind ./$(APP_NAME)

TEST_DATA := "\#"

test: $(APP_NAME)
	echo $(TEST_DATA) | valgrind ./$(APP_NAME)
