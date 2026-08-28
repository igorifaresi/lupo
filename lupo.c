#ifndef Lupo_H
#define Lupo_H

//TODO replace all uint8_t * blob to a buffer structure with optional bound checking

#include <stdint.h>

#ifndef NULL
    #define NULL 0
#endif

#define LUPO_TRUE 1
#define LUPO_FALSE 0
typedef uint8_t Lupo_Bool;

#ifndef LUPO_NO_STDLIB
    #include <stdio.h>
    #include <assert.h>

    #define LUPO_ASSERT assert
    #define lupo_printf printf
#else
    #ifndef LUPO_ASSERT
        #error "LUPO_ASSERT not defined"
    #endif

    #ifndef lupo_printf
        #error "lupo_printf not defined"
    #endif
#endif

#define LUPO_BITS_IN_A_BYTE                            8
#define LUPO_CHUNK_SIZE                                64
#define LUPO_PAGE_SIZE                                 (1024 * 16)
#define LUPO_CHUNK_ADDR_BIT_QNT                        8
#define LUPO_CHUNKS_PER_PAGE                           (LUPO_PAGE_SIZE / LUPO_CHUNK_SIZE)
#define LUPO_CHUNK_ADDR_MASK                           ((1 << LUPO_CHUNK_ADDR_BIT_QNT) - 1)
#define LUPO_CREATE_ADDRESS(page, chunk)               (page << LUPO_CHUNK_ADDR_BIT_QNT | chunk)
#define LUPO_GET_CHUNK_FROM_ADDRESS(addr)              (addr & LUPO_CHUNK_ADDR_MASK)
#define LUPO_GET_PAGE_FROM_ADDRESS(page)               (page >> LUPO_CHUNK_ADDR_BIT_QNT)
#define LUPO_CHUNK_DATA_SECTION_SIZE                   (LUPO_CHUNK_SIZE - sizeof(Lupo_Address) - sizeof(Lupo_Address))
#define LUPO_CLAMP(x, min, max)                        ((x > max) ? max : ((x < min) ? min : x))
#define LUPO_GET_VALUE_FROM_BLOB(type, blob, offset)   (*((type *)&blob[offset]))
#define LUPO_SET_BLOB_VALUE(type, blob, offset, value) *((type *)&blob[offset]) = value
#define LUPO_ZERO_STRUCT(s)                            lupo_memset(s, sizeof(*s), 0)
#define LUPO_STATIC_ARRAY_LENGTH(x)                    (sizeof(x) / sizeof((x)[0]))
#define LUPO_NUMARGS(...)                              (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define LUPO_ROOT_DIR_ID                               4242 // just a random, BUT CONSTANT, id
#define LUPO_ROOT_DIR_ADDRESS                          (LUPO_CREATE_ADDRESS(2, 0))

#ifdef _Static_assert
    #define LUPO_STATIC_ASSERT(COND, MSG) _Static_assert((COND), #MSG)
#else
    #define LUPO_STATIC_ASSERT(COND, MSG) typedef char STATIC_ASSERT_##MSG[(COND) ? 1 : -1]
#endif

#if defined(_MSVC_TRADITIONAL) && _MSVC_TRADITIONAL
    #define LUPO_NARG(...)  LUPO_INTERNAL_EXPAND_ARGS_PRIVATE(LUPO_INTERNAL_ARGS_AUGMENTER(__VA_ARGS__))
    #define LUPO_INTERNAL_ARGS_AUGMENTER(...) unused, __VA_ARGS__
    #define LUPO_INTERNAL_EXPAND(x) x
    #define LUPO_INTERNAL_EXPAND_ARGS_PRIVATE(...) LUPO_INTERNAL_EXPAND(LUPO_INTERNAL_GET_ARG_COUNT_PRIVATE(__VA_ARGS__, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))
    #define LUPO_INTERNAL_GET_ARG_COUNT_PRIVATE(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count
#else
    #define LUPO_NARG(...) LUPO_INTERNAL_GET_ARG_COUNT_PRIVATE(0, ## __VA_ARGS__, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
    #define LUPO_INTERNAL_GET_ARG_COUNT_PRIVATE(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, count, ...) count
#endif

LUPO_STATIC_ASSERT(
    LUPO_NARG() == 0 && LUPO_NARG(1) == 1 && LUPO_NARG(1, 2) == 2 && LUPO_NARG(1, 2, 3) == 3,
    checking_if_arg_count_macro_is_working
);

#define LUPO_API

typedef uint32_t Lupo_Address;
typedef uint64_t Lupo_ID;

#define dbchar char

LUPO_STATIC_ASSERT(sizeof(dbchar) == 1, invalid_char_size);

LUPO_STATIC_ASSERT(
    LUPO_GET_PAGE_FROM_ADDRESS(LUPO_CREATE_ADDRESS(69, 42)) == 69 && LUPO_GET_CHUNK_FROM_ADDRESS(LUPO_CREATE_ADDRESS(69, 42)) == 42,
    address_encoding_its_not_working
);

//-----------------------------------------------------------------------------
// 1:1 File to structures
//-----------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)
    #define LUPO_PACKED(__Declaration__) __Declaration__ __attribute__((__packed__))
#elif defined(_MSC_VER)
    #define LUPO_PACKED(__Declaration__) __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#endif

typedef struct Lupo_Reference Lupo_Reference;
LUPO_PACKED(struct Lupo_Reference {
    Lupo_ID id;
    Lupo_Address address;
});

typedef struct Lupo_PageMetadata Lupo_PageMetadata;
LUPO_PACKED(struct Lupo_PageMetadata {
    uint32_t     index;
    uint32_t     next_free_page_index;
    uint32_t     prev_free_page_index;
    uint32_t     next_page_index;
    uint32_t     prev_page_index;
    Lupo_Reference type_ref;
	uint8_t      chunk_usage_bitfield[LUPO_CHUNKS_PER_PAGE / LUPO_BITS_IN_A_BYTE];
});

typedef struct Lupo_EntityMetadata Lupo_EntityMetadata;
LUPO_PACKED(struct Lupo_EntityMetadata {
    uint32_t     size;
    Lupo_ID        id;
    Lupo_Reference type_ref;
});

typedef struct Lupo_Chunk Lupo_Chunk;
LUPO_PACKED(struct Lupo_Chunk {
    Lupo_Address first_chunk;
    Lupo_Address next_chunk;
    uint8_t    data[LUPO_CHUNK_DATA_SECTION_SIZE];
});

LUPO_STATIC_ASSERT(sizeof(Lupo_PageMetadata) == sizeof(Lupo_Chunk), invalid_page_metadata_size);

typedef struct Lupo_Page Lupo_Page;
LUPO_PACKED(struct Lupo_Page {
    Lupo_PageMetadata metadata;
    Lupo_Chunk        chunks[LUPO_CHUNKS_PER_PAGE - 1];
});

LUPO_STATIC_ASSERT(sizeof(Lupo_Page) == LUPO_PAGE_SIZE, invalid_page_size);

typedef struct Lupo_PagePointers Lupo_PagePointers;
LUPO_PACKED(struct Lupo_PagePointers {
    uint32_t first_free_page_index;
    uint32_t last_free_page_index;
    uint32_t first_page_index;
    uint32_t last_page_index;
});

//-----------------------------------------------------------------------------
// View Structures
//-----------------------------------------------------------------------------

typedef enum {
    LUPO_OPEN_OK,
    LUPO_OPEN_NEW,
    LUPO_OPEN_ERROR,
} Lupo_OpenStatus;

typedef enum {
    LUPO_NO_ERROR,
    LUPO_ERROR_PAGE_NOT_FOUND,
    LUPO_ERROR_INVALID_CHUNK_FIRST_PAGE,
    LUPO_ERROR_CANNOT_READ_ENTITY_BYTES,
    LUPO_ERROR_CANNOT_READ_ENTITY_TYPE_BYTES,
    LUPO_ERROR_CANNOT_READ_ROOT_DIR,
    LUPO_ERROR_CANNOT_READ_KEY,
    LUPO_ERROR_KEY_NOT_FOUND,
    LUPO_ERROR_ENTITY_DOES_NOT_HAVE_A_LINKS_FIELD,
    LUPO_ERROR_ENTITY_HAVE_INVALID_LINKS_FIELD,
    LUPO_ERROR_TYPE_MISSING_FIELDS,
    LUPO_ERROR_TYPE_INVALID_FIELDS,
    LUPO_ERROR_INVALID_TYPE,
    LUPO_ERROR_REF_ID_DOES_NOT_MATCH,
    LUPO_ERROR_INVALID_ENTITY_HEADER,
    LUPO_ERROR_ID_CANNOT_CHANGE,
    LUPO_ERROR_TYPE_CANNOT_CHANGE,
    LUPO_ERROR_CANNOT_GET_PREVIOUS,
    LUPO_ERROR_CANNOT_CREATE_DIR,
    LUPO_ERROR_CANNOT_UPDATE_DIR,
    LUPO_ERROR_CANNOT_CHECK_IF_EXISTS,
    LUPO_ERROR_TYPE_DECLARATION_WITHOUT_HEADER,
    LUPO_ERROR_INVALID_PATH_ARG_QNT,
    LUPO_ERROR_UNSUPPORTED_PATH_ARG,
} Lupo_ErrorCode;

typedef struct Lupo_Arena Lupo_Arena;
struct Lupo_Arena {
    uint8_t *data;
    int capacity;
    int used;
};

typedef struct Lupo_Error Lupo_Error;
struct Lupo_Error {
    int           line;
    const dbchar *function;
    const dbchar *message;
    const dbchar *context;
    Lupo_ErrorCode  code;
    Lupo_Error     *next;
};

typedef enum {
    LUPO_PRIMITIVE_INT8,
    LUPO_PRIMITIVE_UINT8,
    LUPO_PRIMITIVE_INT16,
    LUPO_PRIMITIVE_UINT16,
    LUPO_PRIMITIVE_INT32,
    LUPO_PRIMITIVE_UINT32,
    LUPO_PRIMITIVE_INT64,
    LUPO_PRIMITIVE_UINT64,
    LUPO_PRIMITIVE_FLOAT32,
    LUPO_PRIMITIVE_FLOAT64,
    LUPO_PRIMITIVE_STRING,
    LUPO_PRIMITIVE_REFERENCE,
    LUPO_PRIMITIVE_ID,
    LUPO_PRIMITIVE_FIELD_DECLARATION,
    LUPO_PRIMITIVE_INDEX,
    LUPO_PRIMITIVE_BLOB,
} Lupo_FieldKind;

// TODO: Insert static asserts for checking the type sizes

const dbchar *LUPO_PRIMITIVE_KIND_TO_STRING[] = {
    "INT8",
    "UINT8",
    "INT16",
    "UINT16",
    "INT32",
    "UINT32",
    "INT64",
    "UINT64",
    "FLOAT32",
    "FLOAT64",
    "STRING",
    "REFERENCE",
    "ID",
    "FIELD_DECLARATION",
    "INDEX",
    "BLOB",
};

const dbchar *LUPO_FANCY_PRIMITIVE_KIND_TO_STRING[] = {
    "8-bit integer",
    "8-bit unsigned integer",
    "16-bit integer",
    "16-bit unsigned integer",
    "32-bit integer",
    "32-bit unsigned integer",
    "64-bit integer",
    "64-bit unsigned integer",
    "32-bit float",
    "64-bit float",
    "String",
    "Reference",
    "ID",
    "Field declaration",
    "Index",
    "Binary object",
};

#define LUPO_INDEX_HASH_TABLE_SIZE (1024 * 4)

typedef struct Lupo_IndexEntry Lupo_IndexEntry;
struct Lupo_IndexEntry {
    const dbchar *key;
    Lupo_Reference reference;
    Lupo_IndexEntry *next;
};

typedef struct Lupo_Index Lupo_Index;
struct Lupo_Index {
    Lupo_IndexEntry *table[LUPO_INDEX_HASH_TABLE_SIZE];
};

#define LUPO_FIELD_IS_ARRAY (1 << 0)

typedef struct Lupo_FieldDeclaration Lupo_FieldDeclaration;
struct Lupo_FieldDeclaration {
    const dbchar     *name;
    uint32_t          flags;
    Lupo_FieldKind  kind;
};

const Lupo_FieldDeclaration LUPO_ENTITY_HEADER_FIELDS[] = {
    { .name = "size", .flags = 0, .kind = LUPO_PRIMITIVE_UINT32    },
    { .name = "id"  , .flags = 0, .kind = LUPO_PRIMITIVE_ID        },
    { .name = "type", .flags = 0, .kind = LUPO_PRIMITIVE_REFERENCE },
};

