//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// BEGIN HEADER GUARD
//
#if !defined(FHSTRING_H)
#define FHSTRING_H


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// CONFIG
//
#if !defined(FHSTRING_API)
#  define FHSTRING_API
#endif

#if !defined(FHSTRING_ALLOC) && !defined(FHSTRING_REALLOC) && !defined(FHSTRING_FREE)
#  define FHSTRING_ALLOC(allocator, size_in_bytes) malloc(size_in_bytes)
#  define FHSTRING_REALLOC(allocator, data, size_in_bytes) realloc(data, size_in_bytes)
#  define FHSTRING_FREE(allocator, data) free(data)
#elif !defined(FHSTRING_ALLOC) || !defined(FHSTRING_REALLOC) || !defined(FHSTRING_FREE)
#  error Must define none or all of FHSTRING_ALLOC, FHSTRING_REALLOC, and FHSTRING_FREE.
#endif

#if !defined(FHSTRING_DEFAULT_BUILDER_CAPACITY)
#  define FHSTRING_DEFAULT_BUILDER_CAPACITY 128
#endif

#if !defined(FHSTRING_DEFAULT_BASE_STRING)
#  define FHSTRING_DEFAULT_BASE_STRING "0123456789ABCDEFGHJIKLMNOPQRSTUVWXYZ"
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// MACROS
//
#define FHSTRING_FIELD_ALIAS(T, ...) union { T __VA_ARGS__; }


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// TYPES
//
enum {
    FHSTRING_FALSE = 0,
    FHSTRING_TRUE  = 1,
};

#if !defined(FHSTRING_NO_STDINT)
#  include <stdint.h>
typedef uint8_t  fhstring_u8;
typedef uint16_t fhstring_u16;
typedef uint32_t fhstring_u32;
typedef uint64_t fhstring_u64;
typedef int8_t   fhstring_i8;
typedef int16_t  fhstring_i16;
typedef int32_t  fhstring_i32;
typedef int64_t  fhstring_i64;
#else
typedef unsigned char      fhstring_u8;
typedef unsigned short     fhstring_u16;
typedef unsigned int       fhstring_u32;
typedef unsigned long long fhstring_u64;
typedef signed char        fhstring_i8;
typedef signed short       fhstring_i16;
typedef signed int         fhstring_i32;
typedef signed long long   fhstring_i64;
#endif

// NOTE(Patrik): Negative values indicate an error.
// Other values depend on the function.
typedef fhstring_i32 fhstring_error;

typedef fhstring_u8 fhstring_bool;

//

#if !defined(FHString)
typedef struct FHString {
    char *data;
    FHSTRING_FIELD_ALIAS(fhstring_i32, count, length);
} FHString;

#  define FHString(...) fhstring_set(__VA_ARGS__)
#endif

#if !defined(FHString_Builder)
typedef struct FHString_Builder {
    void *allocator;
    char *data;
    FHSTRING_FIELD_ALIAS(fhstring_i32, count, length);
    fhstring_i32 capacity;
} FHString_Builder;

#  define FHString_Builder(...) fhstring_builder_set(__VA_ARGS__)
#endif

//

// NOTE(Patrik): A FHString_ID of 0 means it is invalid.
// A negative number is an error.
#if !defined(FHString_ID)
typedef fhstring_i32 FHString_ID;
#  define FHString_ID(...) fhstring_id_get(__VA_ARGS__)
#endif

#if !defined(FHString_ID_State)
typedef struct FHString_ID_State {
    FHString_Builder builder;
    FHString_ID last_used_id;
} FHString_ID_State;

#  define FHString_ID_State(...) fhstring_id_state_set(__VA_ARGS__)
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// API
//
FHSTRING_API fhstring_i32 fhstring_get_ntstring_length(const char *ntstring);

// NOTE(Patrik): SET
FHSTRING_API FHString fhstring_set(const char *it_data, fhstring_i32 it_length);
FHSTRING_API FHString fhstring_set_from_ntstring(const char *it);
FHSTRING_API FHString fhstring_set_from_builder(FHString_Builder it);
FHSTRING_API FHString_Builder fhstring_builder_set(void *allocator, const char *it_data, fhstring_i32 it_length, fhstring_i32 it_capacity);
FHSTRING_API FHString_Builder fhstring_builder_set_from_ntstring(const char *it);
FHSTRING_API FHString_Builder fhstring_builder_set_from_fhstring(FHString it);

#if defined(__cplusplus)
FHSTRING_API FHString fhstring_set(const char *it);
FHSTRING_API FHString fhstring_set(FHString_Builder it);
FHSTRING_API FHString_Builder fhstring_builder_set(const char *it);
FHSTRING_API FHString_Builder fhstring_builder_set(FHString it);
#endif

// NOTE(Patrik): MAKE
FHSTRING_API FHString fhstring_make(void *allocator, fhstring_i32 length);
FHSTRING_API FHString_Builder fhstring_builder_make(void *allocator, fhstring_i32 capacity);

// NOTE(Patrik): GROW
FHSTRING_API fhstring_error fhstring_grow(void *allocator, FHString *buffer, fhstring_i32 length);
FHSTRING_API fhstring_error fhstring_builder_grow(FHString_Builder *buffer, fhstring_i32 capacity);
FHSTRING_API fhstring_error fhstring_builder_maybe_grow(FHString_Builder *buffer, fhstring_i32 new_capacity);

#if defined(__cplusplus)
FHSTRING_API fhstring_error fhstring_grow(FHString_Builder *buffer, fhstring_i32 capacity);
FHSTRING_API fhstring_error fhstring_maybe_grow(FHString_Builder *buffer, fhstring_i32 new_capacity);
#endif

