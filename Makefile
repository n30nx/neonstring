build:
	gcc main.c src/neonstring.c -I includes -lm -o main -Wall -Wextra -Werror -std=c99

debug:
	gcc main.c src/neonstring.c -I includes -lm -o main -Wall -Wextra -Werror -std=c99 -fsanitize=address,undefined -D DEBUG -g
