cc=gcc
cflags=-Wall -g -std=gnu11
target=disp
src=src/main.c src/pcb_queue.c src/utils.c

.phony: all
all: $(target)

$(target): $(src:src/*.c=*.o)
	$(cc) -o $(target) $(cflags) $^

$(src:.c=.o): $(src)
	$(cc) -c $(src) $(cflags) $?

.phony: clean
clean:
	rm -f *.o
	rm -f $(target)	

.phony: build_dummy_prog
build_dummy_prog:
	$(cc) -o program src/program.c src/utils.c