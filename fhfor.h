/*
** See end of file for license information.
*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// BEGIN HEADER GUARD
//
#if !defined(FHFOR_H)
#  define FHFOR_H


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// CONFIG
//
#if !defined(FHFOR_DATA)
#  define FHFOR_DATA data
#endif

#if !defined(FHFOR_COUNT)
#  define FHFOR_COUNT count
#endif

#if !defined(FHFOR_INDEX)
#  define FHFOR_INDEX index
#endif

#if !defined(FHFOR_VALUE)
#  define FHFOR_VALUE value
#endif

#if !defined(FHFOR_NEXT)
#  define FHFOR_NEXT next
#endif

#if !defined(FHFOR_PREVIOUS)
#  define FHFOR_PREVIOUS previous
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// NOTE(Patrik): The default for loop, goes from 0 to count.
//
#define fhfor(T, index, count) fhfor_range(T, index, 0, count)
#define fhrfor(T, index, count) fhrfor_range(T, index, 0, count)

#define fhfor_it(I, V, it, data, count) fhfor_it_range(I, V, it, data, 0, count)
#define fhrfor_it(I, V, it, data, count) fhrfor_it_range(I, V, it, data, 0, count)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// EACH
//
#define fhfor_each(T, it, array) for(T *it = (array)->FHFOR_DATA; it < ((array)->FHFOR_DATA + (array)->FHFOR_COUNT); it += 1)
#define fhrfor_each(T, it, array) for(T *it = ((array)->FHFOR_DATA + (array)->FHFOR_COUNT - 1); it >= (array)->FHFOR_DATA; it += 1)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// RANGE
//
#define fhfor_range(T, index, from_inclusive, to_exclusive) for(T index = (from_inclusive); index < (to_exclusive); index = index + 1)
#define fhrfor_range(T, index, from_inclusive, to_exclusive) for(T index = ((to_exclusive) - 1); index >= (from_inclusive); index = index - 1)

#define fhfor_it_range(I, V, it, data, from_inclusive, to_exclusive) for(struct {I FHFOR_INDEX; V FHFOR_VALUE;} it = {(from_inclusive), (data)[from_inclusive]}; it.FHFOR_INDEX < (to_exclusive); it.FHFOR_INDEX = it.FHFOR_INDEX + 1, it.FHFOR_VALUE = (data)[it.FHFOR_INDEX])
#define fhrfor_it_range(I, V, it, data, from_inclusive, to_exclusive) for(struct {I FHFOR_INDEX; V FHFOR_VALUE;} it = {(to_exclusive) - 1, (data)[to_exclusive - 1]}; it.FHFOR_INDEX >= (to_exclusive); it.FHFOR_INDEX = it.FHFOR_INDEX - 1, it.FHFOR_VALUE = (data)[it.FHFOR_INDEX])


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INCLUSIVE RANGE
//
#define fhfor_irange(T, index, from_inclusive, to_inclusive) for(T index = (from_inclusive); index <= (to_inclusive); index = index + 1)
#define fhrfor_irange(T, index, from_inclusive, to_inclusive) for(T index = (to_inclusive); index >= (from_inclusive); index = index - 1)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// NOTE(Patrik): Loops from 0 to the count of the array (duck typing)
//
#define fhfor_array(T, index, array) for(T index = 0; index < (array)->FHFOR_COUNT; index = index + 1)
#define fhrfor_array(T, index, array) fhrfor(T index = ((array)->FHFOR_COUNT) -1; index >= 0; index = index - 1)

#define fhfor_it_array(I, V, it, array) fhfor_it(I, V, it, (array)->FHFOR_DATA, (array)->FHFOR_COUNT)
#define fhrfor_it_array(I, V, it, array) fhrfor_it(I, V, it, (array)->FHFOR_DATA, (array)->FHFOR_COUNT)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// NOTE(Patrik): Goes from 0 to the count of the array.
// Use this for C-style fixed length arrays only!
//
#define fhfor_c_array(T, index, array) for(T index = 0; index < (sizeof(array) / sizeof(*(array))); index = index + 1)
#define fhrfor_c_array(T, index, array) for(T index = (sizeof(array) / sizeof(*(array))) - 1; index >= 0; index = index - 1)

#define fhfor_it_c_array(I, V, it, array) fhfor_it(I, V, it, array, (sizeof(array) / sizeof(*(array))))
#define fhrfor_it_c_array(I, V, it, array) fhrfor_it(I, V, it, array, (sizeof(array) / sizeof(*(array))))

#define fhfor_each_c_array(T, it, array) for(T *it = array; it < array + (sizeof(array) / sizeof(*(array))); it += 1)
#define fhrfor_each_c_array(T, it, array) for(T *it = (array) + ((sizeof(array) / sizeof(*(array))) - 1); it >= (array); it -= 1)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// NOTE(Patrik): Loop through a null-terminated string
//
#define fhfor_ntstr(T, index, ntstring) for(T index = 0; *((ntstring) + index); index = index + 1)

#define fhfor_it_ntstr(I, it, data, ntstring) for(struct {I FHFOR_INDEX; char FHFOR_VALUE;} it = {0, *(ntstring)}; *((ntstring) + it.FHFOR_INDEX); it.FHFOR_INDEX = it.FHFOR_INDEX + 1, it.FHFOR_VALUE = *((ntstring) + it.FHFOR_INDEX))

#define fhfor_each_ntstr(it, ntstring) for(char it = *(ntstring); *(ntstring); it = *(++(ntstring)))


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// NOTE(Patrik): Left-right loop
// This starts at the beginning and end of the array and concludes when met in the middle.
// Useful for reversing arrays.
//
#define fhfor_lr(T, left, right, from_inclusive, to_exclusive) for(T left = (from_inclusive), right = (to_exclusive) - 1; left < right; left = left + 1, right = right - 1)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// NOTE(Patrik): Linked list loop
//
#define fhfor_linked(T, it, start_node) for(T *it = (start_node); it; it = it->FHFOR_NEXT)
#define fhrfor_linked(T, it, start_node) for(T *it = (start_node); it; it = it->FHFOR_PREVIOUS)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// FHORT NAMES
//
#if !defined(FHFOR_NO_SHORT_NAMES)
#  define For fhfor
#  define rFor fhrfor
#  define For_it fhfor_it
#  define rFor_it fhrfor_it
#  define For_each fhfor_each
#  define rFor_each fhrfor_each
#  define For_range fhfor_range
#  define rFor_range fhrfor_range
#  define For_it_range fhfor_it_range
#  define rFor_it_range fhrfor_it_range
#  define For_irange fhfor_irange
#  define rFor_irange fhrfor_irange
#  define For_array fhfor_array
#  define rFor_array fhrfor_array
#  define For_it_array fhfor_it_array
#  define rFor_it_array fhrfor_it_array
#  define For_c_array fhfor_c_array
#  define rFor_c_array fhrfor_c_array
#  define For_it_c_array fhfor_it_c_array
#  define rFor_it_c_array fhrfor_it_c_array
#  define For_each_c_array fhfor_each_c_array
#  define rFor_each_c_array fhrfor_each_c_array
#  define For_ntstr fhfor_ntstr
#  define For_it_ntstr fhfor_it_ntstr
#  define For_each_ntstr fhfor_each_ntstr
#  define For_lr fhfor_lr
#  define For_linked fhfor_linked
#  define rFor_linked fhrfor_linked
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// END HEADER GUARD
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
