/*
** See end of file for license information.
*/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// BEGIN HEADER GUARD
//
#if !defined(FHOS_H)
#  define FHOS_H


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// NOTES
//

// NOTE(Patrik): A path_length parameter that is less than zero
// indicates that the path_data parameter is null terminated.


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// INCLUDES
//
#if !defined(FHOS_DO_NOT_INCLUDE_PLATFORM_HEADERS)
#  if defined(_WIN32) || defined(_WIN64)
#    include <windows.h>
#  else
#    error Unimplemented platform.
#  endif
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// CONFIG
//
#if !defined(FHOS_API)
#  define FHOS_API static
#endif

#if !defined(FHOS_DEFAULT_BUFFER_CAPACITY)
#  define FHOS_DEFAULT_BUFFER_CAPACITY 128
#endif

#if !defined(FHOS_DEFAULT_ALLOCATOR_CAPACITY)
#  define FHOS_DEFAULT_ALLOCATOR_CAPACITY 4096
#endif

#if !defined(FHOS_STACK_PATH_CAPACITY)
#  define FHOS_STACK_PATH_CAPACITY 512
#endif

#if !defined(FHOS_LOG_ERROR)
#  if defined(FUTHARK_LOG)
#    define FHOS_LOG_ERROR(format, ...) FUTHARK_LOG(ERROR, format, __VA_ARGS__)
#  else
#    define FHOS_LOG_ERROR(format, ...) printf(__FILE__ "(%d): [ERROR] " format, __LINE__, __VA_ARGS__)
#  endif
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// MACROS
//
#define FHOS_FIELD_ALIAS(T, ...) union { T __VA_ARGS__; }

//

#define FHOS__GET_NTSTRING_LENGTH(ntstring, length) do {\
(length) = 0;\
if((ntstring)) {\
while(*((ntstring) + (length))) { (length) = (length) + 1; }\
}\
} while(0)

//

#define FHOS__ALLOC_PATH(ctx, path_data, path_length) \
char stack_path[FHOS_STACK_PATH_CAPACITY];\
char *path = stack_path;\
do {\
if(path_length < 0) {\
path = (char *)(path_data);\
} else if(path_length >= FHOS_STACK_PATH_CAPACITY) {\
path = (char *)fhos_context_temp_alloc_non_zero((ctx), (path_length) + 1);\
}\
if(path) {\
for(fhos_i32 i = 0; i < (path_length); i += 1) { path[i] = (path_data)[i]; }\
path[(path_length)] = 0;\
}\
} while(0)

#define FHOS__FREE_PATH(ctx, path_data, path_length) do {\
if(path_length >= FHOS_STACK_PATH_CAPACITY) { fhos_context_temp_free(ctx, path); }\
} while(0)

//

#define FHOS__ALLOC_FROM_TO_PATH(ctx, from_path_data, from_path_length, to_path_data, to_path_length) \
char stack_path[FHOS_STACK_PATH_CAPACITY];\
char *base_path = 0;\
char *from_path = 0;\
char *to_path = 0;\
do {\
if((from_path_length) < 0) { from_path = (char *)(from_path_data); }\
if((to_path_length) < 0) { to_path = (char *)(to_path_data); }\
if(!from_path || !to_path) {\
if((from_path_length) < 0) { FHOS__GET_NTSTRING_LENGTH((from_path_data), (from_path_length)); }\
if((to_path_length) < 0) { FHOS__GET_NTSTRING_LENGTH((to_path_data), (to_path_length)); }\
fhos_i32 total_path_length = (from_path_length) + (to_path_length);\
if(total_path_length + 2 >= FHOS_STACK_PATH_CAPACITY) {\
base_path = (char *)fhos_context_temp_alloc_non_zero(ctx, total_path_length + 2);\
from_path = base_path;\
to_path = base_path + from_path_length + 1;\
} else {\
from_path = stack_path;\
to_path = base_path + from_path_length + 1;\
}\
for(fhos_i32 i = 0; i < (from_path_length); i += 1) { from_path[i] = (from_path_data)[i]; }\
for(fhos_i32 i = 0; i < (to_path_length); i += 1) { to_path[i] = (to_path_data)[i]; }\
}\
} while(0)

#define FHOS__FREE_FROM_TO_PATH(ctx, from_path_data, from_path_length, to_path_data, to_path_length) do {\
if(base_path) { fhos_context_temp_free(ctx, base_path); }\
} while(0)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// TYPES
//
enum {
    FHOS_FALSE = 0,
    FHOS_TRUE = 1,
};

typedef unsigned char FHOS_Allocator_Mode;
enum {
    FHOS_ALLOCATOR_MODE_ALLOC            = 0,
    FHOS_ALLOCATOR_MODE_ALLOC_NON_ZERO   = 1,
    FHOS_ALLOCATOR_MODE_REALLOC          = 2,
    FHOS_ALLOCATOR_MODE_REALLOC_NON_ZERO = 3,
    FHOS_ALLOCATOR_MODE_FREE             = 4,
    FHOS_ALLOCATOR_MODE_FREE_ALL         = 5,
};

#if !defined(FHOS_NO_STDINT)
// NOTE(Patrik): Negative values indicate an error.
// Other values depend on the function
typedef int32_t fhos_error;

// NOTE(Patrik): Only used internally
typedef ptrdiff_t fhos_isize;

typedef int8_t    fhos_bool;
typedef uint8_t   fhos_u8;
typedef int8_t    fhos_i8;
typedef int16_t   fhos_i16;
typedef int32_t   fhos_i32;
typedef int64_t   fhos_i64;
#else
typedef signed int       fhos_error;
typedef signed long long fhos_isize;
typedef signed char      fhos_bool;
typedef unsigned char    fhos_u8;
typedef signed char      fhos_i8;
typedef signed short     fhos_i16;
typedef signed int       fhos_i32;
typedef signed long long fhos_i64;
#endif

#if defined(Futhark_File_Handle)
typedef Futhark_File_Handle FHOS_File_Handle;
#elif !defined(FHOS_File_Handle)
typedef struct FHOS_File_Handle { void *data; } FHOS_File_Handle;

#  define FHOS_File_Handle(...) fhos_set_file_handle(__VA_ARGS__)
#endif

