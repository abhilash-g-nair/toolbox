#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dynstr.h"

//#define RUN_TEST(f) printf("Running test: %-50s result: %s\n", #f, f() ? "Pass" : "Fail")
#define RUN_TEST(f) printf("%s::%s\n", #f, f() ? "Pass" : "Fail")

static void dynstr_print(DynStr *ds) {
    printf("DynStr len: %zu str: %s", dynstr_len(ds), dynstr_str(ds));
}

static bool is_on_heap(DynStr *ds) {
    return dynstr_str(ds) == ds->mem.heap.str;
}

static bool is_on_stack(DynStr *ds) {
    return !is_on_heap(ds);
}

static bool test_dynstr_init(void) {
    DynStr ds; 
    dynstr_init(&ds);
    void *p = calloc(1, sizeof(DynStr));
    return memcmp(&ds, p, sizeof(DynStr)) == 0;
}

static bool test_dynstr_append_str(void) {
    DynStr ds;
    dynstr_init(&ds);
    const char *s = "hello";
    int ret = dynstr_append_str(&ds, s);
    return ret == 0; 
}

static bool test_dynstr_append_str_on_stack(void) {
    DynStr ds;
    dynstr_init(&ds);
    char s[sizeof(ds.mem.stack.str)];
    memset(s, 'a', sizeof(s));
    s[sizeof(s) - 1] = '\0';
    int ret = dynstr_append_str(&ds, s);
    if (ret != 0) return false;
    if (strcmp(s, dynstr_str(&ds)) != 0) return false;
    return is_on_stack(&ds);
}

static bool test_dynstr_append_str_on_heap(void) {
    DynStr ds;
    dynstr_init(&ds);
    char s[sizeof(ds.mem.stack.str) + 1];
    memset(s, 'a', sizeof(s));
    s[sizeof(s) - 1] = '\0';
    int ret = dynstr_append_str(&ds, s);
    if (ret != 0) return false;
    if (strcmp(s, dynstr_str(&ds)) != 0) return false;
    return is_on_heap(&ds);
}


int main(void) {
    RUN_TEST(test_dynstr_init);
    RUN_TEST(test_dynstr_append_str);
    RUN_TEST(test_dynstr_append_str_on_stack);
    RUN_TEST(test_dynstr_append_str_on_heap);
    return 0;
}
