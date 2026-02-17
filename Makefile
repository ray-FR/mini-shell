#Credits: Pablo Rauzy (pablo.rauzy.name)

SRC = $(wildcard *.c)
OBJ = $(subst .c,.o,$(SRC))
BIN = $(subst .c,,$(SRC))

mini-shell: $(OBJ)
	gcc $(OBJ) -o $@

%: %.c
	gcc -W -Wall -Werror -DSTANDALONE $< -o $@

%.o: %.c
	gcc -c -W -Wall -Werror $< -o $@

clean::
	rm -f $(OBJ) $(BIN)
	rm -f .DS_Store
