CC =  clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: banhammer

banhammer: banhammer.o ht.o node.o ll.o speck.o parser.o bf.o bv.o
	 $(CC) -o banhammer $^ -lm

banhammer.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c

format:
	 clang-format -i -style=file banhammer.[ch]

clean:
	 rm -f banhammer banhammer.o ht ht.o ll ll.o bv bv.o bf bf.o node node.o speck speck.o parser parser.o

scan-build: clean
	 scan-build make

