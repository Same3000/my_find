CC= gcc
CFLAGS= -Wall -Werror -Wextra -pedantic -std=c99 -Wvla
DFLAGS= -fsanitize=address -g3
BIN = src/main
OBJ = src/main.o src/simple_ls.o src/tokenizer.o src/stack.o src/shunting.o src/eval.o src/file.o

all: myfind

check:
	 $(CC) $(CFLAGS) $(DFLAGS) src/main.c src/simple_ls.c src/tokenizer.c src/stack.c src/shunting.c src/eval.c src/file.c -o myfind
	tests/testfonc.sh

myfind: $(OBJ)
	$(CC) ${OBJ} -o myfind

clean:
	$(RM) ${OBJ}
	${RM} myfind
	${RM} .find.out
	${RM} .myfind.out


.PHONY: clean $(BIN)
