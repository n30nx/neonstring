#include <stdio.h>
#include <stdlib.h>

#include "includes/neonstring.h"

int main(void) {
	neonstring_t *string = neonstring_new("Hello World", 2);
	printf("%s\n", string->data);

	neonstring_assign(string, "asdabc");
	printf("%s\n", string->data);

	char x = neonstring_pop(string);
	printf("%c %s\n", x, string->data);

	size_t size = neonstring_push(string, 'q');
	printf("%zu %s\n", size, string->data);

	neonstring_append(string, "wwwwqqqq");
	printf("%zu %zu %s\n", string->length, string->capacity, string->data);

	neonstring_free(string);

	return 0;
}