const Lupo_FieldDeclaration LUPO_TYPE_DECLARATION_FIELDS[] = {
    { .name = "size"                 , .flags = 0                , .kind = LUPO_PRIMITIVE_UINT32            },
    { .name = "id"                   , .flags = 0                , .kind = LUPO_PRIMITIVE_ID                },
    { .name = "type"                 , .flags = 0                , .kind = LUPO_PRIMITIVE_REFERENCE         },
    { .name = "first_free_page_index", .flags = 0                , .kind = LUPO_PRIMITIVE_UINT32            },
    { .name = "last_free_page_index" , .flags = 0                , .kind = LUPO_PRIMITIVE_UINT32            },
    { .name = "first_page_index"     , .flags = 0                , .kind = LUPO_PRIMITIVE_UINT32            },
    { .name = "last_page_index"      , .flags = 0                , .kind = LUPO_PRIMITIVE_UINT32            },
    { .name = "name"                 , .flags = 0                , .kind = LUPO_PRIMITIVE_STRING            },
    { .name = "fields"               , .flags = LUPO_FIELD_IS_ARRAY, .kind = LUPO_PRIMITIVE_FIELD_DECLARATION },
};

const Lupo_FieldDeclaration LUPO_BASIC_DIRECTORY_FIELDS[] = {
    { .name = "size", .flags = 0, .kind = LUPO_PRIMITIVE_UINT32    },
    { .name = "id"  , .flags = 0, .kind = LUPO_PRIMITIVE_ID        },
    { .name = "type", .flags = 0, .kind = LUPO_PRIMITIVE_REFERENCE },
    { .name = "links"   , .flags = 0, .kind = LUPO_PRIMITIVE_INDEX     },
};

typedef struct Lupo_Blob Lupo_Blob;
struct Lupo_Blob {
    uint8_t *data;
    uint32_t size;
    const dbchar *extension;
};

typedef union Lupo_Primitive Lupo_Primitive;
union Lupo_Primitive {
    int8_t                  int8;
    uint8_t                 uint8;
    int16_t                 int16;
    uint16_t                uint16;
    int32_t                 int32;
    uint32_t                uint32;
    int64_t                 int64;
    uint64_t                uint64;
    float                   float32;
    double                  float64;
    const dbchar           *string;
    Lupo_Reference            reference;
    Lupo_ID                   id;
    Lupo_FieldDeclaration field_decl;
    Lupo_Index           *index;
    Lupo_Blob             blob;
};

typedef struct Lupo_Field Lupo_Field;
struct Lupo_Field {
    uint32_t         flags;
    const dbchar    *name;
    union {
        union {
            int8_t                  int8;
            uint8_t                 uint8;
            int16_t                 int16;
            uint16_t                uint16;
            int32_t                 int32;
            uint32_t                uint32;
            int64_t                 int64;
            uint64_t                uint64;
            float                   float32;
            double                  float64;
            const dbchar           *string;
            Lupo_Reference            reference;
            Lupo_ID                   id;
            Lupo_FieldDeclaration field_decl;
            Lupo_Index           *index;
            Lupo_Blob             blob;
        };
        Lupo_Primitive value;
        struct {
            Lupo_Primitive *data; // TODO: use the own types as arrays, to avoid memory waste. Imagine a array of u8. And its harder for the user to pass an array of primitives
            int size;
        } array;
    };
#ifdef LUPO_EXTRA_USER_FIELD_STRUCT
    LUPO_EXTRA_USER_FIELD_STRUCT extra;
#endif
    Lupo_FieldKind kind;
};

typedef struct Lupo_Entity Lupo_Entity;
struct Lupo_Entity {
    Lupo_Error     *err;
    Lupo_Reference  _reference; // Why did a put a undescore in this field? Because its calculated value, the entity does not store their own reference
    const dbchar   *_type_name;
    Lupo_Field     *fields;
    int            field_qnt;
};

//-----------------------------------------------------------------------------
// Main context
//-----------------------------------------------------------------------------

typedef struct Lupo_RequestPageCacheNode Lupo_RequestPageCacheNode;
struct Lupo_RequestPageCacheNode {
    Lupo_Page *page;
    Lupo_RequestPageCacheNode *next;
};

typedef struct Lupo_Context Lupo_Context;

// params: db, page index
typedef Lupo_Page * Lupo_LoadPageProc(Lupo_Context *, uint32_t);

// params: db, index (output) ===> returns a new page
typedef Lupo_Page * Lupo_CreateEmptyPageProc(Lupo_Context *, uint32_t *);

// params: db, size, function, line
typedef uint8_t * Lupo_RamAllocProc(Lupo_Context *, uint32_t, const dbchar *, int line);

// params: db, page
typedef void Lupo_CommitPageProc(Lupo_Context *, Lupo_Page *page);

struct Lupo_Context {
    void *handle;
    Lupo_LoadPageProc                  *load_page;
    Lupo_CommitPageProc                *commit_page;
    Lupo_CreateEmptyPageProc           *create_empty_page;
    Lupo_ID                             forced_next_id;
    Lupo_Arena                          request_arena;
    Lupo_Arena                          main_arena;

    Lupo_RequestPageCacheNode          *page_cache;
};

LUPO_STATIC_ASSERT(
    (sizeof(Lupo_EntityMetadata) + sizeof(Lupo_PagePointers)) <= LUPO_CHUNK_DATA_SECTION_SIZE,
    entity_metadata_and_type_decl_pointers_must_fit_in_one_chunk_for_speed_purposes
);

#endif

//-----------------------------------------------------------------------------
// Utils
//-----------------------------------------------------------------------------
void
lupo_memset(void *destination, int size, uint8_t byte)
{
    uint8_t *byte_array = (uint8_t *)destination;
    for (int i = 0; i < size; i += 1) {
        byte_array[i] = byte;
    }
}

uint8_t *
lupo_arena_alloc(Lupo_Arena *arena, int allocation_size)
{
    uint8_t *ptr = &arena->data[arena->used];
    arena->used += allocation_size;

    LUPO_ASSERT(arena->used <= arena->capacity);

    lupo_memset(ptr, allocation_size, 0);

    return ptr;
}

#define lupo_new(arena_ptr, type)       ((type *)lupo_arena_alloc(arena_ptr, sizeof(type)))
#define lupo_make(arena_ptr, type, qnt) ((type *)lupo_arena_alloc(arena_ptr, sizeof(type) * qnt))

#include <immintrin.h>

#if defined(__GNUC__) || defined(__clang__)
    __attribute__((target("rdrnd")))
#endif
uint64_t
lupo_get_random_value()
{
#ifdef __linux__
    unsigned long long value;
#else
    uint64_t value;
#endif
    Lupo_Bool success = _rdrand64_step(&value);
    LUPO_ASSERT(success);
    return value;
}

uint64_t
lupo_get_system_timestamp_counter()
{
    return __rdtsc();
}

int
lupo_strlen(const dbchar *s)
{
    int len = 0;
    while (s[len] != '\0') {
        len += 1;
    }
    return len;
}

int
lupo_memcpy(uint8_t *destination, const uint8_t *source, int qnt, int destination_offset, int source_offset)
{
    int i = 0;
    while (i < qnt) {
        destination[i + destination_offset] = source[i + source_offset];
        i += 1;
    }
    return qnt;
}

Lupo_Bool
lupo_strcmp(const dbchar *a, const dbchar *b)
{
    int len_a = lupo_strlen(a);
    int len_b = lupo_strlen(b);

    if (len_a != len_b) {
        return LUPO_FALSE;
    }

    for (int i = 0; i < len_a; i += 1) {
        if (a[i] != b[i]) {
            return LUPO_FALSE;
        }
    }

    return LUPO_TRUE;
}

dbchar *
lupo_strclone(Lupo_Context *db, const dbchar *s)
{
    int size = lupo_strlen(s);
    dbchar *new_s = lupo_make(&db->request_arena, dbchar, size + 1);

    for (int i = 0; i < size; i += 1) {
        new_s[i] = s[i];
    }

    new_s[size] = '\0';

    return new_s;
}

Lupo_Bool
lupo_compare_reference(Lupo_Reference a, Lupo_Reference b)
{
    return a.id == b.id && a.address == b.address;
}

#define lupo_error(db, code)                            lupo_error_ex(db, LUPO_ERROR_ ## code, __LINE__, __func__, NULL, NULL)
#define lupo_error_append(db, code, next)               lupo_error_ex(db, LUPO_ERROR_ ## code, __LINE__, __func__, NULL, next)
#define lupo_error_with_ctx(db, code, ctx)              lupo_error_ex(db, LUPO_ERROR_ ## code, __LINE__, __func__, ctx, NULL)
#define lupo_error_append_with_ctx(db, code, next, ctx) lupo_error_ex(db, LUPO_ERROR_ ## code, __LINE__, __func__, ctx, next)

Lupo_Error *
lupo_error_ex(
    Lupo_Context *db,
    Lupo_ErrorCode code,
    int line,
    const dbchar *function,
    const dbchar *context,
    Lupo_Error *next
)
{
    Lupo_Error *err = lupo_new(&db->request_arena, Lupo_Error);

    err->line     = line;
    err->function = function;
    err->code     = code;
    err->next     = next;

    switch (code) {
    case LUPO_NO_ERROR: {
        err->message  = "No error";
    } break;
    case LUPO_ERROR_PAGE_NOT_FOUND: {
        err->message  = "Page not found";
    } break;
    case LUPO_ERROR_INVALID_CHUNK_FIRST_PAGE: {
        err->message  = "Invalid chunk first page";
    } break;
    case LUPO_ERROR_CANNOT_READ_ENTITY_BYTES: {
        err->message  = "Can't read entity bytes";
    } break;
    case LUPO_ERROR_CANNOT_READ_ENTITY_TYPE_BYTES: {
        err->message  = "Can't read entity type bytes";
    } break;
    case LUPO_ERROR_CANNOT_READ_ROOT_DIR: {
        err->message  = "Can't read root directory";
    } break;
    case LUPO_ERROR_CANNOT_READ_KEY: {
        err->message  = "Can't read entity referencered in the key";
    } break;
    case LUPO_ERROR_KEY_NOT_FOUND: {
        err->message  = "Key not found";
    } break;
    case LUPO_ERROR_ENTITY_DOES_NOT_HAVE_A_LINKS_FIELD: {
        err->message  = "The entity does not have a 'links' field";
    } break;
    case LUPO_ERROR_ENTITY_HAVE_INVALID_LINKS_FIELD: {
        err->message  = "The entity 'links' is invalid, it must be a index";
    } break;
    case LUPO_ERROR_TYPE_MISSING_FIELDS: {
        err->message = "The entity designated as a type its not valid type because it lacks the 'fields' (array of field declarions) field";
    } break;
    case LUPO_ERROR_TYPE_INVALID_FIELDS: {
        err->message = "The entity designated as a type its not valid type because it has a invalid 'fields' field, it must be an array of field declarations";
    } break;
    case LUPO_ERROR_INVALID_TYPE: {
        err->message = "There is an error in the type, check the previous errors in the stack to see";
    } break;
    case LUPO_ERROR_REF_ID_DOES_NOT_MATCH: {
        err->message = "The ID field in the reference doesn't match to the read entity id";
    } break;
    case LUPO_ERROR_INVALID_ENTITY_HEADER: {
        err->message = "Invalid entity header";
    } break;
    case LUPO_ERROR_ID_CANNOT_CHANGE: {
        err->message = "The ID cannot be updated";
    } break;
    case LUPO_ERROR_TYPE_CANNOT_CHANGE: {
        err->message = "The type cannot be updated";
    } break;
    case LUPO_ERROR_CANNOT_GET_PREVIOUS: {
        err->message = "Cannot get previous version of the entity for checking. This is necessary for guarantee that the update will be successfull";
    } break;
    case LUPO_ERROR_CANNOT_CREATE_DIR: {
        err->message = "Error creating a directory";
    } break;
    case LUPO_ERROR_CANNOT_UPDATE_DIR: {
        err->message = "Error updating (adding or removing a key) a directory";
    } break;
    case LUPO_ERROR_CANNOT_CHECK_IF_EXISTS: {
        err->message = "Cannot check if the entity that will be deleted really exists";
    } break;
    case LUPO_ERROR_TYPE_DECLARATION_WITHOUT_HEADER: {
        err->message = "The type declaration does not have the header fields";
    } break;
    case LUPO_ERROR_INVALID_PATH_ARG_QNT: {
        err->message = "The argument quantity its different from the arguments required in the '?' tags in the path";
    } break;
    case LUPO_ERROR_UNSUPPORTED_PATH_ARG: {
        err->message = "The path argument not supported, only integers, strings or ids can be used as path arguments";
    } break;
    }

    LUPO_ASSERT(LUPO_FALSE);

    return err;
}

Lupo_Entity
lupo_error_entity(Lupo_Error *err)
{
    Lupo_Entity e;
    LUPO_ZERO_STRUCT(&e);

    e.err = err;

    return e;
}

Lupo_ID
lupo_generate_random_id(Lupo_Context *db)
{
    if (db->forced_next_id != 0) {
        Lupo_ID id = db->forced_next_id;
        db->forced_next_id = 0;
        return id;
    }

    uint64_t x = lupo_get_system_timestamp_counter();
    uint64_t y = lupo_get_random_value();
    uint64_t id = ((x + y) * (x + y + 1)) / 2 + y;
    return id;
}

