#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../includes/neonstring.h"

#define CAPACITY 512

static inline size_t round_capacity(size_t capacity) {
	capacity--;
	capacity |= capacity >> 1;
	capacity |= capacity >> 2;
	capacity |= capacity >> 4;
	capacity |= capacity >> 8;
	capacity |= capacity >> 16;
	capacity++;

	return capacity;
}

static inline bool check_capacity(nestring_t *string, size_t size) {
	return (string->capacity > (string->length + size)) ? true : false;
}

static nestring_t *string_alloc(size_t capacity) {
	capacity = capacity ? round_capacity(capacity) : CAPACITY;

	nestring_t *string = (nestring_t*)malloc(sizeof(nestring_t));
	if (string == NULL) {
		fprintf(stderr, "Wasn't able to allocate a string!\n");
		exit(1);
	}

	string->data = (char*)malloc(sizeof(char) * capacity);
	memset(string->data, 0, capacity);

	string->capacity = capacity;
	string->length = 0;

	return string;
}

static void string_realloc(nestring_t *string, size_t minimum_size) {
	minimum_size = round_capacity(minimum_size);
	string->data = (char*)realloc(string->data, minimum_size);
	if (string->data == NULL) {
		fprintf(stderr, "Wasn't able to realloc the string!\n");
		exit(1);
	}

	string->capacity = minimum_size;
	memset(string->data + string->capacity, 0, minimum_size);
}

void string_assign(nestring_t *string, char *restrict data) {
	size_t data_length = strlen(data);
	if (!check_capacity(string, data_length)) {
		string_realloc(string, string->capacity + data_length);
	}

	size_t cur_length = string->length;
	if (cur_length > data_length) {
		memset(string->data + data_length - 1, 0, cur_length - data_length);
	}

	string->length = data_length;
	memcpy(string->data, data, data_length);
}

nestring_t *string_new(char *restrict data, size_t capacity) {
	nestring_t *string = string_alloc(capacity);
	string_assign(string, data);

	return string;
}

void string_append(nestring_t *string, char *restrict data) {
	size_t data_length = strlen(data);
	if (!check_capacity(string, string->length + data_length)) {
		string_realloc(string, string->capacity + data_length);
	}

	size_t cur_length = string->length;

	string->length += data_length;
	memcpy(string->data + cur_length, data, data_length);
}

void string_free(nestring_t *string) {
	free(string->data);
	free(string);
}

size_t string_push(nestring_t *string, char c) {
	if (!check_capacity(string, 1)) {
		string_realloc(string, string->capacity + 1);
	}
	string->data[string->length] = c;
	string->length++;

	return string->length;
}

char string_pop(nestring_t *string) {
	char c = string->data[string->length - 1];
	string->data[string->length - 1] = 0;

	string->length--;

	return c;
}
