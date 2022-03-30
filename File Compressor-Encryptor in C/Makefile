CC =  clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: encode decode entropy error

entropy: entropy.o
	 $(CC) -o entropy $^ -lm

entropy.o: entropy.c
	 $(CC) $(CFLAGS) -c entropy.c

error: error.o
	 $(CC) -o error $^ -lm

error.o: error.c
	 $(CC) $(CFLAGS) -c error.c

encode: encode.o hamming.o bm.o bv.o
	 $(CC) -o encode $^ -lm

encode.o: encode.c
	 $(CC) $(CFLAGS) -c encode.c

decode: decode.o hamming.o bm.o bv.o
	 $(CC) -o decode $^ -lm

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

format:
	 clang-format -i -style=file encode.[ch]
	 clang-format -i -style=file decode.[ch]

clean:
	 rm -f encode encode.o decode decode.o bv bv.o bm bm.o hamming hamming.o entropy entropy.o error error.o

scan-build: clean
	 scan-build make