dbchar **
lupo_split_path(Lupo_Context *db, const dbchar *s, int *parts_qnt)
{
    int size_arr[64] = { 0 };

    int str_size = 0;
    *parts_qnt = 0;

    for (int i = 0; s[i] != '\0'; i += 1) {
        if (s[i] == '/') {
            if (str_size > 0) {
                size_arr[*parts_qnt] = str_size;
                *parts_qnt += 1;
            }
            str_size = 0;
        } else {
            str_size += 1;
        }
    }

    if (str_size > 0) {
        size_arr[*parts_qnt] = str_size;
        *parts_qnt += 1;
    }

    dbchar **parts = lupo_make(&db->request_arena, dbchar *, *parts_qnt);

    for (int i = 0; i < *parts_qnt; i += 1) {
        parts[i] = lupo_make(&db->request_arena, dbchar, size_arr[i] + 1);
    }

    str_size = 0;
    int part_idx = 0;

    for (int i = 0; s[i] != '\0'; i += 1) {
        if (s[i] == '/') {
            if (str_size > 0) {
                part_idx += 1;
            }
            str_size = 0;
        } else {
            parts[part_idx][str_size] = s[i];
            str_size += 1;
        }
    }

    return parts;
}

Lupo_Field *
lupo_get_field(const Lupo_Entity *entity, const dbchar *name)
{
    for (int i = 0; i < entity->field_qnt; i += 1) {
        if (lupo_strcmp(name, entity->fields[i].name)) {
            return &entity->fields[i];
        }
    }
    // TODO: return null field
    LUPO_ASSERT(LUPO_FALSE);
    return NULL;
}

Lupo_Bool
lupo_have_field(const Lupo_Entity *entity, const dbchar *name)
{
    for (int i = 0; i < entity->field_qnt; i += 1) {
        if (lupo_strcmp(name, entity->fields[i].name)) {
            return LUPO_TRUE;
        }
    }
    return LUPO_FALSE;
}

Lupo_EntityMetadata
lupo_fast_map_entity_metadata(Lupo_Context *db, uint8_t *blob, int *cursor)
{
    Lupo_EntityMetadata metadata = LUPO_GET_VALUE_FROM_BLOB(Lupo_EntityMetadata, blob, *cursor);
    *cursor += sizeof(Lupo_EntityMetadata);
    return metadata;
}


//TODO
#include <string.h>

// https://github.com/tidwall/th64
uint64_t
lupo_th64(const void *data, size_t len, uint64_t seed) {
    uint8_t*p=(uint8_t*)data,*e=p+len;
    uint64_t r=0x14020a57acced8b7,x,h=seed;
    while(p+8<=e)memcpy(&x,p,8),x*=r,p+=8,x=x<<31|x>>33,h=h*r^x,h=h<<31|h>>33;
    while(p<e)h=h*r^*(p++);
    return(h=h*r+len,h^=h>>31,h*=r,h^=h>>31,h*=r,h^=h>>31,h*=r,h);
}

Lupo_IndexEntry *
lupo_search_index_key(Lupo_Context *db, Lupo_Index *index, const dbchar *key)
{
    uint64_t key_hash = lupo_th64(key, lupo_strlen(key), 0x4242);
    int slot = key_hash % LUPO_INDEX_HASH_TABLE_SIZE;

    Lupo_IndexEntry *it = index->table[slot];

    while (it != NULL) {
        if (lupo_strcmp(key, it->key)) {
            return it;
        }
        it = it->next;
    }

    return NULL;
}

void
lupo_add_entry_to_index(Lupo_Context *db, Lupo_Index *index, const dbchar *key, Lupo_Reference reference)
{
    uint64_t key_hash = lupo_th64(key, lupo_strlen(key), 0x4242);
    int slot = key_hash % LUPO_INDEX_HASH_TABLE_SIZE;

    Lupo_IndexEntry *entry = lupo_new(&db->request_arena, Lupo_IndexEntry);
    LUPO_ZERO_STRUCT(entry);

    entry->key = lupo_strclone(db, key);
    entry->reference = reference;

    Lupo_IndexEntry *prev = index->table[slot];
    if (prev != NULL) {
        while (prev->next != NULL) {
            prev = prev->next;
        }
        prev->next = entry;
    } else {
        index->table[slot] = entry;
    }
}

void
lupo_remove_entry_to_index(Lupo_Context *db, Lupo_Index *index, const dbchar *key)
{
    uint64_t key_hash = lupo_th64(key, lupo_strlen(key), 0x4242);
    int slot = key_hash % LUPO_INDEX_HASH_TABLE_SIZE;

    Lupo_IndexEntry *entry = lupo_new(&db->request_arena, Lupo_IndexEntry);
    LUPO_ZERO_STRUCT(entry);

    Lupo_IndexEntry *it = index->table[slot];
    if (it != NULL) {
        if (lupo_strcmp(it->key, key)) {
            index->table[slot] = it->next;
        } else {
            while (it->next != NULL) {
                if (lupo_strcmp(it->next->key, key)) {
                    it->next = it->next->next;
                    break;
                }
                it = it->next;
            }
        }
    }
}

Lupo_Entity
lupo_build_empty_entity(Lupo_Context *db, const Lupo_FieldDeclaration *field_declarations, int field_qnt)
{
    Lupo_Entity entity;
    LUPO_ZERO_STRUCT(&entity);

    entity.field_qnt = field_qnt;
    entity.fields = lupo_make(&db->request_arena, Lupo_Field, field_qnt);

    for (int i = 0; i < field_qnt; i += 1) {
        entity.fields[i].name  = field_declarations[i].name;
        entity.fields[i].flags = field_declarations[i].flags;
        entity.fields[i].kind = field_declarations[i].kind;
    }

    return entity;
}

Lupo_Field
lupo_field_declarations_to_field(Lupo_Context *db, const Lupo_FieldDeclaration field_declarations[], int qnt)
{
    Lupo_Field field;
    LUPO_ZERO_STRUCT(&field);

    field.flags = LUPO_FIELD_IS_ARRAY;
    field.array.data = lupo_make(&db->request_arena, Lupo_Primitive, qnt);
    field.kind = LUPO_PRIMITIVE_FIELD_DECLARATION;

    for (int i = 0; i < qnt; i += 1) {
        field.array.data[i].field_decl = field_declarations[i];
    }

    field.array.size = qnt;

    return field;
}

Lupo_Reference
lupo_create_reference(Lupo_ID id, int page, int chunk)
{
    Lupo_Reference ref;
    LUPO_ZERO_STRUCT(&ref);
    ref.id = id;
    ref.address = LUPO_CREATE_ADDRESS(page, chunk);
    return ref;
}

Lupo_Bool
lupo_check_if_entity_has_valid_header(const Lupo_Entity e)
{
    if (e.field_qnt < LUPO_STATIC_ARRAY_LENGTH(LUPO_ENTITY_HEADER_FIELDS)) {
        return LUPO_FALSE;
    }

    for (int i = 0; i < LUPO_STATIC_ARRAY_LENGTH(LUPO_ENTITY_HEADER_FIELDS); i += 1) {
        Lupo_FieldDeclaration d = LUPO_ENTITY_HEADER_FIELDS[i];
        if (d.kind != e.fields[i].kind || d.flags != e.fields[i].flags || !lupo_strcmp(d.name, e.fields[i].name)) {
            return LUPO_FALSE;
        }
    }
    return LUPO_TRUE;
}

Lupo_Bool
lupo_check_if_type_decl_has_header(const Lupo_FieldDeclaration declarations[], int qnt)
{
    if (qnt < LUPO_STATIC_ARRAY_LENGTH(LUPO_ENTITY_HEADER_FIELDS)) {
        return LUPO_FALSE;
    }

    for (int i = 0; i < LUPO_STATIC_ARRAY_LENGTH(LUPO_ENTITY_HEADER_FIELDS); i += 1) {
        Lupo_FieldDeclaration h = LUPO_ENTITY_HEADER_FIELDS[i];
        if (h.kind != declarations[i].kind || h.flags != declarations[i].flags || !lupo_strcmp(h.name, declarations[i].name)) {
            return LUPO_FALSE;
        }
    }
    return LUPO_TRUE;
}

Lupo_Bool
lupo_is_string_number(const dbchar *s)
{
    if (*s == '\0') {
        return LUPO_FALSE;
    }

    int p = 0;

    if (s[p] == '-') {
        p += 1;
    }
    while (s[p] >= '0' && s[p] <= '9' && s[p] != '\0') {
        p += 1;
    }
    if (s[p] != '\0') {
        return LUPO_FALSE;
    }

    return LUPO_TRUE;
}

int64_t
lupo_string_to_number(const dbchar *s)
{
    int p = 0;
    int64_t value = 0;
    Lupo_Bool negative = LUPO_FALSE;

    if (s[p] == '-') {
        negative = LUPO_TRUE;
        p += 1;
    }

    while (s[p] != '\0') {
        value = value * 10 + (s[p] - '0');
        p += 1;
    }

    if (negative) {
        value *= -1;
    }

    return value;
}

const dbchar *
lupo_int64_to_string(Lupo_Context *db, int64_t value)
{
    int size = 1;

    for (int64_t v = value; v >= 10; v /= 10) {
        size += 1;
    }

    if (value < 0) {
        size += 1;
    }

    dbchar *s = lupo_make(&db->request_arena, dbchar, size + 1);
    s[size] = '\0';

    int64_t v = value;
    for (int p = size - 1; v > 0; p -= 1) {
        s[p] = (v % 10) + '0';
        v /= 10;
    }

    if (value < 0) {
        s[0] = '-';
    }

    return s;
}

const dbchar *
lupo_uint64_to_string(Lupo_Context *db, uint64_t value)
{
    int size = 1;

    for (int64_t v = value; v >= 10; v /= 10) {
        size += 1;
    }

    dbchar *s = lupo_make(&db->request_arena, dbchar, size + 1);
    s[size] = '\0';

    int64_t v = value;
    for (int p = size - 1; v > 0; p -= 1) {
        s[p] = (v % 10) + '0';
        v /= 10;
    }

    return s;
}

Lupo_Page *
lupo_get_page(Lupo_Context *db, uint32_t index)
{
    Lupo_RequestPageCacheNode *it = db->page_cache;

    if (it == NULL) {
        Lupo_Page *page = db->load_page(db, index);

        db->page_cache = lupo_new(&db->main_arena, Lupo_RequestPageCacheNode);
        db->page_cache->page = page;
        db->page_cache->next = NULL;

        return page;
    }

    while (LUPO_TRUE) {
        if (it->page->metadata.index == index) {
            return it->page;
        }

        if (it->next != NULL) {
           it = it->next;
        } else {
            break;
        }
    }

    Lupo_Page *page = db->load_page(db, index);

    it->next = lupo_new(&db->main_arena, Lupo_RequestPageCacheNode);
    it->next->page = page;
    it->next->next = NULL;

    return page;
}

Lupo_Page *
lupo_create_page(Lupo_Context *db, uint32_t *index)
{
    //printf("create page\n");

    Lupo_RequestPageCacheNode *it = db->page_cache;

    if (it == NULL) {
        Lupo_Page *page = db->create_empty_page(db, index);

        db->page_cache = lupo_new(&db->request_arena, Lupo_RequestPageCacheNode);
        db->page_cache->page = page;
        db->page_cache->next = NULL;

        return page;
    }

    while (it->next != NULL) {
        it = it->next;
    }

    Lupo_Page *page = db->create_empty_page(db, index);

    it->next = lupo_new(&db->request_arena, Lupo_RequestPageCacheNode);
    it->next->page = page;
    it->next->next = NULL;

    return page;
}

void
lupo_commit_cached_pages(Lupo_Context *db)
{
    for (Lupo_RequestPageCacheNode *it = db->page_cache; it != NULL; it = it->next) {
        db->commit_page(db, it->page);
    }
}

Lupo_Bool
lupo_have_links(Lupo_Entity e)
{
    return lupo_have_field(&e, "links");
}

Lupo_IndexEntry *
lupo_list_links(Lupo_Context *db, Lupo_Entity e, int *qnt)
{
    if (!lupo_have_links(e)) {
        *qnt = 0;
        return NULL;
    }

    uint32_t entries_qnt = 0;

    Lupo_Index *index = lupo_get_field(&e, "links")->index;

    for (int i = 0; i < LUPO_INDEX_HASH_TABLE_SIZE; i += 1) {
        Lupo_IndexEntry *entry = index->table[i];
        while (entry != NULL) {
            entries_qnt += 1;
            entry = entry->next;
        }
    }

    Lupo_IndexEntry *entries = lupo_make(&db->request_arena, Lupo_IndexEntry, entries_qnt);

    for (int i = 0; i < LUPO_INDEX_HASH_TABLE_SIZE; i += 1) {
        Lupo_IndexEntry *entry = index->table[i];
        while (entry != NULL) {
            entries[*qnt] = *entry;
            *qnt += 1;

            entry = entry->next;
        }
    }

    return entries;
}

Lupo_Reference
lupo_root_dir_ref()
{
    Lupo_Reference ref;
    LUPO_ZERO_STRUCT(&ref);

    ref.id = LUPO_ROOT_DIR_ID;
    ref.address = LUPO_ROOT_DIR_ADDRESS;

    return ref;
}

void
lupo_reset_request_arena(Lupo_Context *db)
{
    db->request_arena.used = 0;
}

//-----------------------------------------------------------------------------
// Some macro heavy util functions
//-----------------------------------------------------------------------------

