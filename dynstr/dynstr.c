#include <string.h>
#include <stdlib.h>
#include "dynstr.h"
#include <stdio.h>

#define DYNSTR_INIT_SIZE    32

void dynstr_init(DynStr *ds) {
    ds->len = 0;
    memset(ds->mem.stack.str, 0, sizeof(ds->mem.stack.str));
}

void dynstr_free(DynStr *ds) {
    if (ds->len >= sizeof(ds->mem.stack.str)) free(ds->mem.heap.str);
    ds->len = 0;
    ds->mem.heap.str = NULL;
    ds->mem.heap.size = 0;
}

size_t dynstr_len(const DynStr *ds) {
    return ds->len;
}

char *dynstr_str(const DynStr *ds) {
    const char *s = ds->len < sizeof(ds->mem.stack.str) ? ds->mem.stack.str : ds->mem.heap.str;
    return (char *)s;
}

int dynstr_append_str(DynStr *ds, const char *s) {
    int ret = 0;
    size_t s_len = strlen(s);

    if (ds->len + s_len < sizeof(ds->mem.stack.str)) {
        memcpy(ds->mem.stack.str + ds->len, s, s_len + 1);
        ds->len += s_len;
    }
    else if (ds->len < sizeof(ds->mem.stack.str)) {
        size_t n = DYNSTR_INIT_SIZE;
        while (ds->len + s_len >= n) n *= 2;
        char *p = realloc(NULL, n);
        if (p == NULL) {
            ret = -1;
            goto done;
        }
        memcpy(p, ds->mem.stack.str, ds->len);
        memcpy(p + ds->len, s, s_len + 1);
        ds->mem.heap.str = p;
        ds->mem.heap.size = n;
        ds->len += s_len;
    }
    else {
        if (ds->len + s_len >= ds->mem.heap.size) {
            if (ds->mem.heap.size == 0) ds->mem.heap.size = DYNSTR_INIT_SIZE;
            while (ds->len + s_len >= ds->mem.heap.size) ds->mem.heap.size *= 2;
            char *p = realloc(ds->mem.heap.str, ds->mem.heap.size);
            if (p == NULL) {
                ret = -1;
                goto done;
            }
            ds->mem.heap.str = p;
        }
        memcpy(ds->mem.heap.str + ds->len, s, s_len + 1);
        ds->len += s_len;
    }

done:
    return ret;
}

void dynstr_append_dynstr(DynStr *ds, const DynStr *other) {
    dynstr_append_str(ds, dynstr_str(other));
}
