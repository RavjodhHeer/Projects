# File Compressor-Encryptor

Two programs, encode and decode use the Hamming(8, 4) formula to read in bytes from a file and encode them as well as decode them. The bytes in each file are taken in and multiplied with generator matrices to encode them and return them to a outfile. Encoded bits are then read by the decode function two bytes at a time and any errors due to noise or any other reason are attempted to be fixed and resulting message is returned.

## Build

	$ clang -Wall -Wextra -Werror -Wpedantic -o encode.c encode.o decode.c decode.o hamming.c hamming.o bm.c bm.o bv.c bv.o
 
## Running

	$ ./encode -[hi:o:]
	$ ./decode -[hvi:o:]
	
## Cleaning

	$ rm -f encode encode.c decode decode.c hamming hamming.c bv bv.c bm bm.c