#define X(_type, _prefix, _kind, _alias)  \
Lupo_Field                         \
LUPO_ ## _alias(_type x)                  \
{                                         \
    Lupo_Field field;                     \
    LUPO_ZERO_STRUCT(&field);             \
    field.kind = LUPO_PRIMITIVE_ ## _kind;\
    field._prefix = x;                    \
    return field;                         \
}

X(uint64_t      , uint64   , UINT64   , U64  );
X(int64_t       , int64    , INT64    , I64  );
X(uint32_t      , uint32   , UINT32   , U32  );
X(int32_t       , int32    , INT32    , I32  );
X(uint16_t      , uint16   , UINT16   , U16  );
X(int16_t       , int16    , INT16    , I16  );
X(uint8_t       , uint8    , UINT8    , U8   );
X(int8_t        , int8     , INT8     , I8   );
X(Lupo_ID       , id       , ID       , ID   );
X(const dbchar *, string   , STRING   , STR  );
X(Lupo_Reference, reference, REFERENCE, REF  );
X(Lupo_Index *  , index    , INDEX    , INDEX);

#undef X

#define LUPO_FIELD_DECL(_name, _kind, _flags) _LUPO_FIELD_DECL(_name, LUPO_PRIMITIVE_ ## _kind, _flags)

Lupo_FieldDeclaration
_LUPO_FIELD_DECL(const dbchar *name, Lupo_FieldKind kind, uint32_t flags)
{
    Lupo_FieldDeclaration d;
    LUPO_ZERO_STRUCT(&d);
    d.name = name;
    d.kind = kind;
    d.flags = flags;
    return d;
}

Lupo_Field
LUPO_FIELD(Lupo_Primitive p, Lupo_FieldKind kind)
{
    Lupo_Field f;
    LUPO_ZERO_STRUCT(&f);
    f.value = p;
    f.kind = kind;
    return f;
}

Lupo_Field
LUPO_BLOB(uint8_t *data, uint32_t size, const dbchar *extension)
{
    Lupo_Blob blob;
    LUPO_ZERO_STRUCT(&blob);
    blob.data = data;
    blob.size = size;
    blob.extension = extension;

    Lupo_Field field;
    LUPO_ZERO_STRUCT(&field);
    field.blob = blob;
    field.kind = LUPO_PRIMITIVE_BLOB;

    return field;
}

Lupo_Arena
LUPO_ARENA(uint8_t *data, int capacity)
{
    Lupo_Arena arena;
    LUPO_ZERO_STRUCT(&arena);

    arena.data = data;
    arena.capacity = capacity;
    arena.used = 0;

    return arena;
}

//-----------------------------------------------------------------------------
// IO operations
//-----------------------------------------------------------------------------

Lupo_PagePointers *
lupo_io_fast_get_ref_for_type_page_pointers(Lupo_Context *db, Lupo_Address address)
{
    Lupo_Page *page = lupo_get_page(db, LUPO_GET_PAGE_FROM_ADDRESS(address));
    Lupo_Chunk *chunk = &page->chunks[LUPO_GET_CHUNK_FROM_ADDRESS(address)];

    // it jumps the entity metadata
    Lupo_PagePointers *pointers = (Lupo_PagePointers *)&chunk->data[sizeof(Lupo_EntityMetadata)];

    return pointers;
}

Lupo_Page *
lupo_io_create_and_setup_page(Lupo_Context *db, Lupo_Reference type_ref)
{
    uint32_t index;
    Lupo_Page *new_page = lupo_create_page(db, &index);

    //TODO: You have to commit that
    Lupo_PagePointers *pointers = lupo_io_fast_get_ref_for_type_page_pointers(db, type_ref.address);

    new_page->metadata.prev_page_index = pointers->last_page_index;
    new_page->metadata.prev_free_page_index = pointers->last_free_page_index;
    new_page->metadata.index = index;
    new_page->metadata.type_ref = type_ref;

    if (pointers->last_free_page_index != 0) {
        Lupo_Page *last_page_freelist = lupo_get_page(db, pointers->last_free_page_index);
        last_page_freelist->metadata.next_free_page_index = index;
    }

    if (pointers->last_page_index != 0) {
        Lupo_Page *last_page = lupo_get_page(db, pointers->last_page_index);
        last_page->metadata.next_page_index = index;
    }

    if (pointers->first_page_index == 0) {
        pointers->first_page_index = index;
    }

    if (pointers->first_free_page_index == 0) {
        pointers->first_free_page_index = index;
    }

    pointers->last_free_page_index = index;
    pointers->last_page_index = index;

    return new_page;
}

int
lupo_io_write_data_into_page(
    Lupo_Context *db,
    Lupo_Page *page,
    uint8_t *blob,
    uint32_t size,
    int forced_first_chunk,
    Lupo_Address *first_chunk
)
{
    int first_written_chunk_of_the_page = -1;
    const int bitfield_byte_qnt = LUPO_CHUNKS_PER_PAGE / LUPO_BITS_IN_A_BYTE;
    Lupo_Chunk *last_chunk = NULL;

    int remaining = size;
    while (remaining > 0) {
        int empty_chunk = -1;
        for (int i = 0; i < bitfield_byte_qnt && empty_chunk == -1; i += 1) {
            uint8_t *b = &page->metadata.chunk_usage_bitfield[i];

            for (int bit = 0; bit < LUPO_BITS_IN_A_BYTE; bit += 1) {
                if (~*b & (0b00000001 << bit)) {
                    empty_chunk = (i * LUPO_BITS_IN_A_BYTE) + bit;
                    *b |= 0b00000001 << bit;
                    break;
                }
            }

            // at least one chunk is reserved
            if (empty_chunk == (LUPO_CHUNKS_PER_PAGE - 1)) { // checks if is the last possible index
                empty_chunk = -1;
            }
        }

        if (*first_chunk == 0 && forced_first_chunk != -1) {
            empty_chunk = forced_first_chunk;
        }

        //printf("empty_chunk = %d\n", empty_chunk);

        if (empty_chunk != -1) {
            Lupo_Chunk *chunk = &page->chunks[empty_chunk];
            lupo_memcpy(chunk->data, blob, LUPO_CLAMP(remaining, 0, LUPO_CHUNK_DATA_SECTION_SIZE), 0, 0);
            blob += LUPO_CHUNK_DATA_SECTION_SIZE;
            remaining -= LUPO_CHUNK_DATA_SECTION_SIZE;

            Lupo_Address this_chunk_address = LUPO_CREATE_ADDRESS(page->metadata.index, empty_chunk);

            if (last_chunk != NULL) {
                last_chunk->next_chunk = this_chunk_address;
            }

            if (first_written_chunk_of_the_page == -1) {
                first_written_chunk_of_the_page = this_chunk_address;
            }

            if (*first_chunk == 0) {
                *first_chunk = this_chunk_address;
            }

            chunk->first_chunk = *first_chunk;
            chunk->next_chunk = 0;

            last_chunk = chunk;
        } else {
            int next_free_page_index = page->metadata.next_free_page_index;

            Lupo_Page *next_page;
            if (next_free_page_index != 0) {
                next_page = lupo_get_page(db, next_free_page_index);
            } else {
                next_page = lupo_io_create_and_setup_page(db, page->metadata.type_ref);
            }

            int next_chunk_index = lupo_io_write_data_into_page(db, next_page, blob, remaining, -1, first_chunk);

            last_chunk->next_chunk = LUPO_CREATE_ADDRESS(next_page->metadata.index, next_chunk_index);

            //TODO: fix this
            //lupo_io_remove_page_from_freelist(db, page);

            break;
        }
    }

    return first_written_chunk_of_the_page;
}

Lupo_Address
lupo_io_write_entity_bytes(Lupo_Context *db, uint8_t *blob, uint32_t size, Lupo_Reference type_ref)
{
    Lupo_Page *first_page;

    Lupo_PagePointers *pointers = lupo_io_fast_get_ref_for_type_page_pointers(db, type_ref.address);

    if (pointers->first_free_page_index != 0) {
        first_page = lupo_get_page(db, pointers->first_free_page_index);
    } else {
        first_page = lupo_io_create_and_setup_page(db, type_ref);
    }

    Lupo_Address address = 0;
    lupo_io_write_data_into_page(db, first_page, blob, size, -1, &address);

    return address;
}

#define LUPO_PRESERVE_FIRST_CHUNK (1 << 0)

void
lupo_io_free_entity_chunks(Lupo_Context *db, Lupo_Address address, uint32_t flags)
{
    Lupo_Bool first_chunk = LUPO_TRUE;

    Lupo_Page *page = lupo_get_page(db, LUPO_GET_PAGE_FROM_ADDRESS(address));
    Lupo_Chunk *chunk = &page->chunks[LUPO_GET_CHUNK_FROM_ADDRESS(address)];

    int cursor = 0;
    Lupo_EntityMetadata metadata = lupo_fast_map_entity_metadata(db, chunk->data, &cursor);

    int remaining = metadata.size;
    while (remaining > 0) {
        chunk->next_chunk = 0;
        chunk->first_chunk = 0;

        uint8_t this_chunk_idx = LUPO_GET_CHUNK_FROM_ADDRESS(address);
        uint8_t mask = ~(0b00000001 << (this_chunk_idx % LUPO_BITS_IN_A_BYTE));

        if (flags & LUPO_PRESERVE_FIRST_CHUNK) {
            if (!first_chunk) {
                page->metadata.chunk_usage_bitfield[this_chunk_idx / LUPO_BITS_IN_A_BYTE] &= mask;
            } else {
                first_chunk = LUPO_FALSE;
            }
        } else {
            page->metadata.chunk_usage_bitfield[this_chunk_idx / LUPO_BITS_IN_A_BYTE] &= mask;
        }

        // TODO: check if the page need to be put back in the free list

        if (remaining > LUPO_CHUNK_DATA_SECTION_SIZE) {
            address = chunk->next_chunk;
            if (LUPO_GET_PAGE_FROM_ADDRESS(chunk->next_chunk) != page->metadata.index) {
                page = lupo_get_page(db, LUPO_GET_PAGE_FROM_ADDRESS(chunk->next_chunk));
            }
            chunk = &page->chunks[LUPO_GET_CHUNK_FROM_ADDRESS(chunk->next_chunk)];
        }

        remaining -= LUPO_CHUNK_DATA_SECTION_SIZE;
    }
}

void
lupo_io_update_entity_bytes_preseving_address(Lupo_Context *db, Lupo_Address address, uint8_t *blob, uint32_t size, Lupo_Reference type_ref)
{
    lupo_io_free_entity_chunks(db, address, LUPO_PRESERVE_FIRST_CHUNK);

    Lupo_Page *entity_first_page = lupo_get_page(db, LUPO_GET_PAGE_FROM_ADDRESS(address));

    Lupo_Address _ = 0;
    lupo_io_write_data_into_page(db, entity_first_page, blob, size, LUPO_GET_CHUNK_FROM_ADDRESS(address), &_);
}

uint8_t *
lupo_io_read_entity_bytes(Lupo_Context *db, Lupo_Address address, Lupo_Error **out_error)
{
    *out_error = NULL;

    Lupo_Page *page = lupo_get_page(db, LUPO_GET_PAGE_FROM_ADDRESS(address));

    if (page == NULL) {
        *out_error = lupo_error(db, PAGE_NOT_FOUND);
        return NULL;
    }

    Lupo_Chunk *chunk = &page->chunks[LUPO_GET_CHUNK_FROM_ADDRESS(address)];

    int cursor = 0;
    Lupo_EntityMetadata metadata = lupo_fast_map_entity_metadata(db, chunk->data, &cursor);

    uint8_t *blob = lupo_make(&db->request_arena, uint8_t, metadata.size);

    int remaining = metadata.size;
    while (remaining > 0) {
        if (chunk->first_chunk != address) {
            *out_error = lupo_error(db, INVALID_CHUNK_FIRST_PAGE);
            return NULL;
        }

        lupo_memcpy(blob, chunk->data, LUPO_CLAMP(remaining, 0, LUPO_CHUNK_DATA_SECTION_SIZE), metadata.size - remaining, 0);

        if (remaining > LUPO_CHUNK_DATA_SECTION_SIZE) {
            if (LUPO_GET_PAGE_FROM_ADDRESS(chunk->next_chunk) != page->metadata.index) {
                page = lupo_get_page(db, LUPO_GET_PAGE_FROM_ADDRESS(chunk->next_chunk));
            }
            chunk = &page->chunks[LUPO_GET_CHUNK_FROM_ADDRESS(chunk->next_chunk)];
        }

        remaining -= LUPO_CHUNK_DATA_SECTION_SIZE;
    }

    return blob;
}

//-----------------------------------------------------------------------------
// Printing
//-----------------------------------------------------------------------------

