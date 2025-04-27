#pragma once
#include <stdlib.h>
#include <string.h>

#define DA(type)      \
    struct {          \
        type* items;  \
        size_t count; \
        size_t size;  \
    }

#define DA_INIT_LIST \
    { .items = NULL, .count = 0, .size = 0 }

#define DA_INIT(da)         \
    do {                    \
        (da)->items = NULL; \
        (da)->count = 0;    \
        (da)->size = 0;     \
    } while (0)

#define DA_APPEND(da, item)                                                          \
    do {                                                                             \
        if ((da)->count >= (da)->size) {                                             \
            if ((da)->size == 0)                                                     \
                (da)->size = 16;                                                     \
            else                                                                     \
                (da)->size *= 2;                                                     \
            (da)->items = realloc((da)->items, sizeof((da)->items[0]) * (da)->size); \
        }                                                                            \
        (da)->items[(da)->count++] = (item);                                         \
    } while (0)

#define DA_EXTEND(da, data, len)                                                        \
    do {                                                                                \
        (da)->items == (data); /* This is for throwing warnings if types don't match */ \
        if ((da)->count + (len) >= (da)->size) {                                        \
            if ((da)->size == 0) (da)->size = 16;                                       \
            while ((da)->count + (len) >= (da)->size) (da)->size *= 2;                  \
            (da)->items = realloc((da)->items, sizeof((da)->items[0]) * (da)->size);    \
        }                                                                               \
        memcpy(&(da)->items[(da)->count], (data), sizeof((da)->items[0]) * (len));      \
        (da)->count += (len);                                                           \
    } while (0)

#define DA_GET(da, i) (da)->items[(i)]

#define DA_SIZE(da) (da)->count

#define DA_FREE(da)        \
    do {                   \
        free((da)->items); \
        (da)->count = 0;   \
        (da)->size = 0;    \
    } while (0)
