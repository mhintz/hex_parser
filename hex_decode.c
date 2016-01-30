#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "rustdef.h"
#include "zed/debug.h"

#define println(MSG, ...) printf(MSG "\n", ##__VA_ARGS__)
#define free_if(PTR) if (PTR) free(PTR) 

#define SYMBOLS_PER_COLOR 6 // Six characters per hex code
#define PIX_SIZE 3
#define SCREEN_WIDTH 60
#define SCREEN_HEIGHT 60
#define SCREEN_BUFFER_SIZE SCREEN_WIDTH * SCREEN_HEIGHT * PIX_SIZE

usize make_mult_of(usize original, u8 multiplier) {
	u8 mod = original % multiplier;
	return mod == 0 ? original : original + (multiplier - mod);
}

typedef struct {
	u8 r;
	u8 g;
	u8 b;
} Color;

typedef struct {
	usize max_size;
	usize length;
	u8 * data;
} Buffer;

Buffer * make_buffer(usize byte_count) {
	Buffer * the_buf = malloc(sizeof(Buffer));
	check_mem(the_buf);

	the_buf->length = 0;

	the_buf->data = calloc(byte_count, 1); // calloc stores zeroes in the memory
	check_mem(the_buf->data);

	the_buf->max_size = byte_count;

	return the_buf;

error:
	free_if(the_buf->data);
	free_if(the_buf);
	return NULL;
}

void destroy_buffer(Buffer * the_buf) {
	free_if(the_buf->data);
	free_if(the_buf);
}

u8 * buffer_ptr_at(Buffer * the_buf, usize position) {
	return the_buf->data + position;
}

u8 * buffer_end(Buffer * the_buf) {
	return buffer_ptr_at(the_buf, the_buf->length);
}

void buffer_push(Buffer * the_buf, void * the_data, usize num_bytes) {
	assert(the_buf->data != NULL);
	assert(the_data != NULL);

	assert(the_buf->length + num_bytes <= the_buf->max_size);

	memcpy(buffer_end(the_buf), the_data, num_bytes);
	the_buf->length += num_bytes;
}

void print_buffer(Buffer * const the_buf) {
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			usize i = (y * SCREEN_WIDTH + x) * 3;
			printf("%u,%u,%u ", the_buf->data[i], the_buf->data[i + 1], the_buf->data[i + 2]);
		}
	}
}

// Should send the buffer to the screen
void send_buffer_to_screen(Buffer * the_buf) {
	println("Got sent a screen buffer %p", the_buf);
	print_buffer(the_buf);
}

void swap_buf_ptrs(Buffer * * b1, Buffer * * b2) {
	Buffer * temp = * b1;
	* b1 = * b2;
	* b2 = temp;
}

// Converts an ASCII 0-9, A-F, or a-f to a u8
u8 symbol_to_u8(int source) {
	return source >= 'a' ? source - 'a' + 10 : source >= 'A' ? source - 'A' + 10 : source - '0';
}

// Converts a pair of ASCII 0-9, A-F, or a-f to a u8
u8 pair_to_u8(int d1, int d2) {
	// first digit is shifted left 4 bits, second digit included at the end
	return (symbol_to_u8(d1) << 4) | symbol_to_u8(d2);
}

Color parse_color_buf(int input[SYMBOLS_PER_COLOR]) {
	Color the_color = {
		.r = pair_to_u8(input[0], input[1]),
		.g = pair_to_u8(input[2], input[3]),
		.b = pair_to_u8(input[4], input[5]),
	};

	return the_color;
}

bool symbol_is_command(int symbol) {
	return symbol == '#' || symbol == '.';
}

bool symbol_is_data(int symbol) {
	return (symbol >= '0' && symbol <= '9') || (symbol >= 'A' && symbol <= 'F') || (symbol >= 'a' && symbol <= 'f');
}

// #123456#FFAAEE#AA913D#013EAF.
int main() {
	int input_buffer[SYMBOLS_PER_COLOR];
	Buffer * write_pix = make_buffer(SCREEN_BUFFER_SIZE);
	Buffer * read_pix = make_buffer(SCREEN_BUFFER_SIZE);

	int theinput;
	u8 count = 0;
	while ((theinput = getc(stdin)) != EOF) {
		if (symbol_is_command(theinput)) {
			switch (theinput) {
				case '#':
					count = 0;
				break;
				case '.':
					// Swap the buffer pointers and call for a buffer read
					swap_buf_ptrs(& read_pix, & write_pix);
					send_buffer_to_screen(read_pix);
				break;
				default:
					sentinel("Found unexpected command character");
			}
		} else if (symbol_is_data(theinput)) {
			input_buffer[count] = theinput;
			count++;

			if (count == SYMBOLS_PER_COLOR) {
				Color parsed_color;
				parsed_color = parse_color_buf(input_buffer);
				buffer_push(write_pix, & parsed_color, sizeof(Color));
				count = 0;
			}
		} else if (!isspace(theinput)) {
			log_warn("Strange input detected: %c", theinput);
		}
	}

	destroy_buffer(write_pix);
	destroy_buffer(read_pix);

	return 0;

// The error section
error:
	return 1;
}