#if defined(Futhark_File_Data)
typedef Futhark_File_Data FHOS_List;
#elif !defined(FHOS_List)
typedef struct FHOS_List {
    fhos_u8 *data;
    FHOS_FIELD_ALIAS(fhos_i64, count, length);
    // NOTE(Patrik): A negative capacity should be treated as statically allocated memory
    // Note that zero does not.
    fhos_i64 capacity;
} FHOS_List;

#  define FHOS_List(...) fhos_set_list(__VA_ARGS__)
#endif


#if defined(Futhark_Allocator)
typedef Futhark_Allocator FHOS_Allocator;
#elif !defined(FHOS_Allocator)
struct FHOS_Allocator;
#define FHOS_ALLOCATOR_PROC(name) void *name(FHOS_Allocator *allocator, fhos_u8 mode, void *data, fhos_i64 size_in_bytes)
typedef void *FHOS_Allocator_Proc(struct FHOS_Allocator *allocator, fhos_u8 mode, void *data, fhos_i64 size_in_bytes);

typedef struct FHOS_Allocator {
    FHOS_Allocator_Proc *proc;
    void *data;
} FHOS_Allocator;

#  define FHOS_Allocator(...) fhos_set_allocator(__VA_ARGS__)
#endif

#if defined(Futhark_Context)
typedef Futhark_Context FHOS_Context;
#elif !defined(FHOS_Context)
typedef struct FHOS_Context {
    FHOS_Allocator *allocator;
    FHOS_Allocator *temp_allocator;
    
    void *user_data;
    fhos_i64 user_index;
} FHOS_Context;

#  define FHOS_Context(...) fhos_set_context(__VA_ARGS__)
#endif


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// MEMORY
//
FHOS_API void *fhos_allocate_memory(fhos_i64 size_in_bytes);
FHOS_API void *fhos_allocate_memory_non_zero(fhos_i64 size_in_bytes);
FHOS_API void *fhos_reallocate_memory(void *old_data, fhos_i64 size_in_bytes);
FHOS_API void *fhos_reallocate_memory_non_zero(void *old_data, fhos_i64 size_in_bytes);
FHOS_API void  fhos_free_memory(void *data);

// NOTE(Patrik): This procedure is not intened for speed or memory compactness.
// If you just want something quick and dirty, this works perfectly fine.
// But it is intended to replace it with your own procedure.
FHOS_API void *fhos_default_allocator_proc(FHOS_Allocator *allocator, fhos_u8 mode, void *data, fhos_i64 size_in_bytes);

FHOS_API void *fhos_allocator_alloc(FHOS_Allocator *allocator, fhos_i64 size_in_bytes);
FHOS_API void *fhos_allocator_alloc_non_zero(FHOS_Allocator *allocator, fhos_i64 size_in_bytes);
FHOS_API void *fhos_allocator_realloc(FHOS_Allocator *allocator, void *data, fhos_i64 size_in_bytes);
FHOS_API void *fhos_allocator_realloc_non_zero(FHOS_Allocator *allocator, void *data, fhos_i64 size_in_bytes);
FHOS_API void  fhos_allocator_free(FHOS_Allocator *allocator, void *data);
FHOS_API void  fhos_allocator_free_all(FHOS_Allocator *allocator);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// CONTEXT
//
FHOS_API void *fhos_context_alloc_proc(FHOS_Context *ctx, fhos_u8 mode, void *data, fhos_i64 size_in_bytes);
FHOS_API void *fhos_context_temp_alloc_proc(FHOS_Context *ctx, fhos_u8 mode, void *data, fhos_i64 size_in_bytes);

FHOS_API void *fhos_context_alloc(FHOS_Context *ctx, fhos_i64 size_in_bytes);
FHOS_API void *fhos_context_alloc_non_zero(FHOS_Context *ctx, fhos_i64 size_in_bytes);
FHOS_API void *fhos_context_realloc(FHOS_Context *ctx, void *data, fhos_i64 size_in_bytes);
FHOS_API void *fhos_context_realloc_non_zero(FHOS_Context *ctx, void *data, fhos_i64 size_in_bytes);
FHOS_API void  fhos_context_free(FHOS_Context *ctx, void *data);
FHOS_API void  fhos_context_free_all(FHOS_Context *ctx);

FHOS_API void *fhos_context_temp_alloc(FHOS_Context *ctx, fhos_i64 size_in_bytes);
FHOS_API void *fhos_context_temp_alloc_non_zero(FHOS_Context *ctx, fhos_i64 size_in_bytes);
FHOS_API void *fhos_context_temp_realloc(FHOS_Context *ctx, void *data, fhos_i64 size_in_bytes);
FHOS_API void *fhos_context_temp_realloc_non_zero(FHOS_Context *ctx, void *data, fhos_i64 size_in_bytes);
FHOS_API void  fhos_context_temp_free(FHOS_Context *ctx, void *data);
FHOS_API void  fhos_context_temp_free_all(FHOS_Context *ctx);

FHOS_API void *fhos_context_maybe_grow(FHOS_Context *ctx, void *data, fhos_i64 *capacity, fhos_i64 new_capacity);
FHOS_API void *fhos_context_temp_maybe_grow(FHOS_Context *ctx, void *data, fhos_i64 *capacity, fhos_i64 new_capacity);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// IO
//
FHOS_API FHOS_File_Handle fhos_get_invalid_file_handle(void);
FHOS_API fhos_bool fhos_is_file_handle_valid(FHOS_File_Handle handle);

FHOS_API fhos_bool fhos_close_file(FHOS_File_Handle handle);

FHOS_API FHOS_File_Handle fhos_open_file_for_reading(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length);
FHOS_API FHOS_File_Handle fhos_open_file_for_writing(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length);

// NOTE(Patrik): A negative return value indicates an error.
FHOS_API fhos_i64 fhos_get_size_of_file(FHOS_File_Handle handle);

// NOTE(Patrik): A negative return value indicates an error.
// Otherwise the return value is how many bytes were read/written.
FHOS_API fhos_i64 fhos_read_file(FHOS_Context *ctx, FHOS_File_Handle handle, fhos_u8 *data, fhos_i64 read_amount);
FHOS_API fhos_i64 fhos_write_file(FHOS_File_Handle handle, const fhos_u8 *data, fhos_i64 write_amount);