void
lupo_print_field_value(const Lupo_Field field)
{
    switch (field.kind) {
    case LUPO_PRIMITIVE_INT8:      { lupo_printf("%lli", (int64_t)field.int8);    } break;
    case LUPO_PRIMITIVE_UINT8:     { lupo_printf("%llu", (uint64_t)field.uint8);  } break;
    case LUPO_PRIMITIVE_INT16:     { lupo_printf("%lli", (int64_t)field.int16);   } break;
    case LUPO_PRIMITIVE_UINT16:    { lupo_printf("%llu", (uint64_t)field.uint16); } break;
    case LUPO_PRIMITIVE_INT32:     { lupo_printf("%lli", (int64_t)field.int32);   } break;
    case LUPO_PRIMITIVE_UINT32:    { lupo_printf("%llu", (uint64_t)field.uint32); } break;
    case LUPO_PRIMITIVE_INT64:     { lupo_printf("%lli", field.int64);            } break;
    case LUPO_PRIMITIVE_UINT64:    { lupo_printf("%llu", field.uint64);           } break;
    case LUPO_PRIMITIVE_FLOAT32:   { lupo_printf("%f"  , field.float32);          } break;
    case LUPO_PRIMITIVE_FLOAT64:   { lupo_printf("%lf" , field.float64);          } break;
    case LUPO_PRIMITIVE_STRING:    { lupo_printf("\"%s\""  , field.string);       } break;
    case LUPO_PRIMITIVE_ID:        { lupo_printf("%llu", field.id);               } break;
    case LUPO_PRIMITIVE_REFERENCE: {
        lupo_printf(
            "{ id=%llu,address={page=%d,chunk=%d} }",
            field.reference.id,
            LUPO_GET_PAGE_FROM_ADDRESS(field.reference.address),
            LUPO_GET_CHUNK_FROM_ADDRESS(field.reference.address)
        );
    } break;
    case LUPO_PRIMITIVE_FIELD_DECLARATION: {
        lupo_printf(
            "{ name=\"%s\", flags=%u, kind=%s }",
            field.field_decl.name,
            field.field_decl.flags,
            LUPO_PRIMITIVE_KIND_TO_STRING[field.field_decl.kind]
        );
    } break;
    case LUPO_PRIMITIVE_BLOB: {
        lupo_printf(
            "{ data=..., size=%u, extension=%s }",
            field.blob.size,
            field.blob.extension
        );
    } break;
    case LUPO_PRIMITIVE_INDEX: {
        lupo_printf("[\n");

        for (int i = 0; i < LUPO_INDEX_HASH_TABLE_SIZE; i += 1) {
            Lupo_IndexEntry *entry = field.index->table[i];

            while (entry != NULL) {
                lupo_printf("\tslot=%d, key=\"%s\", reference=", i, entry->key);
                lupo_print_field_value(LUPO_REF(entry->reference));
                lupo_printf("\n");

                entry = entry->next;
            }
        }

        lupo_printf("]\n");
    } break;
    }
}

void
lupo_print_field(const Lupo_Field field)
{
    lupo_printf("%s = ", field.name);

    if (field.flags & LUPO_FIELD_IS_ARRAY) {
        lupo_printf("[\n");
        for (int i = 0; i < field.array.size; i += 1) {
            lupo_printf("\t");
            lupo_print_field_value(LUPO_FIELD(field.array.data[i], field.kind));
            lupo_printf(",\n");
        }
        lupo_printf("]");
    } else {
        lupo_print_field_value(field);
    }

    lupo_printf("\n");
}

void
lupo_print_error(Lupo_Error *err, int tabs)
{
    for (int i = 0; i < tabs; i += 1) {
        lupo_printf("\t");
    }

    lupo_printf("%s():%d error: %s\n", err->function, err->line, err->message);

    if (err->next) {
        lupo_print_error(err->next, tabs + 1);
    }
}

void
lupo_print(const Lupo_Entity entity)
{
    if (entity.err == NULL) {
        lupo_printf(">>>\n");
        lupo_printf(
            "_reference = { id=%llu,address={page=%d,chunk=%d} }\n",
            entity._reference.id,
            LUPO_GET_PAGE_FROM_ADDRESS(entity._reference.address),
            LUPO_GET_CHUNK_FROM_ADDRESS(entity._reference.address)
        );
        lupo_printf("_type_name = %s\n", entity._type_name);
        for (int i = 0; i < entity.field_qnt; i += 1) {
            lupo_print_field(entity.fields[i]);
        }
        lupo_printf("<<<\n");
    } else {
        lupo_printf("\n");
        lupo_print_error(entity.err, 0);
    }
}

//-----------------------------------------------------------------------------
// Measuring
//-----------------------------------------------------------------------------

uint32_t
lupo_measure_serialized_field_value_size(const Lupo_Field field)
{
    switch (field.kind) {
    case LUPO_PRIMITIVE_INT8:      { return 1; } break;
    case LUPO_PRIMITIVE_UINT8:     { return 1; } break;
    case LUPO_PRIMITIVE_INT16:     { return 2; } break;
    case LUPO_PRIMITIVE_UINT16:    { return 2; } break;
    case LUPO_PRIMITIVE_INT32:     { return 4; } break;
    case LUPO_PRIMITIVE_UINT32:    { return 4; } break;
    case LUPO_PRIMITIVE_INT64:     { return 8; } break;
    case LUPO_PRIMITIVE_UINT64:    { return 8; } break;
    case LUPO_PRIMITIVE_FLOAT32:   { return 4; } break;
    case LUPO_PRIMITIVE_FLOAT64:   { return 8; } break;
    case LUPO_PRIMITIVE_STRING:    { return sizeof(uint32_t) + lupo_strlen(field.string); } break;
    case LUPO_PRIMITIVE_REFERENCE: { return sizeof(Lupo_Reference);                   } break;
    case LUPO_PRIMITIVE_ID:        { return sizeof(Lupo_ID);                          } break;
    case LUPO_PRIMITIVE_FIELD_DECLARATION: {
        uint32_t size = 0;
        size += lupo_measure_serialized_field_value_size(LUPO_STR(field.field_decl.name));
        size += lupo_measure_serialized_field_value_size(LUPO_U32(field.field_decl.flags));
        size += lupo_measure_serialized_field_value_size(LUPO_U32((uint32_t)field.field_decl.kind));
        return size;
    } break;
    case LUPO_PRIMITIVE_BLOB: {
        uint32_t size = 0;
        size += lupo_measure_serialized_field_value_size(LUPO_U32(field.blob.size));
        size += field.blob.size;
        size += lupo_measure_serialized_field_value_size(LUPO_STR(field.blob.extension));
        return size;
    } break;
    case LUPO_PRIMITIVE_INDEX: {
        uint32_t size = 0;

        // entries qnt
        size += lupo_measure_serialized_field_value_size(LUPO_U32(0));

        for (int i = 0; i < LUPO_INDEX_HASH_TABLE_SIZE; i += 1) {
            Lupo_IndexEntry *entry = field.index->table[i];
            while (entry != NULL) {
                size += lupo_measure_serialized_field_value_size(LUPO_U32((uint32_t)i));
                size += lupo_measure_serialized_field_value_size(LUPO_STR(entry->key));
                size += lupo_measure_serialized_field_value_size(LUPO_REF(entry->reference));
                entry = entry->next;
            }
        }

        return size;
    } break;
    }
    LUPO_ASSERT(LUPO_FALSE);
    return -1;
}

uint32_t
lupo_measure_serialized_field_size(const Lupo_Field field)
{
    uint32_t size = 0;
    if (field.flags & LUPO_FIELD_IS_ARRAY) {
        // passing the array size its not necessary
        size += lupo_measure_serialized_field_value_size(LUPO_U32(0));
        for (int i = 0; i < field.array.size; i += 1) {
            size += lupo_measure_serialized_field_value_size(LUPO_FIELD(field.array.data[i], field.kind));
        }
    } else {
        size += lupo_measure_serialized_field_value_size(field);
    }
    return size;
}

//-----------------------------------------------------------------------------
// Reading
//-----------------------------------------------------------------------------

Lupo_Primitive
lupo_parse_field_value(Lupo_Context *db, uint8_t *blob, Lupo_FieldKind kind, int *cursor)
{
    Lupo_Primitive u;
    LUPO_ZERO_STRUCT(&u);

    #define X(value) *cursor += lupo_memcpy((uint8_t *)&value, blob, sizeof(value), 0, *cursor)

    switch (kind) {
    case LUPO_PRIMITIVE_INT8:      { X(u.int8);      } break;
    case LUPO_PRIMITIVE_UINT8:     { X(u.uint8);     } break;
    case LUPO_PRIMITIVE_INT16:     { X(u.int16);     } break;
    case LUPO_PRIMITIVE_UINT16:    { X(u.uint16);    } break;
    case LUPO_PRIMITIVE_INT32:     { X(u.int32);     } break;
    case LUPO_PRIMITIVE_UINT32:    { X(u.uint32);    } break;
    case LUPO_PRIMITIVE_INT64:     { X(u.int64);     } break;
    case LUPO_PRIMITIVE_UINT64:    { X(u.uint64);    } break;
    case LUPO_PRIMITIVE_FLOAT32:   { X(u.float32);   } break;
    case LUPO_PRIMITIVE_FLOAT64:   { X(u.float64);   } break;
    case LUPO_PRIMITIVE_REFERENCE: { X(u.reference); } break;
    case LUPO_PRIMITIVE_ID:        { X(u.id);        } break;
    case LUPO_PRIMITIVE_STRING:    {
        uint32_t string_size;
        X(string_size);

        dbchar *s = lupo_make(&db->request_arena, dbchar, string_size + 1);
        *cursor += lupo_memcpy((uint8_t *)s, blob, string_size, 0, *cursor);
        s[string_size] = '\0';

        u.string = s;
    } break;
    case LUPO_PRIMITIVE_FIELD_DECLARATION: {
        u.field_decl.name = lupo_parse_field_value(db, blob, LUPO_PRIMITIVE_STRING, cursor).string;
        u.field_decl.flags = lupo_parse_field_value(db, blob, LUPO_PRIMITIVE_UINT32, cursor).uint32;
        u.field_decl.kind = (Lupo_FieldKind)lupo_parse_field_value(db, blob, LUPO_PRIMITIVE_UINT32, cursor).uint32;
    } break;
    case LUPO_PRIMITIVE_BLOB: {
        u.blob.size = lupo_parse_field_value(db, blob, LUPO_PRIMITIVE_UINT32, cursor).uint32;
        u.blob.data = lupo_make(&db->request_arena, uint8_t, u.blob.size);

        *cursor += lupo_memcpy(u.blob.data, blob, u.blob.size, 0, *cursor);

        u.blob.extension = lupo_parse_field_value(db, blob, LUPO_PRIMITIVE_STRING, cursor).string;
    } break;
    case LUPO_PRIMITIVE_INDEX: {
        u.index = lupo_new(&db->request_arena, Lupo_Index);

        uint32_t entries_qnt = lupo_parse_field_value(db, blob, LUPO_PRIMITIVE_UINT32, cursor).uint32;

        int entries_counter = 0;
        Lupo_IndexEntry *entries = lupo_make(&db->request_arena, Lupo_IndexEntry, entries_qnt);

        for (int i = 0; i < entries_qnt; i += 1) {
            uint32_t slot = lupo_parse_field_value(db, blob, LUPO_PRIMITIVE_UINT32, cursor).uint32;

            Lupo_IndexEntry *entry = &entries[entries_counter];
            LUPO_ZERO_STRUCT(entry);
            entries_counter += 1;

            entry->key = lupo_parse_field_value(db, blob, LUPO_PRIMITIVE_STRING, cursor).string;
            entry->reference = lupo_parse_field_value(db, blob, LUPO_PRIMITIVE_REFERENCE, cursor).reference;

            Lupo_IndexEntry *father = u.index->table[slot];
            if (father != NULL) {
                while (father != NULL) {
                    father = father->next;
                }
                father->next = entry;
            } else {
                u.index->table[slot] = entry;
            }
        }
    } break;
    }

    #undef X

    return u;
}

Lupo_Field
lupo_parse_field(Lupo_Context *db, uint8_t *blob, const Lupo_FieldDeclaration declaration, int *cursor)
{
    Lupo_Field field;
    LUPO_ZERO_STRUCT(&field);

    field.name = declaration.name;
    field.flags = declaration.flags;
    field.kind = declaration.kind;

    if (declaration.flags & LUPO_FIELD_IS_ARRAY) {
        field.array.size = lupo_parse_field_value(db, blob, LUPO_PRIMITIVE_UINT32, cursor).uint32;
        field.array.data = lupo_make(&db->request_arena, Lupo_Primitive, field.array.size);
        for (int i = 0; i < field.array.size; i += 1) {
            field.array.data[i] = lupo_parse_field_value(db, blob, declaration.kind, cursor);
        }
    } else {
        field.value = lupo_parse_field_value(db, blob, declaration.kind, cursor);
    }

    return field;
}

Lupo_Entity
lupo_parse_entity(Lupo_Context *db, uint8_t *blob, const Lupo_FieldDeclaration *field_declarations, int field_qnt)
{
    Lupo_Entity entity;
    LUPO_ZERO_STRUCT(&entity);

    entity.fields = lupo_make(&db->request_arena, Lupo_Field, field_qnt);
    entity.field_qnt = field_qnt;

    int cursor = 0;
    for (int i = 0; i < field_qnt; i += 1) {
        entity.fields[i] = lupo_parse_field(db, blob, field_declarations[i], &cursor);
    }

    return entity;
}

