
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,./obj/%.o,$(SRC))
BIN = $(subst .c,,$(SRC))

mini-shell: $(OBJ)
	gcc -g $(OBJ) -o $@

%: %.c
	gcc -W -Wall -Werror -DSTANDALONE $< -o $@

./obj/%.o: %.c
	@mkdir -p ./obj
	gcc -c -W -Wall -g -Werror $< -o $@

clean::
	rm -rf ./obj/
	rm -f $(BIN)
	rm -f .DS_Store