FHOS_API FHOS_List fhos_read_entire_file(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length, fhos_bool use_temp_allocator);
FHOS_API fhos_bool fhos_write_entire_file(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length, const fhos_u8 *data, fhos_i64 write_amount);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// PATH
//
FHOS_API fhos_bool  fhos_file_exists(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length);
FHOS_API fhos_bool  fhos_remove_file(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length);
FHOS_API fhos_error fhos_move_file(FHOS_Context *ctx, const char *from_path_data, fhos_i32 from_path_length, const char *to_path_data, fhos_i32 to_path_length);
FHOS_API fhos_error fhos_copy_file(FHOS_Context *ctx, const char *from_path_data, fhos_i32 from_path_length, const char *to_path_data, fhos_i32 to_path_length);

FHOS_API fhos_error fhos_is_file_newer(FHOS_Context *ctx, const char *this_path_data, fhos_i32 this_path_length, const char *other_path_data, fhos_i32 other_path_length);

FHOS_API fhos_bool fhos_directory_exists(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length);
FHOS_API fhos_i32  fhos_count_directory_files(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length);

// NOTE(Patrik): If no error, the return value should be treated as a bool.
// Returns false if the directory already exists.
FHOS_API fhos_error fhos_create_directory_if_new(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length);

// NOTE(Patrik): If no error, the return value should be treated as a bool.
FHOS_API fhos_error fhos_remove_directory_recursively(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// ERROR CODES
//
enum {
    // NOTE(Patrik): Generic or unkown error.
    // Ideally none of the errors should have this value.
    FHOS_ERROR_GENERIC_ERROR = -1,
    
    FHOS_ERROR_PATH_IS_NULL = -2,
    FHOS_ERROR_PATH_IS_EMPTY = -3,
    FHOS_ERROR_OUT_OF_MEMORY = -4,
    FHOS_ERROR_INVALID_FILE_HANDLE = -5,
    FHOS_ERROR_BUFFER_IS_NULL = -6,
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
#if defined(FHOS_IMPLEMENTATION) && !defined(FHOS_IMPLEMENTATION_DONE)
#  define FHOS_IMPLEMENTATION_DONE


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// MEMORY
//
FHOS_API void *
fhos_allocate_memory(fhos_i64 size_in_bytes) {
    if(size_in_bytes <= 0) { return 0; }
    void *result = 0;
#if defined(_WIN32) || defined(_WIN64)
    result = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size_in_bytes);
#else
#  error Unimplemented on this platform.
#endif
    return result;
}

FHOS_API void *
fhos_allocate_memory_non_zero(fhos_i64 size_in_bytes) {
    if(size_in_bytes <= 0) { return 0; }
#if defined(_WIN32) || defined(_WIN64)
    void *result = HeapAlloc(GetProcessHeap(), 0, size_in_bytes);
#else
#  error Unimplemented on this platform.
#endif
    return result;
}

FHOS_API void *
fhos_reallocate_memory(void *old_data, fhos_i64 new_size_in_bytes) {
    if(new_size_in_bytes <= 0) { return 0; }
    void *result = 0;
#if defined(_WIN32) || defined(_WIN64)
    if(old_data) {
        result = HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, old_data, new_size_in_bytes);
    } else {
        result = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, new_size_in_bytes);
    }
#else
#  error Unimplemented on this platform.
#endif
    return result;
}

FHOS_API void *
fhos_reallocate_memory_non_zero(void *old_data, fhos_i64 new_size_in_bytes) {
    if(new_size_in_bytes <= 0) { return 0; }
    void *result = 0;
#if defined(_WIN32) || defined(_WIN64)
    if(old_data) {
        result = HeapReAlloc(GetProcessHeap(), 0, old_data, new_size_in_bytes);
    } else {
        result = HeapAlloc(GetProcessHeap(), 0, new_size_in_bytes);
    }
#else
#  error Unimplemented on this platform.
#endif
    return result;
}

FHOS_API void
fhos_free_memory(void *data) {
    if(!data) { return; }
#if defined(_WIN32) || defined(_WIN64)
    HeapFree(GetProcessHeap(), 0, data);
#else
#  error Unimplemented on this platform.
#endif
}

//

