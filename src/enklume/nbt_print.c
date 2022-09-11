#include "cunk/print.h"
#include "cunk/nbt.h"

#pragma GCC diagnostic error "-Wswitch"

static const char* nbt_tag_name[] = {
"End",
#define T(name, _) #name,
NBT_TAG_TYPES(T)
#undef T
};

static void print_nbt_body(Printer* p, NBT_Tag tag, NBT_Body body) {
    switch (tag) {
        case NBT_Tag_End:    cunk_print(p, "END"); return;
        case NBT_Tag_Byte:   cunk_print(p, "%d", body.Byte); break;
        case NBT_Tag_Short:  cunk_print(p, "%d", body.Short); break;
        case NBT_Tag_Int:    cunk_print(p, "%d", body.Int); break;
        case NBT_Tag_Long:   cunk_print(p, "%d", body.Long); break;
        case NBT_Tag_Float:  cunk_print(p, "%f", body.Float); break;
        case NBT_Tag_Double: cunk_print(p, "%f", body.Double); break;
        case NBT_Tag_ByteArray:
            cunk_print(p, "[");
            for (int32_t i = 0; i < body.ByteArray.count; i++) {
                cunk_print(p, "%d", body.ByteArray.arr[i]);
                if (i + 1 < body.ByteArray.count)
                    cunk_print(p, ", ");
            }
            cunk_print(p, "]");
            break;
        case NBT_Tag_String:
            cunk_print(p, "\"%s\"", body.String);
            break;
        case NBT_Tag_List:
            cunk_print(p, "(%s []) [", nbt_tag_name[body.List.tag]);
            for (int32_t i = 0; i < body.List.count; i++) {
                print_nbt_body(p, body.List.tag, body.List.bodies[i]);
                if (i + 1 < body.List.count)
                    cunk_print(p, ", ");
            }
            cunk_print(p, "]");
            break;
        case NBT_Tag_Compound:
            cunk_print(p, "{");
            cunk_indent(p);
            cunk_newline(p);
            for (int32_t i = 0; i < body.Compound.count; i++) {
                cunk_print_nbt(p, body.Compound.objects[i]);
                if (i + 1 < body.Compound.count)
                    cunk_newline(p);
            }
            cunk_deindent(p);
            cunk_newline(p);
            cunk_print(p, "}");
            break;
        case NBT_Tag_IntArray:
            cunk_print(p, "[");
            for (int32_t i = 0; i < body.IntArray.count; i++) {
                cunk_print(p, "%d", body.IntArray.arr[i]);
                if (i + 1 < body.IntArray.count)
                    cunk_print(p, ", ");
            }
            cunk_print(p, "]");
            break;
        case NBT_Tag_LongArray:
            cunk_print(p, "[");
            for (int32_t i = 0; i < body.LongArray.count; i++) {
                cunk_print(p, "%d", body.LongArray.arr[i]);
                if (i + 1 < body.LongArray.count)
                    cunk_print(p, ", ");
            }
            cunk_print(p, "]");
            break;
    }
}

void cunk_print_nbt(Printer* p, NBT_Object* o) {
    cunk_print(p, "%s %s = ", nbt_tag_name[o->tag], o->name);
    print_nbt_body(p, o->tag, o->body);
}
