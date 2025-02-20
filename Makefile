flags=-Wall -O2 -std=c2x
ldflags=

all: clean tree

tree: tree.o
	cc ${flags} $^ -o $@ ${ldflags}

tree.0: tree.c
	cc ${flags} -c $^

clean:
	rm -rf *.o tree
