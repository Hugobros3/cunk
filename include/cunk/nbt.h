#ifndef CUNK_NBT_H
#define CUNK_NBT_H

typedef struct Arena_ Arena;

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef union NBT_Body_ NBT_Body;
typedef struct NBT_Object_ NBT_Object;

#define NBT_TAG_TYPES(T) \
T(Byte, int8_t) \
T(Short, int16_t) \
T(Int, int32_t) \
T(Long, int64_t) \
T(Float, float) \
T(Double, double) \
T(ByteArray, struct { int32_t count; int8_t* arr; }) \
T(String, const char*) \
T(List, NBT_List) \
T(Compound, NBT_Compound) \
T(IntArray,  struct { int32_t count; int32_t* arr; }) \
T(LongArray, struct { int32_t count; int64_t* arr; }) \

typedef enum {
    NBT_Tag_End,
#define T(name, t) NBT_Tag_##name,
    NBT_TAG_TYPES(T)
#undef T
} NBT_Tag;

typedef struct {
    NBT_Tag tag;
    int32_t count;
    NBT_Body* bodies;
} NBT_List;

typedef struct {
    int32_t count;
    NBT_Object** objects;
} NBT_Compound;

union NBT_Body_ {
#define T(name, t) t name;
NBT_TAG_TYPES(T)
#undef T
};

struct NBT_Object_ {
    NBT_Tag tag;
    const char* name;
    NBT_Body body;
};

NBT_Object* cunk_decode_nbt(size_t buffer_size, const char* buffer, Arena*);

typedef struct Printer_ Printer;
void cunk_print_nbt(Printer*, NBT_Object*);

#endif
