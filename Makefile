
SRC = $(wildcard ./src/*.c)
OBJ = $(patsubst ./src/%.c,./obj/%.o,$(SRC))

mini-shell: $(OBJ)
	gcc -g $(OBJ) -o $@

./obj/%.o: ./src/%.c
	@mkdir -p ./obj
	gcc -c -W -Wall -g -Werror $< -o $@

clean::
	rm -rf ./obj/
	rm -f ./mini-shell
	rm -f .DS_Store
