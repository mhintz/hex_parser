// Some typedefs to make it feel more like Rust
// C purists: please forgive me

#ifndef RUSTDEF_INCLUDE
#define RUSTDEF_INCLUDE

#include <stdint.h> // Defined-width integer types
#include <stddef.h> // NULL, size_t, and ptrdiff_t
#include <stdbool.h> // Bring in bool, true, and false

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef ptrdiff_t isize;
typedef size_t usize;

// This isn't in rust, but it's nice to have
typedef unsigned char uchar;

typedef float f32;
typedef double f64;

// Include guard
#endif