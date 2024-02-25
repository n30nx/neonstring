#pragma once

struct nestring {
	size_t length;
	size_t capacity;
	char *data;
};

typedef struct nestring nestring_t;

nestring_t *string_new(char *restrict data, size_t capacity);
void string_free(nestring_t *string);
void string_assign(nestring_t *string, char *restrict data);
void string_append(nestring_t *string, char *restrict data);
size_t string_push(nestring_t *string, char c);
char string_pop(nestring_t *string);
