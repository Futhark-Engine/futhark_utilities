/*
** See end of file for license information.
*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// BEGIN HEADER GUARD
//
#if !defined(FHRANDOM_H)
#define FHRANDOM_H


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// CONFIG
//
#if !defined(FHRANDOM_API)
#  define FHRANDOM_API static
#endif

#if !defined(FHRANDOM_DO_NOT_USE_SIMD)
#  include <xmmintrin.h>
#  include <emmintrin.h>
#  define FHRANDOM_SSE
#  define FHRANDOM_SSE2
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// TYPES
//
#if !defined(FHRANDOM_NO_STDINT)
#  include <stdint.h>
typedef uint8_t  fhrandom_u8;
typedef uint16_t fhrandom_u16;
typedef uint32_t fhrandom_u32;
typedef uint64_t fhrandom_u64;
typedef int8_t   fhrandom_i8;
typedef int16_t  fhrandom_i16;
typedef int32_t  fhrandom_i32;
typedef int64_t  fhrandom_i64;
#else
typedef unsigned char      fhrandom_u8;
typedef unsigned short     fhrandom_u16;
typedef unsigned int       fhrandom_u32;
typedef unsigned long long fhrandom_u64;
typedef signed char        fhrandom_i8;
typedef signed short       fhrandom_i16;
typedef signed int         fhrandom_i32;
typedef signed long long   fhrandom_i64;
#endif

typedef float  fhrandom_f32;
typedef double fhrandom_f64;

typedef union FHRandom {
    fhrandom_u32 u[4];
    fhrandom_i32 i[4];
    fhrandom_f32 f[4];
#if !defined(FHRANDOM_DO_NOT_USE_SIMD)
    __m128 p;
#endif
} FHRandom;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// API
//
FHRANDOM_API FHRandom fhrandom_seed(fhrandom_u32 e0, fhrandom_u32 e1, fhrandom_u32 e2, fhrandom_u32 e3);
FHRANDOM_API FHRandom fhrandom_seed_default(void);
FHRANDOM_API FHRandom fhrandom_seed_from_date(fhrandom_u32 year, fhrandom_u8 month, fhrandom_u8 day, fhrandom_u8 hour, fhrandom_u8 minutes, fhrandom_u8 seconds, fhrandom_u16 milliseconds);

FHRANDOM_API void fhrandom_next(FHRandom *seed);
FHRANDOM_API fhrandom_u8 fhrandom_next_u8(FHRandom *seed);
FHRANDOM_API fhrandom_u16 fhrandom_next_u16(FHRandom *seed);
FHRANDOM_API fhrandom_u32 fhrandom_next_u32(FHRandom *seed);
FHRANDOM_API fhrandom_u64 fhrandom_next_u64(FHRandom *seed);
FHRANDOM_API fhrandom_i8 fhrandom_next_i8(FHRandom *seed);
FHRANDOM_API fhrandom_i16 fhrandom_next_i16(FHRandom *seed);
FHRANDOM_API fhrandom_i32 fhrandom_next_i32(FHRandom *seed);
FHRANDOM_API fhrandom_i32 fhrandom_next_i64(FHRandom *seed);

FHRANDOM_API FHRandom fhrandom_unilateral(FHRandom *seed);
FHRANDOM_API fhrandom_f32 fhrandom_unilateral_f32(FHRandom *seed);
FHRANDOM_API fhrandom_f64 fhrandom_unilateral_f64(FHRandom *seed);

FHRANDOM_API FHRandom fhrandom_bilateral(FHRandom *seed);
FHRANDOM_API fhrandom_f32 fhrandom_bilateral_f32(FHRandom *seed);
FHRANDOM_API fhrandom_f64 fhrandom_bilateral_f64(FHRandom *seed);

FHRANDOM_API fhrandom_u8 fhrandom_between_u8(FHRandom *seed, fhrandom_u8 from, fhrandom_u8 to);
FHRANDOM_API fhrandom_u16 fhrandom_between_u16(FHRandom *seed, fhrandom_u16 from, fhrandom_u16 to);
FHRANDOM_API fhrandom_u32 fhrandom_between_u32(FHRandom *seed, fhrandom_u32 from, fhrandom_u32 to);
FHRANDOM_API fhrandom_u64 fhrandom_between_u64(FHRandom *seed, fhrandom_u64 from, fhrandom_u64 to);
FHRANDOM_API fhrandom_i8 fhrandom_between_i8(FHRandom *seed, fhrandom_i8 from, fhrandom_i8 to);
FHRANDOM_API fhrandom_i16 fhrandom_between_i16(FHRandom *seed, fhrandom_i16 from, fhrandom_i16 to);
FHRANDOM_API fhrandom_i32 fhrandom_between_i32(FHRandom *seed, fhrandom_i32 from, fhrandom_i32 to);
FHRANDOM_API fhrandom_i64 fhrandom_between_i64(FHRandom *seed, fhrandom_i64 from, fhrandom_i64 to);
FHRANDOM_API fhrandom_f32 fhrandom_between_f32(FHRandom *seed, fhrandom_f32 from, fhrandom_f32 to);
FHRANDOM_API fhrandom_f64 fhrandom_between_f64(FHRandom *seed, fhrandom_f64 from, fhrandom_f64 to);

//

#if defined(__cplusplus)
FHRANDOM_API FHRandom fhrandom_seed(void);
FHRANDOM_API FHRandom fhrandom_seed(u32 year, u8 month, u8 day, u8 hour, u8 minutes, u8 seconds, u16 milliseconds);
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// END HEADER GUARD
//
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// BEGIN IMPLEMENTATION GUARD
//
#if defined(FHRANDOM_IMPLEMENTATION) && !defined(FHRANDOM_IMPLEMENTATION_DONE)
#  define FHRANDOM_IMPLEMENTATION_DONE


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// PROCEDURES
//
FHRANDOM_API FHRandom
fhrandom_seed(fhrandom_u32 e0, fhrandom_u32 e1, fhrandom_u32 e2, fhrandom_u32 e3) {
    FHRandom result = {0};
#if defined(FHRANDOM_SSE)
    result.p = _mm_setr_ps(*(float *)&e0, *(float *)&e1, *(float *)&e2, *(float *)&e3);
#else
    result.u[0] = e0;
    result.u[1] = e1;
    result.u[2] = e2;
    result.u[3] = e3;
#endif
    return result;
}

FHRANDOM_API FHRandom
fhrandom_seed_default(void) {
    return fhrandom_seed(0x69420, 0x1337, 0x80085, 0x142857);
}
#if defined(__cplusplus)
FHRANDOM_API FHRandom
fhrandom_seed(void) { return fhrandom_seed_default(); }
#endif

FHRANDOM_API FHRandom
fhrandom_seed_from_date(u32 year, u8 month, u8 day, u8 hour, u8 minutes, u8 seconds, u16 milliseconds) {
    FHRandom result = {0};
    result.u[0] = seconds | (minutes << 8) | (hour    << 16) | (day     << 24);
    result.u[1] = minutes | (hour    << 8) | (day     << 16) | (seconds << 24);
    result.u[2] = hour    | (day     << 8) | (seconds << 16) | (minutes << 24);
    result.u[3] = day     | (seconds << 8) | (minutes << 16) | (hour    << 24);
    
    u8 t = day + hour + minutes;
    
    result.u[0] ^= (result.u[0] << t) * (u32)milliseconds;
    result.u[1] ^= (result.u[1] << t) * (u32)milliseconds;
    result.u[2] ^= (result.u[2] << t) * (u32)milliseconds;
    result.u[3] ^= (result.u[3] << t) * (u32)milliseconds;
    
    result.u[0] *= year;
    result.u[1] *= year;
    result.u[2] *= year;
    result.u[3] *= year;
    return result;
}
#if defined(__cplusplus)
FHRANDOM_API FHRandom
fhrandom_seed(u32 year, u8 month, u8 day, u8 hour, u8 minutes, u8 seconds, u16 milliseconds) {
    return fhrandom_seed_from_date(year, month, day, hour, minutes, seconds, milliseconds);
}
#endif

//

FHRANDOM_API FHRandom
fhrandom_next(FHRandom *seed) {
    FHRandom result = {0};
    if(!seed) { return result; }
    
#if defined(FHRANDOM_SSE)
    __m128i p = _mm_castps_si128(seed->p);
    p = _mm_xor_si128(_mm_slli_epi32(p, 13), p);
    p = _mm_xor_si128(_mm_srli_epi32(p, 17), p);
    p = _mm_xor_si128(_mm_slli_epi32(p, 5), p);
    seed->p = _mm_castsi128_ps(p);
#else
    seed->u[0] = (seed->u[0] << 13) ^ seed->u[0];
    seed->u[1] = (seed->u[1] << 13) ^ seed->u[1];
    seed->u[2] = (seed->u[2] << 13) ^ seed->u[2];
    seed->u[3] = (seed->u[3] << 13) ^ seed->u[3];
    
    seed->u[0] = (seed->u[0] >> 17) ^ seed->u[0];
    seed->u[1] = (seed->u[1] >> 17) ^ seed->u[1];
    seed->u[2] = (seed->u[2] >> 17) ^ seed->u[2];
    seed->u[3] = (seed->u[3] >> 17) ^ seed->u[3];
    
    seed->u[0] = (seed->u[0] << 5) ^ seed->u[0];
    seed->u[1] = (seed->u[1] << 5) ^ seed->u[1];
    seed->u[2] = (seed->u[2] << 5) ^ seed->u[2];
    seed->u[3] = (seed->u[3] << 5) ^ seed->u[3];
#endif
    
    result = *seed;
    return result;
}

FHRANDOM_API fhrandom_u8
fhrandom_next_u8(FHRandom *seed) {
    FHRandom result = fhrandom_next(seed);
    return (result.u[0] & 0xFF);
}

FHRANDOM_API fhrandom_u16
fhrandom_next_u16(FHRandom *seed) {
    FHRandom result = fhrandom_next(seed);
    return (result.u[0] & 0xFFFF);
}

FHRANDOM_API fhrandom_u32
fhrandom_next_u32(FHRandom *seed) {
    FHRandom result = fhrandom_next(seed);
    return result.u[0];
}

FHRANDOM_API fhrandom_u32
fhrandom_next_u64(FHRandom *seed) {
    FHRandom result = fhrandom_next(seed);
    return ((fhrandom_u64)result.u[0] | ((fhrandom_u64)result.u[1] << 32));
}

//

FHRANDOM_API FHRandom
fhrandom_unilateral(FHRandom *seed) {
    FHRandom result = fhrandom_next(seed);
    fhrandom_f32 divisor = 1.0f / (fhrandom_f32)0xFFFFFFFF;
    result.f[0] = (fhrandom_f32)result.u[0] * divisor;
    result.f[1] = (fhrandom_f32)result.u[1] * divisor;
    result.f[2] = (fhrandom_f32)result.u[2] * divisor;
    result.f[3] = (fhrandom_f32)result.u[3] * divisor;
    return result;
}

FHRANDOM_API fhrandom_f32
fhrandom_unilateral_f32(FHRandom *seed) {
    fhrandom_u32 value = fhrandom_next_u32(seed);
    fhrandom_f32 divisor = 1.0f / (fhrandom_f32)0xFFFFFFFF;
    return (fhrandom_f32)value * divisor;
}

FHRANDOM_API fhrandom_f64
fhrandom_unilateral_f64(FHRandom *seed) {
    fhrandom_u64 value = fhrandom_next_u64(seed);
    fhrandom_f64 divisor = 1.0 / (fhrandom_f64)0xFFFFFFFFFFFFFFFFULL;
    return (fhrandom_f64)value * divisor;
}

//

FHRANDOM_API FHRandom
fhrandom_bilateral(FHRandom *seed) {
    FHRandom result = fhrandom_next(seed);
    fhrandom_f32 divisor = 2.0f / (fhrandom_f32)0xFFFFFFFF;
    result.f[0] = (fhrandom_f32)result.u[0] * divisor;
    result.f[1] = (fhrandom_f32)result.u[1] * divisor;
    result.f[2] = (fhrandom_f32)result.u[2] * divisor;
    result.f[3] = (fhrandom_f32)result.u[3] * divisor;
    return result;
}

FHRANDOM_API fhrandom_f32
fhrandom_bilateral_f32(FHRandom *seed) {
    fhrandom_u32 value = fhrandom_next_u32(seed);
    fhrandom_f32 divisor = 2.0f / (fhrandom_f32)0xFFFFFFFF;
    return (fhrandom_f32)value * divisor;
}

FHRANDOM_API fhrandom_f64
fhrandom_bilateral_f64(FHRandom *seed) {
    fhrandom_u64 value = fhrandom_next_u64(seed);
    fhrandom_f64 divisor = 2.0 / (fhrandom_f64)0xFFFFFFFFFFFFFFFFULL;
    return (fhrandom_f64)value * divisor;
}

//

FHRANDOM_API fhrandom_u8
fhrandom_between_u8(FHRandom *seed, fhrandom_u8 from, fhrandom_u8 to) {
    fhrandom_u8 count = to - from;
    fhrandom_u8 value = fhrandom_next_u8(seed) % count;
    return from + value;
}

FHRANDOM_API fhrandom_u16
fhrandom_between_u16(FHRandom *seed, fhrandom_u16 from, fhrandom_u16 to) {
    fhrandom_u16 count = to - from;
    fhrandom_u16 value = fhrandom_next_u16(seed) % count;
    return from + value;
}

FHRANDOM_API fhrandom_u32
fhrandom_between_u32(FHRandom *seed, fhrandom_u32 from, fhrandom_u32 to) {
    fhrandom_u32 count = to - from;
    fhrandom_u32 value = fhrandom_next_u32(seed) % count;
    return from + value;
}

FHRANDOM_API fhrandom_u64
fhrandom_between_u64(FHRandom *seed, fhrandom_u64 from, fhrandom_u64 to) {
    fhrandom_u64 count = to - from;
    fhrandom_u64 value = fhrandom_next_u64(seed) % count;
    return from + value;
}

FHRANDOM_API fhrandom_i8
fhrandom_between_i8(FHRandom *seed, fhrandom_i8 from, fhrandom_i8 to) {
    fhrandom_i8 count = to - from;
    fhrandom_i8 value = fhrandom_next_i8(seed) % count;
    return from + value;
}

FHRANDOM_API fhrandom_i16
fhrandom_between_i16(FHRandom *seed, fhrandom_i16 from, fhrandom_i16 to) {
    fhrandom_i16 count = to - from;
    fhrandom_i16 value = fhrandom_next_i16(seed) % count;
    return from + value;
}

FHRANDOM_API fhrandom_i32
fhrandom_between_i32(FHRandom *seed, fhrandom_i32 from, fhrandom_i32 to) {
    fhrandom_i32 count = to - from;
    fhrandom_i32 value = fhrandom_next_i32(seed) % count;
    return from + value;
}

FHRANDOM_API fhrandom_i64
fhrandom_between_i64(FHRandom *seed, fhrandom_i64 from, fhrandom_i64 to) {
    fhrandom_i64 count = to - from;
    fhrandom_i64 value = fhrandom_next_i64(seed) % count;
    return from + value;
}

FHRANDOM_API fhrandom_f32
fhrandom_between_f32(FHRandom *seed, fhrandom_f32 from, fhrandom_f32 to) {
    fhrandom_f32 result = (fhrandom_unilateral_f32(seed) * (to - from)) + from;
    return result;
}

FHRANDOM_API fhrandom_f64
fhrandom_between_f64(FHRandom *seed, fhrandom_f64 from, fhrandom_f64 to) {
    fhrandom_f64 result = (fhrandom_unilateral_f64(seed) * (to - from)) + from;
    return result;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// END IMPLEMENTATION GUARD
//
#endif


/*
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2025 Patrik Johansson
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