FHOS_API void *
fhos_default_allocator_proc(FHOS_Allocator *allocator, fhos_u8 mode, void *data, fhos_i64 size_in_bytes) {
    if(!allocator) {
        switch(mode) {
            default: {
                // TODO(Patrik): ERROR
            } break;
            
            case FHOS_ALLOCATOR_MODE_ALLOC: return fhos_allocate_memory(size_in_bytes);
            case FHOS_ALLOCATOR_MODE_ALLOC_NON_ZERO: return fhos_allocate_memory_non_zero(size_in_bytes);
            case FHOS_ALLOCATOR_MODE_REALLOC: return fhos_reallocate_memory(data, size_in_bytes);
            case FHOS_ALLOCATOR_MODE_REALLOC_NON_ZERO: return fhos_reallocate_memory_non_zero(data, size_in_bytes);
            
            case FHOS_ALLOCATOR_MODE_FREE: { fhos_free_memory(data); } break;
            case FHOS_ALLOCATOR_MODE_FREE_ALL: break;
        }
        return 0;
    }
    
    if(mode == FHOS_ALLOCATOR_MODE_FREE) {
        if(!data) {
            FHOS_LOG_ERROR("Trying to free a null pointer.\n");
            return 0;
        }
        if(!allocator->data) {
            FHOS_LOG_ERROR("Trying to free before anything has been allocated.\n");
            return 0;
        }
        
        fhos_isize *count = ((fhos_isize *)allocator->data) + 1;
        void **pointers = ((void **)allocator->data) + 2;
        for(fhos_isize i = 0; i < *count; i += 1) {
            if(pointers[i] == data) {
                if(i < *count - 1) { pointers[i] = pointers[*count - 1]; }
                fhos_free_memory(data);
                *count -= 1;
                return 0;
            }
        }
        
        FHOS_LOG_ERROR("Trying to free a pointer that wasn't allocated by the allocator!\n");
        return 0;
    } else if(mode == FHOS_ALLOCATOR_MODE_FREE_ALL) {
        if(!allocator->data) {
            FHOS_LOG_ERROR("Trying to free everything before anything has been allocated.\n");
            return 0;
        }
        
        fhos_isize *count = ((fhos_isize *)allocator->data) + 1;
        void **pointers = ((void **)allocator->data) + 2;
        for(fhos_isize i = 0; i < *count; i += 1) { fhos_free_memory(pointers[i]); }
        fhos_free_memory(allocator->data);
        return 0;
    }
    
    if(!allocator->data) {
        fhos_isize capacity = 64;
        allocator->data = fhos_allocate_memory((capacity + 2) * sizeof(void *));
        ((fhos_isize *)allocator->data)[0] = capacity;
        ((fhos_isize *)allocator->data)[1] = 0;
    }
    
    fhos_isize *capacity = ((fhos_isize *)allocator->data) + 0;
    fhos_isize *count = ((fhos_isize *)allocator->data) + 1;
    void **pointers = ((void **)allocator->data) + 2;
    
    if(size_in_bytes <= 0) {
        FHOS_LOG_ERROR("Trying to allocate zero bytes or less!\n");
        return 0;
    }
    
    if(!data && (mode == FHOS_ALLOCATOR_MODE_REALLOC || mode == FHOS_ALLOCATOR_MODE_REALLOC_NON_ZERO)) {
        if(mode == FHOS_ALLOCATOR_MODE_REALLOC) {
            mode = FHOS_ALLOCATOR_MODE_ALLOC;
        } else {
            mode = FHOS_ALLOCATOR_MODE_ALLOC_NON_ZERO;
        }
    }
    
    if(mode == FHOS_ALLOCATOR_MODE_ALLOC || mode == FHOS_ALLOCATOR_MODE_ALLOC_NON_ZERO) {
        if(*count + 1 >= *capacity) {
            *capacity *= 2;
            void *new_allocator_data = fhos_reallocate_memory(allocator->data, (*capacity + 2) * sizeof(void *));
            if(!new_allocator_data) {
                FHOS_LOG_ERROR("Could not allocate more memory!\n");
                return 0;
            }
            allocator->data = new_allocator_data;
        }
        
        void *new_data = 0;
        if(mode == FHOS_ALLOCATOR_MODE_ALLOC) {
            new_data = fhos_allocate_memory(size_in_bytes);
        } else {
            new_data = fhos_allocate_memory_non_zero(size_in_bytes);
        }
        // TODO(Patrik): Handle zero case
        pointers[*count] = new_data;
        *count += 1;
        return new_data;
    }
    
    if(mode == FHOS_ALLOCATOR_MODE_REALLOC || FHOS_ALLOCATOR_MODE_REALLOC_NON_ZERO) {
        for(fhos_isize i = 0; i < *count; i += 1) {
            if(pointers[i] == data) {
                void *new_data = 0;
                if(mode == FHOS_ALLOCATOR_MODE_REALLOC) {
                    new_data = fhos_reallocate_memory(data, size_in_bytes);
                } else {
                    new_data = fhos_reallocate_memory_non_zero(data, size_in_bytes);
                }
                // TODO(Patrik): Handle zero case
                pointers[i] = new_data;
                return new_data;
            }
        }
        
        FHOS_LOG_ERROR("Trying to reallocate data that was not allocated by this allocator!\n");
        return 0;
    }
    
    FHOS_LOG_ERROR("Unkown allocator mode: %d!\n", mode);
    return 0;
}

//

FHOS_API void *
fhos_allocator_alloc(FHOS_Allocator *allocator, fhos_i64 size_in_bytes) {
    FHOS_Allocator_Proc *proc = fhos_default_allocator_proc;
    if(allocator && allocator->proc) { proc = allocator->proc; }
    void *result = proc(allocator, FHOS_ALLOCATOR_MODE_ALLOC, 0, size_in_bytes);
    return result;
}

FHOS_API void *
fhos_allocator_alloc_non_zero(FHOS_Allocator *allocator, fhos_i64 size_in_bytes) {
    FHOS_Allocator_Proc *proc = fhos_default_allocator_proc;
    if(allocator && allocator->proc) { proc = allocator->proc; }
    void *result = proc(allocator, FHOS_ALLOCATOR_MODE_ALLOC_NON_ZERO, 0, size_in_bytes);
    return result;
}

FHOS_API void *
fhos_allocator_realloc(FHOS_Allocator *allocator, void *data, fhos_i64 size_in_bytes) {
    FHOS_Allocator_Proc *proc = fhos_default_allocator_proc;
    if(allocator && allocator->proc) { proc = allocator->proc; }
    void *result = proc(allocator, FHOS_ALLOCATOR_MODE_REALLOC, data, size_in_bytes);
    return result;
}

FHOS_API void *
fhos_allocator_realloc_non_zero(FHOS_Allocator *allocator, void *data, fhos_i64 size_in_bytes) {
    FHOS_Allocator_Proc *proc = fhos_default_allocator_proc;
    if(allocator && allocator->proc) { proc = allocator->proc; }
    void *result = proc(allocator, FHOS_ALLOCATOR_MODE_REALLOC_NON_ZERO, data, size_in_bytes);
    return result;
}

FHOS_API void
fhos_allocator_free(FHOS_Allocator *allocator, void *data) {
    FHOS_Allocator_Proc *proc = fhos_default_allocator_proc;
    if(allocator && allocator->proc) { proc = allocator->proc; }
    proc(allocator, FHOS_ALLOCATOR_MODE_FREE, data, 0);
}