Lupo_Entity
lupo_read_entity(Lupo_Context *db, Lupo_Reference reference)
{
    Lupo_Error *err;

    uint8_t *blob = lupo_io_read_entity_bytes(db, reference.address, &err);
    if (err != NULL) {
        return lupo_error_entity(lupo_error_append(db, CANNOT_READ_ENTITY_BYTES, err));
    }

    Lupo_Entity header = lupo_parse_entity(db, blob, LUPO_ENTITY_HEADER_FIELDS, LUPO_STATIC_ARRAY_LENGTH(LUPO_ENTITY_HEADER_FIELDS));
    Lupo_Address type_address = lupo_get_field(&header, "type")->reference.address;

    uint8_t *type_blob = lupo_io_read_entity_bytes(db, type_address, &err);
    if (err != NULL) {
        return lupo_error_entity(lupo_error_append(db, CANNOT_READ_ENTITY_TYPE_BYTES, err));
    }

    Lupo_Entity type_declaration = lupo_parse_entity(db, type_blob, LUPO_TYPE_DECLARATION_FIELDS, LUPO_STATIC_ARRAY_LENGTH(LUPO_TYPE_DECLARATION_FIELDS));
    Lupo_Field *field_declarations = lupo_get_field(&type_declaration, "fields");

    Lupo_FieldDeclaration *declarations = lupo_make(&db->request_arena, Lupo_FieldDeclaration, field_declarations->array.size);

    for (int i = 0; i < field_declarations->array.size; i += 1) {
        declarations[i] = field_declarations->array.data[i].field_decl;
    }

    if (!lupo_check_if_type_decl_has_header(declarations, field_declarations->array.size)) {
        return lupo_error_entity(lupo_error(db, TYPE_DECLARATION_WITHOUT_HEADER));
    }

    Lupo_Entity e = lupo_parse_entity(db, blob, declarations, field_declarations->array.size);

    if (lupo_get_field(&e, "id")->id != reference.id) {
        return lupo_error_entity(lupo_error(db, REF_ID_DOES_NOT_MATCH));
    }

    e._reference.id = lupo_get_field(&e, "id")->id;
    e._reference.address = reference.address;
    e._type_name = lupo_get_field(&type_declaration, "name")->string;

    return e;
}

//-----------------------------------------------------------------------------
// Writing
//-----------------------------------------------------------------------------

void
lupo_serialize_field_value(Lupo_Field field, uint8_t *blob, int *cursor)
{
    #define X(value) *cursor += lupo_memcpy(blob, (uint8_t *)&value, sizeof(value), *cursor, 0)

    switch (field.kind) {
    case LUPO_PRIMITIVE_INT8:      { X(field.int8);      } break;
    case LUPO_PRIMITIVE_UINT8:     { X(field.uint8);     } break;
    case LUPO_PRIMITIVE_INT16:     { X(field.int16);     } break;
    case LUPO_PRIMITIVE_UINT16:    { X(field.uint16);    } break;
    case LUPO_PRIMITIVE_INT32:     { X(field.int32);     } break;
    case LUPO_PRIMITIVE_UINT32:    { X(field.uint32);    } break;
    case LUPO_PRIMITIVE_INT64:     { X(field.int64);     } break;
    case LUPO_PRIMITIVE_UINT64:    { X(field.uint64);    } break;
    case LUPO_PRIMITIVE_FLOAT32:   { X(field.float32);   } break;
    case LUPO_PRIMITIVE_FLOAT64:   { X(field.float64);   } break;
    case LUPO_PRIMITIVE_REFERENCE: { X(field.reference); } break;
    case LUPO_PRIMITIVE_ID:        { X(field.id);        } break;
    case LUPO_PRIMITIVE_STRING:    {
        uint32_t string_size = lupo_strlen(field.string);
        *cursor += lupo_memcpy(blob, (uint8_t *)&string_size, sizeof(string_size), *cursor, 0);
        *cursor += lupo_memcpy(blob, (uint8_t *)field.string    , string_size        , *cursor, 0);
    } break;
    case LUPO_PRIMITIVE_FIELD_DECLARATION: {
        lupo_serialize_field_value(LUPO_STR(field.field_decl.name), blob, cursor);
        lupo_serialize_field_value(LUPO_U32(field.field_decl.flags), blob, cursor);
        lupo_serialize_field_value(LUPO_U32((uint32_t)field.field_decl.kind), blob, cursor);
    } break;
    case LUPO_PRIMITIVE_BLOB: {
        lupo_serialize_field_value(LUPO_U32(field.blob.size), blob, cursor);

        *cursor += lupo_memcpy(blob, field.blob.data, field.blob.size, *cursor, 0);

        lupo_serialize_field_value(LUPO_STR(field.blob.extension), blob, cursor);
    } break;
    case LUPO_PRIMITIVE_INDEX: {
        uint32_t entries_qnt = 0;

        for (int i = 0; i < LUPO_INDEX_HASH_TABLE_SIZE; i += 1) {
            Lupo_IndexEntry *entry = field.index->table[i];
            while (entry != NULL) {
                entries_qnt += 1;
                entry = entry->next;
            }
        }

        lupo_serialize_field_value(LUPO_U32(entries_qnt), blob, cursor);

        for (int i = 0; i < LUPO_INDEX_HASH_TABLE_SIZE; i += 1) {
            Lupo_IndexEntry *entry = field.index->table[i];
            while (entry != NULL) {
                lupo_serialize_field_value(LUPO_U32((uint32_t)i), blob, cursor);
                lupo_serialize_field_value(LUPO_STR(entry->key), blob, cursor);
                lupo_serialize_field_value(LUPO_REF(entry->reference), blob, cursor);
                entry = entry->next;
            }
        }
    } break;
    }

    #undef X
}

void
lupo_serialize_field(const Lupo_Field field, uint8_t *blob, int *cursor)
{
    if (field.flags & LUPO_FIELD_IS_ARRAY) {
        lupo_serialize_field_value(LUPO_U32((uint32_t)field.array.size), blob, cursor);
        for (int i = 0; i < field.array.size; i += 1) {
            lupo_serialize_field_value(LUPO_FIELD(field.array.data[i], field.kind), blob, cursor);
        }
    } else {
        lupo_serialize_field_value(field, blob, cursor);
    }
}

uint8_t *
lupo_serialize_entity(Lupo_Context *db, Lupo_Entity *entity, int *size)
{
    *size = 0;
    for (int i = 0; i < entity->field_qnt; i += 1) {
        *size += lupo_measure_serialized_field_size(entity->fields[i]);
    }

    lupo_get_field(entity, "size")->uint32 = *size;

    uint8_t *blob = lupo_make(&db->request_arena, uint8_t, *size);

    int cursor = 0;
    for (int i = 0; i < entity->field_qnt; i += 1) {
        lupo_serialize_field(entity->fields[i], blob, &cursor);
    }

    return blob;
}

void
lupo_write_entity(Lupo_Context *db, Lupo_Entity *entity, Lupo_Reference type_ref)
{
    int size = 0;
    uint8_t *blob = lupo_serialize_entity(db, entity, &size);
    Lupo_Address address = lupo_io_write_entity_bytes(db, blob, size, type_ref);

    entity->_reference.id = lupo_get_field(entity, "id")->id;
    entity->_reference.address = address;
}

void
lupo_update_entity(Lupo_Context *db, Lupo_Entity *entity, Lupo_Reference type_ref)
{
    int size = 0;
    uint8_t *blob = lupo_serialize_entity(db, entity, &size);
    lupo_io_update_entity_bytes_preseving_address(db, entity->_reference.address, blob, size, type_ref);
}

void
lupo_remove_entity(Lupo_Context *db, Lupo_Reference ref)
{
    lupo_io_free_entity_chunks(db, ref.address, 0);
}

//-----------------------------------------------------------------------------
// API (easy to call) aliases (and yes, they have to come first because C)
//-----------------------------------------------------------------------------

#define lupo_update lupo_update_ex
#define lupo_remove lupo_remove_ex

#ifdef __cplusplus
    #include <vector>

    template <typename... Fields>
    Lupo_Entity
    lupo_read(Lupo_Context *db, const char *path, Fields&&... fields)
    {
        static_assert(
            (std::is_same_v<std::decay_t<Fields>, Lupo_Field> && ...),
            "All variadic arguments must be of type Lupo_Field"
        );
        std::vector<Lupo_Field> field_list = {std::forward<Fields>(fields)...};
        return lupo_read_ex(db, path, static_cast<int>(field_list.size()), field_list.data());
    }

    template <typename... Fields>
    Lupo_Entity
    lupo_create(Lupo_Context *db, Lupo_Entity type, Fields&&... fields)
    {
        static_assert(
            (std::is_same_v<std::decay_t<Fields>, Lupo_Field> && ...),
            "All variadic arguments must be of type Lupo_Field"
        );
        std::vector<Lupo_Field> field_list = {std::forward<Fields>(fields)...};
        return lupo_create_ex(db, type, static_cast<int>(field_list.size()), field_list.data());
    }

    template <typename... Fields>
    Lupo_Entity
    lupo_create_type(Lupo_Context *db, const dbchar *name, Fields&&... fields)
    {
        static_assert(
            (std::is_same_v<std::decay_t<Fields>, Lupo_FieldDeclaration> && ...),
            "All variadic arguments must be of type Lupo_FieldDeclaration"
        );
        std::vector<Lupo_FieldDeclaration> field_list = {std::forward<Fields>(fields)...};
        return lupo_create_type_ex(db, name, static_cast<int>(field_list.size()), field_list.data());
    }

    template <typename... Fields>
    void
    lupo_unlink(Lupo_Context *db, const dbchar *path, Fields&&... fields)
    {
        static_assert(
            (std::is_same_v<std::decay_t<Fields>, Lupo_Field> && ...),
            "All variadic arguments must be of type Lupo_Field"
        );
        std::vector<Lupo_Field> field_list = {std::forward<Fields>(fields)...};
        lupo_unlink_ex(db, path, static_cast<int>(field_list.size()), field_list.data());
    }

    template <typename... Fields>
    void
    lupo_link(Lupo_Context *db, Lupo_Entity entity, const dbchar *path, Fields&&... fields)
    {
        static_assert(
            (std::is_same_v<std::decay_t<Fields>, Lupo_Field> && ...),
            "All variadic arguments must be of type Lupo_Field"
        );
        std::vector<Lupo_Field> field_list = {std::forward<Fields>(fields)...};
        lupo_link_ex(db, entity, path, static_cast<int>(field_list.size()), field_list.data());
    }
#else
    //TODO: make &[1] gambiarra
    #define lupo_array(type, ...)     ((type[]){__VA_ARGS__})
    #define lupo_read(db, path, ...) lupo_read_ex(db, path, LUPO_NARG(__VA_ARGS__), lupo_array(const Lupo_Field, (Lupo_Field){0}, __VA_ARGS__) + 1)
    #define lupo_create(db, type, ...) lupo_create_ex(db, type, LUPO_NARG(__VA_ARGS__), lupo_array(const Lupo_Field, (Lupo_Field){0}, __VA_ARGS__) + 1)
    #define lupo_create_type(db, name, ...) lupo_create_type_ex(db, name, LUPO_NARG(__VA_ARGS__), lupo_array(const Lupo_FieldDeclaration, (Lupo_FieldDeclaration){0}, __VA_ARGS__) + 1)

    #define lupo_link(db, entity, path, ...) \
        lupo_link_ex(db, entity, path,       \
                LUPO_NARG(__VA_ARGS__),      \
                lupo_array(const Lupo_Field, (Lupo_Field){0}, __VA_ARGS__) + 1)

    #define lupo_unlink(db, path, ...) lupo_unlink_ex(db, path, LUPO_NARG(__VA_ARGS__), lupo_array(const Lupo_Field, (Lupo_Field){0}, __VA_ARGS__) + 1)
#endif

//-----------------------------------------------------------------------------
// API
//-----------------------------------------------------------------------------

typedef struct Lupo_PathIterator Lupo_PathIterator;
struct Lupo_PathIterator {
    const dbchar **parts;
    int parts_qnt;
    int index;
    Lupo_Error *err;
    Lupo_Entity entity;
};

typedef enum {
    LUPO_PATH_ITER_ERROR,
    LUPO_PATH_ITER_FINISHED,
    LUPO_PATH_ITER_NOT_FOUND,
    LUPO_PATH_ITER_OK,
} Lupo_PathIterationStatus;

const dbchar *
lupo_field_to_string(Lupo_Context *db, Lupo_FieldKind kind, Lupo_Primitive prim)
{
    const dbchar *s = "(lupo_field_to_string error!)";

    switch (kind) {
    case LUPO_PRIMITIVE_INT8:   { s = lupo_int64_to_string(db, (int64_t)prim.int8);     } break;
    case LUPO_PRIMITIVE_UINT8:  { s = lupo_uint64_to_string(db, (uint64_t)prim.uint8);  } break;
    case LUPO_PRIMITIVE_INT16:  { s = lupo_int64_to_string(db, (int64_t)prim.int16);    } break;
    case LUPO_PRIMITIVE_UINT16: { s = lupo_uint64_to_string(db, (uint64_t)prim.uint16); } break;
    case LUPO_PRIMITIVE_INT32:  { s = lupo_int64_to_string(db, (int64_t)prim.int32);    } break;
    case LUPO_PRIMITIVE_UINT32: { s = lupo_uint64_to_string(db, (uint64_t)prim.uint32); } break;
    case LUPO_PRIMITIVE_INT64:  { s = lupo_int64_to_string(db, prim.int64);             } break;
    case LUPO_PRIMITIVE_UINT64: { s = lupo_uint64_to_string(db, prim.uint64);           } break;
    case LUPO_PRIMITIVE_STRING: { s = lupo_strclone(db, prim.string);                   } break;
    case LUPO_PRIMITIVE_ID:     { s = lupo_uint64_to_string(db, prim.id);               } break;
    default: {} break;
    }

    return s;
}