// NOTE(Patrik): REVERSE
FHSTRING_API fhstring_error fhstring_reverse_many(char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_error fhstring_reverse_ntstring(char *it);
FHSTRING_API fhstring_error fhstring_reverse(FHString it);
FHSTRING_API fhstring_error fhstring_reverse_builder(FHString_Builder it);

#if defined(__cplusplus)
FHSTRING_API fhstring_error fhstring_reverse(char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_error fhstring_reverse(char *it);
FHSTRING_API fhstring_error fhstring_reverse(FHString_Builder it);
#endif

// NOTE(Patrik): NULL TERMINATE
FHSTRING_API fhstring_error fhstring_null_terminate(FHString string); // NOTE(Patrik): This assumes that an extra byte has been allocated.
FHSTRING_API fhstring_error fhstring_builder_null_terminate(FHString_Builder *builder);

#if defined(__cplusplus)
FHSTRING_API fhstring_error fhstring_null_terminate(FHString_Builder *builder);
#endif

// NOTE(Patrik): COPY
FHSTRING_API FHString fhstring_copy(void *allocator, FHString it);
FHSTRING_API FHString_Builder fhstring_builder_copy(void *allocator, FHString_Builder it);

#if defined(__cplusplus)
FHSTRING_API FHString_Builder fhstring_copy(void *allocator, FHString_Builder it);
#endif

// NOTE(Patrik): INSERT AT
FHSTRING_API fhstring_i32 fhstring_insert_at(fhstring_i32 at_index, FHString *string, char it);
FHSTRING_API fhstring_i32 fhstring_insert_many_at(fhstring_i32 at_index, FHString *string, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_insert_ntstring_at(fhstring_i32 at_index, FHString *string, const char *it);
FHSTRING_API fhstring_i32 fhstring_insert_string_at(fhstring_i32 at_index, FHString *string, FHString it);
FHSTRING_API fhstring_i32 fhstring_insert_builder_at(fhstring_i32 at_index, FHString *string, FHString_Builder it);

FHSTRING_API fhstring_i32 fhstring_builder_insert_at(fhstring_i32 at_index, FHString_Builder *builder, char it);
FHSTRING_API fhstring_i32 fhstring_builder_insert_many_at(fhstring_i32 at_index, FHString_Builder *builder, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_builder_insert_ntstring_at(fhstring_i32 at_index, FHString_Builder *builder, const char *it);
FHSTRING_API fhstring_i32 fhstring_builder_insert_string_at(fhstring_i32 at_index, FHString_Builder *builder, FHString it);
FHSTRING_API fhstring_i32 fhstring_builder_insert_builder_at(fhstring_i32 at_index, FHString_Builder *builder, FHString_Builder it);

#if defined(__cplusplus)
FHSTRING_API fhstring_i32 fhstring_insert_at(fhstring_i32 at_index, FHString *string, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_insert_at(fhstring_i32 at_index, FHString *string, const char *it);
FHSTRING_API fhstring_i32 fhstring_insert_at(fhstring_i32 at_index, FHString *string, FHString it);
FHSTRING_API fhstring_i32 fhstring_insert_at(fhstring_i32 at_index, FHString *string, FHString_Builder it);

FHSTRING_API fhstring_i32 fhstring_insert_at(fhstring_i32 at_index, FHString_Builder *builder, char it);
FHSTRING_API fhstring_i32 fhstring_insert_at(fhstring_i32 at_index, FHString_Builder *builder, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_insert_at(fhstring_i32 at_index, FHString_Builder *builder, const char *it);
FHSTRING_API fhstring_i32 fhstring_insert_at(fhstring_i32 at_index, FHString_Builder *builder, FHString it);
FHSTRING_API fhstring_i32 fhstring_insert_at(fhstring_i32 at_index, FHString_Builder *builder, FHString_Builder it);
#endif

// NOTE(Patrik): INSERT
FHSTRING_API fhstring_i32 fhstring_insert(FHString *string, char it);
FHSTRING_API fhstring_i32 fhstring_insert_many(FHString *string, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_insert_ntstring(FHString *string, const char *it);
FHSTRING_API fhstring_i32 fhstring_insert_string(FHString *string, FHString it);
FHSTRING_API fhstring_i32 fhstring_insert_builder(FHString *string, FHString_Builder it);

FHSTRING_API fhstring_i32 fhstring_builder_insert(FHString_Builder *builder, char it);
FHSTRING_API fhstring_i32 fhstring_builder_insert_many(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_builder_insert_ntstring(FHString_Builder *builder, const char *it);
FHSTRING_API fhstring_i32 fhstring_builder_insert_string(FHString_Builder *builder, FHString it);
FHSTRING_API fhstring_i32 fhstring_builder_insert_builder(FHString_Builder *builder, FHString_Builder it);

#if defined(__cplusplus)
FHSTRING_API fhstring_i32 fhstring_insert(FHString *string, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_insert(FHString *string, const char *it);
FHSTRING_API fhstring_i32 fhstring_insert(FHString *string, FHString it);
FHSTRING_API fhstring_i32 fhstring_insert(FHString *string, FHString_Builder it);

FHSTRING_API fhstring_i32 fhstring_insert(FHString_Builder *builder, char it);
FHSTRING_API fhstring_i32 fhstring_insert(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_insert(FHString_Builder *builder, const char *it);
FHSTRING_API fhstring_i32 fhstring_insert(FHString_Builder *builder, FHString it);
FHSTRING_API fhstring_i32 fhstring_insert(FHString_Builder *builder, FHString_Builder it);
#endif

// NOTE(Patrik): APPEND
FHSTRING_API fhstring_i32 fhstring_builder_append(FHString_Builder *builder, char it);
FHSTRING_API fhstring_i32 fhstring_builder_append_many(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_builder_append_ntstring(FHString_Builder *builder, const char *it);
FHSTRING_API fhstring_i32 fhstring_builder_append_string(FHString_Builder *builder, FHString it);
FHSTRING_API fhstring_i32 fhstring_builder_append_builder(FHString_Builder *builder, FHString_Builder it);

#if defined(__cplusplus)
FHSTRING_API fhstring_i32 fhstring_append(FHString_Builder *builder, char it);
FHSTRING_API fhstring_i32 fhstring_append(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_append(FHString_Builder *builder, const char *it);
FHSTRING_API fhstring_i32 fhstring_append(FHString_Builder *builder, FHString it);
FHSTRING_API fhstring_i32 fhstring_append(FHString_Builder *builder, FHString_Builder it);
#endif

// NOTE(Patrik): PREPEND
FHSTRING_API fhstring_i32 fhstring_builder_prepend_unordered(FHString_Builder *builder, char it);
FHSTRING_API fhstring_i32 fhstring_builder_prepend_unordered_many(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_builder_prepend_unordered_ntstring(FHString_Builder *builder, const char *it);
FHSTRING_API fhstring_i32 fhstring_builder_prepend_unordered_string(FHString_Builder *builder, FHString it);
FHSTRING_API fhstring_i32 fhstring_builder_prepend_unordered_builder(FHString_Builder *builder, FHString_Builder it);

FHSTRING_API fhstring_i32 fhstring_builder_prepend_ordered(FHString_Builder *builder, char it);
FHSTRING_API fhstring_i32 fhstring_builder_prepend_ordered_many(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_builder_prepend_ordered_ntstring(FHString_Builder *builder, const char *it);
FHSTRING_API fhstring_i32 fhstring_builder_prepend_ordered_string(FHString_Builder *builder, FHString it);
FHSTRING_API fhstring_i32 fhstring_builder_prepend_ordered_builder(FHString_Builder *builder, FHString_Builder it);

#if defined(__cplusplus)
FHSTRING_API fhstring_i32 fhstring_prepend_unordered(FHString_Builder *builder, char it);
FHSTRING_API fhstring_i32 fhstring_prepend_unordered(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_prepend_unordered(FHString_Builder *builder, const char *it);
FHSTRING_API fhstring_i32 fhstring_prepend_unordered(FHString_Builder *builder, FHString it);
FHSTRING_API fhstring_i32 fhstring_prepend_unordered(FHString_Builder *builder, FHString_Builder it);

FHSTRING_API fhstring_i32 fhstring_prepend_ordered(FHString_Builder *builder, char it);
FHSTRING_API fhstring_i32 fhstring_prepend_ordered(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_prepend_ordered(FHString_Builder *builder, const char *it);
FHSTRING_API fhstring_i32 fhstring_prepend_ordered(FHString_Builder *builder, FHString it);
FHSTRING_API fhstring_i32 fhstring_prepend_ordered(FHString_Builder *builder, FHString_Builder it);
#endif

// NOTE(Patrik): REMOVE
FHSTRING_API fhstring_i32 fhstring_builder_remove_unordered(FHString_Builder *builder, fhstring_i32 at_index);
FHSTRING_API fhstring_i32 fhstring_builder_remove_unordered_range(FHString_Builder *builder, fhstring_i32 start_index, fhstring_i32 length);

FHSTRING_API fhstring_i32 fhstring_builder_remove_ordered(FHString_Builder *builder, fhstring_i32 at_index);
FHSTRING_API fhstring_i32 fhstring_builder_remove_ordered_range(FHString_Builder *builder, fhstring_i32 start_index, fhstring_i32 length);

#if defined(__cplusplus)
FHSTRING_API fhstring_i32 fhstring_remove_unordered(FHString_Builder *builder, fhstring_i32 at_index);
FHSTRING_API fhstring_i32 fhstring_remove_unordered(FHString_Builder *builder, fhstring_i32 start_index, fhstring_i32 length);

FHSTRING_API fhstring_i32 fhstring_remove_ordered(FHString_Builder *builder, fhstring_i32 at_index);
FHSTRING_API fhstring_i32 fhstring_remove_ordered(FHString_Builder *builder, fhstring_i32 start_index, fhstring_i32 length);
#endif

// NOTE(Patrik): TO UPPER
FHSTRING_API fhstring_i32 fhstring_many_to_upper_ascii(char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_ntstring_to_upper_ascii(char *it);
FHSTRING_API fhstring_i32 fhstring_to_upper_ascii(FHString it);
FHSTRING_API fhstring_i32 fhstring_builder_to_upper_ascii(FHString_Builder it);

#if defined(__cplusplus)
FHSTRING_API fhstring_i32 fhstring_to_upper_ascii(char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_to_upper_ascii(char *it);
FHSTRING_API fhstring_i32 fhstring_to_upper_ascii(FHString_Builder it);
#endif

// NOTE(Patrik): TO LOWER
FHSTRING_API fhstring_i32 fhstring_many_to_lower_ascii(char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_ntstring_to_lower_ascii(char *it);
FHSTRING_API fhstring_i32 fhstring_to_lower_ascii(FHString it);
FHSTRING_API fhstring_i32 fhstring_builder_to_lower_ascii(FHString_Builder it);

#if defined(__cplusplus)
FHSTRING_API fhstring_i32 fhstring_to_lower_ascii(const char *it_data, fhstring_i32 it_length);
FHSTRING_API fhstring_i32 fhstring_to_lower_ascii(const char *it);
FHSTRING_API fhstring_i32 fhstring_to_lower_ascii(FHString_Builder it);
#endif

// NOTE(Patrik): APPEND INTEGER
FHSTRING_API fhstring_i32 fhstring_builder_append_bits(FHString_Builder *builder, fhstring_u64 value, fhstring_i32 base, const char *digits, fhstring_bool is_signed);
FHSTRING_API fhstring_i32 fhstring_builder_append_u8(FHString_Builder *builder, fhstring_u8 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_builder_append_u16(FHString_Builder *builder, fhstring_u16 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_builder_append_u32(FHString_Builder *builder, fhstring_u32 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_builder_append_u64(FHString_Builder *builder, fhstring_u64 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_builder_append_i8(FHString_Builder *builder, fhstring_i8 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_builder_append_i16(FHString_Builder *builder, fhstring_i16 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_builder_append_i32(FHString_Builder *builder, fhstring_i32 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_builder_append_i64(FHString_Builder *builder, fhstring_i64 value, fhstring_i32 base, const char *digits);

#if defined(__cplusplus)
FHSTRING_API fhstring_i32 fhstring_append_bits(FHString_Builder *builder, fhstring_u64 value, fhstring_i32 base, const char *digits, fhstring_bool is_signed);
FHSTRING_API fhstring_i32 fhstring_append_u8(FHString_Builder *builder, fhstring_u8 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_append_u16(FHString_Builder *builder, fhstring_u16 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_append_u32(FHString_Builder *builder, fhstring_u32 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_append_u64(FHString_Builder *builder, fhstring_u64 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_append_i8(FHString_Builder *builder, fhstring_i8 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_append_i16(FHString_Builder *builder, fhstring_i16 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_append_i32(FHString_Builder *builder, fhstring_i32 value, fhstring_i32 base, const char *digits);
FHSTRING_API fhstring_i32 fhstring_append_i64(FHString_Builder *builder, fhstring_i64 value, fhstring_i32 base, const char *digits);
#endif

// NOTE(Patrik): EQ
FHSTRING_API fhstring_bool fhstring_many_eq(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_many_eq_ntstring(const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_bool fhstring_many_eq_string(const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_bool fhstring_many_eq_builder(const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_eq_many(FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_eq(FHString a, FHString b);
FHSTRING_API fhstring_bool fhstring_eq_ntstring(FHString a, const char *b);
FHSTRING_API fhstring_bool fhstring_eq_builder(FHString a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_ntstring_eq_many(const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_ntstring_eq_string(const char *a, FHString b);
FHSTRING_API fhstring_bool fhstring_ntstring_eq(const char *a, const char *b);
FHSTRING_API fhstring_bool fhstring_ntstring_eq_builder(const char *a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_builder_eq_many(FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_builder_eq_ntstring(FHString_Builder a, const char *b);
FHSTRING_API fhstring_bool fhstring_builder_eq_string(FHString_Builder a, FHString b);
FHSTRING_API fhstring_bool fhstring_builder_eq(FHString_Builder a, FHString_Builder b);

FHSTRING_API fhstring_bool fhstring_many_eq_no_case(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_many_eq_ntstring_no_case(const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_bool fhstring_many_eq_string_no_case(const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_bool fhstring_many_eq_builder_no_case(const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_eq_many_no_case(FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_eq_no_case(FHString a, FHString b);
FHSTRING_API fhstring_bool fhstring_eq_ntstring_no_case(FHString a, const char *b);
FHSTRING_API fhstring_bool fhstring_eq_builder_no_case(FHString a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_ntstring_eq_many_no_case(const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_ntstring_eq_string_no_case(const char *a, FHString b);
FHSTRING_API fhstring_bool fhstring_ntstring_eq_no_case(const char *a, const char *b);
FHSTRING_API fhstring_bool fhstring_ntstring_eq_builder_no_case(const char *a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_builder_eq_many_no_case(FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_builder_eq_ntstring_no_case(FHString_Builder a, const char *b);
FHSTRING_API fhstring_bool fhstring_builder_eq_string_no_case(FHString_Builder a, FHString b);
FHSTRING_API fhstring_bool fhstring_builder_eq_no_case(FHString_Builder a, FHString_Builder b);

#if defined(__cplusplus)
FHSTRING_API fhstring_bool fhstring_eq(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_eq(const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_bool fhstring_eq(const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_bool fhstring_eq(const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_eq(FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_eq(FHString a, const char *b);
FHSTRING_API fhstring_bool fhstring_eq(FHString a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_eq(const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_eq(const char *a, FHString b);
FHSTRING_API fhstring_bool fhstring_eq(const char *a, const char *b);
FHSTRING_API fhstring_bool fhstring_eq(const char *a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_eq(FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_eq(FHString_Builder a, const char *b);
FHSTRING_API fhstring_bool fhstring_eq(FHString_Builder a, FHString b);
FHSTRING_API fhstring_bool fhstring_eq(FHString_Builder a, FHString_Builder b);

FHSTRING_API fhstring_bool fhstring_eq_no_case(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_eq_no_case(const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_bool fhstring_eq_no_case(const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_bool fhstring_eq_no_case(const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_eq_no_case(FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_eq_no_case(FHString a, const char *b);
FHSTRING_API fhstring_bool fhstring_eq_no_case(FHString a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_eq_no_case(const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_eq_no_case(const char *a, FHString b);
FHSTRING_API fhstring_bool fhstring_eq_no_case(const char *a, const char *b);
FHSTRING_API fhstring_bool fhstring_eq_no_case(const char *a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_eq_no_case(FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_eq_no_case(FHString_Builder a, const char *b);
FHSTRING_API fhstring_bool fhstring_eq_no_case(FHString_Builder a, FHString b);
FHSTRING_API fhstring_bool fhstring_eq_no_case(FHString_Builder a, FHString_Builder b);
#endif

// NOTE(Patrik): BEGINS WITH AT
FHSTRING_API fhstring_bool fhstring_many_begins_with_char_at(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, char b);
FHSTRING_API fhstring_bool fhstring_many_begins_with_at(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_many_begins_with_ntstring_at(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_bool fhstring_many_begins_with_string_at(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_bool fhstring_many_begins_with_builder_at(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_ntstring_begins_with_char_at(fhstring_i32 at_index, const char *a, char b);
FHSTRING_API fhstring_bool fhstring_ntstring_begins_with_many_at(fhstring_i32 at_index, const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_ntstring_begins_with_at(fhstring_i32 at_index, const char *a, const char *b);
FHSTRING_API fhstring_bool fhstring_ntstring_begins_with_string_at(fhstring_i32 at_index, const char *a, FHString b);
FHSTRING_API fhstring_bool fhstring_ntstring_begins_with_builder_at(fhstring_i32 at_index, const char *a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_begins_with_char_at(fhstring_i32 at_index, FHString a, char b);
FHSTRING_API fhstring_bool fhstring_begins_with_many_at(fhstring_i32 at_index, FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_begins_with_ntstring_at(fhstring_i32 at_index, FHString a, const char *b);
FHSTRING_API fhstring_bool fhstring_begins_with_at(fhstring_i32 at_index, FHString a, FHString b);
FHSTRING_API fhstring_bool fhstring_begins_with_builder_at(fhstring_i32 at_index, FHString a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_builder_begins_with_char_at(fhstring_i32 at_index, FHString_Builder a, char b);
FHSTRING_API fhstring_bool fhstring_builder_begins_with_many_at(fhstring_i32 at_index, FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_builder_begins_with_ntstring_at(fhstring_i32 at_index, FHString_Builder a, const char *b);
FHSTRING_API fhstring_bool fhstring_builder_begins_with_string_at(fhstring_i32 at_index, FHString_Builder a, FHString b);
FHSTRING_API fhstring_bool fhstring_builder_begins_with_at(fhstring_i32 at_index, FHString_Builder a, FHString_Builder b);

#if defined(__cplusplus)
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, char b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, const char *a, char b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, const char *a, const char *b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, const char *a, FHString b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, const char *a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, FHString a, char b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, FHString a, const char *b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, FHString a, FHString b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, FHString a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, FHString_Builder a, char b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, FHString_Builder a, const char *b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, FHString_Builder a, FHString b);
FHSTRING_API fhstring_bool fhstring_begins_with(fhstring_i32 at_index, FHString_Builder a, FHString_Builder b);
#endif

// NOTE(Patrik): BEGINS WITH
FHSTRING_API fhstring_bool fhstring_many_begins_with_char(const char *a_data, fhstring_i32 a_length, char b);
FHSTRING_API fhstring_bool fhstring_many_begins_with(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_many_begins_with_ntstring(const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_bool fhstring_many_begins_with_string(const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_bool fhstring_many_begins_with_builder(const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_ntstring_begins_with_char(const char *a, char b);
FHSTRING_API fhstring_bool fhstring_ntstring_begins_with_many(const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_ntstring_begins_with(const char *a, const char *b);
FHSTRING_API fhstring_bool fhstring_ntstring_begins_with_string(const char *a, FHString b);
FHSTRING_API fhstring_bool fhstring_ntstring_begins_with_builder(const char *a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_begins_with_char(FHString a, char b);
FHSTRING_API fhstring_bool fhstring_begins_with_many(FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_begins_with_ntstring(FHString a, const char *b);
FHSTRING_API fhstring_bool fhstring_begins_with(FHString a, FHString b);
FHSTRING_API fhstring_bool fhstring_begins_with_builder(FHString a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_builder_begins_with_char(FHString_Builder a, char b);
FHSTRING_API fhstring_bool fhstring_builder_begins_with_many(FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_builder_begins_with_ntstring(FHString_Builder a, const char *b);
FHSTRING_API fhstring_bool fhstring_builder_begins_with_string(FHString_Builder a, FHString b);
FHSTRING_API fhstring_bool fhstring_builder_begins_with(FHString_Builder a, FHString_Builder b);

#if defined(__cplusplus)
FHSTRING_API fhstring_bool fhstring_begins_with(const char *a_data, fhstring_i32 a_length, char b);
FHSTRING_API fhstring_bool fhstring_begins_with(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_begins_with(const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_bool fhstring_begins_with(const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_bool fhstring_begins_with(const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_begins_with(const char *a, char b);
FHSTRING_API fhstring_bool fhstring_begins_with(const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_begins_with(const char *a, const char *b);
FHSTRING_API fhstring_bool fhstring_begins_with(const char *a, FHString b);
FHSTRING_API fhstring_bool fhstring_begins_with(const char *a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_begins_with(FHString a, char b);
FHSTRING_API fhstring_bool fhstring_begins_with(FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_begins_with(FHString a, const char *b);
FHSTRING_API fhstring_bool fhstring_begins_with(FHString a, FHString b);
FHSTRING_API fhstring_bool fhstring_begins_with(FHString a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_begins_with(FHString_Builder a, char b);
FHSTRING_API fhstring_bool fhstring_begins_with(FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_begins_with(FHString_Builder a, const char *b);
FHSTRING_API fhstring_bool fhstring_begins_with(FHString_Builder a, FHString b);
FHSTRING_API fhstring_bool fhstring_begins_with(FHString_Builder a, FHString_Builder b);
#endif

// NOTE(Patrik): ENDS WITH
FHSTRING_API fhstring_bool fhstring_many_ends_with_char(const char *a_data, fhstring_i32 a_length, char b);
FHSTRING_API fhstring_bool fhstring_many_ends_with(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_many_ends_with_ntstring(const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_bool fhstring_many_ends_with_string(const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_bool fhstring_many_ends_with_builder(const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_ntstring_ends_with_char(const char *a, char b);
FHSTRING_API fhstring_bool fhstring_ntstring_ends_with_many(const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_ntstring_ends_with(const char *a, const char *b);
FHSTRING_API fhstring_bool fhstring_ntstring_ends_with_string(const char *a, FHString b);
FHSTRING_API fhstring_bool fhstring_ntstring_ends_with_builder(const char *a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_ends_with_char(FHString a, char b);
FHSTRING_API fhstring_bool fhstring_ends_with_many(FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_ends_with_ntstring(FHString a, const char *b);
FHSTRING_API fhstring_bool fhstring_ends_with(FHString a, FHString b);
FHSTRING_API fhstring_bool fhstring_ends_with_builder(FHString a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_builder_ends_with_char(FHString_Builder a, char b);
FHSTRING_API fhstring_bool fhstring_builder_ends_with_many(FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_builder_ends_with_ntstring(FHString_Builder a, const char *b);
FHSTRING_API fhstring_bool fhstring_builder_ends_with_string(FHString_Builder a, FHString b);
FHSTRING_API fhstring_bool fhstring_builder_ends_with(FHString_Builder a, FHString_Builder b);

#if defined(__cplusplus)
FHSTRING_API fhstring_bool fhstring_ends_with(const char *a_data, fhstring_i32 a_length, char b);
FHSTRING_API fhstring_bool fhstring_ends_with(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_ends_with(const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_bool fhstring_ends_with(const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_bool fhstring_ends_with(const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_ends_with(const char *a, char b);
FHSTRING_API fhstring_bool fhstring_ends_with(const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_ends_with(const char *a, const char *b);
FHSTRING_API fhstring_bool fhstring_ends_with(const char *a, FHString b);
FHSTRING_API fhstring_bool fhstring_ends_with(const char *a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_ends_with(FHString a, char b);
FHSTRING_API fhstring_bool fhstring_ends_with(FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_ends_with(FHString a, const char *b);
FHSTRING_API fhstring_bool fhstring_ends_with(FHString a, FHString b);
FHSTRING_API fhstring_bool fhstring_ends_with(FHString a, FHString_Builder b);
FHSTRING_API fhstring_bool fhstring_ends_with(FHString_Builder a, char b);
FHSTRING_API fhstring_bool fhstring_ends_with(FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_bool fhstring_ends_with(FHString_Builder a, const char *b);
FHSTRING_API fhstring_bool fhstring_ends_with(FHString_Builder a, FHString b);
FHSTRING_API fhstring_bool fhstring_ends_with(FHString_Builder a, FHString_Builder b);
#endif

// NOTE(Patrik): INDEX OF
FHSTRING_API fhstring_i32 fhstring_many_index_of_char(const char *a_data, fhstring_i32 a_length, char b);
FHSTRING_API fhstring_i32 fhstring_many_index_of(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_many_index_of_ntstring(const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_i32 fhstring_many_index_of_string(const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_i32 fhstring_many_index_of_builder(const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_i32 fhstring_ntstring_index_of_char(const char *a, char b);
FHSTRING_API fhstring_i32 fhstring_ntstring_index_of_many(const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_ntstring_index_of(const char *a, const char *b);
FHSTRING_API fhstring_i32 fhstring_ntstring_index_of_string(const char *a, FHString b);
FHSTRING_API fhstring_i32 fhstring_ntstring_index_of_builder(const char *a, FHString_Builder b);
FHSTRING_API fhstring_i32 fhstring_index_of_char(FHString a, char b);
FHSTRING_API fhstring_i32 fhstring_index_of_many(FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_index_of_ntstring(FHString a, const char *b);
FHSTRING_API fhstring_i32 fhstring_index_of(FHString a, FHString b);
FHSTRING_API fhstring_i32 fhstring_index_of_builder(FHString a, FHString_Builder b);
FHSTRING_API fhstring_i32 fhstring_builder_index_of_char(FHString_Builder a, char b);
FHSTRING_API fhstring_i32 fhstring_builder_index_of_many(FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_builder_index_of_ntstring(FHString_Builder a, const char *b);
FHSTRING_API fhstring_i32 fhstring_builder_index_of_string(FHString_Builder a, FHString b);
FHSTRING_API fhstring_i32 fhstring_builder_index_of(FHString_Builder a, FHString_Builder b);

#if defined(__cplusplus)
FHSTRING_API fhstring_i32 fhstring_index_of(const char *a_data, fhstring_i32 a_length, char b);
FHSTRING_API fhstring_i32 fhstring_index_of(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_index_of(const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_i32 fhstring_index_of(const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_i32 fhstring_index_of(const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_i32 fhstring_index_of(const char *a, char b);
FHSTRING_API fhstring_i32 fhstring_index_of(const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_index_of(const char *a, const char *b);
FHSTRING_API fhstring_i32 fhstring_index_of(const char *a, FHString b);
FHSTRING_API fhstring_i32 fhstring_index_of(const char *a, FHString_Builder b);
FHSTRING_API fhstring_i32 fhstring_index_of(FHString a, char b);
FHSTRING_API fhstring_i32 fhstring_index_of(FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_index_of(FHString a, const char *b);
FHSTRING_API fhstring_i32 fhstring_index_of(FHString a, FHString b);
FHSTRING_API fhstring_i32 fhstring_index_of(FHString a, FHString_Builder b);
FHSTRING_API fhstring_i32 fhstring_index_of(FHString_Builder a, char b);
FHSTRING_API fhstring_i32 fhstring_index_of(FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_index_of(FHString_Builder a, const char *b);
FHSTRING_API fhstring_i32 fhstring_index_of(FHString_Builder a, FHString b);
FHSTRING_API fhstring_i32 fhstring_index_of(FHString_Builder a, FHString_Builder b);
#endif

// NOTE(Patrik): LAST INDEX OF
FHSTRING_API fhstring_i32 fhstring_many_last_index_of_char(const char *a_data, fhstring_i32 a_length, char b);
FHSTRING_API fhstring_i32 fhstring_many_last_index_of(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_many_last_index_of_ntstring(const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_i32 fhstring_many_last_index_of_string(const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_i32 fhstring_many_last_index_of_builder(const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_i32 fhstring_ntstring_last_index_of_char(const char *a, char b);
FHSTRING_API fhstring_i32 fhstring_ntstring_last_index_of_many(const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_ntstring_last_index_of(const char *a, const char *b);
FHSTRING_API fhstring_i32 fhstring_ntstring_last_index_of_string(const char *a, FHString b);
FHSTRING_API fhstring_i32 fhstring_ntstring_last_index_of_builder(const char *a, FHString_Builder b);
FHSTRING_API fhstring_i32 fhstring_last_index_of_char(FHString a, char b);
FHSTRING_API fhstring_i32 fhstring_last_index_of_many(FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_last_index_of_ntstring(FHString a, const char *b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(FHString a, FHString b);
FHSTRING_API fhstring_i32 fhstring_last_index_of_builder(FHString a, FHString_Builder b);
FHSTRING_API fhstring_i32 fhstring_builder_last_index_of_char(FHString_Builder a, char b);
FHSTRING_API fhstring_i32 fhstring_builder_last_index_of_many(FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_builder_last_index_of_ntstring(FHString_Builder a, const char *b);
FHSTRING_API fhstring_i32 fhstring_builder_last_index_of_string(FHString_Builder a, FHString b);
FHSTRING_API fhstring_i32 fhstring_builder_last_index_of(FHString_Builder a, FHString_Builder b);

#if defined(__cplusplus)
FHSTRING_API fhstring_i32 fhstring_last_index_of(const char *a_data, fhstring_i32 a_length, char b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_last_index_of(const char *a_data, fhstring_i32 a_length, const char *b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(const char *a_data, fhstring_i32 a_length, FHString b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(const char *a_data, fhstring_i32 a_length, FHString_Builder b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(const char *a, char b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(const char *a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_last_index_of(const char *a, const char *b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(const char *a, FHString b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(const char *a, FHString_Builder b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(FHString a, char b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(FHString a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_last_index_of(FHString a, const char *b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(FHString a, FHString b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(FHString a, FHString_Builder b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(FHString_Builder a, char b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(FHString_Builder a, const char *b_data, fhstring_i32 b_length);
FHSTRING_API fhstring_i32 fhstring_last_index_of(FHString_Builder a, const char *b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(FHString_Builder a, FHString b);
FHSTRING_API fhstring_i32 fhstring_last_index_of(FHString_Builder a, FHString_Builder b);
#endif

// NOTE(Patrik): STRING ID
FHSTRING_API fhstring_bool fhstring_id_exists(FHString_ID_State state, FHString_ID id);
FHSTRING_API fhstring_i32 fhstring_id_remove(FHString_ID_State *state, FHString_ID id);

FHSTRING_API FHString_ID fhstring_id_from_many(FHString_ID_State state, const char *it_data, fhstring_i32 it_length);
FHSTRING_API FHString_ID fhstring_id_from_ntstring(FHString_ID_State state, const char *it);
FHSTRING_API FHString_ID fhstring_id_from_string(FHString_ID_State state, FHString it);
FHSTRING_API FHString_ID fhstring_id_from_builder(FHString_ID_State state, FHString_Builder it);

#if defined(__cplusplus)
FHSTRING_API FHString_ID fhstring_id_get(FHString_ID_State state, const char *it_data, fhstring_i32 it_length);
FHSTRING_API FHString_ID fhstring_id_get(FHString_ID_State state, const char *it);
FHSTRING_API FHString_ID fhstring_id_get(FHString_ID_State state, FHString it);
FHSTRING_API FHString_ID fhstring_id_get(FHString_ID_State state, FHString_Builder it);
#endif

FHSTRING_API FHString_ID fhstring_id_add_from_many(FHString_ID_State *state, const char *it_data, fhstring_i32 it_length);
FHSTRING_API FHString_ID fhstring_id_add_from_ntstring(FHString_ID_State *state, const char *it);
FHSTRING_API FHString_ID fhstring_id_add_from_string(FHString_ID_State *state, FHString it);
FHSTRING_API FHString_ID fhstring_id_add_from_builder(FHString_ID_State *state, FHString_Builder it);

#if defined(__cplusplus)
FHSTRING_API FHString_ID fhstring_id_add(FHString_ID_State *state, const char *it_data, fhstring_i32 it_length);
FHSTRING_API FHString_ID fhstring_id_add(FHString_ID_State *state, const char *it);
FHSTRING_API FHString_ID fhstring_id_add(FHString_ID_State *state, FHString it);
FHSTRING_API FHString_ID fhstring_id_add(FHString_ID_State *state, FHString_Builder it);
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// ERROR CODES
//
enum {
    FHSTRING_ERROR_NONE = 0,
    FHSTRING_ERROR_UNDOCUMENTED_ERROR = -1,
    FHSTRING_ERROR_PATTERN_NOT_FOUND = -2,
    FHSTRING_ERROR_ARGUMENT_A_DATA_IS_NULL = -3,
    FHSTRING_ERROR_ARGUMENT_A_LENGTH_IS_EMPTY = -4,
    FHSTRING_ERROR_ARGUMENT_B_DATA_IS_NULL = -5,
    FHSTRING_ERROR_ARGUMENT_B_LENGTH_IS_EMPTY = -6,
    FHSTRING_ERROR_BASE_STRING_IS_NULL = -7,
    FHSTRING_ERROR_BASE_IS_TOO_LOW = -8,
    FHSTRING_ERROR_BASE_IS_TOO_HIGH = -9,
    FHSTRING_ERROR_INDEX_OUT_OF_BOUNDS = -10,
    FHSTRING_ERROR_IT_DATA_IS_NULL = -11,
    FHSTRING_ERROR_IT_LENGTH_IS_EMPTY = -12,
    FHSTRING_ERROR_INPUT_BUFFER_IS_NULL = -13,
    FHSTRING_ERROR_INPUT_BUFFER_DATA_IS_NULL = -14,
    FHSTRING_ERROR_INPUT_BUFFER_IS_EMPTY = -15,
    FHSTRING_ERROR_TRYING_TO_ALLOCATE_ZERO_BYTES_OR_LESS = -16,
    FHSTRING_ERROR_RAN_OUT_OF_MEMORY = -17,
};


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// END HEADER GUARD
//
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// BEGIN IMPLEMENTATION GUARD
//
#if defined(FHSTRING_IMPLEMENTATION) && !defined(FHSTRING_IMPLEMENTATION_DONE)
#  define FHSTRING_IMPLEMENTATION_DONE


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// UTILITY
//
FHSTRING_API fhstring_i32
fhstring_get_ntstring_length(const char *ntstring) {
    if(!ntstring) { return 0; }
    
    fhstring_i32 result = 0;
    while(ntstring[result]) { result += 1; }
    return result;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// SET
//
FHSTRING_API FHString
fhstring_set(const char *it_data, fhstring_i32 it_length) {
    FHString result = {0};
    result.data = (char *)it_data;
    result.length = it_length;
    return result;
}

FHSTRING_API FHString
fhstring_set_from_ntstring(const char *it) {
    FHString result = {0};
    result.data = (char *)it;
    result.length = fhstring_get_ntstring_length(result.data);
    return result;
}

FHSTRING_API FHString
fhstring_set_from_builder(FHString_Builder it) {
    FHString result = {0};
    result.data = (char *)it.data;
    result.length = it.length;
    return result;
}


FHSTRING_API FHString_Builder
fhstring_builder_set(void *allocator, const char *it_data, fhstring_i32 it_length, fhstring_i32 it_capacity) {
    FHString_Builder result = {0};
    result.allocator = allocator;
    result.data = (char *)it_data;
    result.length = it_length;
    result.capacity = it_capacity;
    return result;
}

FHSTRING_API FHString_Builder
fhstring_builder_set_from_ntstring(const char *it) {
    FHString_Builder result = {0};
    result.data = (char *)it;
    result.length = fhstring_get_ntstring_length(it);
    result.capacity = -1;
    return result;
}

FHSTRING_API FHString_Builder
fhstring_builder_set_from_fhstring(FHString it) {
    FHString_Builder result = {0};
    result.data = (char *)it.data;
    result.length = it.length;
    result.capacity = -1;
    return result;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// MAKE
//
FHSTRING_API FHString
fhstring_make(void *allocator, fhstring_i32 length) {
    FHString result = {0};
    if(length <= 0) { return result; }
    
    result.length = length;
    result.data = (char *)FHSTRING_ALLOC(allocator, result.length);
    return result;
}

FHSTRING_API FHString_Builder
fhstring_builder_make(void *allocator, fhstring_i32 capacity) {
    FHString_Builder result = {0};
    if(capacity <= 0) { return result; }
    
    result.capacity = FHSTRING_DEFAULT_BUILDER_CAPACITY;
    while(result.capacity < capacity) { result.capacity *= 2; }
    
    result.data = (char *)FHSTRING_ALLOC(allocator, result.capacity);
    return result;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// GROW
//
FHSTRING_API fhstring_error
fhstring_grow(void *allocator, FHString *buffer, fhstring_i32 length) {
    if(!buffer) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(length <= 0) { return FHSTRING_ERROR_TRYING_TO_ALLOCATE_ZERO_BYTES_OR_LESS; }
    
    char *new_data = buffer->data;
    if(new_data) {
        new_data = (char *)FHSTRING_REALLOC(allocator, new_data, length);
    } else {
        new_data = (char *)FHSTRING_ALLOC(allocator, length);
    }
    
    if(!new_data) { return FHSTRING_ERROR_RAN_OUT_OF_MEMORY; }
    
    buffer->data = new_data;
    buffer->length = length;
    return FHSTRING_ERROR_NONE;
}

FHSTRING_API fhstring_error
fhstring_builder_grow(FHString_Builder *buffer, fhstring_i32 capacity) {
    if(!buffer) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(capacity <= 0) { return FHSTRING_ERROR_TRYING_TO_ALLOCATE_ZERO_BYTES_OR_LESS; }
    
    char *new_data = buffer->data;
    if(new_data) {
        new_data = (char *)FHSTRING_REALLOC(buffer->allocator, new_data, capacity);
    } else {
        new_data = (char *)FHSTRING_ALLOC(buffer->allocator, capacity);
    }
    
    if(!new_data) { return FHSTRING_ERROR_RAN_OUT_OF_MEMORY; }
    buffer->data = new_data;
    buffer->capacity = capacity;
    return FHSTRING_ERROR_NONE;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_error
fhstring_grow(FHString_Builder *buffer, fhstring_i32 capacity) {
    return fhstring_builder_grow(buffer, capacity);
}
#endif

FHSTRING_API fhstring_error
fhstring_builder_maybe_grow(FHString_Builder *buffer, fhstring_i32 new_capacity) {
    if(!buffer) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(new_capacity <= 0) { return FHSTRING_ERROR_TRYING_TO_ALLOCATE_ZERO_BYTES_OR_LESS; }
    
    fhstring_i32 capacity = buffer->capacity;
    if(capacity <= 0) { capacity = FHSTRING_DEFAULT_BUILDER_CAPACITY; }
    while(capacity < new_capacity) { capacity *= 2; }
    
    char *new_data = buffer->data;
    if(new_data) {
        new_data = (char *)FHSTRING_REALLOC(buffer->allocator, new_data, capacity);
    } else {
        new_data = (char *)FHSTRING_ALLOC(buffer->allocator, capacity);
    }
    
    if(!new_data) { return FHSTRING_ERROR_RAN_OUT_OF_MEMORY; }
    buffer->data = new_data;
    buffer->capacity = capacity;
    return FHSTRING_ERROR_NONE;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_error
fhstring_maybe_grow(FHString_Builder *buffer, fhstring_i32 new_capacity) {
    return fhstring_builder_maybe_grow(buffer, new_capacity);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// COPY
//
FHSTRING_API FHString
fhstring_copy(void *allocator, FHString it) {
    FHString result = fhstring_make(allocator, it.length + 1);
    
    result.length = it.length;
    result.data[result.length] = 0;
    for(fhstring_i32 i = 0; i < result.length; i += 1) { result.data[i] = it.data[i]; }
    return result;
}

FHSTRING_API FHString_Builder
fhstring_builder_copy(void *allocator, FHString_Builder it) {
    FHString_Builder result = fhstring_builder_make(allocator, it.length + 1);
    
    result.length = it.length;
    result.data[result.length] = 0;
    for(fhstring_i32 i = 0; i < result.length; i += 1) { result.data[i] = it.data[i]; }
    return result;
}
#if defined(__cplusplus)
FHSTRING_API FHString_Builder
fhstring_copy(void *allocator, FHString_Builder it) {
    return fhstring_builder_copy(allocator, it);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// REVERSE
//
FHSTRING_API fhstring_error
fhstring_reverse_many(char *it_data, fhstring_i32 it_length) {
    if(!it_data) { return FHSTRING_ERROR_IT_DATA_IS_NULL; }
    if(it_length <= 0) { return FHSTRING_ERROR_IT_LENGTH_IS_EMPTY; }
    
    for(fhstring_i32 left = 0, right = (it_length - 1); left < right; left += 1, right += 1) {
        char tmp = it_data[left];
        it_data[left] = it_data[right];
        it_data[right] = tmp;
    }
    
    return FHSTRING_ERROR_NONE;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_error
fhstring_reverse(char *it_data, fhstring_i32 it_length) {
    return fhstring_reverse_many(it_data, it_length);
}
#endif

FHSTRING_API fhstring_error
fhstring_reverse_ntstring(char *it) {
    fhstring_i32 it_length = fhstring_get_ntstring_length(it);
    return fhstring_reverse_many(it, it_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_error
fhstring_reverse(char *it) {
    return fhstring_reverse_ntstring(it);
}
#endif

FHSTRING_API fhstring_error
fhstring_reverse(FHString it) {
    return fhstring_reverse_many(it.data, it.length);
}

FHSTRING_API fhstring_error
fhstring_reverse_builder(FHString_Builder it) {
    return fhstring_reverse_many(it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_error
fhstring_reverse(FHString_Builder it) {
    return fhstring_reverse_builder(it);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// NULL TERMINATE
//
FHSTRING_API fhstring_error
fhstring_null_terminate(FHString buffer) {
    if(!buffer.data) { return FHSTRING_ERROR_INPUT_BUFFER_DATA_IS_NULL; }
    if(buffer.length <= 0) { return FHSTRING_ERROR_INPUT_BUFFER_IS_EMPTY; }
    
    buffer.data[buffer.length - 1] = 0;
    return FHSTRING_ERROR_NONE;
}

FHSTRING_API fhstring_error
fhstring_builder_null_terminate(FHString_Builder *buffer) {
    if(!buffer) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    
    fhstring_error error = fhstring_builder_maybe_grow(buffer, buffer->capacity + 1);
    if(error < 0) { return error; }
    buffer->data[buffer->length - 1] = 0;
    return FHSTRING_ERROR_NONE;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_error
fhstring_null_terminate(FHString_Builder *buffer) {
    return fhstring_null_terminate(buffer);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INSERT AT
//
FHSTRING_API fhstring_i32
fhstring_insert_at(fhstring_i32 at_index, FHString *string, char it) {
    if(!string) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(!string->data) { return FHSTRING_ERROR_INPUT_BUFFER_DATA_IS_NULL; }
    if(at_index < 0 || at_index >= string->length) { return FHSTRING_ERROR_INDEX_OUT_OF_BOUNDS; }
    
    string->data[at_index] = it;
    return 1;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(fhstring_i32 at_index, FHString *string, char it) {
    return fhstring_insert_at(at_index, string, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_insert_many_at(fhstring_i32 at_index, FHString *string, const char *it_data, fhstring_i32 it_length) {
    if(!string) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(!string->data) { return FHSTRING_ERROR_INPUT_BUFFER_DATA_IS_NULL; }
    if(at_index < 0 || at_index >= string->length) { return FHSTRING_ERROR_INDEX_OUT_OF_BOUNDS; }
    if(!it_data) { return FHSTRING_ERROR_IT_DATA_IS_NULL; }
    if(it_length <= 0) { return 0; }
    
    fhstring_i32 length = at_index + it_length;
    if(length > string->length) { length = string->length; }
    length = string->length - length;
    
    fhstring_i32 bytes_written = 0;
    for(fhstring_i32 i = 0; i < length; i += 1) {
        string->data[at_index + i] = it_data[i];
        bytes_written += 1;
    }
    return bytes_written;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(fhstring_i32 at_index, FHString *string, const char *it_data, fhstring_i32 it_length) {
    return fhstring_insert_many_at(at_index, string, it_data, it_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_insert_ntstring_at(fhstring_i32 at_index, FHString *string, const char *it) {
    fhstring_i32 it_length = fhstring_get_ntstring_length(it);
    return fhstring_insert_many_at(at_index, string, it, it_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(fhstring_i32 at_index, FHString *string, const char *it) {
    return fhstring_insert_ntstring_at(at_index, string, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_insert_string_at(fhstring_i32 at_index, FHString *string, FHString it) {
    return fhstring_insert_many_at(at_index, string, it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(fhstring_i32 at_index, FHString *string, FHString it) {
    return fhstring_insert_string_at(at_index, string, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_insert_builder_at(fhstring_i32 at_index, FHString *string, FHString_Builder it) {
    return fhstring_insert_many_at(at_index, string, it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(fhstring_i32 at_index, FHString *string, FHString_Builder it) {
    return fhstring_insert_builder_at(at_index, string, it);
}
#endif

//

FHSTRING_API fhstring_i32
fhstring_builder_insert_at(fhstring_i32 at_index, FHString_Builder *builder, char it) {
    if(!builder) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    
    fhstring_builder_maybe_grow(builder, at_index + 1);
    builder->data[at_index] = it;
    return 1;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(fhstring_i32 at_index, FHString_Builder *builder, char it) {
    return fhstring_builder_insert_at(at_index, builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_insert_many_at(fhstring_i32 at_index, FHString_Builder *builder,
                                const char *it_data, fhstring_i32 it_length)
{
    if(!builder) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(!it_data) { return FHSTRING_ERROR_IT_DATA_IS_NULL; }
    if(it_length <= 0) { return 0; }
    
    fhstring_builder_maybe_grow(builder, at_index + it_length);
    
    fhstring_i32 bytes_written = 0;
    for(fhstring_i32 i = 0; i < it_length; i += 1) {
        builder->data[at_index + i] = it_data[i];
        bytes_written += 1;
    }
    return bytes_written;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(fhstring_i32 at_index, FHString_Builder *builder, const char *it_data, fhstring_i32 it_length) {
    return fhstring_builder_insert_many(at_index, builder, it_data, it_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_insert_ntstring_at(fhstring_i32 at_index, FHString_Builder *builder, const char *it) {
    fhstring_i32 it_length = fhstring_get_ntstring_length(it);
    return fhstring_builder_insert_many_at(at_index, builder, it, it_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(fhstring_i32 at_index, FHString_Builder *builder, const char *it) {
    return fhstring_builder_insert_ntstring_at(at_index, builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_insert_string_at(fhstring_i32 at_index, FHString_Builder *builder, FHString it) {
    return fhstring_builder_insert_many_at(at_index, builder, it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(fhstring_i32 at_index, FHString_Builder *builder, FHString it) {
    return fhstring_builder_insert_string_at(at_index, builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_insert_builder_at(fhstring_i32 at_index, FHString_Builder *builder, FHString_Builder it) {
    return fhstring_builder_insert_many_at(at_index, builder, it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(fhstring_i32 at_index, FHString_Builder *builder, FHString_Builder it) {
    return fhstring_builder_insert_builder_at(at_index, builder, it);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INSERT
//
FHSTRING_API fhstring_i32
fhstring_insert(FHString *string, char it) {
    return fhstring_insert_at(0, string, it);
}

FHSTRING_API fhstring_i32
fhstring_insert_many(FHString *string, const char *it_data, fhstring_i32 it_length) {
    return fhstring_insert_many_at(0, string, it_data, it_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(FHString *string, const char *it_data, fhstring_i32 it_length) {
    return fhstring_insert_many(string, it_data, it_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_insert_ntstring(FHString *string, const char *it) {
    return fhstring_insert_ntstring_at(0, string, it);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(FHString *string, const char *it) {
    return fhstring_insert_ntstring(string, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_insert_string(FHString *string, FHString it) {
    return fhstring_insert_string_at(0, string, it);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(FHString *string, FHString it) {
    return fhstring_insert_string(string, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_insert_builder(FHString *string, FHString_Builder it) {
    return fhstring_insert_builder_at(0, string, it);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(FHString *string, FHString_Builder it) {
    return fhstring_insert_builder(string, it);
}
#endif

//

FHSTRING_API fhstring_i32
fhstring_builder_insert(FHString_Builder *builder, char it) {
    return fhstring_builder_insert_at(0, builder, it);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(FHString_Builder *builder, char it) {
    return fhstring_builder_insert(builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_insert_many(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length) {
    return fhstring_builder_insert_many_at(0, builder, it_data, it_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length) {
    return fhstring_builder_insert_many(builder, it_data, it_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_insert_ntstring(FHString_Builder *builder, const char *it) {
    return fhstring_builder_insert_ntstring_at(0, builder, it);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(FHString_Builder *builder, const char *it) {
    return fhstring_builder_insert_ntstring(builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_insert_string(FHString_Builder *builder, FHString it) {
    return fhstring_builder_insert_string_at(0, builder, it);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(FHString_Builder *builder, FHString it) {
    return fhstring_builder_insert_string(builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_insert_builder(FHString_Builder *builder, FHString_Builder it) {
    return fhstring_builder_insert_builder_at(0, builder, it);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_insert(FHString_Builder *builder, FHString_Builder it) {
    return fhstring_builder_insert_builder(builder, it);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// APPEND
//
FHSTRING_API fhstring_i32
fhstring_builder_append(FHString_Builder *builder, char it) {
    if(!builder) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    
    fhstring_builder_maybe_grow(builder, builder->length + 1);
    builder->data[builder->length] = it;
    builder->length += 1;
    return 1;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append(FHString_Builder *builder, char it) {
    return fhstring_builder_append(builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_append_many(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length) {
    if(!builder) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(!it_data) { return FHSTRING_ERROR_IT_DATA_IS_NULL; }
    if(it_length <= 0) { return 0; }
    
    fhstring_builder_maybe_grow(builder, builder->length + it_length);
    for(fhstring_i32 i = 0; i < it_length; i += 1) {
        builder->data[builder->length + i] = it_data[i];
    }
    builder->length += it_length;
    return it_length;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length) {
    return fhstring_builder_append_many(builder, it_data, it_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_append_ntstring(FHString_Builder *builder, const char *it) {
    fhstring_i32 it_length = fhstring_get_ntstring_length(it);
    return fhstring_builder_append_many(builder, it, it_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append(FHString_Builder *builder, const char *it) {
    return fhstring_builder_append_ntstring(builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_append_string(FHString_Builder *builder, FHString it) {
    return fhstring_builder_append_many(builder, it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append(FHString_Builder *builder, FHString it) {
    return fhstring_builder_append_string(builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_append_builder(FHString_Builder *builder, FHString_Builder it) {
    return fhstring_builder_append_many(builder, it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append(FHString_Builder *builder, FHString_Builder it) {
    return fhstring_builder_append_builder(builder, it);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// PREPPEND
//
FHSTRING_API fhstring_i32
fhstring_builder_prepend_unordered(FHString_Builder *builder, char it) {
    if(!builder) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    
    fhstring_builder_maybe_grow(builder, builder->length + 1);
    builder->data[builder->length] = builder->data[0];
    builder->data[0] = it;
    builder->length += 1;
    return 1;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_prepend_unordered(FHString_Builder *builder, char it) {
    return fhstring_builder_prepend_unordered(builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_prepend_unordered_many(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length) {
    if(!builder) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(!it_data) { return FHSTRING_ERROR_IT_DATA_IS_NULL; }
    if(it_length <= 0) { return 0; }
    
    fhstring_builder_maybe_grow(builder, builder->length + it_length);
    for(fhstring_i32 i = 0; i < it_length; i += 1) {
        builder->data[builder->length + i] = builder->data[i];
        builder->data[i] = it_data[i];
    }
    builder->length += it_length;
    return it_length;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_prepend_unordered(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length) {
    return fhstring_builder_prepend_unordered_many(builder, it_data, it_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_prepend_unordered_ntstring(FHString_Builder *builder, const char *it) {
    fhstring_i32 it_length = fhstring_get_ntstring_length(it);
    return fhstring_builder_prepend_unordered_many(builder, it, it_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_prepend_unordered(FHString_Builder *builder, const char *it) {
    return fhstring_builder_prepend_unordered_ntstring(builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_prepend_unordered_string(FHString_Builder *builder, FHString it) {
    return fhstring_builder_prepend_unordered_many(builder, it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_prepend_unordered(FHString_Builder *builder, FHString it) {
    return fhstring_builder_prepend_unordered_string(builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_prepend_unordered_builder(FHString_Builder *builder, FHString_Builder it) {
    return fhstring_builder_prepend_unordered_many(builder, it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_prepend_unordered(FHString_Builder *builder, FHString_Builder it) {
    return fhstring_builder_prepend_unordered_builder(builder, it);
}
#endif

//

FHSTRING_API fhstring_i32
fhstring_builder_prepend_ordered(FHString_Builder *builder, char it) {
    if(!builder) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    
    fhstring_builder_maybe_grow(builder, builder->length + 1);
    for(fhstring_i32 i = builder->length; i >= 1; i -= 1) {
        builder->data[i] = builder->data[i - 1];
    }
    builder->data[0] = it;
    builder->length += 1;
    return 1;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_prepend_ordered(FHString_Builder *builder, char it) {
    return fhstring_builder_prepend_ordered(builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_prepend_ordered_many(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length) {
    if(!builder) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(!it_data) { return FHSTRING_ERROR_IT_DATA_IS_NULL; }
    if(it_length <= 0) { return 0; }
    
    fhstring_builder_maybe_grow(builder, builder->length + it_length);
    for(fhstring_i32 i = builder->length + (it_length - 1); i >= it_length; i -= 1) {
        builder->data[i] = builder->data[i - it_length];
    }
    for(fhstring_i32 i = 0; i < it_length; i += 1) {
        builder->data[i] = it_data[i];
    }
    builder->length += it_length;
    return it_length;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_prepend_ordered(FHString_Builder *builder, const char *it_data, fhstring_i32 it_length) {
    return fhstring_builder_prepend_ordered_many(builder, it_data, it_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_prepend_ordered_ntstring(FHString_Builder *builder, const char *it) {
    fhstring_i32 it_length = fhstring_get_ntstring_length(it);
    return fhstring_builder_prepend_ordered_many(builder, it, it_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_prepend_ordered(FHString_Builder *builder, const char *it) {
    return fhstring_builder_prepend_ordered_ntstring(builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_prepend_ordered_string(FHString_Builder *builder, FHString it) {
    return fhstring_builder_prepend_ordered_many(builder, it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_prepend_ordered(FHString_Builder *builder, FHString it) {
    return fhstring_builder_prepend_ordered_string(builder, it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_prepend_ordered_builder(FHString_Builder *builder, FHString_Builder it) {
    return fhstring_builder_prepend_ordered_many(builder, it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_prepend_ordered(FHString_Builder *builder, FHString_Builder it) {
    return fhstring_builder_prepend_ordered_builder(builder, it);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// REMOVE
//
FHSTRING_API fhstring_i32
fhstring_builder_remove_unordered(FHString_Builder *buffer, fhstring_i32 at_index) {
    if(!buffer) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(at_index < 0) { return FHSTRING_ERROR_INDEX_OUT_OF_BOUNDS; }
    if(buffer->length <= at_index) { return FHSTRING_ERROR_INDEX_OUT_OF_BOUNDS; }
    
    buffer->data[at_index] = buffer->data[buffer->length - 1];
    buffer->length -= 1;
    return 1;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_remove_unordered(FHString_Builder *builder, fhstring_i32 at_index) {
    return fhstring_builder_remove_unordered(builder, at_index);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_remove_unordered_range(FHString_Builder *builder, fhstring_i32 start_index, fhstring_i32 length) {
    if(!builder) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(start_index < 0) { return FHSTRING_ERROR_INDEX_OUT_OF_BOUNDS; }
    if(start_index >= builder->length) { return FHSTRING_ERROR_INDEX_OUT_OF_BOUNDS; }
    
    fhstring_i32 bytes_removed = 0;
    for(fhstring_i32 i = start_index + (length - 1); i >= start_index; i -= 1) {
        builder->data[i] = builder->data[builder->length - 1];
        builder->length -= 1;
        bytes_removed += 1;
    }
    return bytes_removed;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_remove_unordered(FHString_Builder *builder, fhstring_i32 start_index, fhstring_i32 length) {
    return fhstring_builder_remove_unordered_range(builder, start_index, length);
}
#endif

//

FHSTRING_API fhstring_i32
fhstring_builder_remove_ordered(FHString_Builder *builder, fhstring_i32 at_index) {
    if(!builder) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(at_index < 0) { return FHSTRING_ERROR_INDEX_OUT_OF_BOUNDS; }
    if(builder->length <= at_index) { return FHSTRING_ERROR_INDEX_OUT_OF_BOUNDS; }
    
    if(at_index > 0) {
        for(fhstring_i32 i = at_index; i < builder->length - 1; i += 1) {
            builder->data[i] = builder->data[i + 1];
        }
    } else {
        for(fhstring_i32 i = 1; i < builder->length; i += 1) {
            builder->data[i - 1] = builder->data[i];
        }
    }
    
    builder->length -= 1;
    return 1;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_remove_ordered(FHString_Builder *builder, fhstring_i32 at_index) {
    return fhstring_builder_remove_ordered(builder, at_index);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_remove_ordered_range(FHString_Builder *builder, fhstring_i32 start_index, fhstring_i32 length) {
    if(!builder) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(start_index < 0) { return FHSTRING_ERROR_INDEX_OUT_OF_BOUNDS; }
    if(start_index >= builder->length) { return FHSTRING_ERROR_INDEX_OUT_OF_BOUNDS; }
    
    for(fhstring_i32 i = start_index + length; i < builder->length; i += 1) {
        builder->data[i - length] = builder->data[i];
    }
    
    fhstring_i32 bytes_removed = length;
    if(start_index + length >= builder->length) {
        bytes_removed = (start_index + length) - builder->length;
    }
    builder->length -= bytes_removed;
    return bytes_removed;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_remove_ordered(FHString_Builder *builder, fhstring_i32 start_index, fhstring_i32 length) {
    return fhstring_builder_remove_ordered_range(builder, start_index, length);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// TO UPPER
//
FHSTRING_API fhstring_i32
fhstring_many_to_upper_ascii(char *it_data, fhstring_i32 it_length) {
    if(!it_data) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    
    fhstring_i32 bytes_converted = 0;
    for(fhstring_i32 i = 0; i < it_length; i += 1) {
        if(it_data[i] >= 'a' && it_data[i] <= 'z') {
            it_data[i] &= ~0x20;
            bytes_converted += 1;
        }
    }
    return bytes_converted;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_to_upper_ascii(char *it_data, fhstring_i32 it_length) {
    return fhstring_many_to_upper_ascii(it_data, it_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_ntstring_to_upper_ascii(char *it) {
    fhstring_i32 it_length = fhstring_get_ntstring_length(it);
    return fhstring_many_to_upper_ascii(it, it_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_to_upper_ascii(char *it) {
    return fhstring_ntstring_to_upper_ascii(it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_to_upper_ascii(FHString it) {
    return fhstring_many_to_upper_ascii(it.data, it.length);
}

FHSTRING_API fhstring_i32
fhstring_builder_to_upper_ascii(FHString_Builder it) {
    return fhstring_many_to_upper_ascii(it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_to_upper_ascii(FHString_Builder it) {
    return fhstring_builder_to_upper_ascii(it);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// TO LOWER
//
FHSTRING_API fhstring_i32
fhstring_many_to_lower_ascii(char *it_data, fhstring_i32 it_length) {
    if(!it_data) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    
    fhstring_i32 bytes_converted = 0;
    for(fhstring_i32 i = 0; i < it_length; i += 1) {
        if(it_data[i] >= 'A' && it_data[i] <= 'Z') {
            it_data[i] |= 0x20;
            bytes_converted += 1;
        }
    }
    return bytes_converted;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_to_lower_ascii(char *it_data, fhstring_i32 it_length) {
    return fhstring_many_to_lower_ascii(it_data, it_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_ntstring_to_lower_ascii(char *it) {
    fhstring_i32 it_length = fhstring_get_ntstring_length(it);
    return fhstring_many_to_lower_ascii(it, it_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_to_lower_ascii(char *it) {
    return fhstring_ntstring_to_lower_ascii(it);
}
#endif

FHSTRING_API fhstring_i32
fhstring_to_lower_ascii(FHString it) {
    return fhstring_many_to_lower_ascii(it.data, it.length);
}

FHSTRING_API fhstring_i32
fhstring_builder_to_lower_ascii(FHString_Builder it) {
    return fhstring_many_to_lower_ascii(it.data, it.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_to_lower_ascii(FHString_Builder it) {
    return fhstring_builder_to_lower_ascii(it);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// APPEND INTEGER
//
FHSTRING_API fhstring_i32
fhstring_builder_append_bits(FHString_Builder *builder, fhstring_u64 value,
                             fhstring_i32 base, const char *digits, fhstring_bool is_signed)
{
    if(!builder) { return FHSTRING_ERROR_INPUT_BUFFER_IS_NULL; }
    if(base < 2) { return FHSTRING_ERROR_BASE_IS_TOO_LOW; }
    if(!digits) {
        digits = FHSTRING_DEFAULT_BASE_STRING;
        if(!digits) { return FHSTRING_ERROR_BASE_STRING_IS_NULL; }
        fhstring_i32 digits_length = fhstring_get_ntstring_length(digits);
        if(base > digits_length) { return FHSTRING_ERROR_BASE_IS_TOO_HIGH; }
    }
    
    fhstring_bool is_negative = FHSTRING_FALSE;
    if(is_signed) {
        is_negative = (value & 0x8000000000000000ULL) == 0x8000000000000000ULL;
        value &= 0x7FFFFFFFFFFFFFFFULL;
    }
    
    fhstring_i32 offset = builder->length;
    do {
        char c = digits[value % base];
        fhstring_builder_append(builder, c);
        value /= base;
    } while(value >= base);
    
    if(is_negative) { fhstring_builder_append(builder, '-'); }
    
    fhstring_error error = fhstring_reverse_many(builder->data + offset, builder->length - offset);
    if(error < 0) { return error; }
    
    return builder->length - offset;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append_bits(FHString_Builder *builder, fhstring_u64 value,
                     fhstring_i32 base, const char *digits, fhstring_bool is_signed)
{
    return fhstring_builder_append_bits(builder, value, base, digits, is_signed);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_append_u8(FHString_Builder *builder, fhstring_u8 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_bits(builder, (fhstring_u64)value, base, digits, FHSTRING_FALSE);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append_u8(FHString_Builder *builder, fhstring_u8 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_u8(builder, value, base, digits);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_append_u16(FHString_Builder *builder, fhstring_u16 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_bits(builder, (fhstring_u64)value, base, digits, FHSTRING_FALSE);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append_u16(FHString_Builder *builder, fhstring_u16 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_u16(builder, value, base, digits);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_append_u32(FHString_Builder *builder, fhstring_u32 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_bits(builder, (fhstring_u64)value, base, digits, FHSTRING_FALSE);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append_u32(FHString_Builder *builder, fhstring_u32 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_u32(builder, value, base, digits);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_append_u64(FHString_Builder *builder, fhstring_u64 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_bits(builder, value, base, digits, FHSTRING_FALSE);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append_u64(FHString_Builder *builder, fhstring_u64 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_u64(builder, value, base, digits);
}
#endif

//

FHSTRING_API fhstring_i32
fhstring_builder_append_i8(FHString_Builder *builder, fhstring_i8 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_i64(builder, (fhstring_i64)value, base, digits);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append_i8(FHString_Builder *builder, fhstring_i8 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_i8(builder, value, base, digits);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_append_i16(FHString_Builder *builder, fhstring_i16 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_i64(builder, (fhstring_i64)value, base, digits);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append_i16(FHString_Builder *builder, fhstring_i16 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_i16(builder, value, base, digits);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_append_i32(FHString_Builder *builder, fhstring_i32 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_i64(builder, (fhstring_i64)value, base, digits);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append_i32(FHString_Builder *builder, fhstring_i32 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_i32(builder, value, base, digits);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_append_i64(FHString_Builder *builder, fhstring_i64 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_bits(builder, *(fhstring_u64 *)&value, base, digits, FHSTRING_TRUE);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_append_i64(FHString_Builder *builder, fhstring_i64 value, fhstring_i32 base, const char *digits) {
    return fhstring_builder_append_i64(builder, value, base, digits);
}
#endif



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// EQ
//
FHSTRING_API fhstring_bool
fhstring_many_eq(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length) {
    if(a_length != b_length || a_length < 0) { return FHSTRING_FALSE; }
    if(a_data == b_data) { return FHSTRING_TRUE; }
    if(a_data == 0 || b_data == 0) { return FHSTRING_FALSE; }
    
    for(fhstring_i32 i = 0; i < a_length; i += 1) {
        if(a_data[i] != b_data[i]) { return FHSTRING_FALSE; }
    }
    return FHSTRING_TRUE;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_eq(a_data, a_length, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_eq_ntstring(const char *a_data, fhstring_i32 a_length, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_eq(a_data, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(const char *a_data, fhstring_i32 a_length, const char *b) {
    return fhstring_many_eq_ntstring(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_eq_string(const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_eq(a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_eq_string(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_eq_builder(const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_eq(a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_eq_builder(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_eq_many(FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_eq(a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_eq_many(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_eq(FHString a, FHString b) {
    return fhstring_many_eq(a.data, a.length, b.data, b.length);
}

FHSTRING_API fhstring_bool
fhstring_eq_ntstring(FHString a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_eq(a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(FHString a, const char *b) {
    return fhstring_eq_ntstring(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_eq_builder(FHString a, FHString_Builder b) {
    return fhstring_many_eq(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(FHString a, FHString_Builder b) {
    return fhstring_eq_builder(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_eq_many(const char *a, const char *b_data, fhstring_i32 b_length) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_eq(a, a_length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(const char *a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_ntstring_eq_many(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_eq_string(const char *a, FHString b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_eq(a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(const char *a, FHString b) {
    return fhstring_ntstring_eq_string(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_eq(const char *a, const char *b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_eq(a, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(const char *a, const char *b) {
    return fhstring_ntstring_eq(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_eq_builder(const char *a, FHString_Builder b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_eq(a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(const char *a, FHString_Builder b) {
    return fhstring_ntstring_eq_builder(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_eq_many(FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_eq(a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_builder_eq_many(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_eq_ntstring(FHString_Builder a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_eq(a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(FHString_Builder a, const char *b) {
    return fhstring_builder_eq_ntstring(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_eq_string(FHString_Builder a, FHString b) {
    return fhstring_many_eq(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(FHString_Builder a, FHString b) {
    return fhstring_builder_eq_string(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_eq(FHString_Builder a, FHString_Builder b) {
    return fhstring_many_eq(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq(FHString_Builder a, FHString_Builder b) {
    return fhstring_builder_eq(a, b);
}
#endif

//

FHSTRING_API fhstring_bool
fhstring_many_eq_no_case(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length) {
    if(a_length != b_length || a_length < 0) { return FHSTRING_FALSE; }
    if(a_data == b_data) { return FHSTRING_TRUE; }
    if(a_data == 0 || b_data == 0) { return FHSTRING_FALSE; }
    
    for(fhstring_i32 i = 0; i < a_length; i += 1) {
        char a = a_data[i];
        char b = b_data[i];
        
        if(a != b) {
            if((a | 0x20) != (b | 0x20)) { return FHSTRING_FALSE; }
            if((a < 'A' || (a > 'Z' && a < 'a') || a > 'z') && (b < 'A' || (b > 'Z' && b < 'a') || b > 'z')) {
                return FHSTRING_FALSE;
            }
        }
    }
    return FHSTRING_TRUE;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_eq_no_case(a_data, a_length, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_eq_ntstring_no_case(const char *a_data, fhstring_i32 a_length, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_eq_no_case(a_data, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(const char *a_data, fhstring_i32 a_length, const char *b) {
    return fhstring_many_eq_ntstring_no_case(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_eq_string_no_case(const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_eq_no_case(a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_eq_string_no_case(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_eq_builder_no_case(const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_eq_no_case(a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_eq_builder_no_case(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_eq_many_no_case(FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_eq_no_case(a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_eq_many_no_case(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_eq_no_case(FHString a, FHString b) {
    return fhstring_many_eq_no_case(a.data, a.length, b.data, b.length);
}

FHSTRING_API fhstring_bool
fhstring_eq_ntstring_no_case(FHString a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_eq_no_case(a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(FHString a, const char *b) {
    return fhstring_eq_ntstring_no_case(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_eq_builder_no_case(FHString a, FHString_Builder b) {
    return fhstring_many_eq_no_case(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(FHString a, FHString_Builder b) {
    return fhstring_eq_builder_no_case(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_eq_many_no_case(const char *a, const char *b_data, fhstring_i32 b_length) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_eq_no_case(a, a_length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(const char *a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_ntstring_eq_many_no_case(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_eq_string_no_case(const char *a, FHString b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_eq_no_case(a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(const char *a, FHString b) {
    return fhstring_ntstring_eq_string_no_case(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_eq_no_case(const char *a, const char *b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_eq_no_case(a, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(const char *a, const char *b) {
    return fhstring_ntstring_eq_no_case(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_eq_builder_no_case(const char *a, FHString_Builder b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_eq_no_case(a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(const char *a, FHString_Builder b) {
    return fhstring_ntstring_eq_builder_no_case(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_eq_many_no_case(FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_eq_no_case(a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_builder_eq_many_no_case(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_eq_ntstring_no_case(FHString_Builder a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_eq_no_case(a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(FHString_Builder a, const char *b) {
    return fhstring_builder_eq_ntstring_no_case(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_eq_string_no_case(FHString_Builder a, FHString b) {
    return fhstring_many_eq_no_case(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(FHString_Builder a, FHString b) {
    return fhstring_builder_eq_string_no_case(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_eq_builder_no_case(FHString_Builder a, FHString_Builder b) {
    return fhstring_many_eq_no_case(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_eq_no_case(FHString_Builder a, FHString_Builder b) {
    return fhstring_builder_eq_builder_no_case(a, b);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// BEGINS WITH AT
//
FHSTRING_API fhstring_bool
fhstring_many_begins_with_char_at(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, char b) {
    if(!a_data) { return FHSTRING_FALSE; }
    if(at_index < 0 || at_index >= a_length) { return FHSTRING_FALSE; }
    if(a_length <= 0) { return FHSTRING_FALSE; }
    
    return (a_data[0] == b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, char b) {
    return fhstring_many_begins_with_char_at(at_index, a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_begins_with_at(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length,
                             const char *b_data, fhstring_i32 b_length)
{
    if(!a_data || !b_data) { return FHSTRING_FALSE; }
    if(a_length <= 0 || b_length <= 0) { return FHSTRING_FALSE; }
    if(at_index < 0 || at_index >= a_length) { return FHSTRING_FALSE; }
    if(a_length < at_index + b_length) { return FHSTRING_FALSE; }
    
    for(fhstring_i32 i = 0; i < b_length; i += 1) {
        if(a_data[at_index + i] != b_data[i]) { return FHSTRING_FALSE; }
    }
    return FHSTRING_TRUE;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length,
                     const char *b_data, fhstring_i32 b_length)
{
    return fhstring_many_begins_with_at(at_index, a_data, a_length, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_begins_with_ntstring_at(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length,
                                      const char *b)
{
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_begins_with_at(at_index, a_data, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, const char *b) {
    return fhstring_many_begins_with_ntstring_at(at_index, a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_begins_with_string_at(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_begins_with_at(at_index, a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_begins_with_string_at(at_index, a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_begins_with_builder_at(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length,
                                     FHString_Builder b)
{
    return fhstring_many_begins_with_at(at_index, a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_begins_with_builder_at(at_index, a_data, a_length, b);
}
#endif

//

FHSTRING_API fhstring_bool
fhstring_ntstring_begins_with_char_at(fhstring_i32 at_index, const char *a, char b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_begins_with_char_at(at_index, a, a_length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, const char *a, char b) {
    return fhstring_ntstring_begins_with_char_at(at_index, a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_begins_with_many_at(fhstring_i32 at_index, const char *a,
                                      const char *b_data, fhstring_i32 b_length)
{
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_begins_with_at(at_index, a, a_length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, const char *a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_ntstring_begins_with_many_at(at_index, a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_begins_with_at(fhstring_i32 at_index, const char *a, const char *b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_begins_with_at(at_index, a, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, const char *a, const char *b) {
    return fhstring_ntstring_begins_with_at(at_index, a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_begins_with_string_at(fhstring_i32 at_index, const char *a, FHString b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_begins_with_at(at_index, a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, const char *a, FHString b) {
    return fhstring_ntstring_begins_with_string_at(at_index, a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_begins_with_builder_at(fhstring_i32 at_index, const char *a, FHString_Builder b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_begins_with_at(at_index, a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, const char *a, FHString_Builder b) {
    return fhstring_ntstring_begins_with_builder_at(at_index, a, b);
}
#endif

//

FHSTRING_API fhstring_bool
fhstring_begins_with_char_at(fhstring_i32 at_index, FHString a, char b) {
    return fhstring_many_begins_with_char_at(at_index, a.data, a.length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, FHString a, char b) {
    return fhstring_begins_with_char_at(at_index, a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_begins_with_many_at(fhstring_i32 at_index, FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_begins_with_at(at_index, a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_begins_with_many_at(at_index, a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_begins_with_ntstring_at(fhstring_i32 at_index, FHString a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_begins_with_at(at_index, a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, FHString a, const char *b) {
    return fhstring_begins_with_ntstring_at(at_index, a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_begins_with_at(fhstring_i32 at_index, FHString a, FHString b) {
    return fhstring_many_begins_with_at(at_index, a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, FHString a, FHString b) {
    return fhstring_begins_with_at(at_index, a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_begins_with_builder_at(fhstring_i32 at_index, FHString a, FHString_Builder b) {
    return fhstring_many_begins_with_at(at_index, a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, FHString a, FHString_Builder b) {
    return fhstring_begins_with_builder_at(at_index, a, b);
}
#endif

//

FHSTRING_API fhstring_bool
fhstring_builder_begins_with_char_at(fhstring_i32 at_index, FHString_Builder a, char b) {
    return fhstring_many_begins_with_char_at(at_index, a.data, a.length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, FHString_Builder a, char b) {
    return fhstring_builder_begins_with_char_at(at_index, a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_begins_with_many_at(fhstring_i32 at_index, FHString_Builder a,
                                     const char *b_data, fhstring_i32 b_length)
{
    return fhstring_many_begins_with_at(at_index, a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_builder_begins_with_many_at(at_index, a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_begins_with_ntstring_at(fhstring_i32 at_index, FHString_Builder a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_begins_with_at(at_index, a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, FHString_Builder a, const char *b) {
    return fhstring_builder_begins_with_ntstring_at(at_index, a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_begins_with_string_at(fhstring_i32 at_index, FHString_Builder a, FHString b) {
    return fhstring_many_begins_with_at(at_index, a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, FHString_Builder a, FHString b) {
    return fhstring_builder_begins_with_string_at(at_index, a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_begins_with_at(fhstring_i32 at_index, FHString_Builder a, FHString_Builder b) {
    return fhstring_many_begins_with_at(at_index, a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(fhstring_i32 at_index, FHString_Builder a, FHString_Builder b) {
    return fhstring_builder_begins_with_at(at_index, a, b);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// BEGINS WITH
//
FHSTRING_API fhstring_bool
fhstring_many_begins_with_char(const char *a_data, fhstring_i32 a_length, char b) {
    return fhstring_many_begins_with_char_at(0, a_data, a_length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(const char *a_data, fhstring_i32 a_length, char b) {
    return fhstring_many_begins_with_char(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_begins_with(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_begins_with_at(0, a_data, a_length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_begins_with(a_data, a_length, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_begins_with_ntstring(const char *a_data, fhstring_i32 a_length, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_begins_with(a_data, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(const char *a_data, fhstring_i32 a_length, const char *b) {
    return fhstring_many_begins_with_ntstring(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_begins_with_string(const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_begins_with(a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_begins_with_string(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_begins_with_builder(const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_begins_with(a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_begins_with_builder(a_data, a_length, b);
}
#endif

//

FHSTRING_API fhstring_bool
fhstring_ntstring_begins_with_char(const char *a, char b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_begins_with_char(a, a_length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(const char *a, char b) {
    return fhstring_ntstring_begins_with_char(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_begins_with_many(const char *a, const char *b_data, fhstring_i32 b_length) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_begins_with(a, a_length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(const char *a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_ntstring_begins_with_many(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_begins_with(const char *a, const char *b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_begins_with(a, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(const char *a, const char *b) {
    return fhstring_ntstring_begins_with(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_begins_with_string(const char *a, FHString b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_begins_with(a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(const char *a, FHString b) {
    return fhstring_ntstring_begins_with_string(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_begins_with_builder(const char *a, FHString_Builder b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_begins_with(a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(const char *a, FHString_Builder b) {
    return fhstring_ntstring_begins_with_builder(a, b);
}
#endif

//

FHSTRING_API fhstring_bool
fhstring_begins_with_char(FHString a, char b) {
    return fhstring_many_begins_with_char(a.data, a.length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(FHString a, char b) {
    return fhstring_begins_with_char(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_begins_with_many(FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_begins_with(a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_begins_with_many(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_begins_with_ntstring(FHString a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_begins_with(a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(FHString a, const char *b) {
    return fhstring_begins_with_ntstring(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_begins_with(FHString a, FHString b) {
    return fhstring_many_begins_with(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(FHString a, FHString b) {
    return fhstring_begins_with(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_begins_with_builder(FHString a, FHString_Builder b) {
    return fhstring_many_begins_with(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(FHString a, FHString_Builder b) {
    return fhstring_begins_with_builder(a, b);
}
#endif

//

FHSTRING_API fhstring_bool
fhstring_builder_begins_with_char(FHString_Builder a, char b) {
    return fhstring_many_begins_with_char(a.data, a.length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(FHString_Builder a, char b) {
    return fhstring_builder_begins_with_char(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_begins_with_many(FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_begins_with(a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_builder_begins_with_many(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_begins_with_ntstring(FHString_Builder a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_begins_with(a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(FHString_Builder a, const char *b) {
    return fhstring_builder_begins_with_ntstring(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_begins_with_string(FHString_Builder a, FHString b) {
    return fhstring_many_begins_with(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(FHString_Builder a, FHString b) {
    return fhstring_builder_begins_with_string(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_begins_with(FHString_Builder a, FHString_Builder b) {
    return fhstring_many_begins_with(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_begins_with(FHString_Builder a, FHString_Builder b) {
    return fhstring_builder_begins_with(a, b);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// ENDS WITH
//
FHSTRING_API fhstring_bool
fhstring_many_ends_with_char(const char *a_data, fhstring_i32 a_length, char b) {
    if(!a_data) { return FHSTRING_FALSE; }
    if(a_length <= 0) { return FHSTRING_FALSE; }
    return (a_data[a_length - 1] == b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(const char *a_data, fhstring_i32 a_length, char b) {
    return fhstring_many_ends_with_char(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_ends_with(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length) {
    if(!a_data || !b_data) { return FHSTRING_FALSE; }
    if(a_length < b_length || b_length <= 0) { return FHSTRING_FALSE; }
    
    fhstring_i32 offset = a_length - b_length;
    for(fhstring_i32 i = 0; i < b_length; i += 1) {
        if(a_data[offset + i] != b_data[i]) { return FHSTRING_FALSE; }
    }
    return FHSTRING_TRUE;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_ends_with(a_data, a_length, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_ends_with_ntstring(const char *a_data, fhstring_i32 a_length, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_ends_with(a_data, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(const char *a_data, fhstring_i32 a_length, const char *b) {
    return fhstring_many_ends_with_ntstring(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_ends_with_string(const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_ends_with(a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_ends_with_string(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_many_ends_with_builder(const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_ends_with(a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_ends_with_builder(a_data, a_length, b);
}
#endif

//

FHSTRING_API fhstring_bool
fhstring_ntstring_ends_with_char(const char *a, char b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_ends_with_char(a, a_length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(const char *a, char b) {
    return fhstring_ntstring_ends_with_char(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_ends_with_many(const char *a, const char *b_data, fhstring_i32 b_length) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_ends_with(a, a_length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(const char *a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_ntstring_ends_with_many(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_ends_with(const char *a, const char *b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_ends_with(a, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(const char *a, const char *b) {
    return fhstring_ntstring_ends_with(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_ends_with_string(const char *a, FHString b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_ends_with(a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(const char *a, FHString b) {
    return fhstring_ntstring_ends_with_string(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ntstring_ends_with_builder(const char *a, FHString_Builder b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_ends_with(a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(const char *a, FHString_Builder b) {
    return fhstring_ntstring_ends_with_builder(a, b);
}
#endif

//

FHSTRING_API fhstring_bool
fhstring_ends_with_char(FHString a, char b) {
    return fhstring_many_ends_with_char(a.data, a.length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(FHString a, char b) {
    return fhstring_ends_with_char(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ends_with_many(FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_ends_with(a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_ends_with_many(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ends_with_ntstring(FHString a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_ends_with(a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(FHString a, const char *b) {
    return fhstring_ends_with_ntstring(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ends_with(FHString a, FHString b) {
    return fhstring_many_ends_with(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(FHString a, FHString b) {
    return fhstring_ends_with(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_ends_with_builder(FHString a, FHString_Builder b) {
    return fhstring_many_ends_with(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(FHString a, FHString_Builder b) {
    return fhstring_ends_with_builder(a, b);
}
#endif

//

FHSTRING_API fhstring_bool
fhstring_builder_ends_with_char(FHString_Builder a, char b) {
    return fhstring_many_ends_with_char(a.data, a.length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(FHString_Builder a, char b) {
    return fhstring_builder_ends_with_char(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_ends_with_many(FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_ends_with(a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_builder_ends_with_many(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_ends_with_ntstring(FHString_Builder a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_ends_with(a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(FHString_Builder a, const char *b) {
    return fhstring_builder_ends_with_ntstring(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_ends_with_string(FHString_Builder a, FHString b) {
    return fhstring_many_ends_with(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(FHString_Builder a, FHString b) {
    return fhstring_builder_ends_with_string(a, b);
}
#endif

FHSTRING_API fhstring_bool
fhstring_builder_ends_with(FHString_Builder a, FHString_Builder b) {
    return fhstring_many_ends_with(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_bool
fhstring_ends_with(FHString_Builder a, FHString_Builder b) {
    return fhstring_builder_ends_with(a, b);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INDEX OF
//
FHSTRING_API fhstring_i32
fhstring_many_index_of_char(const char *a_data, fhstring_i32 a_length, char b) {
    if(!a_data) { return FHSTRING_ERROR_ARGUMENT_A_DATA_IS_NULL; }
    if(a_length <= 0) { return FHSTRING_ERROR_ARGUMENT_A_LENGTH_IS_EMPTY; }
    
    for(fhstring_i32 i = 0; i < a_length; i += 1) {
        if(a_data[i] == b) { return i; }
    }
    return FHSTRING_ERROR_PATTERN_NOT_FOUND;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(const char *a_data, fhstring_i32 a_length, char b) {
    return fhstring_many_index_of_char(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_many_index_of(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length) {
    if(!a_data) { return FHSTRING_ERROR_ARGUMENT_A_DATA_IS_NULL; }
    if(a_length <= 0) { return FHSTRING_ERROR_ARGUMENT_A_LENGTH_IS_EMPTY; }
    if(!b_data) { return FHSTRING_ERROR_ARGUMENT_B_DATA_IS_NULL; }
    if(b_length <= 0) { return FHSTRING_ERROR_ARGUMENT_B_LENGTH_IS_EMPTY; }
    
    for(fhstring_i32 a_index = 0; a_index < a_length - b_length; a_index += 1) {
        fhstring_bool found = FHSTRING_TRUE;
        for(fhstring_i32 b_index = 0; b_index < b_length; b_index += 1) {
            if(a_data[a_index + b_index] != b_data[b_index]) {
                found = FHSTRING_FALSE;
                break;
            }
        }
        
        if(found) { return a_index; }
    }
    return FHSTRING_ERROR_PATTERN_NOT_FOUND;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_index_of(a_data, a_length, b_data, b_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_many_index_of_ntstring(const char *a_data, fhstring_i32 a_length, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_index_of(a_data, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(const char *a_data, fhstring_i32 a_length, const char *b) {
    return fhstring_many_index_of_ntstring(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_many_index_of_string(const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_index_of(a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_index_of_string(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_many_index_of_builder(const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_index_of(a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_index_of_builder(a_data, a_length, b);
}
#endif

//

FHSTRING_API fhstring_i32
fhstring_ntstring_index_of_char(const char *a, char b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_index_of_char(a, a_length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(const char *a, char b) {
    return fhstring_ntstring_index_of_char(a, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_ntstring_index_of_many(const char *a, const char *b_data, fhstring_i32 b_length) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_index_of(a, a_length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(const char *a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_ntstring_index_of_many(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_ntstring_index_of(const char *a, const char *b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_index_of(a, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(const char *a, const char *b) {
    return fhstring_ntstring_index_of(a, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_ntstring_index_of_string(const char *a, FHString b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_index_of(a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(const char *a, FHString b) {
    return fhstring_ntstring_index_of_string(a, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_ntstring_index_of_builder(const char *a, FHString_Builder b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_index_of(a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(const char *a, FHString_Builder b) {
    return fhstring_ntstring_index_of_builder(a, b);
}
#endif

//

FHSTRING_API fhstring_i32
fhstring_index_of_char(FHString a, char b) {
    return fhstring_many_index_of_char(a.data, a.length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(FHString a, char b) {
    return fhstring_index_of_char(a.data, a.length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_index_of_many(FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_index_of(a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_index_of_many(a.data, a.length, b_data, b_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_index_of_ntstring(FHString a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_index_of(a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(FHString a, const char *b) {
    return fhstring_index_of_ntstring(a.data, a.length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_index_of_string(FHString a, FHString b) {
    return fhstring_many_index_of(a.data, a.length, b.data, b.length);
}

FHSTRING_API fhstring_i32
fhstring_index_of_builder(FHString a, FHString_Builder b) {
    return fhstring_many_index_of(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(FHString a, FHString_Builder b) {
    return fhstring_index_of_builder(a.data, a.length, b);
}
#endif

//

FHSTRING_API fhstring_i32
fhstring_builder_index_of_char(FHString_Builder a, char b) {
    return fhstring_many_index_of_char(a.data, a.length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(FHString_Builder a, char b) {
    return fhstring_builder_index_of_char(a.data, a.length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_index_of_many(FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_index_of(a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_builder_index_of_many(a.data, a.length, b_data, b_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_index_of_ntstring(FHString_Builder a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_index_of(a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(FHString_Builder a, const char *b) {
    return fhstring_builder_index_of_ntstring(a.data, a.length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_index_of_string(FHString_Builder a, FHString b) {
    return fhstring_many_index_of(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(FHString_Builder a, FHString b) {
    return fhstring_builder_index_of_string(a.data, a.length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_index_of(FHString_Builder a, FHString_Builder b) {
    return fhstring_many_index_of(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_index_of(FHString_Builder a, FHString_Builder b) {
    return fhstring_builder_index_of(a.data, a.length, b);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// LAST INDEX OF
//
FHSTRING_API fhstring_i32
fhstring_many_last_index_of_char(const char *a_data, fhstring_i32 a_length, char b) {
    if(!a_data) { return FHSTRING_ERROR_ARGUMENT_A_DATA_IS_NULL; }
    if(a_length <= 0) { return FHSTRING_ERROR_ARGUMENT_A_LENGTH_IS_EMPTY; }
    
    for(fhstring_i32 i = a_length - 1; i >= 0; i -= 1) {
        if(a_data[i] == b) { return i; }
    }
    return FHSTRING_ERROR_PATTERN_NOT_FOUND;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(const char *a_data, fhstring_i32 a_length, char b) {
    return fhstring_many_last_index_of_char(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_many_last_index_of(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length) {
    if(!a_data) { return FHSTRING_ERROR_ARGUMENT_A_DATA_IS_NULL; }
    if(a_length <= 0) { return FHSTRING_ERROR_ARGUMENT_A_LENGTH_IS_EMPTY; }
    if(!b_data) { return FHSTRING_ERROR_ARGUMENT_B_DATA_IS_NULL; }
    if(b_length <= 0) { return FHSTRING_ERROR_ARGUMENT_B_LENGTH_IS_EMPTY; }
    
    for(fhstring_i32 a_index = (a_length - b_length) - 1; a_index >= 0; a_index -= 1) {
        fhstring_bool found = FHSTRING_TRUE;
        for(fhstring_i32 b_index = 0; b_index < b_length; b_index += 1) {
            if(a_data[a_index + b_index] != b_data[b_index]) {
                found = FHSTRING_FALSE;
                break;
            }
        }
        
        if(found) { return a_index; }
    }
    return FHSTRING_ERROR_PATTERN_NOT_FOUND;
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(const char *a_data, fhstring_i32 a_length, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_last_index_of(a_data, a_length, b_data, b_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_many_last_index_of_ntstring(const char *a_data, fhstring_i32 a_length, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_last_index_of(a_data, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(const char *a_data, fhstring_i32 a_length, const char *b) {
    return fhstring_many_last_index_of_ntstring(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_many_last_index_of_string(const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_last_index_of(a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(const char *a_data, fhstring_i32 a_length, FHString b) {
    return fhstring_many_last_index_of_string(a_data, a_length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_many_last_index_of_builder(const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_last_index_of(a_data, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(const char *a_data, fhstring_i32 a_length, FHString_Builder b) {
    return fhstring_many_last_index_of_builder(a_data, a_length, b);
}
#endif

//

FHSTRING_API fhstring_i32
fhstring_ntstring_last_index_of_char(const char *a, char b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_last_index_of_char(a, a_length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(const char *a, char b) {
    return fhstring_ntstring_last_index_of_char(a, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_ntstring_last_index_of_many(const char *a, const char *b_data, fhstring_i32 b_length) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_last_index_of(a, a_length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(const char *a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_ntstring_last_index_of_many(a, b_data, b_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_ntstring_last_index_of(const char *a, const char *b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_last_index_of(a, a_length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(const char *a, const char *b) {
    return fhstring_ntstring_last_index_of(a, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_ntstring_last_index_of_string(const char *a, FHString b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_last_index_of(a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(const char *a, FHString b) {
    return fhstring_ntstring_last_index_of_string(a, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_ntstring_last_index_of_builder(const char *a, FHString_Builder b) {
    fhstring_i32 a_length = fhstring_get_ntstring_length(a);
    return fhstring_many_last_index_of(a, a_length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(const char *a, FHString_Builder b) {
    return fhstring_ntstring_last_index_of_builder(a, b);
}
#endif

//

FHSTRING_API fhstring_i32
fhstring_last_index_of_char(FHString a, char b) {
    return fhstring_many_last_index_of_char(a.data, a.length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(FHString a, char b) {
    return fhstring_last_index_of_char(a.data, a.length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_last_index_of_many(FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_last_index_of(a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(FHString a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_last_index_of_many(a.data, a.length, b_data, b_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_last_index_of_ntstring(FHString a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_last_index_of(a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(FHString a, const char *b) {
    return fhstring_last_index_of_ntstring(a.data, a.length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_last_index_of_string(FHString a, FHString b) {
    return fhstring_many_last_index_of(a.data, a.length, b.data, b.length);
}

FHSTRING_API fhstring_i32
fhstring_last_index_of_builder(FHString a, FHString_Builder b) {
    return fhstring_many_last_index_of(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(FHString a, FHString_Builder b) {
    return fhstring_last_index_of_builder(a.data, a.length, b);
}
#endif

//

FHSTRING_API fhstring_i32
fhstring_builder_last_index_of_char(FHString_Builder a, char b) {
    return fhstring_many_last_index_of_char(a.data, a.length, b);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(FHString_Builder a, char b) {
    return fhstring_builder_last_index_of_char(a.data, a.length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_last_index_of_many(FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_many_last_index_of(a.data, a.length, b_data, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(FHString_Builder a, const char *b_data, fhstring_i32 b_length) {
    return fhstring_builder_last_index_of_many(a.data, a.length, b_data, b_length);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_last_index_of_ntstring(FHString_Builder a, const char *b) {
    fhstring_i32 b_length = fhstring_get_ntstring_length(b);
    return fhstring_many_last_index_of(a.data, a.length, b, b_length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(FHString_Builder a, const char *b) {
    return fhstring_builder_last_index_of_ntstring(a.data, a.length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_last_index_of_string(FHString_Builder a, FHString b) {
    return fhstring_many_last_index_of(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(FHString_Builder a, FHString b) {
    return fhstring_builder_last_index_of_string(a.data, a.length, b);
}
#endif

FHSTRING_API fhstring_i32
fhstring_builder_last_index_of(FHString_Builder a, FHString_Builder b) {
    return fhstring_many_last_index_of(a.data, a.length, b.data, b.length);
}
#if defined(__cplusplus)
FHSTRING_API fhstring_i32
fhstring_last_index_of(FHString_Builder a, FHString_Builder b) {
    return fhstring_builder_last_index_of(a.data, a.length, b);
}
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// END IMPLEMENTATION GUARD
//
#endif