FHOS_API void
fhos_allocator_free_all(FHOS_Allocator *allocator) {
    FHOS_Allocator_Proc *proc = fhos_default_allocator_proc;
    if(allocator && allocator->proc) { proc = allocator->proc; }
    proc(allocator, FHOS_ALLOCATOR_MODE_FREE_ALL, 0, 0);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// CONTEXT
//
FHOS_API void *
fhos_context_alloc_proc(FHOS_Context *ctx, fhos_u8 mode, void *data, fhos_i64 size_in_bytes)  {
    FHOS_Allocator_Proc *proc = fhos_default_allocator_proc;
    FHOS_Allocator *allocator = 0;
    if(ctx && ctx->allocator) {
        allocator = ctx->allocator;
        if(allocator->proc) { proc = allocator->proc; }
    }
    void *result = proc(allocator, mode, data, size_in_bytes);
    return result;
}

FHOS_API void *
fhos_context_temp_alloc_proc(FHOS_Context *ctx, fhos_u8 mode, void *data, fhos_i64 size_in_bytes)  {
    FHOS_Allocator_Proc *proc = fhos_default_allocator_proc;
    FHOS_Allocator *allocator = 0;
    if(ctx && ctx->temp_allocator) {
        allocator = ctx->temp_allocator;
        if(allocator->proc) { proc = allocator->proc; }
    }
    void *result = proc(allocator, mode, data, size_in_bytes);
    return result;
}

//

FHOS_API void *
fhos_context_alloc(FHOS_Context *ctx, fhos_i64 size_in_bytes)  {
    return fhos_context_alloc_proc(ctx, FHOS_ALLOCATOR_MODE_ALLOC, 0, size_in_bytes);
}

FHOS_API void *
fhos_context_alloc_non_zero(FHOS_Context *ctx, fhos_i64 size_in_bytes) {
    return fhos_context_alloc_proc(ctx, FHOS_ALLOCATOR_MODE_ALLOC_NON_ZERO, 0, size_in_bytes);
}

FHOS_API void *
fhos_context_realloc(FHOS_Context *ctx, void *data, fhos_i64 size_in_bytes) {
    return fhos_context_alloc_proc(ctx, FHOS_ALLOCATOR_MODE_REALLOC, data, size_in_bytes);
}

FHOS_API void *
fhos_context_realloc_non_zero(FHOS_Context *ctx, void *data, fhos_i64 size_in_bytes) {
    return fhos_context_alloc_proc(ctx, FHOS_ALLOCATOR_MODE_REALLOC_NON_ZERO, data, size_in_bytes);
}

FHOS_API void
fhos_context_free(FHOS_Context *ctx, void *data) {
    fhos_context_alloc_proc(ctx, FHOS_ALLOCATOR_MODE_FREE, data, 0);
}

FHOS_API void
fhos_context_free_all(FHOS_Context *ctx) {
    fhos_context_alloc_proc(ctx, FHOS_ALLOCATOR_MODE_FREE_ALL, 0, 0);
}

//

FHOS_API void *
fhos_context_temp_alloc(FHOS_Context *ctx, fhos_i64 size_in_bytes)  {
    return fhos_context_temp_alloc_proc(ctx, FHOS_ALLOCATOR_MODE_ALLOC, 0, size_in_bytes);
}

FHOS_API void *
fhos_context_temp_alloc_non_zero(FHOS_Context *ctx, fhos_i64 size_in_bytes) {
    return fhos_context_temp_alloc_proc(ctx, FHOS_ALLOCATOR_MODE_ALLOC_NON_ZERO, 0, size_in_bytes);
}

FHOS_API void *
fhos_context_temp_realloc(FHOS_Context *ctx, void *data, fhos_i64 size_in_bytes) {
    return fhos_context_temp_alloc_proc(ctx, FHOS_ALLOCATOR_MODE_REALLOC, data, size_in_bytes);
}

FHOS_API void *
fhos_context_temp_realloc_non_zero(FHOS_Context *ctx, void *data, fhos_i64 size_in_bytes) {
    return fhos_context_temp_alloc_proc(ctx, FHOS_ALLOCATOR_MODE_REALLOC_NON_ZERO, data, size_in_bytes);
}

FHOS_API void
fhos_context_temp_free(FHOS_Context *ctx, void *data) {
    fhos_context_temp_alloc_proc(ctx, FHOS_ALLOCATOR_MODE_FREE, data, 0);
}

FHOS_API void
fhos_context_temp_free_all(FHOS_Context *ctx) {
    fhos_context_temp_alloc_proc(ctx, FHOS_ALLOCATOR_MODE_FREE_ALL, 0, 0);
}

//

FHOS_API void *
fhos_context_maybe_grow(FHOS_Context *ctx, void *data, fhos_i64 *capacity, fhos_i64 new_capacity) {
    if(!capacity) { return 0; }
    if(*capacity < new_capacity) { return data; }
    
    if(!data || *capacity <= 0) {
        *capacity = 16;
        while(*capacity < new_capacity) { *capacity *= 2; }
        return fhos_context_alloc(ctx, *capacity);
    }
    
    while(*capacity < new_capacity) { *capacity *= 2; }
    return fhos_context_realloc(ctx, data, *capacity);
}

FHOS_API void *
fhos_context_temp_maybe_grow(FHOS_Context *ctx, void *data, fhos_i64 *capacity, fhos_i64 new_capacity) {
    if(!capacity) { return 0; }
    if(*capacity < new_capacity) { return data; }
    
    if(!data || *capacity <= 0) {
        *capacity = 16;
        while(*capacity < new_capacity) { *capacity *= 2; }
        return fhos_context_temp_alloc(ctx, *capacity);
    }
    
    while(*capacity < new_capacity) { *capacity *= 2; }
    return fhos_context_temp_realloc(ctx, data, *capacity);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// IO
//
FHOS_API FHOS_File_Handle
fhos_get_invalid_file_handle(void) {
#if defined(_WIN32) || defined(_WIN64)
    FHOS_File_Handle result = { (void *)(fhos_isize)INVALID_HANDLE_VALUE };
#else
#  error Unimplemented on this platform.
#endif
    return result;
}

FHOS_API fhos_bool
fhos_is_file_handle_valid(FHOS_File_Handle handle) {
#if defined(_WIN32) || defined(_WIN64)
    return ((HANDLE)handle.data != INVALID_HANDLE_VALUE);
#else
#  error Unimplemented on this platform.
#endif
}

FHOS_API fhos_bool
fhos_close_file(FHOS_File_Handle handle) {
    if(!fhos_is_file_handle_valid(handle)) { return FHOS_FALSE; }
    
#if defined(_WIN32) || defined(_WIN64)
    if(!CloseHandle((HANDLE)handle.data)) {
        FHOS_LOG_ERROR("Could not close file handle (%lu)\n", GetLastError());
        return FHOS_FALSE;
    }
    return FHOS_TRUE;
#else
#  error Unimplemented on this platform.
#endif
}

FHOS_API FHOS_File_Handle
fhos_open_file_for_reading(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length) {
    FHOS_File_Handle result = fhos_get_invalid_file_handle();
    
    if(!path_data) {
        FHOS_LOG_ERROR("Trying to read from a file with a null path.\n");
        return result;
    }
    
    if(path_length == 0) {
        FHOS_LOG_ERROR("Trying to read from a file with an empty path.\n");
        return result;
    }
    
    FHOS__ALLOC_PATH(ctx, path_data, path_length);
    if(!path) {
        FHOS_LOG_ERROR("Could not allocate memory for the path.\n");
        return result;
    }
    
#if defined(_WIN32) || defined(_WIN64)
    result.data = (void *)CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,
                                      FILE_ATTRIBUTE_NORMAL, 0);
#else
#  error Unimplemented on this platform.
#endif
    
    FHOS__FREE_PATH(ctx, path_data, path_length);
    
    return result;
}

FHOS_API FHOS_File_Handle
fhos_open_file_for_writing(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length) {
    FHOS_File_Handle result = fhos_get_invalid_file_handle();
    
    if(!path_data) {
        FHOS_LOG_ERROR("Trying to write from a file with a null path\n");
        return result;
    }
    
    if(path_length == 0) {
        FHOS_LOG_ERROR("Trying to write from a file with an empty path\n");
        return result;
    }
    
    FHOS__ALLOC_PATH(ctx, path_data, path_length);
    if(!path) {
        FHOS_LOG_ERROR("Could not allocate memory for the path.\n");
        return result;
    }
    
#if defined(_WIN32) || defined(_WIN64)
    // TODO(Patrik): Flags for file access?
    result.data = (void *)CreateFileA(path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
                                      FILE_ATTRIBUTE_NORMAL, 0);
#else
#  error Unimplemented on this platform.
#endif
    
    FHOS__FREE_PATH(ctx, path_data, path_length);
    
    return result;
}

FHOS_API fhos_i64
fhos_get_size_of_file(FHOS_File_Handle handle) {
    if(!fhos_is_file_handle_valid(handle)) { return FHOS_ERROR_INVALID_FILE_HANDLE; }
    
#if defined(_WIN32) || defined(_WIN64)
    LARGE_INTEGER file_size;
    if(!GetFileSizeEx((HANDLE)handle.data, &file_size)) {
        FHOS_LOG_ERROR("Could not get file size. (%lu)\n", GetLastError());
        return -1;
    }
    return file_size.QuadPart;
#else
#  error Unimplemented on this platform.
#endif
}

FHOS_API fhos_i64
fhos_read_file(FHOS_Context *ctx, FHOS_File_Handle handle, fhos_u8 *data, fhos_i64 read_amount) {
    if(fhos_is_file_handle_valid(handle)) {
        FHOS_LOG_ERROR("Trying to read from an invalid file handle.\n");
        return FHOS_ERROR_INVALID_FILE_HANDLE;
    }
    
    if(!data) {
        FHOS_LOG_ERROR("Trying to fill a null buffer.\n");
        return FHOS_ERROR_BUFFER_IS_NULL;
    }
    
    if(read_amount < 0) {
        FHOS_LOG_ERROR("Trying to read negative amount of bytes.\n");
        return -1;
    }
    
    if(read_amount == 0) { return 0; }
    
    fhos_i64 result = 0;
#if defined(_WIN32) || defined(_WIN64)
    DWORD bytes_read = 0;
    
    if(read_amount > I32_MAX) {
        if(!ReadFile((HANDLE)handle.data, data, I32_MAX, &bytes_read, 0)) {
            FHOS_LOG_ERROR("Could not read file. (%lu)\n", GetLastError());
            return -1;
        }
        
        data += bytes_read;
        result += bytes_read;
        read_amount -= bytes_read;
        bytes_read = 0;
    }
    
    if(!ReadFile((HANDLE)handle.data, data, (DWORD)read_amount, &bytes_read, 0)) {
        FHOS_LOG_ERROR("Could not read file. (%lu)\n", GetLastError());
        return -1;
    }
    
    result += bytes_read;
#else
#  error Unimplemented on this platform.
#endif
    
    return result;
}

FHOS_API fhos_i64
fhos_write_file(FHOS_File_Handle handle, const fhos_u8 *data, fhos_i64 write_amount) {
    if(fhos_is_file_handle_valid(handle)) {
        FHOS_LOG_ERROR("Trying to write to an invalid file handle.\n");
        return -1;
    }
    
    if(!data) {
        FHOS_LOG_ERROR("Trying to read from a null buffer.\n");
        return -1;
    }
    
    if(write_amount < 0) {
        FHOS_LOG_ERROR("Trying to write negative amount of bytes.\n");
        return -1;
    }
    
    if(write_amount == 0) { return 0; }
    
    fhos_i64 result = 0;
#if defined(_WIN32) || defined(_WIN64)
    DWORD bytes_written = 0;
    
    if(write_amount > I32_MAX) {
        if(!WriteFile((HANDLE)handle.data, data, I32_MAX, &bytes_written, 0)) {
            FHOS_LOG_ERROR("Could not to write file. (%lu)\n", GetLastError());
            return -1;
        }
        
        data += bytes_written;
        result += bytes_written;
        write_amount -= bytes_written;
        bytes_written = 0;
    }
    
    if(!WriteFile((HANDLE)handle.data, data, (DWORD)write_amount, &bytes_written, 0)) {
        FHOS_LOG_ERROR("Could not to write file. (%lu)\n", GetLastError());
        return -1;
    }
#else
#  error Unimplemented on this platform.
#endif
    
    return result;
}

FHOS_API FHOS_List
fhos_read_entire_file(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length, fhos_bool use_temp_allocator) {
    FHOS_List result = {0};
    
    FHOS_File_Handle handle = fhos_open_file_for_reading(ctx, path_data, path_length);
    if(!fhos_is_file_handle_valid(handle)) { return result; }
    
    result.capacity = fhos_get_size_of_file(handle);
    
    if(use_temp_allocator) {
        result.data = fhos_context_alloc_non_zero(ctx, result.capacity);
    } else {
        result.data = fhos_context_temp_alloc_non_zero(ctx, result.capacity);
    }
    
    result.count = fhos_read_file(ctx, handle, result.data, result.capacity);
    fhos_close_file(handle);
    
    return result;
}

FHOS_API fhos_bool
fhos_write_entire_file(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length,
                       const fhos_u8 *data, fhos_i64 write_amount)
{
    if(write_amount <= 0) { return FHOS_FALSE; }
    
    FHOS_File_Handle handle = fhos_open_file_for_writing(ctx, path_data, path_length);
    if(!fhos_is_file_handle_valid(handle)) { return FHOS_FALSE; }
    
    fhos_i64 bytes_written = fhos_write_file(handle, data, write_amount);
    fhos_close_file(handle);
    
    return bytes_written == write_amount;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// PATH
//
FHOS_API fhos_bool
fhos_file_exists(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length) {
    fhos_bool result = FHOS_FALSE;
    FHOS__ALLOC_PATH(ctx, path_data, path_length);
    if(!path) {
        FHOS_LOG_ERROR("Could not allocate memory for the path.\n");
        return result;
    }
    
#if defined(_WIN32) || defined(_WIN64)
    DWORD attribute = GetFileAttributesA(path);
    if(attribute == INVALID_FILE_ATTRIBUTES) {
        DWORD last_error = GetLastError();
        if(last_error != ERROR_FILE_NOT_FOUND && last_error != ERROR_PATH_NOT_FOUND) {
            FHOS_LOG_ERROR("Could not check if file \"%s\" exists. (%lu)\n", path, last_error);
        }
    }
    
    result = (attribute != INVALID_FILE_ATTRIBUTES);
#else
#  error Unimplemented on this platform.
#endif
    
    FHOS__FREE_PATH(ctx, path_data, path_length);
    return result;
}

FHOS_API fhos_bool
fhos_remove_file(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length) {
    fhos_bool result = FHOS_FALSE;
    FHOS__ALLOC_PATH(ctx, path_data, path_length);
    if(!path) {
        FHOS_LOG_ERROR("Could not allocate memory for the path.\n");
        return result;
    }
    
#if defined(_WIN32) || defined(_WIN64)
    BOOL success = DeleteFile(path);
    if(!success) {
        DWORD last_error = GetLastError();
        if(last_error != ERROR_FILE_NOT_FOUND) {
            FUTHARK_LOG(ERROR, "Could not delete the file \"%s\". (%lu)\n", path, last_error);
        }
    }
    
    result = (success != 0);
#else
#  error Unimplemented on this platform.
#endif
    
    FHOS__FREE_PATH(ctx, path_data, path_length);
    return result;
}


FHOS_API fhos_error
fhos_move_file(FHOS_Context *ctx, const char *from_path_data, fhos_i32 from_path_length,
               const char *to_path_data, fhos_i32 to_path_length)
{
    if(!from_path_data) {
        FHOS_LOG_ERROR("fhos_move_file -> The parameter from_path_data is null.\n");
        return FHOS_ERROR_PATH_IS_NULL;
    } else if(from_path_length == 0) {
        FHOS_LOG_ERROR("fhos_move_file -> The parameter from_path_length is zero.\n");
        return FHOS_ERROR_PATH_IS_EMPTY;
    }
    
    if(!to_path_data) {
        FHOS_LOG_ERROR("fhos_move_file -> The parameter to_path_data is null.\n");
        return FHOS_ERROR_PATH_IS_NULL;
    } else if(to_path_length == 0) {
        FHOS_LOG_ERROR("fhos_move_file -> The parameter to_path_length is zero.\n");
        return FHOS_ERROR_PATH_IS_EMPTY;
    }
    
    FHOS__ALLOC_FROM_TO_PATH(ctx, from_path_data, from_path_length, to_path_data, to_path_length);
    if(!from_path) {
        FHOS_LOG_ERROR("Could not allocate memory for the path.\n");
        return FHOS_ERROR_OUT_OF_MEMORY;
    }
    
    fhos_error result = FHOS_FALSE;
    
#if defined(_WIN32) || defined(_WIN64)
    DWORD move_flags = (MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING);
    if(MoveFileExA(from_path, to_path, move_flags)) {
        result = FHOS_TRUE;
    } else {
        DWORD last_error = GetLastError();
        FHOS_LOG_ERROR("(%lu) Could not move \"%s\" to \"%s\".\n", last_error, from_path, to_path);
        result = -1;
    }
#else
#  error Unimplemented on this platform.
#endif
    
    FHOS__FREE_FROM_TO_PATH(ctx, from_path_data, from_path_length, to_path_data, to_path_length);
    return result;
}

FHOS_API fhos_error
fhos_copy_file(FHOS_Context *ctx, const char *from_path_data, fhos_i32 from_path_length,
               const char *to_path_data, fhos_i32 to_path_length)
{
    if(!from_path_data) {
        FHOS_LOG_ERROR("fhos_move_file -> The parameter from_path_data is null.\n");
        return -1;
    } else if(from_path_length == 0) {
        FHOS_LOG_ERROR("fhos_move_file -> The parameter from_path_length is zero.\n");
        return -1;
    }
    
    if(!to_path_data) {
        FHOS_LOG_ERROR("fhos_move_file -> The parameter to_path_data is null.\n");
        return -1;
    } else if(to_path_length == 0) {
        FHOS_LOG_ERROR("fhos_move_file -> The parameter to_path_length is zero.\n");
        return -1;
    }
    
    FHOS__ALLOC_FROM_TO_PATH(ctx, from_path_data, from_path_length, to_path_data, to_path_length);
    if(!from_path) {
        FHOS_LOG_ERROR("Could not allocate memory for the path.\n");
        return -1;
    }
    
    fhos_error result = FHOS_FALSE;
    
#if defined(_WIN32) || defined(_WIN64)
    if(CopyFileA(from_path, to_path, 0)) {
        result = FHOS_TRUE;
    } else {
        DWORD last_error = GetLastError();
        FHOS_LOG_ERROR("(%lu) Could not move \"%s\" to \"%s\".\n", last_error, from_path, to_path);
        result = -1;
    }
#else
#  error Unimplemented on this platform.
#endif
    
    FHOS__FREE_FROM_TO_PATH(ctx, from_path_data, from_path_length, to_path_data, to_path_length);
    return result;
}

FHOS_API fhos_error
fhos_is_file_newer(FHOS_Context *ctx, const char *this_path_data, fhos_i32 this_path_length,
                   const char *other_path_data, fhos_i32 other_path_length)
{
    if(!this_path_data) {
        FHOS_LOG_ERROR("fhos_is_file_newer -> The parameter this_path_data is null.\n");
        return -1;
    } else if(this_path_length == 0) {
        FHOS_LOG_ERROR("fhos_is_file_newer -> The parameter this_path_length is zero.\n");
        return -1;
    }
    
    if(!other_path_data) {
        FHOS_LOG_ERROR("fhos_is_file_newer -> The parameter other_path_data is null.\n");
        return -1;
    } else if(other_path_length == 0) {
        FHOS_LOG_ERROR("fhos_is_file_newer -> The parameter other_path_length is zero.\n");
        return -1;
    }
    
    FHOS__ALLOC_FROM_TO_PATH(ctx, this_path_data, this_path_length, other_path_data, other_path_length);
    if(!this_path) {
        FHOS_LOG_ERROR("Could not allocate memory for the path.\n");
        return -1;
    }
}

//

FHOS_API fhos_bool
fhos_directory_exists(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length) {
    fhos_bool result = FHOS_FALSE;
    FHOS__ALLOC_PATH(ctx, path_data, path_length);
    if(!path) {
        FHOS_LOG_ERROR("Could not allocate memory for the path.\n");
        return result;
    }
    
#if defined(_WIN32) || defined(_WIN64)
    WIN32_FILE_ATTRIBUTE_DATA attribute = {0};
    if(GetFileAttributesExA(path, GetFileExInfoStandard, &attribute)) {
        if((attribute.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
            result = FHOS_TRUE;
        }
    }
    
#else
#  error Unimplemented on this platform.
#endif
    
    FHOS__FREE_PATH(ctx, path_data, path_length);
    return result;
}

FHOS_API fhos_i32
fhos_count_directory_files(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length) {
    if(!path_data) {
        FHOS_LOG_ERROR("Cannot remove a directory with a null path.\n");
        return -1;
    }
    
    fhos_i32 result = 0;
#if defined(_WIN32) || defined(_WIN64)
    
#else
#  error Unimplemented on this platform.
#endif
    
    return result;
}

FHOS_API fhos_error
fhos_create_directory_if_new(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length) {
    fhos_error result = 0;
    FHOS__ALLOC_PATH(ctx, path_data, path_length);
    if(!path) {
        FHOS_LOG_ERROR("Could not allocate memory for the path.\n");
        return -1;
    }
    
#if defined(_WIN32) || defined(_WIN64)
    result = (fhos_error)CreateDirectoryA(path, 0);
    if(!result) {
        DWORD last_error = GetLastError();
        if(last_error != ERROR_ALREADY_EXISTS) {
            FHOS_LOG_ERROR("(%lu) Could not create directory %s\n", last_error, path);
            result = -((fhos_error)last_error);
        }
    }
#else
#  error Unimplemented on this platform.
#endif
    
    FHOS__FREE_PATH(ctx, path_data, path_length);
    return result;
}

FHOS_API fhos_error
fhos_remove_directory_recursively(FHOS_Context *ctx, const char *path_data, fhos_i32 path_length) {
    if(!path_data) {
        FHOS_LOG_ERROR("Cannot remove a directory with a null path.\n");
        return -1;
    }
    
#if defined(_WIN32) || defined(_WIN64)
    if(path_length < 0) {
        path_length = 0;
        while(path_data[path_length]) { path_length += 1; }
    }
    
    FHOS_List path = {0};
    path.capacity = 256;
    while(path_length + 3 >= path.capacity) { path.capacity += 256; }
    path.data = (fhos_u8 *)fhos_context_temp_alloc(ctx, path.capacity);
    if(!path.data) {
        FHOS_LOG_ERROR("Could not allocate memory for the path.\n");
        return -1;
    }
    
    for(fhos_i32 i = 0; i < path_length; i += 1) {
        if(path_data[i] == '/') {
            path.data[i] = '\\';
        } else {
            path.data[i] = ((fhos_u8 *)path_data)[i];
        }
        path.length += 1;
    }
    path.data[path.length] = 0;
    
    DWORD attributes = GetFileAttributesA((char *)path.data);
    if((attributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
        FHOS_LOG_ERROR("The path \"%s\" is not a directory.\n", (char *)path.data);
        fhos_context_temp_free(ctx, path.data);
        return -1;
    }
    
    if(path.data[path.length - 1] != '/' && path.data[path.length - 1] != '\\') {
        path.data[path.length] = '\\';
        path.length += 1;
    }
    path.data[path.length] = '*';
    path.data[path.length + 1] = 0;
    path_length = (fhos_i32)path.length;
    
    fhos_i32 handle_count = 1;
    HANDLE handle_stack[128];
    WIN32_FIND_DATAA find_data = {0};
    handle_stack[0] = FindFirstFileA((char *)path.data, &find_data);
    
    if(handle_stack[0] == INVALID_HANDLE_VALUE) {
        DWORD last_error = GetLastError();
        FHOS_LOG_ERROR("(%lu) Could not find the first file in the directory %s\n", last_error, path.data);
        fhos_context_temp_free(ctx, path.data);
        return -1;
    }
    
    fhos_error error = 1;
    while(handle_count > 0) {
        fhos_bool should_process = FHOS_TRUE;
        if(find_data.cFileName[0] == '.' &&
           (find_data.cFileName[1] == 0 || (find_data.cFileName[1] == '.' && find_data.cFileName[2] == 0)))
        {
            should_process = FHOS_FALSE;
        }
        
        if(should_process) {
            fhos_i32 ntstring_length = 0;
            while(find_data.cFileName[ntstring_length]) { ntstring_length += 1; }
            
            if(path.length + ntstring_length + 3 >= path.capacity) {
                while(path.length + ntstring_length >= path.capacity) { path.capacity += 256; }
                path.data = (fhos_u8 *)fhos_context_temp_realloc(ctx, path.data, path.capacity);
            }
            
            for(fhos_i32 i = 0; i < ntstring_length; i += 1) { path.data[path.length + i] = find_data.cFileName[i]; }
            
            if((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
                path.length += ntstring_length;
                path.data[path.length] = '\\';
                path.data[path.length + 1] = '*';
                path.data[path.length + 2] = 0;
                path.length += 1;
                
                handle_stack[handle_count] = FindFirstFileA((char *)path.data, &find_data);
                handle_count += 1;
                continue;
            } else {
                path.data[path.length + ntstring_length] = 0;
                DeleteFile((char *)path.data);
            }
        }
        
        while(!FindNextFileA(handle_stack[handle_count - 1], &find_data)) {
            FindClose(handle_stack[handle_count - 1]);
            handle_count -= 1;
            
            path.data[path.length] = 0;
            if(!RemoveDirectory((char *)path.data)) {
                FHOS_LOG_ERROR("(%lu) Could not remove the directory: \"%s\"\n", GetLastError(), path.data);
                error = -1;
                
                for(fhos_i32 i = 0; i < handle_count; i += 1) { FindClose(handle_stack[i]); }
                handle_count = 0;
            }
            
            if(handle_count <= 0) { break; }
            
            // NOTE(Patrik): The last character is '/' or '\\' so we don't count it
            for(fhos_i64 i = path.length - 2; i >= 0; i -= 1) {
                if(path.data[i] == '\\' || path.data[i] == '/') {
                    path.length = i + 1;
                    break;
                }
            }
        }
    }
    
    fhos_context_temp_free(ctx, path.data);
    return error;
#else
#  error Unimplemented on this platform.
#endif
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