Lupo_PathIterator
lupo_create_path_iterator(Lupo_Context *db, const dbchar *path, const Lupo_Field args[], int args_qnt)
{
    Lupo_PathIterator it;
    LUPO_ZERO_STRUCT(&it);

    it.parts = (const dbchar **)lupo_split_path(db, (const dbchar *)path, &it.parts_qnt);
    it.index = 0;
    it.err = NULL;

    int used_args = 0;
    for (int i = 0; i < it.parts_qnt; i += 1) {
        if (lupo_strcmp(it.parts[i], "?")) {
            if (used_args >= args_qnt) {
                it.err = lupo_error(db, INVALID_PATH_ARG_QNT);
                return it;
            }

            Lupo_FieldKind kind = args[used_args].kind;

            Lupo_Bool supported =
                kind == LUPO_PRIMITIVE_INT8   ||
                kind == LUPO_PRIMITIVE_UINT8  ||
                kind == LUPO_PRIMITIVE_INT16  ||
                kind == LUPO_PRIMITIVE_UINT16 ||
                kind == LUPO_PRIMITIVE_INT32  ||
                kind == LUPO_PRIMITIVE_UINT32 ||
                kind == LUPO_PRIMITIVE_INT64  ||
                kind == LUPO_PRIMITIVE_UINT64 ||
                kind == LUPO_PRIMITIVE_STRING ||
                kind == LUPO_PRIMITIVE_ID;

            if (!supported) {
                it.err = lupo_error(db, UNSUPPORTED_PATH_ARG);
                return it;
            }

            it.parts[i] = lupo_field_to_string(db, kind, args[used_args].value);
            used_args += 1;
        }
    }

    if (used_args != args_qnt) {
        it.err = lupo_error(db, INVALID_PATH_ARG_QNT);
        return it;
    }

    it.entity = lupo_read_entity(db, lupo_root_dir_ref());
    if (it.entity.err != NULL) {
        it.err = lupo_error_append(db, CANNOT_READ_ROOT_DIR, it.entity.err);
        return it;
    }

    return it;
}

Lupo_PathIterationStatus
lupo_path_iterator_next(Lupo_Context *db, Lupo_PathIterator *it)
{
    if (it->index >= it->parts_qnt) {
        return LUPO_PATH_ITER_FINISHED;
    }

    Lupo_Field *links_field = lupo_get_field(&it->entity, "links");
    if (links_field == NULL) {
        it->err = lupo_error(db, ENTITY_DOES_NOT_HAVE_A_LINKS_FIELD);
        return LUPO_PATH_ITER_ERROR;
    }
    if (links_field->kind != LUPO_PRIMITIVE_INDEX || links_field->flags & LUPO_FIELD_IS_ARRAY) {
        it->err = lupo_error(db, ENTITY_HAVE_INVALID_LINKS_FIELD);
        return LUPO_PATH_ITER_ERROR;
    }

    Lupo_Index *index = links_field->index;
    Lupo_IndexEntry *entry = lupo_search_index_key(db, index, it->parts[it->index]);

    if (entry != NULL) {
        it->entity = lupo_read_entity(db, entry->reference);
        if (it->entity.err != NULL) {
            it->err = lupo_error_append(db, CANNOT_READ_KEY, it->entity.err);
            return LUPO_PATH_ITER_ERROR;
        }
    } else {
        return LUPO_PATH_ITER_NOT_FOUND;
    }

    it->index += 1;

    return LUPO_PATH_ITER_OK;
}

Lupo_Entity
lupo_read_ex_proc(Lupo_Context *db, const dbchar *path, int args_qnt, const Lupo_Field args[])
{
    Lupo_PathIterator it = lupo_create_path_iterator(db, path, args, args_qnt);
    if (it.err != NULL) {
        return lupo_error_entity(it.err);
    }

    Lupo_Bool stop = LUPO_FALSE;
    while (!stop) {
        switch (lupo_path_iterator_next(db, &it)) {
        case LUPO_PATH_ITER_OK:        {                                                          } break;
        case LUPO_PATH_ITER_FINISHED:  { stop = LUPO_TRUE;                                             } break;
        case LUPO_PATH_ITER_NOT_FOUND: { return lupo_error_entity(lupo_error(db, KEY_NOT_FOUND)); } break;
        case LUPO_PATH_ITER_ERROR:     { return lupo_error_entity(it.err);                        } break;
        }
    }

    return it.entity;
}


Lupo_Entity
lupo_read_ex(Lupo_Context *db, const dbchar *path, int args_qnt, const Lupo_Field args[])
{
    Lupo_Entity e = lupo_read_ex_proc(db, path, args_qnt, args);
    return e;
}

Lupo_Entity
lupo_create_ex_proc(Lupo_Context *db, Lupo_Entity type, int args_qnt, const Lupo_Field args[])
{
    if (type.err != NULL)
        return lupo_error_entity(lupo_error_append(db, INVALID_TYPE, type.err));

    Lupo_Field *tmp = lupo_get_field(&type, "fields");
    if (tmp == NULL) {
        return lupo_error_entity(lupo_error(db, TYPE_MISSING_FIELDS));
    }
    if (tmp->kind != LUPO_PRIMITIVE_FIELD_DECLARATION || !(tmp->flags & LUPO_FIELD_IS_ARRAY)) {
        return lupo_error_entity(lupo_error(db, TYPE_INVALID_FIELDS));
    }

    Lupo_FieldDeclaration *declarations = lupo_make(&db->request_arena, Lupo_FieldDeclaration, tmp->array.size);

    for (int i = 0; i < tmp->array.size; i += 1) {
        declarations[i] = tmp->array.data[i].field_decl;
    }

    Lupo_Entity entity = lupo_build_empty_entity(db, declarations, tmp->array.size);

    entity.fields[0].uint32    = 0;                         // size
    entity.fields[1].id        = lupo_generate_random_id(db); // id
    entity.fields[2].reference = type._reference;           // type

    int header_field_qnt = LUPO_STATIC_ARRAY_LENGTH(LUPO_ENTITY_HEADER_FIELDS);

    for (int i = 0; i < args_qnt; i += 1) {
        Lupo_Field field = args[i];
        LUPO_ASSERT(entity.fields[i + header_field_qnt].kind == field.kind);
        LUPO_ASSERT(entity.fields[i + header_field_qnt].flags == field.flags);

        if (field.kind == LUPO_PRIMITIVE_INDEX && field.index == NULL) {
            entity.fields[i + header_field_qnt].index = lupo_new(&db->request_arena, Lupo_Index);
        } else {
            entity.fields[i + header_field_qnt].value = field.value;
        }
    }

    lupo_write_entity(db, &entity, type._reference);

    return entity;
}

Lupo_Entity
lupo_create_ex(Lupo_Context *db, Lupo_Entity type, int args_qnt, const Lupo_Field args[])
{
    Lupo_Entity e = lupo_create_ex_proc(db, type, args_qnt, args);
    lupo_commit_cached_pages(db);
    return e;
}

Lupo_Entity
lupo_create_type_ex_proc(Lupo_Context *db, const dbchar *name, int args_qnt, const Lupo_FieldDeclaration args[])
{
    int fields_qnt = LUPO_STATIC_ARRAY_LENGTH(LUPO_ENTITY_HEADER_FIELDS) + args_qnt;
    Lupo_FieldDeclaration *fields = lupo_make(&db->request_arena, Lupo_FieldDeclaration, fields_qnt);

    for (int i = 0; i < LUPO_STATIC_ARRAY_LENGTH(LUPO_ENTITY_HEADER_FIELDS); i += 1) {
        fields[i] = LUPO_ENTITY_HEADER_FIELDS[i];
    }

    for (int i = 0; i < args_qnt; i += 1) {
        fields[i + LUPO_STATIC_ARRAY_LENGTH(LUPO_ENTITY_HEADER_FIELDS)] = args[i];
    }

    Lupo_Entity type = lupo_create(
        db,
        lupo_read_ex(db, "/types/type_declaration", 0, 0),
        LUPO_U32(0),
        LUPO_U32(0),
        LUPO_U32(0),
        LUPO_U32(0),
        LUPO_STR(name),
        lupo_field_declarations_to_field(db, fields, fields_qnt)
    );

    return type;
}

Lupo_Entity
lupo_create_type_ex(Lupo_Context *db, const dbchar *name, int args_qnt, const Lupo_FieldDeclaration args[])
{
    Lupo_Entity e = lupo_create_type_ex_proc(db, name, args_qnt, args);
    return e;
}

Lupo_Error *
lupo_update_ex_proc(Lupo_Context *db, Lupo_Entity *entity)
{
    if (!lupo_check_if_entity_has_valid_header(*entity)) {
        return lupo_error(db, INVALID_ENTITY_HEADER);
    }

    Lupo_Entity prev = lupo_read_entity(db, entity->_reference);
    if (prev.err != NULL) {
        return lupo_error_append(db, CANNOT_GET_PREVIOUS, prev.err);
    }

    if (lupo_get_field(&prev, "id")->id != lupo_get_field(entity, "id")->id) {
        return lupo_error(db, ID_CANNOT_CHANGE);
    }

    if (!lupo_compare_reference(lupo_get_field(&prev, "type")->reference, lupo_get_field(entity, "type")->reference)) {
        return lupo_error(db, TYPE_CANNOT_CHANGE);
    }

    lupo_update_entity(db, entity, lupo_get_field(entity, "type")->reference);

    return NULL;
}

Lupo_Error *
lupo_update_ex(Lupo_Context *db, Lupo_Entity *entity)
{
    Lupo_Error *err = lupo_update_ex_proc(db, entity);
    lupo_commit_cached_pages(db);
    return err;
}

Lupo_Error *
lupo_remove_ex_proc(Lupo_Context *db, Lupo_Entity entity)
{
    if (!lupo_check_if_entity_has_valid_header(entity)) {
        return lupo_error(db, INVALID_ENTITY_HEADER);
    }

    Lupo_Entity prev = lupo_read_entity(db, entity._reference);
    if (prev.err != NULL) {
        return lupo_error_append(db, CANNOT_GET_PREVIOUS, prev.err);
    }

    lupo_remove_entity(db, entity._reference);

    return NULL;
}

Lupo_Error *
lupo_remove_ex(Lupo_Context *db, Lupo_Entity entity)
{
    Lupo_Error *err = lupo_remove_ex_proc(db, entity);
    lupo_commit_cached_pages(db);
    return err;
}

Lupo_Error *
lupo_link_ex_proc(Lupo_Context *db, Lupo_Entity entity, const dbchar *path, int args_qnt, const Lupo_Field args[])
{
    Lupo_PathIterator it = lupo_create_path_iterator(db, path, args, args_qnt);
    if (it.err != NULL) {
        return it.err;
    }

    Lupo_Bool stop = LUPO_FALSE;
    while (!stop) {
        switch (lupo_path_iterator_next(db, &it)) {
        case LUPO_PATH_ITER_FINISHED:  { stop = LUPO_TRUE;   } break;
        case LUPO_PATH_ITER_ERROR:     { return it.err; } break;
        case LUPO_PATH_ITER_OK:        {                } break;
        case LUPO_PATH_ITER_NOT_FOUND: {
            Lupo_Entity new_folder = lupo_create(
                db,
                lupo_read_ex(db, "/types/directory", 0, 0),
                LUPO_INDEX(NULL)
            );
            if (new_folder.err != NULL) {
                return lupo_error_append(db, CANNOT_CREATE_DIR, new_folder.err);
            }

            lupo_add_entry_to_index(
                db,
                lupo_get_field(&it.entity, "links")->index, // links field already checked by iterator
                it.parts[it.index],
                new_folder._reference
            );
            Lupo_Error *dir_update_err = lupo_update(db, &it.entity);
            if (dir_update_err != NULL) {
                return lupo_error_append(db, CANNOT_UPDATE_DIR, dir_update_err);
            }

            it.entity = new_folder;
            it.index += 1;
        } break;
        }

        if (it.index >= (it.parts_qnt - 1)) {
            stop = LUPO_TRUE;
        }
    }

    Lupo_Field *links_field = lupo_get_field(&it.entity, "links");
    if (links_field == NULL) {
        return lupo_error(db, ENTITY_DOES_NOT_HAVE_A_LINKS_FIELD);
    }
    if (links_field->kind != LUPO_PRIMITIVE_INDEX || links_field->flags & LUPO_FIELD_IS_ARRAY) {
        return lupo_error(db, ENTITY_HAVE_INVALID_LINKS_FIELD);
    }

    lupo_add_entry_to_index(
        db,
        links_field->index,
        it.parts[it.parts_qnt - 1],
        entity._reference
    );
    Lupo_Error *dir_update_err = lupo_update(db, &it.entity);
    if (dir_update_err != NULL) {
        return lupo_error_append(db, CANNOT_UPDATE_DIR, dir_update_err);
    }

    return NULL;
}

