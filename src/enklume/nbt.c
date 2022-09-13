#include "cunk/nbt.h"
#include "cunk/memory.h"
#include "cunk/log.h"

#include "../common/common_private.h"

#include <assert.h>
#include <string.h>

#pragma GCC diagnostic error "-Wswitch"

static_assert(sizeof(float) == sizeof(int32_t),  "what kind of platform is this");
static_assert(sizeof(double) == sizeof(int64_t), "what kind of platform is this");

static const char* validate_in_bounds(const char* buf, const char* bound, size_t off) {
    assert(buf + off <= bound);
    return buf + off;
}

#define read(T) (T) swap_endianness(sizeof(T), *(T*) ((*buffer = validate_in_bounds(*buffer, buffer_end, sizeof(T))) - sizeof(T)) )
#define advance_bytes(b) ((*buffer = validate_in_bounds(*buffer, buffer_end, (b))) - (b))

static NBT_Object* cunk_decode_nbt_impl(const char*, const char*, const char**, Arena* arena);

static NBT_Body cunk_decode_nbt_body(NBT_Tag tag, const char* buffer_start, const char* const buffer_end, const char** const buffer, Arena* arena) {
    NBT_Body body;
    switch (tag) {
        case NBT_Tag_Byte:   body.Byte   = read(int8_t);  break;
        case NBT_Tag_Short:  body.Short  = read(int16_t); break;
        case NBT_Tag_Int:    body.Int    = read(int32_t); break;
        case NBT_Tag_Long:   body.Long   = read(int64_t); break;
        case NBT_Tag_Float:  body.Float  = read(float);   break;
        case NBT_Tag_Double: body.Double = read(double);  break;
        case NBT_Tag_ByteArray: {
            int32_t size = body.ByteArray.count = read(int32_t);
            assert(size >= 0);
            int8_t* data = body.ByteArray.arr = cunk_arena_alloc_bytes(arena, sizeof(int8_t) * size);
            advance_bytes(sizeof(int8_t) * size);
            memcpy(data, *buffer, sizeof(int8_t) * size);
            break;
        } case NBT_Tag_String: {
            uint16_t size = read(uint16_t);
            char* str;
            body.String = str = cunk_arena_alloc_bytes(arena, size + 1);
            memcpy(str, *buffer, size * sizeof(int8_t));
            advance_bytes(size * sizeof(int8_t));
            str[size] = '\0';
            break;
        }
        case NBT_Tag_List: {
            NBT_Tag elements_tag = body.List.tag = read(uint8_t);
            int32_t elements_count = body.List.count = read(int32_t);
            assert(elements_count >= 0);
            NBT_Body* arr = body.List.bodies = cunk_arena_alloc_bytes(arena, sizeof(NBT_Body) * elements_count);
            for (int32_t i = 0; i < elements_count; i++)
                arr[i] = cunk_decode_nbt_body(elements_tag, buffer_start, buffer_end, buffer, arena);
            break;
        }
        case NBT_Tag_Compound: {
            Growy* tmp = cunk_new_growy();
            int32_t size = 0;
            while (true) {
                NBT_Object* o = cunk_decode_nbt_impl(buffer_start, buffer_end, buffer, arena);
                if (!o)
                    break;
                size++;
                cunk_growy_append(tmp, o);
            }
            body.Compound.count = size;
            NBT_Object** arr = body.Compound.objects = cunk_arena_alloc_bytes(arena, sizeof(NBT_Object*) * size);
            memcpy(arr, cunk_growy_data(tmp), sizeof(NBT_Object*) * size);
            cunk_growy_destroy(tmp);
            break;
        }
        case NBT_Tag_IntArray: {
            int32_t size = body.IntArray.count = read(int32_t);
            assert(size >= 0);
            int32_t* data = body.IntArray.arr = cunk_arena_alloc_bytes(arena, sizeof(int32_t) * size);
            advance_bytes(sizeof(int32_t) * size);
            memcpy(data, *buffer, sizeof(int32_t) * size);
            break;
        }
        case NBT_Tag_LongArray: {
            int32_t size = body.LongArray.count = read(int32_t);
            assert(size >= 0);
            int64_t* data = body.LongArray.arr = cunk_arena_alloc_bytes(arena, sizeof(int64_t) * size);
            advance_bytes(sizeof(int64_t) * size);
            memcpy(data, *buffer, sizeof(int64_t) * size);
            break;
        }
        default: error("unknown tag");
    }
    return body;
}

static NBT_Object* cunk_decode_nbt_impl(const char* buffer_start, const char* const buffer_end, const char** const buffer, Arena* arena) {
    NBT_Tag tag = read(uint8_t);
    if (tag == NBT_Tag_End)
        return NULL;

    NBT_Object* o = cunk_arena_alloc(NBT_Object, arena);
    o->tag = tag;

    uint16_t name_size = read(uint16_t);
    char* name = cunk_arena_alloc_bytes(arena, name_size + 1);
    memcpy(name, *buffer, name_size * sizeof(int8_t));
    advance_bytes(name_size * sizeof(int8_t));
    name[name_size] = '\0';
    o->name = name;

    o->body = cunk_decode_nbt_body(tag, buffer_start, buffer_end, buffer, arena);
    return o;
}

NBT_Object* cunk_decode_nbt(size_t buffer_size, const char* buffer, Arena* arena) {
    return cunk_decode_nbt_impl(buffer, buffer + buffer_size, &buffer, arena);
}
