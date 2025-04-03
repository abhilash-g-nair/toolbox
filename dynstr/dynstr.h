#pragma once

#include <stddef.h>

struct DynStrHeap {
    char *str;
    size_t size;
};

struct DynStrStack {
    char str[sizeof(struct DynStrHeap)];
};

union DynStrMem {
    struct DynStrHeap heap;
    struct DynStrStack stack;
};

struct DynStr {
    size_t len;
    union DynStrMem mem;
};

typedef struct DynStr DynStr;

void dynstr_init(DynStr *ds);
void dynstr_free(DynStr *ds);
size_t dynstr_len(const DynStr *ds);
char *dynstr_str(const DynStr *ds);
int dynstr_append_str(DynStr *ds, const char *s);
void dynstr_append_dynstr(DynStr *ds, const DynStr *other);