Lupo_Error *
lupo_link_ex(Lupo_Context *db, Lupo_Entity entity, const dbchar *path, int args_qnt, const Lupo_Field args[])
{
    Lupo_Error *err = lupo_link_ex_proc(db, entity, path, args_qnt, args);
    lupo_commit_cached_pages(db);
    return err;
}

Lupo_Error *
lupo_unlink_ex_proc(Lupo_Context *db, const dbchar *path, int args_qnt, const Lupo_Field args[])
{
    Lupo_PathIterator it = lupo_create_path_iterator(db, path, args, args_qnt);
    if (it.err != NULL) {
        return it.err;
    }

    Lupo_Bool stop = LUPO_FALSE;
    while (!stop) {
        switch (lupo_path_iterator_next(db, &it)) {
        case LUPO_PATH_ITER_OK:        {                                       } break;
        case LUPO_PATH_ITER_FINISHED:  { stop = LUPO_TRUE;                          } break;
        case LUPO_PATH_ITER_NOT_FOUND: { return lupo_error(db, KEY_NOT_FOUND); } break;
        case LUPO_PATH_ITER_ERROR:     { return it.err;                        } break;
        }

        if (it.index >= (it.parts_qnt - 1)) {
            stop = LUPO_TRUE;
        }
    }

    Lupo_Field *links_field = lupo_get_field(&it.entity, "links");
    if (links_field == NULL) {
        return lupo_error(db, ENTITY_DOES_NOT_HAVE_A_LINKS_FIELD);
    }
    if (links_field->kind != LUPO_PRIMITIVE_INDEX || links_field->flags & LUPO_FIELD_IS_ARRAY) {
        return lupo_error(db, ENTITY_HAVE_INVALID_LINKS_FIELD);
    }

    lupo_remove_entry_to_index(db, links_field->index, it.parts[it.parts_qnt - 1]);
    Lupo_Error *dir_update_err = lupo_update(db, &it.entity);
    if (dir_update_err != NULL) {
        return lupo_error_append(db, CANNOT_UPDATE_DIR, dir_update_err);
    }

    return NULL;
}

Lupo_Error *
lupo_unlink_ex(Lupo_Context *db, const dbchar *path, int args_qnt, const Lupo_Field args[])
{
    Lupo_Error *err = lupo_unlink_ex_proc(db, path, args_qnt, args);
    lupo_commit_cached_pages(db);
    return err;
}

void
lupo_print_link_tree_rec(Lupo_Context *db, Lupo_Reference ref, int tabs_qnt)
{
    Lupo_Entity e = lupo_read_entity(db, ref);
    if (e.err != NULL) {
        lupo_print_error(e.err, tabs_qnt);
        return;
    }

    int qnt = 0;
    Lupo_IndexEntry *entries = lupo_list_links(db, e, &qnt);
    for (int i = 0; i < qnt; i += 1) {
        for (int tabs = 0; tabs < tabs_qnt; tabs += 1) {
            lupo_printf("\t");
        }
        lupo_printf("%s\n", entries[i].key);
        lupo_print_link_tree_rec(db, entries[i].reference, tabs_qnt + 1);
    }
}

void
lupo_print_link_tree(Lupo_Context *db)
{
    lupo_printf("/\n");
    lupo_print_link_tree_rec(db, lupo_root_dir_ref(), 1);
}

//-----------------------------------------------------------------------------
// Starup
//-----------------------------------------------------------------------------

void
lupo_startup_db(Lupo_Context *db)
{
    uint32_t null_page_index;
    Lupo_Page *null_page = lupo_create_page(db, &null_page_index);
    LUPO_ASSERT(null_page_index == 0);

    uint32_t types_page_index;
    Lupo_Page *types_page = lupo_create_page(db, &types_page_index);
    LUPO_ASSERT(types_page_index == 1);

    Lupo_ID id = lupo_generate_random_id(db);
    Lupo_Reference type_type_ref = lupo_create_reference(id, 1, 0);
    types_page->metadata.index = 1;
    types_page->metadata.type_ref = type_type_ref;

    //~~~ Creating type type
    // Yes, the type declaration needs a type, its weird, I know but I am doing this for the sake of consistency and less codepaths

    Lupo_Entity type_type = lupo_build_empty_entity(db, LUPO_TYPE_DECLARATION_FIELDS, LUPO_STATIC_ARRAY_LENGTH(LUPO_TYPE_DECLARATION_FIELDS));

    type_type.fields[0].uint32    = 0;
    type_type.fields[1].id        = id;
    type_type.fields[2].reference = type_type_ref;

    type_type.fields[3].uint32 = 1;
    type_type.fields[4].uint32 = 1;
    type_type.fields[5].uint32 = 1;
    type_type.fields[6].uint32 = 1;

    type_type.fields[7].string = "type_declaration";

    type_type.fields[8] = lupo_field_declarations_to_field(db, LUPO_TYPE_DECLARATION_FIELDS, LUPO_STATIC_ARRAY_LENGTH(LUPO_TYPE_DECLARATION_FIELDS));
    type_type.fields[8].name = "fields";

    int size = 0;
    uint8_t *blob = lupo_serialize_entity(db, &type_type, &size);

    Lupo_Address address = 0;
    lupo_io_write_data_into_page(db, types_page, blob, size, -1, &address);
    LUPO_ASSERT(LUPO_GET_PAGE_FROM_ADDRESS(address) == 1 && LUPO_GET_CHUNK_FROM_ADDRESS(address) == 0);

    type_type._reference = type_type_ref;

    //~~~ Ending creating type type

    //~~~ Creating base directories

    Lupo_Entity basic_dir_type = lupo_create(
        db,
        type_type,
        LUPO_U32(0),
        LUPO_U32(0),
        LUPO_U32(0),
        LUPO_U32(0),
        LUPO_STR("directory"),
        lupo_field_declarations_to_field(db, LUPO_BASIC_DIRECTORY_FIELDS, LUPO_STATIC_ARRAY_LENGTH(LUPO_BASIC_DIRECTORY_FIELDS))
    );

    Lupo_Index *types_dir_index = lupo_new(&db->request_arena, Lupo_Index);
    lupo_add_entry_to_index(db, types_dir_index, "type_declaration", type_type._reference);
    lupo_add_entry_to_index(db, types_dir_index, "directory", basic_dir_type._reference);

    Lupo_Index *root_dir_index = lupo_new(&db->request_arena, Lupo_Index);

    db->forced_next_id = LUPO_ROOT_DIR_ID; // way to have a constant root dir id. Never should be used elsewhere

    Lupo_Entity root_dir = lupo_create(
        db,
        basic_dir_type,
        LUPO_INDEX(root_dir_index)
    );

    Lupo_Entity types_dir = lupo_create(
        db,
        basic_dir_type,
        LUPO_INDEX(types_dir_index)
    );

    lupo_add_entry_to_index(db, root_dir_index, "types", types_dir._reference);
    lupo_update(db, &root_dir);
    LUPO_ASSERT(root_dir._reference.id == LUPO_ROOT_DIR_ID);
    LUPO_ASSERT(root_dir._reference.address == LUPO_ROOT_DIR_ADDRESS);
}

//-----------------------------------------------------------------------------
// File implementation
//-----------------------------------------------------------------------------

#ifndef LUPO_NO_STDLIB

#include <stdio.h>
#include <stdlib.h>

Lupo_Page *
lupo__file_load_page_callback(Lupo_Context *db, uint32_t page_index)
{
    //printf("-> loading page %d\n", page_index);

    FILE *fp = (FILE *)db->handle;

    fseek(fp, LUPO_PAGE_SIZE * page_index, SEEK_SET);

    Lupo_Page *page = lupo_new(&db->main_arena, Lupo_Page);

    fread(page, sizeof(*page), 1, fp);

    return page;
}

Lupo_Page *
lupo__file_create_empty_page_callback(Lupo_Context *db, uint32_t *index)
{
    FILE *fp = (FILE *)db->handle;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    long size_in_pages = size / LUPO_PAGE_SIZE;

    Lupo_Page *page = lupo_new(&db->main_arena, Lupo_Page);

    fwrite(page, sizeof(*page), 1, fp);

    *index = size_in_pages;

    //printf("-> creating page %d\n", *index);

    return page;
}

void
lupo__file_commit_page_callback(Lupo_Context *db, Lupo_Page *page)
{
    //printf("-> commit page %d\n", page->metadata.index);

    FILE *fp = (FILE *)db->handle;

    fseek(fp, LUPO_PAGE_SIZE * page->metadata.index, SEEK_SET);
    fwrite(page, sizeof(*page), 1, fp);
}

Lupo_Arena
lupo_malloc_arena(int size)
{
    void *ptr = malloc(size);
    lupo_memset(ptr, size, 0);

    Lupo_Arena arena;
    LUPO_ZERO_STRUCT(&arena);

    arena.used = 0;
    arena.capacity = size;
    arena.data = ptr;

    return arena;
}

Lupo_OpenStatus
lupo_open_from_file(Lupo_Context *db, Lupo_Arena request_arena, Lupo_Arena main_arena, const dbchar *file_name)
{
    Lupo_Bool exists = LUPO_FALSE;

    FILE *fp_tmp = fopen(file_name, "r");
    if (fp_tmp != NULL) {
        exists = LUPO_TRUE;
        fclose(fp_tmp);
    }

    if (!exists) {
        FILE *fp_tmp = fopen(file_name, "w");
        if (fp_tmp != NULL) {
            fclose(fp_tmp);
        }
    }

    FILE *fp = fopen(file_name, "rb+");
    if (fp == NULL) {
        return LUPO_OPEN_ERROR;
    }

    db->request_arena     = request_arena;
    db->main_arena        = main_arena;
    db->handle            = fp;
    db->create_empty_page = lupo__file_create_empty_page_callback;
    db->load_page         = lupo__file_load_page_callback;
    db->commit_page       = lupo__file_commit_page_callback;

    if (!exists) {
        //printf("starting database\n");
        lupo_startup_db(db);
    }

    return exists ? LUPO_OPEN_OK : LUPO_OPEN_NEW;
}

#endif

//-----------------------------------------------------------------------------
// Testing
//-----------------------------------------------------------------------------

#ifdef LUPO_TEST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

uint8_t *buffer;
uint8_t *ram_buffer;
int      ram_buffer_used = 0;
int      pages_qnt = 0;

uint8_t *
test_ram_alloc(Lupo_Context *db, uint32_t size, const dbchar *func, int line)
{
    ram_buffer_used += 8 - (ram_buffer_used % 8);
    uint8_t *b = &ram_buffer[ram_buffer_used];
    ram_buffer_used += size;
    return b;
}

Lupo_Page *
test_load_page(Lupo_Context *db, uint32_t page_index)
{
    return (Lupo_Page *)&buffer[LUPO_PAGE_SIZE * page_index];
}

Lupo_Page *
test_create_empty_page(Lupo_Context *db, uint32_t *index)
{
    *index = pages_qnt;
    pages_qnt += 1;

    return (Lupo_Page *)&buffer[LUPO_PAGE_SIZE * (*index)];
}

void
test_commit_page(Lupo_Context *db, Lupo_Page *page)
{

}

int
main()
{
    buffer = (uint8_t *)malloc(1024 * 1024);
    ram_buffer = (uint8_t *)malloc(1024 * 1024);

    memset(buffer, 0, 1024 * 1024);
    memset(ram_buffer, 0, 1024 * 1024);

    Lupo_Context db = { 0 };
    db.ram_alloc = test_ram_alloc;
    db.create_empty_page = test_create_empty_page;
    db.load_page = test_load_page;
    db.commit_page = test_commit_page;
    lupo_startup_db(&db);

    //lupo_printf("<<<:::--- READING ---:::>>>\n");

    Lupo_Entity user_type = lupo_create_type(
        &db,
        "users",
        LUPO_FIELD_DECL("name", STRING, 0),
        LUPO_FIELD_DECL("age", UINT32, 0)
    );


    LUPO_ASSERT(lupo_link(&db, user_type, "/types/yay/user") == NULL);

    Lupo_Entity user = lupo_create(
        &db,
        lupo_read(&db, "/types/yay/user"),
        LUPO_STR("Igor"),
        LUPO_U32(24)
    );

    LUPO_ASSERT(lupo_link(&db, user, "/types/yay/igor") == NULL);

    //printf("type:\n");
    lupo_print(lupo_read(&db, "/types/yay/user"));

    //printf("igor:\n");
    lupo_print(lupo_read(&db, "/types/yay/igor"));

    lupo_get_field(&user, "name")->string = "Yay";
    lupo_update(&db, &user);

    //printf("not existent:\n");
    lupo_print(lupo_read(&db, "/types/batatinha"));

    //lupo_unlink(&db, "/types/user");

    //lupo_print(lupo_read(&db, "/types"));

    lupo_print_link_tree(&db);

    return 0;
}

#endif
