#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../includes/neonstring.h"

#define CAPACITY 512

#ifdef DEBUG
#define debug(fmt, ...) do {                                              \
        fprintf(stdout, "%s:%s:%d: ", __FILE__, __FUNCTION__, __LINE__);  \
        fprintf(stdout, fmt, __VA_ARGS__);                                \
        fprintf(stdout, "\n");                                            \
} while (0)
#else
#define debug(...)
#endif

static const int log_table[64] = {
    63,  0, 58,  1, 59, 47, 53,  2,
    60, 39, 48, 27, 54, 33, 42,  3,
    61, 51, 37, 40, 49, 18, 28, 20,
    55, 30, 34, 11, 43, 14, 22,  4,
    62, 57, 46, 52, 38, 26, 32, 41,
    50, 36, 17, 19, 29, 10, 13, 21,
    56, 45, 25, 31, 35, 16,  9, 12,
    44, 24, 15,  8, 23,  7,  6,  5
};

/*
 * de Brujin sequence
 * http://supertech.csail.mit.edu/papers/debruijn.pdf
 */
__attribute__((always_inline))
static inline int u64_log2(size_t cap) {
    cap |= cap >> 1;
    cap |= cap >> 2;
    cap |= cap >> 4;
    cap |= cap >> 8;
    cap |= cap >> 16;
    cap |= cap >> 32;
    return log_table[((cap - (cap >> 1)) * 0x07EDD5E59A4E28C2) >> 58];
}

__attribute__((always_inline))
static inline size_t round_capacity(size_t capacity) {
    size_t cur = capacity;
    debug("cur: %ld", cur); 

    int log_cap = u64_log2(capacity);
    capacity = 1 << ((size_t)log_cap + 1);
    debug("capacity: %ld", capacity);

    if (cur >= capacity) {
        return 0;
    }

    return capacity;
}

__attribute__((always_inline))
static inline bool check_capacity(neonstring_t *string, size_t size) {
    return (string->capacity > (string->length + size)) ? true : false;
}

static neonstring_t *neonstring_alloc(size_t capacity) {
    capacity = capacity ? round_capacity(capacity) : CAPACITY;

    neonstring_t *string = (neonstring_t*)malloc(sizeof(neonstring_t));
    if (string == NULL) {
        fprintf(stderr, "Wasn't able to allocate a string!\n");
        exit(1);
    }

    string->data = (char*)malloc(sizeof(char) * capacity);
    if (string->data == NULL) {
        fprintf(stderr, "Wasn't able to allocate data in a string!\n");
        exit(1);
    }
    memset(string->data, 0, capacity);

    string->capacity = capacity;
    string->length = 0;

    return string;
}

static void neonstring_realloc(neonstring_t *string, size_t minimum_size) {
    size_t before = minimum_size;
    minimum_size = round_capacity(minimum_size);
    if (minimum_size == 0) {
        fprintf(stderr, "Cannot allocate a bigger string than size %ld\n", before);
        exit(1);
    }
    debug("minimum_size: %ld", minimum_size);
    string->data = (char*)realloc(string->data, sizeof(char) * minimum_size);
    if (string->data == NULL) {
        fprintf(stderr, "Wasn't able to realloc the string!\n");
        exit(1);
    }

    string->capacity = minimum_size;
    memset(string->data + string->length, 0, minimum_size - string->length);
}

void neonstring_assign(neonstring_t *string, char *restrict data) {
    size_t data_length = strlen(data);
    if (string->capacity < data_length - 1) {
        debug("realloc: %s %ld %ld %ld", string->data, string->capacity, string->length, data_length);
        neonstring_realloc(string, string->capacity + data_length);
    }

    size_t cur_length = string->length;
    if (cur_length > data_length) {
        memset(string->data + data_length - 1, 0, cur_length - data_length);
    }

    string->length = data_length;
    memcpy(string->data, data, data_length);
}

neonstring_t *neonstring_new(char *restrict data, size_t capacity) {
    neonstring_t *string = neonstring_alloc(capacity);
    neonstring_assign(string, data);

    return string;
}

void neonstring_append(neonstring_t *string, char *restrict data) {
    size_t data_length = strlen(data);
    if (!check_capacity(string, string->length + data_length)) {
        neonstring_realloc(string, string->capacity + data_length);
    }

    size_t cur_length = string->length;

    string->length += data_length;
    memcpy(string->data + cur_length, data, data_length);
}

void neonstring_free(neonstring_t *string) {
    free(string->data);
    free(string);
}

size_t neonstring_push(neonstring_t *string, char c) {
    if (!check_capacity(string, 1)) {
        neonstring_realloc(string, string->capacity + 1);
    }
    string->data[string->length] = c;
    string->length++;

    return string->length;
}

char neonstring_pop(neonstring_t *string) {
    char c = string->data[string->length - 1];
    string->data[string->length - 1] = 0;

    string->length--;

    return c;
}
