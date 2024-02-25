#include <stdio.h>
#include <stdlib.h>

#include "includes/neonstring.h"

int main(void) {
	nestring_t *string = string_new("Hello World", 0);
	printf("%s\n", string->data);

	string_assign(string, "asdabc");
	printf("%s\n", string->data);

	char x = string_pop(string);
	printf("%c %s\n", x, string->data);

	size_t size = string_push(string, 'q');
	printf("%zu %s\n", size, string->data);

	string_append(string, "wwwwqqqq");
	printf("%zu %zu %s\n", string->length, string->capacity, string->data);

	free(string);

	return 0;
}
