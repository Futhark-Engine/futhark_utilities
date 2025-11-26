/*
** See end of file for license information.
*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// BEGIN INCLUDE GUARD
//
#if !defined(FHDUCK_H)
#define FHDUCK_H


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// NOTE(Patrik): This header is a utility header for duck typed arrays.
// Duck typing is usually a good way to get around the fact that C
// does not have function overloading. As duck typing is a way to test on specific
// fields of a type instead of the actual underlying type itself.
//
// If it looks like a duck, swims like a duck, and quacks like a duck, then it probably is a duck.
//


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// CONFIG
//
#define FHDUCK_LIST_DEFAULT_CAPACITY 128

#if !defined(FHDUCK_ALLOC) && !defined(FHDUCK_REALLOC) && !defined(FHDUCK_FREE)
#  if defined(Futhark_Context) && defined(FUTHARK_CTX_ALLOC_SIZE) && defined(FUTHARK_CTX_REALLOC_SIZE)
#    define FHDUCK_ALLOC(size_in_bytes) FUTHARK_CTX_ALLOC_SIZE(futhark_context, size_in_bytes)
#    define FHDUCK_REALLOC(data, size_in_bytes) FUTHARK_CTX_REALLOC_SIZE(futhark_context, data, size_in_bytes)
#  elif defined(FUTHARK_ALLOC_SIZE) && defined(FUTHARK_REALLOC_SIZE)
#    define FHDUCK_ALLOC(size_in_bytes) FUTHARK_ALLOC_SIZE(size_in_bytes)
#    define FHDUCK_REALLOC(data, size_in_bytes) FUTHARK_REALLOC_SIZE(data, size_in_bytes)
#  else
#    include <stdlib.h>
#    define FHDUCK_ALLOC(size_in_bytes) malloc(size_in_bytes)
#    define FHDUCK_REALLOC(data, size_in_bytes) realloc(data, size_in_bytes)
#  endif
#elif !defined(FHDUCK_ALLOC) || !defined(FHDUCK_REALLOC)
#  error Must define both or none of FHDUCK_ALLOC and FHDUCK_REALLOC
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// STRUCT MACROS
//
#define FHDUCK_ARRAY(Index_T, Element_T) \
Element_T *data;\
union { Index_T count, length; }

#define FHDUCK_LIST(Index_T, Element_T) \
FHDUCK_ARRAY(Index_T, Element_T);\
Index_T capacity

#define FHDUCK_ARRAYN(N, Index_T, ...) FHDUCK_ARRAY##N(Index_T, __VA_ARGS__)
#define FHDUCK_LISTN(N, Index_T, ...) FHDUCK_LIST##N(Index_T, __VA_ARGS__)


// TODO(Patrik): Move these?

#define ARRAY(Index_T, Element_T) struct ____ {\
FHDUCK_ARRAY(Index_T, Element_T);\
}

#define LIST(Index_T, Element_T) struct ____ {\
FHDUCK_LIST(Index_T, Element_T);\
}

#define ARRAYN(N, Index_T, ...) ARRAY##N(Index_T, __VA_ARGS__)
#define LISTN(N, Index_T, ...) LIST##N(Index_T, __VA_ARGS__)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// TODO(Patrik): Move this?
// GENERAL MACROS
//
#if defined(__cplusplus)
// NOTE(Patrik): This is to cast to/from a void pointer.
// In C you can just do that by default, but C++ wants you to suffer.
#  define FHDUCK_DECLTYPE(var) (decltype(var))
#else
#  define FHDUCK_DECLTYPE(var)
#endif

#define FHDUCK_VA(T, ...) FHDUCK_VA_DATA(T, __VA_ARGS__), FHDUCK_VA_COUNT(T, __VA_ARGS__)
#define FHDUCK_VA_DATA(T, ...) ((const T *[]){__VA_ARGS__})
#define FHDUCK_VA_COUNT(T, ...) (sizeof((const T *[]){__VA_ARGS__}) / sizeof(const T *))


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// NTSTRING LENGTH
//
#define fhduck_get_ntstring_length(result, ntstring) do {\
(result) = 0;\
if(ntstring) {\
while((ntstring)[(result)]) {\
(result) = (result) + 1;\
}\
}\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// EQ
//
#define fhduck_many_eq_many(result, a_data, a_count, b_data, b_count) do {\
(result) = 0;\
if((a_count) == (b_count) && (a_count) >= 0) {\
if((a_data) == (b_data)) {\
(result) = 1;\
} else if((a_data) && (b_data)) {\
(result) = 1;\
for(ptrdiff_t _fhduck_i_ = 0; _fhduck_i_ < (a_count); _fhduck_i_ += 1) {\
if((a_data)[_fhduck_i_] != (b_data)[_fhduck_i_]) {\
(result) = 0;\
break;\
}\
}\
}\
}\
} while(0)

#define fhduck_many_eq_array(result, a_data, a_count, b) \
fhduck_many_eq_many(result, a_data, a_count, (b).data, (b).count)

#define fhduck_many_eq_ntstring(result, a_data, a_count, b) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, b);\
fhduck_many_eq_many(result, a_data, a_count, b, _fhduck_b_length_);\
} while(0)

#define fhduck_many_eq_str_lit(result, a_data, a_count, b) \
fhduck_many_eq_many(result, a_data, a_count, FHDUCK_STR_LIT(b))

//

#define fhduck_array_eq_many(result, a, b_data, b_count) \
fhduck_many_eq_many(result, (a).data, (a).count, b_data, b_count)

#define fhduck_array_eq_array(result, a, b) \
fhduck_many_eq_array(result, (a).data, (a).count, b)

#define fhduck_array_eq_ntstring(result, a, b) \
fhduck_many_eq_ntstring(result, (a).data, (a).count, b)

#define fhduck_array_eq_str_lit(result, a, b) \
fhduck_array_eq_str_lit(result, (a).data, (a).count, b)

//

#define fhduck_ntstring_eq_many(result, a, b_data, b_count) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, a);\
fhduck_many_eq_many(result, a, _fhduck_a_length_, b_data, b_count);\
} while(0)

#define fhduck_ntstring_eq_array(result, a, b) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, a);\
fhduck_many_eq_array(result, a, _fhduck_a_length_, b);\
} while(0)

#define fhduck_ntstring_eq_ntstring(result, a, b) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, a);\
fhduck_many_eq_ntstring(result, a, _fhduck_a_length_, b);\
} while(0)

#define fhduck_ntstring_eq_str_lit(result, a, b) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, a);\
fhduck_array_eq_str_lit(result, a, _fhduck_a_length_, b);\
} while(0)

//

#define fhducka_eq_many(result, this, b_data, b_count) \
fhduck_many_eq_many(result, (this)->data, (this)->count, b_data, b_count)

#define fhducka_many_eq_array(result, a_data, a_count, b) \
fhduck_many_eq_array(result, (this)->data, (this)->count, b)

#define fhducka_eq_ntstring(result, a_data, a_count, b) \
fhduck_many_eq_ntstring(result, (this)->data, (this)->count, b)

#define fhducka_eq_str_lit(result, a_data, a_count, b) \
fhduck_many_eq_str_lit(result, (this)->data, (this)->count, b)

//

#define fhduckl_eq_many(result, this, b_data, b_count) \
fhduck_many_eq_many(result, (this)->data, (this)->count, b_data, b_count)

#define fhduckl_many_eq_array(result, a_data, a_count, b) \
fhduck_many_eq_array(result, (this)->data, (this)->count, b)

#define fhduckl_eq_ntstring(result, a_data, a_count, b) \
fhduck_many_eq_ntstring(result, (this)->data, (this)->count, b)

#define fhduckl_eq_str_lit(result, a_data, a_count, b) \
fhduck_many_eq_str_lit(result, (this)->data, (this)->count, b)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// EQ NO CASE
//
#define fhduck_many_eq_many_no_case(result, a_data, a_count, b_data, b_count) do {\
(result) = 0;\
if((a_count) == (b_count) && (a_count) >= 0) {\
if((a_data) == (b_data)) {\
(result) = 1;\
} else if((a_data) && (b_data)) {\
(result) = 1;\
for(ptrdiff_t _fhduck_i_ = 0; _fhduck_i_ < (a_count); _fhduck_i_ += 1) {\
if((a_data)[_fhduck_i_] != (b_data)[_fhduck_i_]) {\
if(((a_data)[_fhduck_i_] | 0x20) != ((b_data)[_fhduck_i_] | 0x20)) {\
(result) = 0;\
break;\
} else {\
if(((a_data)[_fhduck_i_] < 'A' || (a_data)[_fhduck_i_] > 'z') ||\
((a_data)[_fhduck_i_] > 'Z' && (a_data)[_fhduck_i_] < 'a') ||\
((b_data)[_fhduck_i_] < 'A' || (b_data)[_fhduck_i_] > 'z') ||\
((b_data)[_fhduck_i_] > 'Z' && (b_data)[_fhduck_i_] < 'a'))\
{\
(result) = 0;\
break;\
}\
}\
}\
}\
}\
}\
} while(0)

#define fhduck_many_eq_array_no_case(result, a_data, a_count, b) \
fhduck_many_eq_many_no_case(result, a_data, a_count, (b).data, (b).count)

#define fhduck_many_eq_ntstring_no_case(result, a_data, a_count, b) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, b);\
fhduck_many_eq_many_no_case(result, a_data, a_count, b, _fhduck_b_length_);\
} while(0)

#define fhduck_many_eq_str_lit_no_case(result, a_data, a_count, b) \
fhduck_many_eq_many_no_case(result, a_data, a_count, FHDUCK_STR_LIT(b))

//

#define fhduck_array_eq_many_no_case(result, a, b_data, b_count) \
fhduck_many_eq_many_no_case(result, (a).data, (a).count, b_data, b_count)

#define fhduck_array_eq_array_no_case(result, a, b) \
fhduck_many_eq_array_no_case(result, (a).data, (a).count, b)

#define fhduck_array_eq_ntstring_no_case(result, a, b) \
fhduck_many_eq_ntstring_no_case(result, (a).data, (a).count, b)

#define fhduck_array_eq_str_lit_no_case(result, a, b) \
fhduck_array_eq_str_lit_no_case(result, (a).data, (a).count, b)

//

#define fhduck_ntstring_eq_many_no_case(result, a, b_data, b_count) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, a);\
fhduck_many_eq_many_no_case(result, a, _fhduck_a_length_, b_data, b_count);\
} while(0)

#define fhduck_ntstring_eq_array_no_case(result, a, b) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, a);\
fhduck_many_eq_array_no_case(result, a, _fhduck_a_length_, b);\
} while(0)

#define fhduck_ntstring_eq_ntstring_no_case(result, a, b) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, a);\
fhduck_many_eq_ntstring_no_case(result, a, _fhduck_a_length_, b);\
} while(0)

#define fhduck_ntstring_eq_str_lit_no_case(result, a, b) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, a);\
fhduck_array_eq_str_lit_no_case(result, a, _fhduck_a_length_, b);\
} while(0)

//

#define fhducka_eq_many_no_case(result, this, b_data, b_count) \
fhduck_many_eq_many_no_case(result, (this)->data, (this)->count, b_data, b_count)

#define fhducka_many_eq_array_no_case(result, a_data, a_count, b) \
fhduck_many_eq_array_no_case(result, (this)->data, (this)->count, b)

#define fhducka_eq_ntstring_no_case(result, a_data, a_count, b) \
fhduck_many_eq_ntstring_no_case(result, (this)->data, (this)->count, b)

#define fhducka_eq_str_lit_no_case(result, a_data, a_count, b) \
fhduck_many_eq_str_lit_no_case(result, (this)->data, (this)->count, b)

//

#define fhduckl_eq_many_no_case(result, this, b_data, b_count) \
fhduck_many_eq_many_no_case(result, (this)->data, (this)->count, b_data, b_count)

#define fhduckl_many_eq_array_no_case(result, a_data, a_count, b) \
fhduck_many_eq_array_no_case(result, (this)->data, (this)->count, b)

#define fhduckl_eq_ntstring_no_case(result, a_data, a_count, b) \
fhduck_many_eq_ntstring_no_case(result, (this)->data, (this)->count, b)

#define fhduckl_eq_str_lit_no_case(result, a_data, a_count, b) \
fhduck_many_eq_str_lit_no_case(result, (this)->data, (this)->count, b)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// MEMORY
//
#define fhduck_alloc(data, count) do {\
(data) = FHDUCK_DECLTYPE(data)(FHDUCK_ALLOC((count) * sizeof(*(data))));\
} while(0)

#define fhduck_realloc(data, count) do {\
(data) = FHDUCK_DECLTYPE(data)(FHDUCK_REALLOC((data), (count) * sizeof(*(data))));\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// GROW
//
#define fhduck_grow(this_data, this_capacity, new_capacity) do {\
if((new_capacity) > 0) {\
(this_capacity) = (new_capacity);\
if((this_data)) {\
fhduck_realloc(this_data, (this_capacity));\
} else {\
fhduck_alloc(this_data, (this_capacity));\
}\
}\
} while(0)

#define fhducka_grow(this, new_count) \
fhduck_grow((this)->data, (this)->count, new_count)

#define fhduckl_grow(this, new_capacity) \
fhduck_grow((this)->data, (this)->capacity, new_capacity)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// MAYBE GROW
//
#define fhduck_maybe_grow(this_data, this_capacity, new_capacity) do {\
if((new_capacity) > 0 && (new_capacity) >= (this_capacity)) {\
if((this_capacity) <= 0) { (this_capacity) = FHDUCK_LIST_DEFAULT_CAPACITY; }\
while((this_capacity) < (new_capacity)) { (this_capacity) *= 2; }\
if((this_data)) {\
fhduck_realloc(this_data, (this_capacity));\
} else {\
fhduck_alloc(this_data, (this_capacity));\
}\
}\
} while(0)

#define fhduckl_maybe_grow(this, new_capacity) \
fhduck_maybe_grow((this)->data, (this)->capacity, new_capacity)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// REVERSE
//
#define fhduck_reverse_many(T, this_data, this_count) do {\
if(this_data) {\
for(ptrdiff_t _fhduck_left_ = 0, _fhduck_right_ = (this_count);\
_fhduck_left_ < _fhduck_right_;\
_fhduck_left_ += 1, _fhduck_right_ -= 1)\
{\
T _fhtmp_                   = (this_data)[_fhduck_left_];\
(this_data)[_fhduck_left_]  = (this_data)[_fhduck_right_];\
(this_data)[_fhduck_right_] = _fhtmp_;\
}\
}\
} while(0)

#define fhduck_reverse_ntstring(ntstring) do {\
ptrdiff_t _fhduck_ntstring_;\
fhduck_get_ntstring_length(_fhduck_ntstring_, ntstring);\
fhduck_reverse_many(ntstring, _fhduck_ntstring_, char);\
} while(0)

#define fhduck_reverse_array(this, T) \
fhduck_reverse_many((this)->data, (this)->count, T)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// NULL TERMINATE
//
#define fhduck_null_terminate(this_data, this_count) do {\
(this_data)[(this_count)] = 0;\
} while(0)

#define fhducka_null_terminate(this) \
fhduck_null_terminate((this)->data, (this)->count)

#define fhduckl_null_terminate(this) do {\
fhduckl_maybe_grow(this, (this)->count + 1);\
fhduck_null_terminate((this)->data, (this)->count);\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INSERT AT
//
#define fhducka_insert_at(at_index, this, it) do {\
if((at_index) >= 0 && (at_index) < (this)->count) {\
(this)->data[(at_index)] = (it);\
}\
} while(0)

#define fhducka_insert_many_at(at_index, this, it_data, it_count) do {\
if((at_index) >= 0 && (at_index) + (it_count) < (this)->count) {\
for(ptrdiff_t _fhduck_i_ = (at_index);\
_fhduck_i_ < ((at_index) + (it_count));\
_fhduck_i_ += 1)\
{\
(this)->data[(at_index) + _fhduck_i_] = (it_data)[_fhduck_i_];\
}\
}\
} while(0)

#define fhducka_insert_array_at(at_index, this, array) \
fhducka_insert_many_at(at_index, this, (array).data, (array).count)

#define fhducka_insert_str_lit_at(at_index, this, string_literal) \
fhducka_insert_many_at(at_index, this, string_literal, sizeof(string_literal) - 1)

#define fhducka_insert_va_at(at_index, this, T, ...) \
fhducka_insert_many_at(at_index, this, FHDUCK_VA(T, __VA_ARGS__))

#define fhducka_insert_ntstring_at(at_index, this, ntstring) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, ntstring);\
fhducka_insert_many_at(at_index, this, ntstring, _fhduck_b_length_);\
} while(0)

//

#define fhduckl_insert_at(at_index, this, it) do {\
if((at_index) >= 0) {\
fhduckl_maybe_grow(this, (at_index) + 1);\
(this)->data[(at_index)] = (it);\
}\
} while(0)

#define fhduckl_insert_many_at(at_index, this, it_data, it_count) do {\
if((at_index) >= 0) {\
fhduckl_maybe_grow(this, (at_index) + (it_count));\
for(ptrdiff_t _fhduck_i_ = (at_index);\
_fhduck_i_ < ((at_index) + (it_count));\
_fhduck_i_ += 1)\
{\
(this)->data[(at_index) + _fhduck_i_] = (it_data)[_fhduck_i_];\
}\
}\
} while(0)

#define fhduckl_insert_array_at(at_index, this, array) \
fhduckl_insert_many_at(at_index, this, (array).data, (array).count)

#define fhduckl_insert_str_lit_at(at_index, this, T, ...) \
fhduckl_insert_many_at(at_index, this, string_literal, sizeof(string_literal) - 1)

#define fhduckl_insert_va_at(at_index, this, T, ...) \
fhduckl_insert_many_at(at_index, this, FHDUCK_VA(T, __VA_ARGS__))

#define fhduckl_insert_ntstring_at(at_index, this, ntstring) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, ntstring);\
fhduckl_insert_many_at(at_index, this, ntstring, _fhduck_b_length_);\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INSERT
//
#define fhducka_insert(this, it) \
fhducka_insert_at(0, this, it)

#define fhducka_insert_many(this, it_data, it_count) \
fhducka_insert_many_at(0, this, it_data, it_count)

#define fhducka_insert_array(this, array) \
fhducka_insert_array_at(0, this, array)

#define fhducka_insert_str_lit(this, string_literal) \
fhducka_insert_str_lit_at(0, this, string_literal)

#define fhducka_insert_va(this, T, ...) \
fhducka_insert_va_at(0, this, T, __VA_ARGS__)

#define fhducka_insert_ntstring(this, ntstring) \
fhducka_insert_ntstring_at(0, this, ntstring)

//

#define fhduckl_insert(this, it) \
fhduckl_insert_at(0, this, it)

#define fhduckl_insert_many(this, it_data, it_count) \
fhduckl_insert_many_at(0, this, it_data, it_count)

#define fhduckl_insert_array(this, array) \
fhduckl_insert_array_at(0, this, array)

#define fhduckl_insert_str_lit(this, string_literal) \
fhduckl_insert_str_lit_at(0, this, string_literal)

#define fhduckl_insert_va(this, T, ...) \
fhduckl_insert_va_at(0, this, T, __VA_ARGS__)

#define fhduckl_insert_ntstring(this, ntstring) \
fhduckl_insert_ntstring_at(0, this, ntstring)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// APPEND
//
#define fhduckl_append(this, it) do {\
fhduckl_maybe_grow(this, (this)->count + 1);\
(this)->data[(this)->count] = it;\
(this)->count += 1;\
} while(0)

#define fhduckl_append_many(this, it_data, it_count) do {\
fhduckl_maybe_grow(this, (this)->count + (it_count));\
for(ptrdiff_t _fhduck_i_ = 0; _fhduck_i_ < (it_count); _fhduck_i_ += 1) {\
(this)->data[(this)->count + _fhduck_i_] = (it_data)[_fhduck_i_];\
}\
(this)->count += (it_count);\
} while(0)

#define fhduckl_append_array(this, array) \
fhduckl_append_many(this, (array).data, (array).count)

#define fhduckl_append_va(this, T, ...) \
fhduckl_append_many(this, FHDUCK_VA(T, __VA_ARGS__))

#define fhduckl_append_str_lit(this, string_literal) \
fhduckl_append_many(this, string_literal, sizeof(string_literal) - 1)

#define fhduckl_append_ntstring(this, ntstring) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, ntstring);\
fhduckl_append_many(this, ntstring, _fhduck_b_length_);\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// APPEND REPEATED
//
#define fhduckl_append_repeated(this, it_value, it_count) do {\
fhduckl_maybe_grow(this, (this)->count + (it_count));\
for(ptrdiff_t _fhduck_i_ = 0; _fhduck_i_ < (it_count); _fhduck_i_ += 1) {\
(this)->data[(this)->count + _fhduck_i_] = (it_value);\
}\
(this)->count += (it_count);\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// PREPEND UNORDERED
//
#define fhduckl_prepend_unordered(this, it) do {\
fhduckl_maybe_grow(this, (this)->count + 1);\
(this)->data[(this)->count] = (this)->data[0];\
(this)->data[0] = it;\
(this)->count += 1;\
} while(0)

#define fhduckl_prepend_unordered_many(this, it_data, it_count) do {\
fhduckl_maybe_grow(this, (this)->count + (it_count));\
for(ptrdiff_t _fhduck_i_ = 0; _fhduck_i_ < (it_count); _fhduck_i_ += 1) {\
(this)->data[(this)->count + _fhduck_i_] = (this)->data[_fhduck_i_];\
(this)->data[_fhduck_i_] = (it_data)[_fhduck_i_];\
}\
(this)->count += (it_count);\
} while(0)

#define fhduckl_prepend_unordered_array(this, array) \
fhduckl_prepend_unordered_many(this, (array).data, (array).count)

#define fhduckl_prepend_unordered_va(this, T, ...) \
fhduckl_prepend_unordered_many(this, FHDUCK_VA(T, __VA_ARGS__))

#define fhduckl_prepend_unordered_str_lit(this, string_literal) \
fhduckl_prepend_unordered_many(this, string_literal, sizeof(string_literal) - 1)

#define fhduckl_prepend_unordered_ntstring(this, ntstring) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, ntstring);\
fhduckl_prepend_unordered_many(this, ntstring, _fhduck_b_length_)\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// PREPEND ORDERED
//
#define fhduckl_prepend_ordered(this, it) do {\
fhduckl_maybe_grow(this, (this)->count + 1);\
for(ptrdiff_t _fhduck_i_ = (this)->count; _fhduck_i_ >= 1; _fhduck_i_ -= 1) {\
(this)->data[_fhduck_i_] = (this)->data[_fhduck_i_ - 1];\
}\
(this)->data[0] = it;\
(this)->count += 1;\
} while(0)

#define fhduckl_prepend_ordered_many(this, it_data, it_count) do {\
fhduckl_maybe_grow(this, (this)->count + (it_count));\
for(ptrdiff_t _fhduck_i_ = (this)->count + ((it_count) - 1);\
_fhduck_i_ >= (it_count);\
_fhduck_i_ -= 1)\
{\
(this)->data[_fhduck_i_] = (this)->data[_fhduck_i_ - (it_count)];\
}\
for(ptrdiff_t _fhduck_i_ = 0; _fhduck_i_ < (it_count); _fhduck_i_ += 1) {\
(this)->data[(this)->count + _fhduck_i_] = (it_data)[_fhduck_i_];\
}\
(this)->count += (it_count);\
} while(0)

#define fhduckl_prepend_ordered_array(this, array) \
fhduckl_prepend_ordered_many(this, (array).data, (array).count)

#define fhduckl_prepend_ordered_va(this, T, ...) \
fhduckl_prepend_ordered_many(this, FHDUCK_VA(T, __VA_ARGS__))

#define fhduckl_prepend_ordered_str_lit(this, string_literal) \
fhduckl_prepend_ordered_many(this, string_literal, sizeof(string_literal) - 1)

#define fhduckl_prepend_ordered_ntstring(this, ntstring) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, ntstring);\
fhduckl_prepend_ordered_many(this, ntstring, _fhduck_b_length_)\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// REMOVE
//
#define fhduckl_remove_unordered(this, index) do {\
if((this)->count >= 1) {\
(this)->data[(index)] = (this)->data[(this)->count - 1];\
(this)->count -= 1;\
}\
} while(0)

#define fhduckl_remove_unordered_range(this, start_index, it_length) do {\
if((start_index) >= 0) {\
for(ptrdiff_t _fhduck_i_ = (start_index) + ((it_length) - 1);\
_fhduck_i_ >= (start_index);\
_fhduck_i_ -= 1)\
{\
(this)->data[_fhduck_i_] = (this)->data[((this)->count) - 1];\
(this)->count -= 1;\
}\
}\
} while(0)

//

#define fhduckl_remove_ordered(this, index) do {\
if((this)->count >= 1 && (index) < (this)->count) {\
if((index) > 0) {\
for(ptrdiff_t _fhduck_i_ = (index);\
_fhduck_i_ < (this)->count - 1;\
_fhduck_i_ += 1)\
{\
(this)->data[_fhduck_i_] = (this)->data[_fhduck_i_ + 1];\
}\
} else {\
for(ptrdiff_t _fhduck_i_ = 1;\
_fhduck_i_ < (this)->count;\
_fhduck_i_ += 1)\
{\
(this)->data[_fhduck_i_ - 1] = (this)->data[_fhduck_i_];\
}\
}\
(this)->count -= 1;\
}\
} while(0)

#define fhduckl_remove_ordered_range(this, start_index, it_length) do {\
if((start_index) >= 0 && (start_index) < (this)->count) {\
for(ptrdiff_t _fhduck_i_ = ((start_index) + (it_length));\
_fhduck_i_ < (this)->count;\
_fhduck_i_ += 1)\
{\
(this)->data[_fhduck_i_ - (it_length)] = (this)->data[_fhduck_i_];\
}\
if(((start_index) + (it_length)) >= (this)->count) {\
(this)->count -= ((start_index) + (it_length)) - (this)->count;\
} else {\
(this)->count -= (it_length);\
}\
}\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// BEGINS WITH AT
//
#define fhduck_many_begins_with_at(result, at_index, this_data, this_count, it) do {\
(result) = 0;\
if((this_data) && ((at_index) >= 0) && ((this_count) >= (at_index) + 1)) {\
if((this_data)[at_index] == (it)) {\
(result) = 1;\
}\
}\
} while(0)

#define fhduck_many_begins_with_many_at(result, at_index, this_data, this_count, it_data, it_count) do {\
(result) = 0;\
if((this_data) && (it_data) && ((at_index) >= 0) && ((this_count) >= ((at_index) + (it_count)))) {\
(result) = 1;\
for(ptrdiff_t _fhduck_i_ = (at_index);\
_fhduck_i_ < ((at_index) + (it_count));\
_fhduck_i_ += 1)\
{\
if((this_data)[(at_index) + _fhduck_i_] != (it_data)[_fhduck_i_]) {\
(result) = 0;\
break;\
}\
}\
}\
} while(0)

#define fhduck_many_begins_with_array_at(result, at_index, this_data, this_count, it) do {\
fhduck_many_begins_with_many_at(result, at_index, this_data, this_count, (it).data, (it).count);\
} while(0)

#define fhduck_many_begins_with_va_at(result, at_index, this_data, this_count, T, ...) do {\
fhduck_many_begins_with_many_at(result, at_index, this_data, this_count, FHDUCK_VA(T, __VA_ARGS__));\
} while(0)

#define fhduck_many_begins_with_str_lit_at(result, at_index, this_data, this_count, string_literal) do {\
fhduck_many_begins_with_many_at(result, at_index, this_data, this_count, string_literal, sizeof(string_literal) - 1);\
} while(0)

#define fhduck_many_begins_with_ntstring_at(result, at_index, this_data, this_count, ntstring) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, ntstring);\
fhduck_many_begins_with_many_at(result, at_index, this_data, this_count, ntstring, _fhduck_b_length_);\
} while(0)

//

#define fhduck_array_begins_with_at(result, at_index, this, it) \
fhduck_many_begins_with_at(result, at_index, (this).data, (this).count, it)

#define fhduck_array_begins_with_many_at(result, at_index, this, it_data, it_count) \
fhduck_many_begins_with_many_at(result, at_index, (this).data, (this).count, it_data, it_count)

#define fhduck_array_begins_with_array_at(result, at_index, this, it) \
fhduck_many_begins_with_array_at(result, at_index, (this).data, (this).count, it)

#define fhduck_array_begins_with_va_at(result, at_index, this, T, ...) \
fhduck_many_begins_with_va_at(result, at_index, (this).data, (this).count, T, __VA_ARGS__)

#define fhduck_array_begins_with_str_lit_at(result, at_index, this, string_literal) \
fhduck_many_begins_with_str_lit_at(result, at_index, (this).data, (this).count, string_literal)

#define fhduck_array_begins_with_ntstring_at(result, at_index, this, ntstring) \
fhduck_many_begins_with_ntstring_at(result, at_index, (this).data, (this).count, ntstring)

//

#define fhduck_str_lit_begins_with_at(result, at_index, this, it) \
fhduck_many_begins_with_at(result, at_index, FHDUCK_STR_LIT(this), it)

#define fhduck_str_lit_begins_with_many_at(result, at_index, this, it_data, it_count) \
fhduck_many_begins_with_at(result, at_index, FHDUCK_STR_LIT(this), it_data, it_count)

#define fhduck_str_lit_begins_with_array_at(result, at_index, this, it) \
fhduck_many_begins_with_array_at(result, at_index, FHDUCK_STR_LIT(this), it)

#define fhduck_str_lit_begins_with_va_at(result, at_index, this, T, ...) \
fhduck_many_begins_with_va_at(result, at_index, FHDUCK_STR_LIT(this), T, __VA_ARGS__)

#define fhduck_str_lit_begins_with_str_lit_at(result, at_index, this, string_literal) \
fhduck_many_begins_with_str_lit_at(result, at_index, FHDUCK_STR_LIT(this), string_literal)

#define fhduck_str_lit_begins_with_ntstring_at(result, at_index, this, ntstring) \
fhduck_many_begins_with_ntstring_at(result, at_index, FHDUCK_STR_LIT(this), ntstring)

//

#define fhduck_ntstring_begins_with_at(result, at_index, this, it) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_begins_with_at(result, at_index, this, _fhduck_a_length_, it);\
} while(0)

#define fhduck_ntstring_begins_with_many_at(result, at_index, this, it_data, it_count) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_begins_with_at(result, at_index, this, _fhduck_a_length_, it_data, it_count);\
} while(0)

#define fhduck_ntstring_begins_with_array_at(result, at_index, this, it) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_begins_with_array_at(result, at_index, this, _fhduck_a_length_, it);\
} while(0)

#define fhduck_ntstring_begins_with_va_at(result, at_index, this, T, ...) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_begins_with_va_at(result, at_index, this, _fhduck_a_length_, T, __VA_ARGS__);\
} while(0)

#define fhduck_ntstring_begins_with_str_lit_at(result, at_index, this, string_literal) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_begins_with_str_lit_at(result, at_index, this, _fhduck_a_length_, string_literal);\
} while(0)

#define fhduck_ntstring_begins_with_ntstring_at(result, at_index, this, ntstring) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_begins_with_ntstring_at(result, at_index, this, _fhduck_a_length_, ntstring)\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// BEGINS WITH
//
#define fhduck_many_begins_with(result, this_data, this_count, it) \
fhduck_many_begins_with_at(result, 0, this_data, this_count, it)

#define fhduck_many_begins_with_many(result, this_data, this_count, it_data, it_count) \
fhduck_many_begins_with_many_at(result, 0, this_data, this_count, it_data, it_count)

#define fhduck_many_begins_with_array(result, this_data, this_count, it) \
fhduck_many_begins_with_array_at(result, 0, this_data, this_count, it)

#define fhduck_many_begins_with_va(result, this_data, this_count, T, ...) \
fhduck_many_begins_with_va_at(result, 0, this_data, this_count, T, __VA_ARGS__)

#define fhduck_many_begins_with_str_lit(result, this_data, this_count, string_literal) \
fhduck_many_begins_with_str_lit_at(result, 0, this_data, this_count, string_literal)

#define fhduck_many_begins_with_ntstring(result, this_data, this_count, ntstring) \
fhduck_many_begins_with_ntstring_at(result, 0, this_data, this_count, ntstring)

//

#define fhduck_array_begins_with(result, this, it) \
fhduck_array_begins_with_at(result, 0, this, it)

#define fhduck_array_begins_with_many(result, this, it_data, it_count) \
fhduck_array_begins_with_many_at(result, 0, this, it_data, it_count)

#define fhduck_array_begins_with_array(result, this, it) \
fhduck_array_begins_with_array_at(result, 0, this, it)

#define fhduck_array_begins_with_va(result, this, T, ...) \
fhduck_array_begins_with_va_at(result, 0, this, T, __VA_ARGS__)

#define fhduck_array_begins_with_str_lit(result, this, string_literal) \
fhduck_array_begins_with_str_lit_at(result, 0, this, string_literal)

#define fhduck_array_begins_with_ntstring(result, this, ntstring) \
fhduck_array_begins_with_ntstring_at(result, 0, this, ntstring)

//

#define fhduck_str_lit_begins_with(result, this, it) \
fhduck_str_lit_begins_with_at(result, 0, this, it)

#define fhduck_str_lit_begins_with_many(result, this, it_data, it_count) \
fhduck_str_lit_begins_with_many_at(result, 0, this, it_data, it_count)

#define fhduck_str_lit_begins_with_array(result, this, it) \
fhduck_str_lit_begins_with_array_at(result, 0, this, it)

#define fhduck_str_lit_begins_with_va(result, this, T, ...) \
fhduck_str_lit_begins_with_va_at(result, 0, this, T, ...)

#define fhduck_str_lit_begins_with_str_lit(result, this, string_literal) \
fhduck_str_lit_begins_with_str_lit_at(result, 0, this, string_literal)

#define fhduck_str_lit_begins_with_ntstring(result, this, ntstring) \
fhduck_str_lit_begins_with_ntstring_at(result, 0, this, ntstring)

//

#define fhduck_ntstring_begins_with(result, this, it) \
fhduck_ntstring_begins_with_at(result, 0, this, it)

#define fhduck_ntstring_begins_with_many(result, this, it_data, it_count) \
fhduck_ntstring_begins_with_many_at(result, 0, this, it_data, it_count)

#define fhduck_ntstring_begins_with_array(result, this, it) \
fhduck_ntstring_begins_with_array_at(result, 0, this, it)

#define fhduck_ntstring_begins_with_va(result, this, T, ...) \
fhduck_ntstring_begins_with_va_at(result, 0, this, T, __VA_ARGS__)

#define fhduck_ntstring_begins_with_str_lit(result, this, string_literal) \
fhduck_ntstring_begins_with_str_lit_at(result, 0, this, string_literal)

#define fhduck_ntstring_begins_with_ntstring(result, this, ntstring) \
fhduck_ntstring_begins_with_ntstring_at(result, 0, this, ntstring)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// ENDS WITH
//
#define fhduck_many_ends_with(result, this_data, this_count, it) do {\
(result) = 0;\
if((this_data) && ((this_count) >= 1)) {\
if((this_data)[(this_count) - 1] == (it)) {\
(result) = 1;\
}\
}\
} while(0)

#define fhduck_many_ends_with_many(result, this_data, this_count, it_data, it_count) do {\
(result) = 0;\
if((this_data) && (it_data) && ((it_count) > 0) && ((this_count) >= (it_count))) {\
(result) = 1;\
for(ptrdiff_t _fhduck_i_ = ((this_count) - (it_count));\
_fhduck_i_ < (it_count);\
_fhduck_i_ += 1)\
{\
if((this_data)[((this_count) - (it_count)) + _fhduck_i_] != (it_data)[_fhduck_i_]) {\
(result) = 0;\
break;\
}\
}\
}\
} while(0)

#define fhduck_many_ends_with_array(result, this_data, this_count, it) do {\
fhduck_many_ends_with_many(result, this_data, this_count, (it).data, (it).count);\
} while(0)

#define fhduck_many_ends_with_va(result, this_data, this_count, T, ...) do {\
fhduck_many_ends_with_many(result, this_data, this_count, FHDUCK_VA(T, __VA_ARGS__));\
} while(0)

#define fhduck_many_ends_with_str_lit(result, this_data, this_count, string_literal) do {\
fhduck_many_ends_with_many(result, this_data, this_count, string_literal, sizeof(string_literal) - 1);\
} while(0)

#define fhduck_many_ends_with_ntstring(result, this_data, this_count, ntstring) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, ntstring);\
fhduck_many_ends_with_many(result, this_data, this_count, ntstring, _fhduck_b_length_);\
} while(0)

//

#define fhduck_array_ends_with(result, this, it) \
fhduck_many_ends_with(result, (this).data, (this).count, it)

#define fhduck_array_ends_with_many(result, this, it_data, it_count) \
fhduck_many_ends_with(result, (this).data, (this).count, it_data, it_count)

#define fhduck_array_ends_with_array(result, this, it) \
fhduck_many_ends_with_array(result, (this).data, (this).count, it)

#define fhduck_array_ends_with_va(result, this, T, ...) \
fhduck_many_ends_with_va(result, (this).data, (this).count, T, __VA_ARGS__)

#define fhduck_array_ends_with_str_lit(result, this, string_literal) \
fhduck_many_ends_with_str_lit(result, (this).data, (this).count, string_literal)

#define fhduck_array_ends_with_ntstring(result, this, ntstring) \
fhduck_many_ends_with_ntstring(result, (this).data, (this).count, ntstring)

//

#define fhduck_str_lit_ends_with(result, this, it) \
fhduck_many_ends_with(result, FHDUCK_STR_LIT(this), it)

#define fhduck_str_lit_ends_with_many(result, this, it_data, it_count) \
fhduck_many_ends_with(result, FHDUCK_STR_LIT(this), it_data, it_count)

#define fhduck_str_lit_ends_with_array(result, this, it) \
fhduck_many_ends_with_array(result, FHDUCK_STR_LIT(this), it)

#define fhduck_str_lit_ends_with_va(result, this, T, ...) \
fhduck_many_ends_with_va(result, FHDUCK_STR_LIT(this), T, __VA_ARGS__)

#define fhduck_str_lit_ends_with_str_lit(result, this, string_literal) \
fhduck_many_ends_with_str_lit(result, FHDUCK_STR_LIT(this), string_literal)

#define fhduck_str_lit_ends_with_ntstring(result, this, ntstring) \
fhduck_many_ends_with_ntstring(result, FHDUCK_STR_LIT(this), ntstring)

//

#define fhduck_ntstring_ends_with(result, this, it) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_ends_with(result, this, _fhduck_a_length_, it);\
} while(0)

#define fhduck_ntstring_ends_with_many(result, this, it_data, it_count) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_ends_with(result, this, _fhduck_a_length_, it_data, it_count);\
} while(0)

#define fhduck_ntstring_ends_with_array(result, this, it) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_ends_with_array(result, this, _fhduck_a_length_, it);\
} while(0)

#define fhduck_ntstring_ends_with_va(result, this, T, ...) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_ends_with_va(result, this, _fhduck_a_length_, T, __VA_ARGS__);\
} while(0)

#define fhduck_ntstring_ends_with_str_lit(result, this, string_literal) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_ends_with_str_lit(result, this, _fhduck_a_length_, string_literal);\
} while(0)

#define fhduck_ntstring_ends_with_ntstring(result, this, ntstring) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_ends_with_ntstring(result, this, _fhduck_a_length_, ntstring)\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INDEX OF
//
#define fhduck_many_index_of(result, this_data, this_count, it) do {\
(result) = -1;\
if(this_data) {\
for(ptrdiff_t _fhduck_i_ = 0; _fhduck_i_ < (this_count); _fhduck_i_ += 1) {\
if((this_data)[_fhduck_i_] == (it)) {\
(result) = _fhduck_i_;\
break;\
}\
}\
}\
} while(0)

#define fhduck_many_index_of_many(result, this_data, this_count, it_data, it_count) do {\
(result) = -1;\
if((this_data) && (it_data) && ((this_count) >= (it_count)))) {\
for(ptrdiff_t _fhduck_i_ = 0;\
_fhduck_i_ < ((this_count) - (it_count));\
_fhduck_i_ += 1)\
{\
(result) = _fhduck_i_;\
for(ptrdiff_t _fhduck_j_ = 0; _fhduck_j_ < (it_count); _fhduck_j_ += 1) {\
if((this_data)[_fhduck_i_ + _fhduck_j_] != (it_data)[_fhduck_j_]) {\
(result) = -1;\
break;\
}\
}\
if((result) > -1) { break; }\
}\
}\
} while(0)

#define fhduck_many_index_of_array(result, this_data, this_count, it) do {\
fhduck_many_index_of_many(result, this_data, this_count, (it).data, (it).count);\
} while(0)

#define fhduck_many_index_of_va(result, this_data, this_count, T, ...) do {\
fhduck_many_index_of_many(result, this_data, this_count, FHDUCK_VA(T, __VA_ARGS__));\
} while(0)

#define fhduck_many_index_of_str_lit(result, this_data, this_count, string_literal) do {\
fhduck_many_index_of_many(result, this_data, this_count, string_literal, sizeof(string_literal) - 1);\
} while(0)

#define fhduck_many_index_of_ntstring(result, this_data, this_count, ntstring) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, ntstring);\
fhduck_many_index_of_many(result, this_data, this_count, ntstring, _fhduck_b_length_);\
} while(0)

//

#define fhduck_array_index_of(result, this, it) \
fhduck_many_index_of(result, (this).data, (this).count, it)

#define fhduck_array_index_of_many(result, this, it_data, it_count) \
fhduck_many_index_of(result, (this).data, (this).count, it_data, it_count)

#define fhduck_array_index_of_array(result, this, it) \
fhduck_many_index_of_array(result, (this).data, (this).count, it)

#define fhduck_array_index_of_va(result, this, T, ...) \
fhduck_many_index_of_va(result, (this).data, (this).count, T, __VA_ARGS__)

#define fhduck_array_index_of_str_lit(result, this, string_literal) \
fhduck_many_index_of_str_lit(result, (this).data, (this).count, string_literal)

#define fhduck_array_index_of_ntstring(result, this, ntstring) \
fhduck_many_index_of_ntstring(result, (this).data, (this).count, ntstring)

//

#define fhduck_str_lit_index_of(result, this, it) \
fhduck_many_index_of(result, FHDUCK_STR_LIT(this), it)

#define fhduck_str_lit_index_of_many(result, this, it_data, it_count) \
fhduck_many_index_of(result, FHDUCK_STR_LIT(this), it_data, it_count)

#define fhduck_str_lit_index_of_array(result, this, it) \
fhduck_many_index_of_array(result, FHDUCK_STR_LIT(this), it)

#define fhduck_str_lit_index_of_va(result, this, T, ...) \
fhduck_many_index_of_va(result, FHDUCK_STR_LIT(this), T, __VA_ARGS__)

#define fhduck_str_lit_index_of_str_lit(result, this, string_literal) \
fhduck_many_index_of_str_lit(result, FHDUCK_STR_LIT(this), string_literal)

#define fhduck_str_lit_index_of_ntstring(result, this, ntstring) \
fhduck_many_index_of_ntstring(result, FHDUCK_STR_LIT(this), ntstring)

//

#define fhduck_ntstring_index_of(result, this, it) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_index_of(result, this, _fhduck_a_length_, it);\
} while(0)

#define fhduck_ntstring_index_of_many(result, this, it_data, it_count) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_index_of(result, this, _fhduck_a_length_, it_data, it_count);\
} while(0)

#define fhduck_ntstring_index_of_array(result, this, it) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_index_of_array(result, this, _fhduck_a_length_, it);\
} while(0)

#define fhduck_ntstring_index_of_va(result, this, T, ...) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_index_of_va(result, this, _fhduck_a_length_, T, __VA_ARGS__);\
} while(0)

#define fhduck_ntstring_index_of_str_lit(result, this, string_literal) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_index_of_str_lit(result, this, _fhduck_a_length_, string_literal);\
} while(0)

#define fhduck_ntstring_index_of_ntstring(result, this, ntstring) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_index_of_ntstring(result, this, _fhduck_a_length_, ntstring)\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INDEX OF ANY
//
#define fhduck_many_index_of_any_many(result, this_data, this_count, it_data, it_count) do {\
(result) = -1;\
if((this_data) && (it_data) && ((this_count) >= (it_count)))) {\
for(ptrdiff_t _fhduck_i_ = 0;\
_fhduck_i_ < ((this_count) - (it_count));\
_fhduck_i_ += 1)\
{\
for(ptrdiff_t _fhduck_j_ = 0; _fhduck_j_ < (it_count); _fhduck_j_ += 1) {\
if((this_data)[_fhduck_i_ + _fhduck_j_] == (it_data)[_fhduck_j_]) {\
(result) = _fhduck_i_;\
break;\
}\
}\
if((result) > -1) { break; }\
}\
}\
} while(0)

#define fhduck_many_index_of_any_array(result, this_data, this_count, it) do {\
fhduck_many_index_of_any_many(result, this_data, this_count, (it).data, (it).count);\
} while(0)

#define fhduck_many_index_of_any_va(result, this_data, this_count, T, ...) do {\
fhduck_many_index_of_any_many(result, this_data, this_count, FHDUCK_VA(T, __VA_ARGS__));\
} while(0)

#define fhduck_many_index_of_any_str_lit(result, this_data, this_count, string_literal) do {\
fhduck_many_index_of_any_many(result, this_data, this_count, string_literal, sizeof(string_literal) - 1);\
} while(0)

#define fhduck_many_index_of_any_ntstring(result, this_data, this_count, ntstring) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, ntstring);\
fhduck_many_index_of_any_many(result, this_data, this_count, ntstring, _fhduck_b_length_);\
} while(0)

//

#define fhduck_array_index_of_any_many(result, this, it_data, it_count) \
fhduck_many_index_of_any(result, (this).data, (this).count, it_data, it_count)

#define fhduck_array_index_of_any_array(result, this, it) \
fhduck_many_index_of_any_array(result, (this).data, (this).count, it)

#define fhduck_array_index_of_any_va(result, this, T, ...) \
fhduck_many_index_of_any_va(result, (this).data, (this).count, T, __VA_ARGS__)

#define fhduck_array_index_of_any_str_lit(result, this, string_literal) \
fhduck_many_index_of_any_str_lit(result, (this).data, (this).count, string_literal)

#define fhduck_array_index_of_any_ntstring(result, this, ntstring) \
fhduck_many_index_of_any_ntstring(result, (this).data, (this).count, ntstring)

//

#define fhduck_str_lit_index_of_any_many(result, this, it_data, it_count) \
fhduck_many_index_of_any(result, FHDUCK_STR_LIT(this), it_data, it_count)

#define fhduck_str_lit_index_of_any_array(result, this, it) \
fhduck_many_index_of_any_array(result, FHDUCK_STR_LIT(this), it)

#define fhduck_str_lit_index_of_any_va(result, this, T, ...) \
fhduck_many_index_of_any_va(result, FHDUCK_STR_LIT(this), T, __VA_ARGS__)

#define fhduck_str_lit_index_of_any_str_lit(result, this, string_literal) \
fhduck_many_index_of_any_str_lit(result, FHDUCK_STR_LIT(this), string_literal)

#define fhduck_str_lit_index_of_any_ntstring(result, this, ntstring) \
fhduck_many_index_of_any_ntstring(result, FHDUCK_STR_LIT(this), ntstring)

//

#define fhduck_ntstring_index_of_any_many(result, this, it_data, it_count) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_index_of_any(result, this, _fhduck_a_length_, it_data, it_count);\
} while(0)

#define fhduck_ntstring_index_of_any_array(result, this, it) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_index_of_any_array(result, this, _fhduck_a_length_, it);\
} while(0)

#define fhduck_ntstring_index_of_any_va(result, this, T, ...) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_index_of_any_va(result, this, _fhduck_a_length_, T, __VA_ARGS__);\
} while(0)

#define fhduck_ntstring_index_of_any_str_lit(result, this, string_literal) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_index_of_any_str_lit(result, this, _fhduck_a_length_, string_literal);\
} while(0)

#define fhduck_ntstring_index_of_any_ntstring(result, this, ntstring) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_index_of_any_ntstring(result, this, _fhduck_a_length_, ntstring)\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// LAST INDEX OF
//
#define fhduck_many_last_index_of(result, this_data, this_count, it) do {\
(result) = -1;\
if(this_data) {\
for(ptrdiff_t _fhduck_i_ = (this_count) - 1; _fhduck_i_ >= 0; _fhduck_i_ -= 1) {\
if((this_data)[_fhduck_i_] == (it)) {\
(result) = _fhduck_i_;\
break;\
}\
}\
}\
} while(0)

#define fhduck_many_last_index_of_many(result, this_data, this_count, it_data, it_count) do {\
(result) = -1;\
if((this_data) && (it_data) && ((this_count) >= (it_count)))) {\
for(ptrdiff_t _fhduck_i_ = ((this_count) - (it_count)) - 1;\
_fhduck_i_ >= 0;\
_fhduck_i_ -= 1)\
{\
(result) = _fhduck_i_;\
for(ptrdiff_t _fhduck_j_ = 0; _fhduck_j_ < (it_count); _fhduck_j_ += 1) {\
if((this_data)[_fhduck_i_ + _fhduck_j_] != (it_data)[_fhduck_j_]) {\
(result) = -1;\
break;\
}\
}\
if((result) > -1) { break; }\
}\
}\
} while(0)

#define fhduck_many_last_index_of_array(result, this_data, this_count, it) do {\
fhduck_many_last_index_of_many(result, this_data, this_count, (it).data, (it).count);\
} while(0)

#define fhduck_many_last_index_of_va(result, this_data, this_count, T, ...) do {\
fhduck_many_last_index_of_many(result, this_data, this_count, FHDUCK_VA(T, __VA_ARGS__));\
} while(0)

#define fhduck_many_last_index_of_str_lit(result, this_data, this_count, string_literal) do {\
fhduck_many_last_index_of_many(result, this_data, this_count, string_literal, sizeof(string_literal) - 1);\
} while(0)

#define fhduck_many_last_index_of_ntstring(result, this_data, this_count, ntstring) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, ntstring);\
fhduck_many_last_index_of_many(result, this_data, this_count, ntstring, _fhduck_b_length_);\
} while(0)

//

#define fhduck_array_last_index_of(result, this, it) \
fhduck_many_last_index_of(result, (this).data, (this).count, it)

#define fhduck_array_last_index_of_many(result, this, it_data, it_count) \
fhduck_many_last_index_of(result, (this).data, (this).count, it_data, it_count)

#define fhduck_array_last_index_of_array(result, this, it) \
fhduck_many_last_index_of_array(result, (this).data, (this).count, it)

#define fhduck_array_last_index_of_va(result, this, T, ...) \
fhduck_many_last_index_of_va(result, (this).data, (this).count, T, __VA_ARGS__)

#define fhduck_array_last_index_of_str_lit(result, this, string_literal) \
fhduck_many_last_index_of_str_lit(result, (this).data, (this).count, string_literal)

#define fhduck_array_last_index_of_ntstring(result, this, ntstring) \
fhduck_many_last_index_of_ntstring(result, (this).data, (this).count, ntstring)

//

#define fhduck_str_lit_last_index_of(result, this, it) \
fhduck_many_last_index_of(result, FHDUCK_STR_LIT(this), it)

#define fhduck_str_lit_last_index_of_many(result, this, it_data, it_count) \
fhduck_many_last_index_of(result, FHDUCK_STR_LIT(this), it_data, it_count)

#define fhduck_str_lit_last_index_of_array(result, this, it) \
fhduck_many_last_index_of_array(result, FHDUCK_STR_LIT(this), it)

#define fhduck_str_lit_last_index_of_va(result, this, T, ...) \
fhduck_many_last_index_of_va(result, FHDUCK_STR_LIT(this), T, __VA_ARGS__)

#define fhduck_str_lit_last_index_of_str_lit(result, this, string_literal) \
fhduck_many_last_index_of_str_lit(result, FHDUCK_STR_LIT(this), string_literal)

#define fhduck_str_lit_last_index_of_ntstring(result, this, ntstring) \
fhduck_many_last_index_of_ntstring(result, FHDUCK_STR_LIT(this), ntstring)

//

#define fhduck_ntstring_last_index_of(result, this, it) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_last_index_of(result, this, _fhduck_a_length_, it);\
} while(0)

#define fhduck_ntstring_last_index_of_many(result, this, it_data, it_count) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_last_index_of(result, this, _fhduck_a_length_, it_data, it_count);\
} while(0)

#define fhduck_ntstring_last_index_of_array(result, this, it) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_last_index_of_array(result, this, _fhduck_a_length_, it);\
} while(0)

#define fhduck_ntstring_last_index_of_va(result, this, T, ...) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_last_index_of_va(result, this, _fhduck_a_length_, T, __VA_ARGS__);\
} while(0)

#define fhduck_ntstring_last_index_of_str_lit(result, this, string_literal) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_last_index_of_str_lit(result, this, _fhduck_a_length_, string_literal);\
} while(0)

#define fhduck_ntstring_last_index_of_ntstring(result, this, ntstring) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_last_index_of_ntstring(result, this, _fhduck_a_length_, ntstring)\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// LAST INDEX OF ANY
//
#define fhduck_many_last_index_of_any_many(result, this_data, this_count, it_data, it_count) do {\
(result) = -1;\
if((this_data) && (it_data) && ((this_count) >= (it_count)))) {\
for(ptrdiff_t _fhduck_i_ = ((this_count) - (it_count)) - 1;\
_fhduck_i_ >= 0;\
_fhduck_i_ -= 1)\
{\
for(ptrdiff_t _fhduck_j_ = 0; _fhduck_j_ < (it_count); _fhduck_j_ += 1) {\
if((this_data)[_fhduck_i_ + _fhduck_j_] == (it_data)[_fhduck_j_]) {\
(result) = _fhduck_i_;\
break;\
}\
}\
if((result) > -1) { break; }\
}\
}\
} while(0)

#define fhduck_many_last_index_of_any_array(result, this_data, this_count, it) do {\
fhduck_many_last_index_of_any_many(result, this_data, this_count, (it).data, (it).count);\
} while(0)

#define fhduck_many_last_index_of_any_va(result, this_data, this_count, T, ...) do {\
fhduck_many_last_index_of_any_many(result, this_data, this_count, FHDUCK_VA(T, __VA_ARGS__));\
} while(0)

#define fhduck_many_last_index_of_any_str_lit(result, this_data, this_count, string_literal) do {\
fhduck_many_last_index_of_any_many(result, this_data, this_count, string_literal, sizeof(string_literal) - 1);\
} while(0)

#define fhduck_many_last_index_of_any_ntstring(result, this_data, this_count, ntstring) do {\
ptrdiff_t _fhduck_b_length_;\
fhduck_get_ntstring_length(_fhduck_b_length_, ntstring);\
fhduck_many_last_index_of_any_many(result, this_data, this_count, ntstring, _fhduck_b_length_);\
} while(0)

//

#define fhduck_array_last_index_of_any_many(result, this, it_data, it_count) \
fhduck_many_last_index_of_any(result, (this).data, (this).count, it_data, it_count)

#define fhduck_array_last_index_of_any_array(result, this, it) \
fhduck_many_last_index_of_any_array(result, (this).data, (this).count, it)

#define fhduck_array_last_index_of_any_va(result, this, T, ...) \
fhduck_many_last_index_of_any_va(result, (this).data, (this).count, T, __VA_ARGS__)

#define fhduck_array_last_index_of_any_str_lit(result, this, string_literal) \
fhduck_many_last_index_of_any_str_lit(result, (this).data, (this).count, string_literal)

#define fhduck_array_last_index_of_any_ntstring(result, this, ntstring) \
fhduck_many_last_index_of_any_ntstring(result, (this).data, (this).count, ntstring)

//

#define fhduck_str_lit_last_index_of_any_many(result, this, it_data, it_count) \
fhduck_many_last_index_of_any(result, FHDUCK_STR_LIT(this), it_data, it_count)

#define fhduck_str_lit_last_index_of_any_array(result, this, it) \
fhduck_many_last_index_of_any_array(result, FHDUCK_STR_LIT(this), it)

#define fhduck_str_lit_last_index_of_any_va(result, this, T, ...) \
fhduck_many_last_index_of_any_va(result, FHDUCK_STR_LIT(this), T, __VA_ARGS__)

#define fhduck_str_lit_last_index_of_any_str_lit(result, this, string_literal) \
fhduck_many_last_index_of_any_str_lit(result, FHDUCK_STR_LIT(this), string_literal)

#define fhduck_str_lit_last_index_of_any_ntstring(result, this, ntstring) \
fhduck_many_last_index_of_any_ntstring(result, FHDUCK_STR_LIT(this), ntstring)

//

#define fhduck_ntstring_last_index_of_any_many(result, this, it_data, it_count) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_last_index_of_any(result, this, _fhduck_a_length_, it_data, it_count);\
} while(0)

#define fhduck_ntstring_last_index_of_any_array(result, this, it) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_last_index_of_any_array(result, this, _fhduck_a_length_, it);\
} while(0)

#define fhduck_ntstring_last_index_of_any_va(result, this, T, ...) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_last_index_of_any_va(result, this, _fhduck_a_length_, T, __VA_ARGS__);\
} while(0)

#define fhduck_ntstring_last_index_of_any_str_lit(result, this, string_literal) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_last_index_of_any_str_lit(result, this, _fhduck_a_length_, string_literal);\
} while(0)

#define fhduck_ntstring_last_index_of_any_ntstring(result, this, ntstring) do {\
ptrdiff_t _fhduck_a_length_;\
fhduck_get_ntstring_length(_fhduck_a_length_, this);\
fhduck_many_last_index_of_any_ntstring(result, this, _fhduck_a_length_, ntstring)\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// ALLOC
//
#define fhduckN_alloc(N, this, count) fhduck##N##_alloc(this, count)

#define fhduck0_alloc(this, count) do {\
fhduck_alloc((this)->data, count);\
} while(0)

#define fhduck1_alloc(this, count) do {\
fhduck_alloc((this)->data0, count);\
} while(0)

#define fhduck2_alloc(this, count) do {\
fhduck_alloc((this)->data0, count);\
fhduck_alloc((this)->data1, count);\
} while(0)

#define fhduck3_alloc(this, count) do {\
fhduck_alloc((this)->data0, count);\
fhduck_alloc((this)->data1, count);\
fhduck_alloc((this)->data2, count);\
} while(0)

#define fhduck4_alloc(this, count) do {\
fhduck_alloc((this)->data0, count);\
fhduck_alloc((this)->data1, count);\
fhduck_alloc((this)->data2, count);\
fhduck_alloc((this)->data3, count);\
} while(0)

#define fhduck5_alloc(this, count) do {\
fhduck_alloc((this)->data0, count);\
fhduck_alloc((this)->data1, count);\
fhduck_alloc((this)->data2, count);\
fhduck_alloc((this)->data3, count);\
fhduck_alloc((this)->data4, count);\
} while(0)

#define fhduck6_alloc(this, count) do {\
fhduck_alloc((this)->data0, count);\
fhduck_alloc((this)->data1, count);\
fhduck_alloc((this)->data2, count);\
fhduck_alloc((this)->data3, count);\
fhduck_alloc((this)->data4, count);\
fhduck_alloc((this)->data5, count);\
} while(0)

#define fhduck7_alloc(this, count) do {\
fhduck_alloc((this)->data0, count);\
fhduck_alloc((this)->data1, count);\
fhduck_alloc((this)->data2, count);\
fhduck_alloc((this)->data3, count);\
fhduck_alloc((this)->data4, count);\
fhduck_alloc((this)->data5, count);\
fhduck_alloc((this)->data6, count);\
} while(0)

#define fhduck8_alloc(this, count) do {\
fhduck_alloc((this)->data0, count);\
fhduck_alloc((this)->data1, count);\
fhduck_alloc((this)->data2, count);\
fhduck_alloc((this)->data3, count);\
fhduck_alloc((this)->data4, count);\
fhduck_alloc((this)->data5, count);\
fhduck_alloc((this)->data6, count);\
fhduck_alloc((this)->data7, count);\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// REALLOC
//
#define fhduckN_realloc(N, this, count) fhduck##N##_realloc(this, count)

#define fhduck0_realloc(this, count) do {\
fhduck_realloc((this)->data, count);\
} while(0)

#define fhduck1_realloc(this, count) do {\
fhduck_realloc((this)->data0, count);\
} while(0)

#define fhduck2_realloc(this, count) do {\
fhduck_realloc((this)->data0, count);\
fhduck_realloc((this)->data1, count);\
} while(0)

#define fhduck3_realloc(this, count) do {\
fhduck_realloc((this)->data0, count);\
fhduck_realloc((this)->data1, count);\
fhduck_realloc((this)->data2, count);\
} while(0)

#define fhduck4_realloc(this, count) do {\
fhduck_realloc((this)->data0, count);\
fhduck_realloc((this)->data1, count);\
fhduck_realloc((this)->data2, count);\
fhduck_realloc((this)->data3, count);\
} while(0)

#define fhduck5_realloc(this, count) do {\
fhduck_realloc((this)->data0, count);\
fhduck_realloc((this)->data1, count);\
fhduck_realloc((this)->data2, count);\
fhduck_realloc((this)->data3, count);\
fhduck_realloc((this)->data4, count);\
} while(0)

#define fhduck6_realloc(this, count) do {\
fhduck_realloc((this)->data0, count);\
fhduck_realloc((this)->data1, count);\
fhduck_realloc((this)->data2, count);\
fhduck_realloc((this)->data3, count);\
fhduck_realloc((this)->data4, count);\
fhduck_realloc((this)->data5, count);\
} while(0)

#define fhduck7_realloc(this, count) do {\
fhduck_realloc((this)->data0, count);\
fhduck_realloc((this)->data1, count);\
fhduck_realloc((this)->data2, count);\
fhduck_realloc((this)->data3, count);\
fhduck_realloc((this)->data4, count);\
fhduck_realloc((this)->data5, count);\
fhduck_realloc((this)->data6, count);\
} while(0)

#define fhduck8_realloc(this, count) do {\
fhduck_realloc((this)->data0, count);\
fhduck_realloc((this)->data1, count);\
fhduck_realloc((this)->data2, count);\
fhduck_realloc((this)->data3, count);\
fhduck_realloc((this)->data4, count);\
fhduck_realloc((this)->data5, count);\
fhduck_realloc((this)->data6, count);\
fhduck_realloc((this)->data7, count);\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// STRUCT FIELDS (MULTI)
//
#define FHDUCK_ARRAY0(Index_T, Element_T, _data) \
union { Element_T *_data, *data; };\
union { Index_T count, length; }

#define FHDUCK_ARRAY1(Index_T, Element_T0, _data0) \
union { Element_T0 *_data0, *data0; };\
union { Index_T count, length; }

#define FHDUCK_ARRAY2(Index_T, Element_T0, _data0, Element_T1, _data1) \
union { Element_T0 *_data0, *data0; };\
union { Element_T1 *_data1, *data1; };\
union { Index_T count, length; }

#define FHDUCK_ARRAY3(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2) \
union { Element_T0 *_data0, *data0; };\
union { Element_T1 *_data1, *data1; };\
union { Element_T2 *_data2, *data2; };\
union { Index_T count, length; }

#define FHDUCK_ARRAY4(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3) \
union { Element_T0 *_data0, *data0; };\
union { Element_T1 *_data1, *data1; };\
union { Element_T2 *_data2, *data2; };\
union { Element_T3 *_data3, *data3; };\
union { Index_T count, length; }

#define FHDUCK_ARRAY5(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3, Element_T4, _data4) \
union { Element_T0 *_data0, *data0; };\
union { Element_T1 *_data1, *data1; };\
union { Element_T2 *_data2, *data2; };\
union { Element_T3 *_data3, *data3; };\
union { Element_T4 *_data4, *data4; };\
union { Index_T count, length; }

#define FHDUCK_ARRAY6(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3, Element_T4, _data4, Element_T5, _data5) \
union { Element_T0 *_data0, *data0; };\
union { Element_T1 *_data1, *data1; };\
union { Element_T2 *_data2, *data2; };\
union { Element_T3 *_data3, *data3; };\
union { Element_T4 *_data4, *data4; };\
union { Element_T5 *_data5, *data5; };\
union { Index_T count, length; }

#define FHDUCK_ARRAY7(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3, Element_T4, _data4, Element_T5, _data5, Element_T6, _data6) \
union { Element_T0 *_data0, *data0; };\
union { Element_T1 *_data1, *data1; };\
union { Element_T2 *_data2, *data2; };\
union { Element_T3 *_data3, *data3; };\
union { Element_T4 *_data4, *data4; };\
union { Element_T5 *_data5, *data5; };\
union { Element_T6 *_data6, *data6; };\
union { Index_T count, length; }

#define FHDUCK_ARRAY8(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3, Element_T4, _data4, Element_T5, _data5, Element_T6, _data6, Element_T7, _data7) \
union { Element_T0 *_data0, *data0; };\
union { Element_T1 *_data1, *data1; };\
union { Element_T2 *_data2, *data2; };\
union { Element_T3 *_data3, *data3; };\
union { Element_T4 *_data4, *data4; };\
union { Element_T5 *_data5, *data5; };\
union { Element_T6 *_data6, *data6; };\
union { Element_T7 *_data7, *data7; };\
union { Index_T count, length; }

//

#define FHDUCK_LIST0(Index_T, Element_T, _data) \
FHDUCK_ARRAY0(Index_T, Element_T, _data);\
Index_T capacity

#define FHDUCK_LIST1(Index_T, Element_T0, _data0) \
FHDUCK_ARRAY1(Index_T, Element_T0, _data0);\
Index_T capacity

#define FHDUCK_LIST2(Index_T, Element_T0, _data0, Element_T1, _data1) \
FHDUCK_ARRAY2(Index_T, Element_T0, _data0, Element_T1, _data1);\
Index_T capacity

#define FHDUCK_LIST3(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2) \
FHDUCK_ARRAY3(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2);\
Index_T capacity

#define FHDUCK_LIST4(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3) \
FHDUCK_ARRAY4(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3);\
Index_T capacity

#define FHDUCK_LIST5(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3, Element_T4, _data4) \
FHDUCK_ARRAY5(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3, Element_T4, _data4);\
Index_T capacity

#define FHDUCK_LIST6(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3, Element_T4, _data4, Element_T5, _data5) \
FHDUCK_ARRAY6(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3, Element_T4, _data4, Element_T5, _data5);\
Index_T capacity

#define FHDUCK_LIST7(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3, Element_T4, _data4, Element_T5, _data5, Element_T6, _data6) \
FHDUCK_ARRAY7(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3, Element_T4, _data4, Element_T5, _data5, Element_T6, _data6);\
Index_T capacity

#define FHDUCK_LIST8(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3, Element_T4, _data4, Element_T5, _data5, Element_T6, _data6, Element_T7, _data7) \
FHDUCK_ARRAY8(Index_T, Element_T0, _data0, Element_T1, _data1, Element_T2, _data2, Element_T3, _data3, Element_T4, _data4, Element_T5, _data5, Element_T6, _data6, Element_T7, _data7);\
Index_T capacity

//

#define ARRAY0(Index_T, Element_T, data) struct ____ {\
FHDUCK_ARRAY0(Index_T, Element_T, data);\
}

#define ARRAY1(Index_T, Element_T0, data0) struct ____ {\
FHDUCK_ARRAY1(Index_T, Element_T0, data0);\
}

#define ARRAY2(Index_T, Element_T0, data0, Element_T1, data1) struct ____ {\
FHDUCK_ARRAY2(Index_T, Element_T0, data0, Element_T1, data1);\
}

#define ARRAY3(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2) struct ____ {\
FHDUCK_ARRAY3(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2);\
}

#define ARRAY4(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3) struct ____ {\
FHDUCK_ARRAY4(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3);\
}

#define ARRAY5(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4) struct ____ {\
FHDUCK_ARRAY5(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4);\
}

#define ARRAY6(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4, Element_T5, data5) struct ____ {\
FHDUCK_ARRAY6(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4, Element_T5, data5);\
}

#define ARRAY7(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4, Element_T5, data5, Element_T6, data6) struct ____ {\
FHDUCK_ARRAY7(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4, Element_T5, data5, Element_T6, data6);\
}

#define ARRAY8(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4, Element_T5, data5, Element_T6, data6, Element_T7, data7) struct ____ {\
FHDUCK_ARRAY8(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4, Element_T5, data5, Element_T6, data6, Element_T7, data7);\
}

//

#define LIST0(Index_T, Element_T, data) struct ____ {\
FHDUCK_LIST0(Index_T, Element_T, data);\
}

#define LIST1(Index_T, Element_T0, data0) struct ____ {\
FHDUCK_LIST1(Index_T, Element_T0, data0);\
}

#define LIST2(Index_T, Element_T0, data0, Element_T1, data1) struct ____ {\
FHDUCK_LIST2(Index_T, Element_T0, data0, Element_T1, data1);\
}

#define LIST3(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2) struct ____ {\
FHDUCK_LIST3(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2);\
}

#define LIST4(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3) struct ____ {\
FHDUCK_LIST4(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3);\
}

#define LIST5(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4) struct ____ {\
FHDUCK_LIST5(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4);\
}

#define LIST6(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4, Element_T5, data5) struct ____ {\
FHDUCK_LIST6(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4, Element_T5, data5);\
}

#define LIST7(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4, Element_T5, data5, Element_T6, data6) struct ____ {\
FHDUCK_LIST7(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4, Element_T5, data5, Element_T6, data6);\
}

#define LIST8(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4, Element_T5, data5, Element_T6, data6, Element_T7, data7) struct ____ {\
FHDUCK_LIST8(Index_T, Element_T0, data0, Element_T1, data1, Element_T2, data2, Element_T3, data3, Element_T4, data4, Element_T5, data5, Element_T6, data6, Element_T7, data7);\
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// GROW (MULTI)
//
#define fhduckaN_grow(N, this, new_count) do {\
if(new_count > 0) {\
(this)->count = new_count;\
if((this)->data0) {\
fhduckN_realloc(N, this, (this)->count);\
} else {\
fhduckN_alloc(N, this, (this)->count);\
}\
}\
} while(0)

#define fhducka0_grow(this, new_count) fhducka_grow(this, new_count)
#define fhducka1_grow(this, new_count) fhduckaN_grow(1, this, new_count)
#define fhducka2_grow(this, new_count) fhduckaN_grow(2, this, new_count)
#define fhducka3_grow(this, new_count) fhduckaN_grow(3, this, new_count)
#define fhducka4_grow(this, new_count) fhduckaN_grow(4, this, new_count)
#define fhducka5_grow(this, new_count) fhduckaN_grow(5, this, new_count)
#define fhducka6_grow(this, new_count) fhduckaN_grow(6, this, new_count)
#define fhducka7_grow(this, new_count) fhduckaN_grow(7, this, new_count)
#define fhducka8_grow(this, new_count) fhduckaN_grow(8, this, new_count)

//

#define fhducklN_grow(N, this, new_capacity) do {\
if(new_capacity > 0) {\
(this)->capacity = new_capacity;\
if((this)->data0) {\
fhduckN_realloc(N, this, (this)->capacity);\
} else {\
fhduckN_alloc(N, this, (this)->capacity);\
}\
}\
} while(0)

#define fhduckl0_grow(this, new_capacity) fhduckl_grow(this, new_capacity)
#define fhduckl1_grow(this, new_capacity) fhducklN_grow(1, this, new_capacity)
#define fhduckl2_grow(this, new_capacity) fhducklN_grow(2, this, new_capacity)
#define fhduckl3_grow(this, new_capacity) fhducklN_grow(3, this, new_capacity)
#define fhduckl4_grow(this, new_capacity) fhducklN_grow(4, this, new_capacity)
#define fhduckl5_grow(this, new_capacity) fhducklN_grow(5, this, new_capacity)
#define fhduckl6_grow(this, new_capacity) fhducklN_grow(6, this, new_capacity)
#define fhduckl7_grow(this, new_capacity) fhducklN_grow(7, this, new_capacity)
#define fhduckl8_grow(this, new_capacity) fhducklN_grow(8, this, new_capacity)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// MAYBE GROW (MULTI)
//
#define fhducklN_maybe_grow(N, this, new_capacity) do {\
if(((new_capacity) > 0) && ((new_capacity) >= (this)->capacity)) {\
if((this)->capacity <= 0) { (this)->capacity = FHDUCK_LIST_DEFAULT_CAPACITY; }\
while((new_capacity) >= (this)->capacity) { (this)->capacity *= 2; }\
fhducklN_grow(N, this, new_capacity);\
}\
} while(0)

#define fhduckl0_maybe_grow(this, new_capacity) fhducklN_maybe_grow(0, this, new_capacity)
#define fhduckl1_maybe_grow(this, new_capacity) fhducklN_maybe_grow(1, this, new_capacity)
#define fhduckl2_maybe_grow(this, new_capacity) fhducklN_maybe_grow(2, this, new_capacity)
#define fhduckl3_maybe_grow(this, new_capacity) fhducklN_maybe_grow(3, this, new_capacity)
#define fhduckl4_maybe_grow(this, new_capacity) fhducklN_maybe_grow(4, this, new_capacity)
#define fhduckl5_maybe_grow(this, new_capacity) fhducklN_maybe_grow(5, this, new_capacity)
#define fhduckl6_maybe_grow(this, new_capacity) fhducklN_maybe_grow(6, this, new_capacity)
#define fhduckl7_maybe_grow(this, new_capacity) fhducklN_maybe_grow(7, this, new_capacity)
#define fhduckl8_maybe_grow(this, new_capacity) fhducklN_maybe_grow(8, this, new_capacity)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// REVERSE (MULTI)
//
#define fhduck_reverse_arrayN(this, ...) do {\
if((this)->data0) {\
for(ptrdiff_t _fhduck_left_ = 0, _fhduck_right_ = (this_count);\
_fhduck_left_ < _fhduck_right_;\
_fhduck_left_ += 1, _fhduck_right_ -= 1)\
{\
FHDUCK_MULTI_TMP_ASSIGN_N(N, this, _fhduck_left_, __VA_ARGS__);\
FHDUCK_MULTI_ASSIGN_VAR_N(N, this, _fhduck_left_, _fhduck_right_);\
FHDUCK_MULTI_ASSIGN_TMP_N(N, this, _fhduck_right_);\
}\
}\
} while(0)

#define fhduck_reverse_array0(this, T) \
fhduck_reverse_many((this)->data, (this)->count, T)

#define fhduck_reverse_array1(this, T0) \
fhduck_reverse_arrayN(1, this, T0)

#define fhduck_reverse_array2(this, T0, T1) \
fhduck_reverse_arrayN(2, this, T0, T1)

#define fhduck_reverse_array3(this, T0, T1, T2) \
fhduck_reverse_arrayN(3, this, T0, T1, T2)

#define fhduck_reverse_array4(this, T0, T1, T2, T3) \
fhduck_reverse_arrayN(4, this, T0, T1, T2, T3)

#define fhduck_reverse_array5(this, T0, T1, T2, T3, T4) \
fhduck_reverse_arrayN(5, this, T0, T1, T2, T3, T4)

#define fhduck_reverse_array6(this, T0, T1, T2, T3, T4, T5) \
fhduck_reverse_arrayN(6, this, T0, T1, T2, T3, T4, T5)

#define fhduck_reverse_array7(this, T0, T1, T2, T3, T4, T5, T6) \
fhduck_reverse_arrayN(7, this, T0, T1, T2, T3, T4, T5, T6)

#define fhduck_reverse_array8(this, T0, T1, T2, T3, T4, T5, T6, T7) \
fhduck_reverse_arrayN(8, this, T0, T1, T2, T3, T4, T5, T6, T7)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INSERT AT (MULTI)
//
#define fhduckaN_insert_at(N, at_index, this, ...) do {\
if((at_index) >= 0 && (at_index) < (this)->count) {\
FHDUCK_MULTI_ASSIGN_VAR_N(N, this, at_index, __VA_ARGS__);\
}\
} while(0)

#define fhducka0_insert_at(at_index, this, it) \
fhduckaN_insert_at(0, at_index, this, it)

#define fhducka1_insert_at(at_index, this, it0) \
fhduckaN_insert_at(1, at_index, this, it0)

#define fhducka2_insert_at(at_index, this, it0, it1) \
fhduckaN_insert_at(2, at_index, this, it0, it1)

#define fhducka3_insert_at(at_index, this, it0, it1, it2) \
fhduckaN_insert_at(3, at_index, this, it0, it1, it2)

#define fhducka4_insert_at(at_index, this, it0, it1, it2, it3) \
fhduckaN_insert_at(4, at_index, this, it0, it1, it2, it3)

#define fhducka5_insert_at(at_index, this, it0, it1, it2, it3, it4) \
fhduckaN_insert_at(5, at_index, this, it0, it1, it2, it3, it4)

#define fhducka6_insert_at(at_index, this, it0, it1, it2, it3, it4, it5) \
fhduckaN_insert_at(6, at_index, this, it0, it1, it2, it3, it4, it5)

#define fhducka7_insert_at(at_index, this, it0, it1, it2, it3, it4, it5, it6) \
fhduckaN_insert_at(7, at_index, this, it0, it1, it2, it3, it4, it5, it6)

#define fhducka8_insert_at(at_index, this, it0, it1, it2, it3, it4, it5, it6, it7) \
fhduckaN_insert_at(8, at_index, this, it0, it1, it2, it3, it4, it5, it6, it7)

//

#define fhducklN_insert_at(N, at_index, this, ...) do {\
fhducklN_maybe_grow(N, this, (at_index) + 1);\
fhduckaN_insert_at(N, at_index, this, __VA_ARGS__);\
} while(0)

#define fhduckl0_insert_at(at_index, this, ...) fhducklN_insert_at(0, at_index, this, __VA_ARGS__)
#define fhduckl1_insert_at(at_index, this, ...) fhducklN_insert_at(1, at_index, this, __VA_ARGS__)
#define fhduckl2_insert_at(at_index, this, ...) fhducklN_insert_at(2, at_index, this, __VA_ARGS__)
#define fhduckl3_insert_at(at_index, this, ...) fhducklN_insert_at(3, at_index, this, __VA_ARGS__)
#define fhduckl4_insert_at(at_index, this, ...) fhducklN_insert_at(4, at_index, this, __VA_ARGS__)
#define fhduckl5_insert_at(at_index, this, ...) fhducklN_insert_at(5, at_index, this, __VA_ARGS__)
#define fhduckl6_insert_at(at_index, this, ...) fhducklN_insert_at(6, at_index, this, __VA_ARGS__)
#define fhduckl7_insert_at(at_index, this, ...) fhducklN_insert_at(7, at_index, this, __VA_ARGS__)
#define fhduckl8_insert_at(at_index, this, ...) fhducklN_insert_at(8, at_index, this, __VA_ARGS__)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INSERT MANY AT (MULTI)
//
#define fhduckaN_insert_many_at(N, at_index, this, it_count, ...) do {\
if((at_index) >= 0 && (at_index) + (it_count) < (this)->count) {\
for(ptrdiff_t _fhduck_i_ = (at_index);\
_fhduck_i_ < ((at_index) + (it_count));\
_fhduck_i_ += 1)\
{\
FHDUCK_MULTI_ASSIGN_ARRAY_N(N, this, ((at_index) + _fhduck_i_), _fhduck_i_, __VA_ARGS__);\
}\
}\
} while(0)

#define fhducka0_insert_many_at(at_index, this, it_count, it_data) \
fhduckaN_insert_many_at(0, at_index, this, it_count, it_data)

#define fhducka1_insert_many_at(at_index, this, it_count, it_data0) \
fhduckaN_insert_many_at(1, at_index, this, it_count, it_data0)

#define fhducka2_insert_many_at(at_index, this, it_count, it_data0, it_data1) \
fhduckaN_insert_many_at(2, at_index, this, it_count, it_data0, it_data1)

#define fhducka3_insert_many_at(at_index, this, it_count, it_data0, it_data1, it_data2) \
fhduckaN_insert_many_at(3, at_index, this, it_count, it_data0, it_data1, it_data2)

#define fhducka4_insert_many_at(at_index, this, it_count, it_data0, it_data1, it_data2, it_data3) \
fhduckaN_insert_many_at(4, at_index, this, it_count, it_data0, it_data1, it_data2, it_data3)

#define fhducka5_insert_many_at(at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4) \
fhduckaN_insert_many_at(5, at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4)

#define fhducka6_insert_many_at(at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5) \
fhduckaN_insert_many_at(6, at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5)

#define fhducka7_insert_many_at(at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6) \
fhduckaN_insert_many_at(7, at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6)

#define fhducka8_insert_many_at(at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6, it_data7) \
fhduckaN_insert_many_at(8, at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6, it_data7)

//

#define fhducklN_insert_many_at(N, at_index, this, it_count, ...) do {\
if((at_index) >= 0) {\
fhducklN_maybe_grow(N, this, (at_index) + (it_count));\
fhduckaN_insert_many_at(N, at_index, this, it_count, __VA_ARGS__);\
}\
} while(0)

#define fhduckl0_insert_many_at(at_index, this, it_count, it_data) \
fhducklN_insert_many_at(0, at_index, this, it_count, it_data)

#define fhduckl1_insert_many_at(at_index, this, it_count, it_data0) \
fhducklN_insert_many_at(1, at_index, this, it_count, it_data0)

#define fhduckl2_insert_many_at(at_index, this, it_count, it_data0, it_data1) \
fhducklN_insert_many_at(2, at_index, this, it_count, it_data0, it_data1)

#define fhduckl3_insert_many_at(at_index, this, it_count, it_data0, it_data1, it_data2) \
fhducklN_insert_many_at(3, at_index, this, it_count, it_data0, it_data1, it_data2)

#define fhduckl4_insert_many_at(at_index, this, it_count, it_data0, it_data1, it_data2, it_data3) \
fhducklN_insert_many_at(4, at_index, this, it_count, it_data0, it_data1, it_data2, it_data3)

#define fhduckl5_insert_many_at(at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4) \
fhducklN_insert_many_at(5, at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4)

#define fhduckl6_insert_many_at(at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5) \
fhducklN_insert_many_at(6, at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5)

#define fhduckl7_insert_many_at(at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6) \
fhducklN_insert_many_at(7, at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6)

#define fhduckl8_insert_many_at(at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6, it_data7) \
fhducklN_insert_many_at(8, at_index, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6, it_data7)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INSERT (MULTI)
//
#define fhduckaN_insert(N, this, ...) fhduckaN_insert_at(N, 0, this, __VA_ARGS__)

#define fhducka1_insert(this, ...) fhducka1_insert_at(1, 0, this, __VA_ARGS__)
#define fhducka2_insert(this, ...) fhducka2_insert_at(2, 0, this, __VA_ARGS__)
#define fhducka3_insert(this, ...) fhducka3_insert_at(3, 0, this, __VA_ARGS__)
#define fhducka4_insert(this, ...) fhducka4_insert_at(4, 0, this, __VA_ARGS__)
#define fhducka5_insert(this, ...) fhducka5_insert_at(5, 0, this, __VA_ARGS__)
#define fhducka6_insert(this, ...) fhducka6_insert_at(6, 0, this, __VA_ARGS__)
#define fhducka7_insert(this, ...) fhducka7_insert_at(7, 0, this, __VA_ARGS__)
#define fhducka8_insert(this, ...) fhducka8_insert_at(8, 0, this, __VA_ARGS__)

//

#define fhducklN_insert(N, this, ...) fhducklN_insert_at(N, 0, this, __VA_ARGS__)

#define fhduckl1_insert(this, ...) fhduckl1_insert_at(1, 0, this, __VA_ARGS__)
#define fhduckl2_insert(this, ...) fhduckl2_insert_at(2, 0, this, __VA_ARGS__)
#define fhduckl3_insert(this, ...) fhduckl3_insert_at(3, 0, this, __VA_ARGS__)
#define fhduckl4_insert(this, ...) fhduckl4_insert_at(4, 0, this, __VA_ARGS__)
#define fhduckl5_insert(this, ...) fhduckl5_insert_at(5, 0, this, __VA_ARGS__)
#define fhduckl6_insert(this, ...) fhduckl6_insert_at(6, 0, this, __VA_ARGS__)
#define fhduckl7_insert(this, ...) fhduckl7_insert_at(7, 0, this, __VA_ARGS__)
#define fhduckl8_insert(this, ...) fhduckl8_insert_at(8, 0, this, __VA_ARGS__)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INSERT MANY (MULTI)
//
#define fhduckaN_insert_many(N, this, it_count, ...) fhduckaN_insert_many_at(N, 0, this, it_count, __VA_ARGS__)

#define fhducka1_insert_many(this, it_count, ...) fhducka1_insert_many_at(1, 0, this, it_count, __VA_ARGS__)
#define fhducka2_insert_many(this, it_count, ...) fhducka2_insert_many_at(2, 0, this, it_count, __VA_ARGS__)
#define fhducka3_insert_many(this, it_count, ...) fhducka3_insert_many_at(3, 0, this, it_count, __VA_ARGS__)
#define fhducka4_insert_many(this, it_count, ...) fhducka4_insert_many_at(4, 0, this, it_count, __VA_ARGS__)
#define fhducka5_insert_many(this, it_count, ...) fhducka5_insert_many_at(5, 0, this, it_count, __VA_ARGS__)
#define fhducka6_insert_many(this, it_count, ...) fhducka6_insert_many_at(6, 0, this, it_count, __VA_ARGS__)
#define fhducka7_insert_many(this, it_count, ...) fhducka7_insert_many_at(7, 0, this, it_count, __VA_ARGS__)
#define fhducka8_insert_many(this, it_count, ...) fhducka8_insert_many_at(8, 0, this, it_count, __VA_ARGS__)

//

#define fhducklN_insert_many(this, it_count, ...) fhducklN_insert_many_at(N, 0, this, it_count, __VA_ARGS__)

#define fhduckl1_insert_many(this, it_count, ...) fhduckl1_insert_many_at(1, 0, this, it_count, __VA_ARGS__)
#define fhduckl2_insert_many(this, it_count, ...) fhduckl2_insert_many_at(2, 0, this, it_count, __VA_ARGS__)
#define fhduckl3_insert_many(this, it_count, ...) fhduckl3_insert_many_at(3, 0, this, it_count, __VA_ARGS__)
#define fhduckl4_insert_many(this, it_count, ...) fhduckl4_insert_many_at(4, 0, this, it_count, __VA_ARGS__)
#define fhduckl5_insert_many(this, it_count, ...) fhduckl5_insert_many_at(5, 0, this, it_count, __VA_ARGS__)
#define fhduckl6_insert_many(this, it_count, ...) fhduckl6_insert_many_at(6, 0, this, it_count, __VA_ARGS__)
#define fhduckl7_insert_many(this, it_count, ...) fhduckl7_insert_many_at(7, 0, this, it_count, __VA_ARGS__)
#define fhduckl8_insert_many(this, it_count, ...) fhduckl8_insert_many_at(8, 0, this, it_count, __VA_ARGS__)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// APPEND (MULTI)
//
#define fhducklN_append(N, this, ...) do {\
fhducklN_maybe_grow(N, this, (this)->count + 1);\
FHDUCK_MULTI_ASSIGN_VAR_N(N, this, (this)->count, __VA_ARGS__);\
(this)->count += 1;\
} while(0)

#define fhduckl0_append(this, it) do {\
fhduckl0_maybe_grow(this, (this)->count + 1);\
(this)->data[(this)->count] = (it);\
(this)->count += 1;\
} while(0)

#define fhduckl1_append(this, it0) do {\
fhduckl1_maybe_grow(this, (this)->count + 1);\
(this)->data0[(this)->count] = (it0);\
(this)->count += 1;\
} while(0)

#define fhduckl2_append(this, it0, it1) do {\
fhduckl2_maybe_grow(this, (this)->count + 1);\
(this)->data0[(this)->count] = (it0);\
(this)->data1[(this)->count] = (it1);\
(this)->count += 1;\
} while(0)

#define fhduckl3_append(this, it0, it1, it2) do {\
fhduckl3_maybe_grow(this, (this)->count + 1);\
(this)->data0[(this)->count] = (it0);\
(this)->data1[(this)->count] = (it1);\
(this)->data2[(this)->count] = (it2);\
(this)->count += 1;\
} while(0)

#define fhduckl4_append(this, it0, it1, it2, it3) do {\
fhduckl4_maybe_grow(this, (this)->count + 1);\
(this)->data0[(this)->count] = (it0);\
(this)->data1[(this)->count] = (it1);\
(this)->data2[(this)->count] = (it2);\
(this)->data3[(this)->count] = (it3);\
(this)->count += 1;\
} while(0)

#define fhduckl5_append(this, it0, it1, it2, it3, it4) do {\
fhduckl5_maybe_grow(this, (this)->count + 1);\
(this)->data0[(this)->count] = (it0);\
(this)->data1[(this)->count] = (it1);\
(this)->data2[(this)->count] = (it2);\
(this)->data3[(this)->count] = (it3);\
(this)->data4[(this)->count] = (it4);\
(this)->count += 1;\
} while(0)

#define fhduckl6_append(this, it0, it1, it2, it3, it4, it5) do {\
fhduckl6_maybe_grow(this, (this)->count + 1);\
(this)->data0[(this)->count] = (it0);\
(this)->data1[(this)->count] = (it1);\
(this)->data2[(this)->count] = (it2);\
(this)->data3[(this)->count] = (it3);\
(this)->data4[(this)->count] = (it4);\
(this)->data5[(this)->count] = (it5);\
(this)->count += 1;\
} while(0)

#define fhduckl7_append(this, it0, it1, it2, it3, it4, it5, it6) do {\
fhduckl7_maybe_grow(this, (this)->count + 1);\
(this)->data0[(this)->count] = (it0);\
(this)->data1[(this)->count] = (it1);\
(this)->data2[(this)->count] = (it2);\
(this)->data3[(this)->count] = (it3);\
(this)->data4[(this)->count] = (it4);\
(this)->data5[(this)->count] = (it5);\
(this)->data6[(this)->count] = (it6);\
(this)->count += 1;\
} while(0)

#define fhduckl8_append(this, it0, it1, it2, it3, it4, it5, it6, it7) do {\
fhduckl8_maybe_grow(this, (this)->count + 1);\
(this)->data0[(this)->count] = (it0);\
(this)->data1[(this)->count] = (it1);\
(this)->data2[(this)->count] = (it2);\
(this)->data3[(this)->count] = (it3);\
(this)->data4[(this)->count] = (it4);\
(this)->data5[(this)->count] = (it5);\
(this)->data6[(this)->count] = (it6);\
(this)->data7[(this)->count] = (it7);\
(this)->count += 1;\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// APPEND MANY (MULTI)
//
#define fhducklN_append_many(N, this, it_count, ...) do {\
fhduckl_maybe_grow(this, (this)->count + (it_count));\
for(ptrdiff_t _fhduck_i_ = 0; _fhduck_i_ < (it_count); _fhduck_i_ += 1) {\
FHDUCK_MULTI_ASSIGN_ARRAY_N(N, this, ((this)->count + _fhduck_i_), _fhduck_i_, __VA_ARGS__);\
}\
(this)->count += (it_count);\
} while(0)

#define fhduckl0_append_many(this, it_count, it_data) \
fhducklN_append_many(0, this, it_count, it_data)

#define fhduckl1_append_many(this, it_count, it_data0) \
fhducklN_append_many(1, this, it_count, it_data0)

#define fhduckl2_append_many(this, it_count, it_data0, it_data1) \
fhducklN_append_many(2, this, it_count, it_data0, it_data1)

#define fhduckl3_append_many(this, it_count, it_data0, it_data1, it_data2) \
fhducklN_append_many(3, this, it_count, it_data0, it_data1, it_data2)

#define fhduckl4_append_many(this, it_count, it_data0, it_data1, it_data2, it_data3) \
fhducklN_append_many(4, this, it_count, it_data0, it_data1, it_data2, it_data3)

#define fhduckl5_append_many(this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4) \
fhducklN_append_many(5, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4)

#define fhduckl6_append_many(this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5) \
fhducklN_append_many(6, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5)

#define fhduckl7_append_many(this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6) \
fhducklN_append_many(7, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6)

#define fhduckl8_append_many(this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6, it_data7) \
fhducklN_append_many(8, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6, it_data7)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// PREPEND UNORDERED (MULTI)
//
#define fhducklN_prepend_unordered(N, this, ...) do {\
fhducklN_maybe_grow(N, this, (this)->count + 1);\
FHDUCK_MULTI_MOVE_N(N, this, (this)->count, 0);\
FHDUCK_MULTI_ASSIGN_VAR_N(N, this, 0, __VA_ARGS__);\
(this)->count += 1;\
} while(0)

#define fhduckl0_prepend_unordered(this, it) \
fhducklN_prepend_unordered(0, this, it)

#define fhduckl1_prepend_unordered(this, it0) \
fhducklN_prepend_unordered(1, this, it0)

#define fhduckl2_prepend_unordered(this, it0, it1) \
fhducklN_prepend_unordered(2, this, it0, it1)

#define fhduckl3_prepend_unordered(this, it0, it1, it2) \
fhducklN_prepend_unordered(3, this, it0, it1, it2)

#define fhduckl4_prepend_unordered(this, it0, it1, it2, it3) \
fhducklN_prepend_unordered(4, this, it0, it1, it2, it3)

#define fhduckl5_prepend_unordered(this, it0, it1, it2, it3, it4) \
fhducklN_prepend_unordered(5, this, it0, it1, it2, it3, it4)

#define fhduckl6_prepend_unordered(this, it0, it1, it2, it3, it4, it5) \
fhducklN_prepend_unordered(6, this, it0, it1, it2, it3, it4, it5)

#define fhduckl7_prepend_unordered(this, it0, it1, it2, it3, it4, it5, it6) \
fhducklN_prepend_unordered(7, this, it0, it1, it2, it3, it4, it5, it6)

#define fhduckl8_prepend_unordered(this, it0, it1, it2, it3, it4, it5, it6, it7) \
fhducklN_prepend_unordered(8, this, it0, it1, it2, it3, it4, it5, it6, it7)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// PREPEND UNORDERED MANY (MULTI)
//
#define fhducklN_prepend_unordered_many(N, this, it_count, ...) do {\
fhduckl_maybe_grow(this, (this)->count + (it_count));\
for(ptrdiff_t _fhduck_i_ = 0; _fhduck_i_ < (it_count); _fhduck_i_ += 1) {\
FHDUCK_MULTI_MOVE_N(N, this, (this)->count + _fhduck_i_, _fhduck_i_);\
FHDUCK_MULTI_ASSIGN_ARRAY_N(N, this, _fhduck_i_, _fhduck_i_, __VA_ARGS__);\
}\
(this)->count += (it_count);\
} while(0)

#define fhduckl0_prepend_unordered_many(this, it_count, it_data) \
fhducklN_prepend_unordered_many(0, this, it_count, it_data)

#define fhduckl1_prepend_unordered_many(this, it_count, it_data0) \
fhducklN_prepend_unordered_many(1, this, it_count, it_data0)

#define fhduckl2_prepend_unordered_many(this, it_count, it_data0, it_data1) \
fhducklN_prepend_unordered_many(2, this, it_count, it_data0, it_data1)

#define fhduckl3_prepend_unordered_many(this, it_count, it_data0, it_data1, it_data2) \
fhducklN_prepend_unordered_many(3, this, it_count, it_data0, it_data1, it_data2)

#define fhduckl4_prepend_unordered_many(this, it_count, it_data0, it_data1, it_data2, it_data3) \
fhducklN_prepend_unordered_many(4, this, it_count, it_data0, it_data1, it_data2, it_data3)

#define fhduckl5_prepend_unordered_many(this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4) \
fhducklN_prepend_unordered_many(5, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4)

#define fhduckl6_prepend_unordered_many(this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5) \
fhducklN_prepend_unordered_many(6, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5)

#define fhduckl7_prepend_unordered_many(this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6) \
fhducklN_prepend_unordered_many(7, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6)

#define fhduckl8_prepend_unordered_many(this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6, it_data7) \
fhducklN_prepend_unordered_many(8, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6, it_data7)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// PREPEND ORDERED (MULTI)
//
#define fhducklN_prepend_ordered(N, this, ...) do {\
fhducklN_maybe_grow(N, this, (this)->count + 1);\
for(ptrdiff_t _fhduck_i_ = (this)->count - 1; _fhduck_i_ >= 1; _fhduck_i_ -= 1) {\
FHDUCK_MULTI_MOVE_N(N, this, _fhduck_i_, _fhduck_i_ - 1);\
}\
FHDUCK_MULTI_ASSIGN_VAR_N(N, this, 0, __VA_ARGS__);\
(this)->count += 1;\
} while(0)

#define fhduckl0_prepend_ordered(this, it) \
fhducklN_prepend_ordered(0, this, it)

#define fhduckl1_prepend_ordered(this, it0) \
fhducklN_prepend_ordered(1, this, it0)

#define fhduckl2_prepend_ordered(this, it0, it1) \
fhducklN_prepend_ordered(2, this, it0, it1)

#define fhduckl3_prepend_ordered(this, it0, it1, it2) \
fhducklN_prepend_ordered(3, this, it0, it1, it2)

#define fhduckl4_prepend_ordered(this, it0, it1, it2, it3) \
fhducklN_prepend_ordered(4, this, it0, it1, it2, it3)

#define fhduckl5_prepend_ordered(this, it0, it1, it2, it3, it4) \
fhducklN_prepend_ordered(5, this, it0, it1, it2, it3, it4)

#define fhduckl6_prepend_ordered(this, it0, it1, it2, it3, it4, it5) \
fhducklN_prepend_ordered(6, this, it0, it1, it2, it3, it4, it5)

#define fhduckl7_prepend_ordered(this, it0, it1, it2, it3, it4, it5, it6) \
fhducklN_prepend_ordered(7, this, it0, it1, it2, it3, it4, it5, it6)

#define fhduckl8_prepend_ordered(this, it0, it1, it2, it3, it4, it5, it6, it7) \
fhducklN_prepend_ordered(8, this, it0, it1, it2, it3, it4, it5, it6, it7)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// PREPEND ORDERED MANY (MULTI)
//
#define fhducklN_prepend_ordered_many(N, this, it_count, ...) do {\
fhduckl_maybe_grow(this, (this)->count + (it_count));\
for(ptrdiff_t _fhduck_i_ = ((this)->count + (it_count)) - 1;\
_fhduck_i_ >= (it_count);\
_fhduck_i_ -= 1)\
{\
FHDUCK_MULTI_MOVE_N(N, this, _fhduck_i_, _fhduck_i_ - (it_count));\
}\
for(ptrdiff_t _fhduck_i_ = 0; _fhduck_i_ < (it_count); _fhduck_i_ += 1) {\
FHDUCK_MULTI_ASSIGN_ARRAY_N(N, this, _fhduck_i_, _fhduck_i_, __VA_ARGS__);\
}\
(this)->count += (it_count);\
} while(0)

#define fhduckl0_prepend_ordered_many(this, it_count, it_data) \
fhducklN_prepend_ordered_many(0, this, it_count, it_data)

#define fhduckl1_prepend_ordered_many(this, it_count, it_data0) \
fhducklN_prepend_ordered_many(1, this, it_count, it_data0)

#define fhduckl2_prepend_ordered_many(this, it_count, it_data0, it_data1) \
fhducklN_prepend_ordered_many(2, this, it_count, it_data0, it_data1)

#define fhduckl3_prepend_ordered_many(this, it_count, it_data0, it_data1, it_data2) \
fhducklN_prepend_ordered_many(3, this, it_count, it_data0, it_data1, it_data2)

#define fhduckl4_prepend_ordered_many(this, it_count, it_data0, it_data1, it_data2, it_data3) \
fhducklN_prepend_ordered_many(4, this, it_count, it_data0, it_data1, it_data2, it_data3)

#define fhduckl5_prepend_ordered_many(this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4) \
fhducklN_prepend_ordered_many(5, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4)

#define fhduckl6_prepend_ordered_many(this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5) \
fhducklN_prepend_ordered_many(6, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5)

#define fhduckl7_prepend_ordered_many(this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6) \
fhducklN_prepend_ordered_many(7, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6)

#define fhduckl8_prepend_ordered_many(this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6, it_data7) \
fhducklN_prepend_ordered_many(8, this, it_count, it_data0, it_data1, it_data2, it_data3, it_data4, it_data5, it_data6, it_data7)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// REMOVE UNORDERED (MULTI)
//
#define fhducklN_remove_unordered(N, this, index) do {\
if((this)->count >= 1) {\
FHDUCK_MULTI_MOVE_N(N, this, index, (this)->count - 1);\
(this)->count -= 1;\
}\
} while(0)

#define fhduckl0_remove_unordered(this, index) fhducklN_remove_unordered(0, this, index)
#define fhduckl1_remove_unordered(this, index) fhducklN_remove_unordered(1, this, index)
#define fhduckl2_remove_unordered(this, index) fhducklN_remove_unordered(2, this, index)
#define fhduckl3_remove_unordered(this, index) fhducklN_remove_unordered(3, this, index)
#define fhduckl4_remove_unordered(this, index) fhducklN_remove_unordered(4, this, index)
#define fhduckl5_remove_unordered(this, index) fhducklN_remove_unordered(5, this, index)
#define fhduckl6_remove_unordered(this, index) fhducklN_remove_unordered(6, this, index)
#define fhduckl7_remove_unordered(this, index) fhducklN_remove_unordered(7, this, index)
#define fhduckl8_remove_unordered(this, index) fhducklN_remove_unordered(8, this, index)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// REMOVE ORDERED (MULTI)
//
#define fhducklN_remove_ordered(N, this, index) do {\
if((this)->count >= 1 && (index) < (this)->count) {\
if((index) < (this)->count - 1) {\
for(ptrdiff_t _fhduck_i_ = (index); _fhduck_i_ < (this)->count - 1; _fhduck_i_ += 1) {\
FHDUCK_MULTI_MOVE_N(N, this, _fhduck_i_, _fhduck_i_ - 1);\
}\
}\
(this)->count -= 1;\
}\
} while(0)

#define fhduckl0_remove_ordered(this, index) fhducklN_remove_ordered(0, this, index)
#define fhduckl1_remove_ordered(this, index) fhducklN_remove_ordered(1, this, index)
#define fhduckl2_remove_ordered(this, index) fhducklN_remove_ordered(2, this, index)
#define fhduckl3_remove_ordered(this, index) fhducklN_remove_ordered(3, this, index)
#define fhduckl4_remove_ordered(this, index) fhducklN_remove_ordered(4, this, index)
#define fhduckl5_remove_ordered(this, index) fhducklN_remove_ordered(5, this, index)
#define fhduckl6_remove_ordered(this, index) fhducklN_remove_ordered(6, this, index)
#define fhduckl7_remove_ordered(this, index) fhducklN_remove_ordered(7, this, index)
#define fhduckl8_remove_ordered(this, index) fhducklN_remove_ordered(8, this, index)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//
//
#define FHDUCK_MULTI_MOVE_N(N, this, a_index, b_index) FHDUCK_MULTI_MOVE##N(this, a_index, b_index)

#define FHDUCK_MULTI_MOVE0(this, a_index, b_index) do {\
(this)->data[(a_index)] = (this)->data[(b_index)];\
} while(0)

#define FHDUCK_MULTI_MOVE1(this, a_index, b_index) do {\
(this)->data0[(a_index)] = (this)->data0[(b_index)];\
} while(0)

#define FHDUCK_MULTI_MOVE2(this, a_index, b_index) do {\
(this)->data0[(a_index)] = (this)->data0[(b_index)];\
(this)->data1[(a_index)] = (this)->data1[(b_index)];\
} while(0)

#define FHDUCK_MULTI_MOVE3(this, a_index, b_index) do {\
(this)->data0[(a_index)] = (this)->data0[(b_index)];\
(this)->data1[(a_index)] = (this)->data1[(b_index)];\
(this)->data2[(a_index)] = (this)->data2[(b_index)];\
} while(0)

#define FHDUCK_MULTI_MOVE4(this, a_index, b_index) do {\
(this)->data0[(a_index)] = (this)->data0[(b_index)];\
(this)->data1[(a_index)] = (this)->data1[(b_index)];\
(this)->data2[(a_index)] = (this)->data2[(b_index)];\
(this)->data3[(a_index)] = (this)->data3[(b_index)];\
} while(0)

#define FHDUCK_MULTI_MOVE5(this, a_index, b_index) do {\
(this)->data0[(a_index)] = (this)->data0[(b_index)];\
(this)->data1[(a_index)] = (this)->data1[(b_index)];\
(this)->data2[(a_index)] = (this)->data2[(b_index)];\
(this)->data3[(a_index)] = (this)->data3[(b_index)];\
(this)->data4[(a_index)] = (this)->data4[(b_index)];\
} while(0)

#define FHDUCK_MULTI_MOVE6(this, a_index, b_index) do {\
(this)->data0[(a_index)] = (this)->data0[(b_index)];\
(this)->data1[(a_index)] = (this)->data1[(b_index)];\
(this)->data2[(a_index)] = (this)->data2[(b_index)];\
(this)->data3[(a_index)] = (this)->data3[(b_index)];\
(this)->data4[(a_index)] = (this)->data4[(b_index)];\
(this)->data5[(a_index)] = (this)->data5[(b_index)];\
} while(0)

#define FHDUCK_MULTI_MOVE7(this, a_index, b_index) do {\
(this)->data0[(a_index)] = (this)->data0[(b_index)];\
(this)->data1[(a_index)] = (this)->data1[(b_index)];\
(this)->data2[(a_index)] = (this)->data2[(b_index)];\
(this)->data3[(a_index)] = (this)->data3[(b_index)];\
(this)->data4[(a_index)] = (this)->data4[(b_index)];\
(this)->data5[(a_index)] = (this)->data5[(b_index)];\
(this)->data6[(a_index)] = (this)->data6[(b_index)];\
} while(0)

#define FHDUCK_MULTI_MOVE8(this, a_index, b_index) do {\
(this)->data0[(a_index)] = (this)->data0[(b_index)];\
(this)->data1[(a_index)] = (this)->data1[(b_index)];\
(this)->data2[(a_index)] = (this)->data2[(b_index)];\
(this)->data3[(a_index)] = (this)->data3[(b_index)];\
(this)->data4[(a_index)] = (this)->data4[(b_index)];\
(this)->data5[(a_index)] = (this)->data5[(b_index)];\
(this)->data6[(a_index)] = (this)->data6[(b_index)];\
(this)->data7[(a_index)] = (this)->data7[(b_index)];\
} while(0)

//

#define FHDUCK_MULTI_ASSIGN_VAR_N(N, this, this_index, ...) \
FHDUCK_MULTI_ASSIGN_VAR##N(this, this_index, __VA_ARGS__)

#define FHDUCK_MULTI_ASSIGN_VAR0(this, this_index, it) do {\
(this)->data[(this_index)] = (it);\
} while(0)

#define FHDUCK_MULTI_ASSIGN_VAR1(this, this_index, it0) do {\
(this)->data0[(this_index)] = (it0);\
} while(0)

#define FHDUCK_MULTI_ASSIGN_VAR2(this, this_index, it0, it1) do {\
(this)->data0[(this_index)] = (it0);\
(this)->data1[(this_index)] = (it1);\
} while(0)

#define FHDUCK_MULTI_ASSIGN_VAR3(this, this_index, it0, it1, it2) do {\
(this)->data0[(this_index)] = (it0);\
(this)->data1[(this_index)] = (it1);\
(this)->data2[(this_index)] = (it2);\
} while(0)

#define FHDUCK_MULTI_ASSIGN_VAR4(this, this_index, it0, it1, it2, it3) do {\
(this)->data0[(this_index)] = (it0);\
(this)->data1[(this_index)] = (it1);\
(this)->data2[(this_index)] = (it2);\
(this)->data3[(this_index)] = (it3);\
} while(0)

#define FHDUCK_MULTI_ASSIGN_VAR5(this, this_index, it0, it1, it2, it3, it4) do {\
(this)->data0[(this_index)] = (it0);\
(this)->data1[(this_index)] = (it1);\
(this)->data2[(this_index)] = (it2);\
(this)->data3[(this_index)] = (it3);\
(this)->data4[(this_index)] = (it4);\
} while(0)

#define FHDUCK_MULTI_ASSIGN_VAR6(this, this_index, it0, it1, it2, it3, it4, it5) do {\
(this)->data0[(this_index)] = (it0);\
(this)->data1[(this_index)] = (it1);\
(this)->data2[(this_index)] = (it2);\
(this)->data3[(this_index)] = (it3);\
(this)->data4[(this_index)] = (it4);\
(this)->data5[(this_index)] = (it5);\
} while(0)

#define FHDUCK_MULTI_ASSIGN_VAR7(this, this_index, it0, it1, it2, it3, it4, it5, it6) do {\
(this)->data0[(this_index)] = (it0);\
(this)->data1[(this_index)] = (it1);\
(this)->data2[(this_index)] = (it2);\
(this)->data3[(this_index)] = (it3);\
(this)->data4[(this_index)] = (it4);\
(this)->data5[(this_index)] = (it5);\
(this)->data6[(this_index)] = (it6);\
} while(0)

#define FHDUCK_MULTI_ASSIGN_VAR8(this, this_index, it0, it1, it2, it3, it4, it5, it6, it7) do {\
(this)->data0[(this_index)] = (it0);\
(this)->data1[(this_index)] = (it1);\
(this)->data2[(this_index)] = (it2);\
(this)->data3[(this_index)] = (it3);\
(this)->data4[(this_index)] = (it4);\
(this)->data5[(this_index)] = (it5);\
(this)->data6[(this_index)] = (it6);\
(this)->data7[(this_index)] = (it7);\
} while(0)

//

#define FHDUCK_MULTI_ASSIGN_ARRAY_N(N, this, this_index, it_index, ...) \
FHDUCK_MULTI_ASSIGN_ARRAY##N(this, this_index, it_index, __VA_ARGS__)

#define FHDUCK_MULTI_ASSIGN_ARRAY0(this, this_index, it_index, it) do {\
(this)->data[(this_index)] = (it)[(it_index)];\
} while(0)

#define FHDUCK_MULTI_ASSIGN_ARRAY1(this, this_index, it_index, it0) do {\
(this)->data0[(this_index)] = (it0)[(it_index)];\
} while(0)

#define FHDUCK_MULTI_ASSIGN_ARRAY2(this, this_index, it_index, it0, it1) do {\
(this)->data0[(this_index)] = (it0)[(it_index)];\
(this)->data1[(this_index)] = (it1)[(it_index)];\
} while(0)

#define FHDUCK_MULTI_ASSIGN_ARRAY3(this, this_index, it_index, it0, it1, it2) do {\
(this)->data0[(this_index)] = (it0)[(it_index)];\
(this)->data1[(this_index)] = (it1)[(it_index)];\
(this)->data2[(this_index)] = (it2)[(it_index)];\
} while(0)

#define FHDUCK_MULTI_ASSIGN_ARRAY4(this, this_index, it_index, it0, it1, it2, it3) do {\
(this)->data0[(this_index)] = (it0)[(it_index)];\
(this)->data1[(this_index)] = (it1)[(it_index)];\
(this)->data2[(this_index)] = (it2)[(it_index)];\
(this)->data3[(this_index)] = (it3)[(it_index)];\
} while(0)

#define FHDUCK_MULTI_ASSIGN_ARRAY5(this, this_index, it_index, it0, it1, it2, it3, it4) do {\
(this)->data0[(this_index)] = (it0)[(it_index)];\
(this)->data1[(this_index)] = (it1)[(it_index)];\
(this)->data2[(this_index)] = (it2)[(it_index)];\
(this)->data3[(this_index)] = (it3)[(it_index)];\
(this)->data4[(this_index)] = (it4)[(it_index)];\
} while(0)

#define FHDUCK_MULTI_ASSIGN_ARRAY6(this, this_index, it_index, it0, it1, it2, it3, it4, it5) do {\
(this)->data0[(this_index)] = (it0)[(it_index)];\
(this)->data1[(this_index)] = (it1)[(it_index)];\
(this)->data2[(this_index)] = (it2)[(it_index)];\
(this)->data3[(this_index)] = (it3)[(it_index)];\
(this)->data4[(this_index)] = (it4)[(it_index)];\
(this)->data5[(this_index)] = (it5)[(it_index)];\
} while(0)

#define FHDUCK_MULTI_ASSIGN_ARRAY7(this, this_index, it_index, it0, it1, it2, it3, it4, it5, it6) do {\
(this)->data0[(this_index)] = (it0)[(it_index)];\
(this)->data1[(this_index)] = (it1)[(it_index)];\
(this)->data2[(this_index)] = (it2)[(it_index)];\
(this)->data3[(this_index)] = (it3)[(it_index)];\
(this)->data4[(this_index)] = (it4)[(it_index)];\
(this)->data5[(this_index)] = (it5)[(it_index)];\
(this)->data6[(this_index)] = (it6)[(it_index)];\
} while(0)

#define FHDUCK_MULTI_ASSIGN_ARRAY8(this, this_index, it_index, it0, it1, it2, it3, it4, it5, it6, it7) do {\
(this)->data0[(this_index)] = (it0)[(it_index)];\
(this)->data1[(this_index)] = (it1)[(it_index)];\
(this)->data2[(this_index)] = (it2)[(it_index)];\
(this)->data3[(this_index)] = (it3)[(it_index)];\
(this)->data4[(this_index)] = (it4)[(it_index)];\
(this)->data5[(this_index)] = (it5)[(it_index)];\
(this)->data6[(this_index)] = (it6)[(it_index)];\
(this)->data7[(this_index)] = (it7)[(it_index)];\
} while(0)

//

#define FHDUCK_MULTI_TMP_ASSIGN_N(N, this, index, ...) \
FHDUCK_MULTI_TMP_ASSIGN##N(this, index, __VA_ARGS__)

#define FHDUCK_MULTI_TMP_ASSIGN0(this, index, T) do {\
T _fhtmp_ = (this)->data[(index)];\
} while(0)

#define FHDUCK_MULTI_TMP_ASSIGN1(this, index, T0) do {\
T0 _fhtmp0_ = (this)->data0[(index)];\
} while(0)

#define FHDUCK_MULTI_TMP_ASSIGN2(this, index, T0, T1) do {\
T0 _fhtmp0_ = (this)->data0[(index)];\
T1 _fhtmp1_ = (this)->data1[(index)];\
} while(0)

#define FHDUCK_MULTI_TMP_ASSIGN3(this, index, T0, T1, T2) do {\
T0 _fhtmp0_ = (this)->data0[(index)];\
T1 _fhtmp1_ = (this)->data1[(index)];\
T2 _fhtmp2_ = (this)->data2[(index)];\
} while(0)

#define FHDUCK_MULTI_TMP_ASSIGN4(this, index, T0, T1, T2, T3) do {\
T0 _fhtmp0_ = (this)->data0[(index)];\
T1 _fhtmp1_ = (this)->data1[(index)];\
T2 _fhtmp2_ = (this)->data2[(index)];\
T3 _fhtmp3_ = (this)->data3[(index)];\
} while(0)

#define FHDUCK_MULTI_TMP_ASSIGN5(this, index, T0, T1, T2, T3, T4) do {\
T0 _fhtmp0_ = (this)->data0[(index)];\
T1 _fhtmp1_ = (this)->data1[(index)];\
T2 _fhtmp2_ = (this)->data2[(index)];\
T3 _fhtmp3_ = (this)->data3[(index)];\
T4 _fhtmp4_ = (this)->data4[(index)];\
} while(0)

#define FHDUCK_MULTI_TMP_ASSIGN6(this, index, T0, T1, T2, T3, T4, T5) do {\
T0 _fhtmp0_ = (this)->data0[(index)];\
T1 _fhtmp1_ = (this)->data1[(index)];\
T2 _fhtmp2_ = (this)->data2[(index)];\
T3 _fhtmp3_ = (this)->data3[(index)];\
T4 _fhtmp4_ = (this)->data4[(index)];\
T5 _fhtmp5_ = (this)->data5[(index)];\
} while(0)

#define FHDUCK_MULTI_TMP_ASSIGN7(this, index, T0, T1, T2, T3, T4, T5, T6) do {\
T0 _fhtmp0_ = (this)->data0[(index)];\
T1 _fhtmp1_ = (this)->data1[(index)];\
T2 _fhtmp2_ = (this)->data2[(index)];\
T3 _fhtmp3_ = (this)->data3[(index)];\
T4 _fhtmp4_ = (this)->data4[(index)];\
T5 _fhtmp5_ = (this)->data5[(index)];\
T6 _fhtmp6_ = (this)->data6[(index)];\
} while(0)

#define FHDUCK_MULTI_TMP_ASSIGN8(this, index, T0, T1, T2, T3, T4, T5, T6, T7) do {\
T0 _fhtmp0_ = (this)->data0[(index)];\
T1 _fhtmp1_ = (this)->data1[(index)];\
T2 _fhtmp2_ = (this)->data2[(index)];\
T3 _fhtmp3_ = (this)->data3[(index)];\
T4 _fhtmp4_ = (this)->data4[(index)];\
T5 _fhtmp5_ = (this)->data5[(index)];\
T6 _fhtmp6_ = (this)->data6[(index)];\
T7 _fhtmp7_ = (this)->data7[(index)];\
} while(0)

//

#define FHDUCK_MULTI_ASSIGN_TMP_N(N, this, index) \
FHDUCK_MULTI_ASSIGN_TMP##N(this, index)

#define FHDUCK_MULTI_ASSIGN_TMP0(this, index) do {\
(this)->data[(index)] = _fhtmp_;\
} while(0)

#define FHDUCK_MULTI_ASSIGN_TMP1(this, index) do {\
(this)->data0[(index)] = _fhtmp0_;\
} while(0)

#define FHDUCK_MULTI_ASSIGN_TMP2(this, index) do {\
(this)->data0[(index)] = _fhtmp0_;\
(this)->data1[(index)] = _fhtmp1_;\
} while(0)

#define FHDUCK_MULTI_ASSIGN_TMP3(this, index) do {\
(this)->data0[(index)] = _fhtmp0_;\
(this)->data1[(index)] = _fhtmp1_;\
(this)->data2[(index)] = _fhtmp2_;\
} while(0)

#define FHDUCK_MULTI_ASSIGN_TMP4(this, index) do {\
(this)->data0[(index)] = _fhtmp0_;\
(this)->data1[(index)] = _fhtmp1_;\
(this)->data2[(index)] = _fhtmp2_;\
(this)->data3[(index)] = _fhtmp3_;\
} while(0)

#define FHDUCK_MULTI_ASSIGN_TMP5(this, index) do {\
(this)->data0[(index)] = _fhtmp0_;\
(this)->data1[(index)] = _fhtmp1_;\
(this)->data2[(index)] = _fhtmp2_;\
(this)->data3[(index)] = _fhtmp3_;\
(this)->data4[(index)] = _fhtmp4_;\
} while(0)

#define FHDUCK_MULTI_ASSIGN_TMP6(this, index) do {\
(this)->data0[(index)] = _fhtmp0_;\
(this)->data1[(index)] = _fhtmp1_;\
(this)->data2[(index)] = _fhtmp2_;\
(this)->data3[(index)] = _fhtmp3_;\
(this)->data4[(index)] = _fhtmp4_;\
(this)->data5[(index)] = _fhtmp5_;\
} while(0)

#define FHDUCK_MULTI_ASSIGN_TMP7(this, index) do {\
(this)->data0[(index)] = _fhtmp0_;\
(this)->data1[(index)] = _fhtmp1_;\
(this)->data2[(index)] = _fhtmp2_;\
(this)->data3[(index)] = _fhtmp3_;\
(this)->data4[(index)] = _fhtmp4_;\
(this)->data5[(index)] = _fhtmp5_;\
(this)->data6[(index)] = _fhtmp6_;\
} while(0)

#define FHDUCK_MULTI_ASSIGN_TMP8(this, index) do {\
(this)->data0[(index)] = _fhtmp0_;\
(this)->data1[(index)] = _fhtmp1_;\
(this)->data2[(index)] = _fhtmp2_;\
(this)->data3[(index)] = _fhtmp3_;\
(this)->data4[(index)] = _fhtmp4_;\
(this)->data5[(index)] = _fhtmp5_;\
(this)->data6[(index)] = _fhtmp6_;\
(this)->data7[(index)] = _fhtmp7_;\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// END INCLUDE GUARD
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
