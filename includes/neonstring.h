#pragma once

struct neonstring {
    size_t length;
    size_t capacity;
    char *data;
} __attribute__((packed));

typedef struct neonstring neonstring_t;

neonstring_t *neonstring_new(char *restrict data, size_t capacity);
void neonstring_free(neonstring_t *string);
void neonstring_assign(neonstring_t *string, char *restrict data);
void neonstring_append(neonstring_t *string, char *restrict data);
size_t neonstring_push(neonstring_t *string, char c);
char neonstring_pop(neonstring_t *string);
