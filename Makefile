COMPILER := gcc-5
FLAGS := -g -Wall -Wextra

HEADER_DIRS := \
	bstrlib

HEADER_SEARCH = $(addprefix -I, $(HEADER_DIRS))

DEP_FILES := \
	hex_decode.c

OUT := out/

APP_NAME := out/hex_decode

.PHONY: all strict clean run memcheck_run test

all: clean $(APP_NAME)

strict: clean memcheck_run

# Tasks

clean:
	rm -rf $(APP_NAME) $(APP_NAME).dSYM
	rm test/testinput test/testoutput

$(APP_NAME): $(DEP_FILES)
	mkdir -p $(dir $@)
	$(COMPILER) -o $@ $(FLAGS) $(HEADER_SEARCH) $^

run: $(APP_NAME)
	./$(APP_NAME)

memcheck_run: $(APP_NAME) test/testinput
	cat test/testinput | valgrind ./$(APP_NAME)

test/testinput:
	node test/gendata.js

test/testoutput:
	node test/gendata.js

test: $(APP_NAME) test/testinput test/testoutput
	cat test/testinput | ./$(APP_NAME) | diff test/testoutput -
