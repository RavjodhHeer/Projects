# Text Censor-Firewall

The main program banhammer is run with a given file and takes in two files of badspeak and newspeak containing forbidden and changed words. Based on those words, the program reads through an input and detects any forbidden or new replacemente words and outputs the corresponding message for the "crime" that the citizen of the GPSRC has comitted. The program implements the use of a hashtable of linked lists and a bloomfilter to filter and check words and confirm if the input contains any badspeak or replacement newspeak.

## Build

	$ clang -Wall -Wextra -Werror -Wpedantic -o banhammer.c banhammer.o ht.c ht.o ll.c ll.o node.c node.o bv.c bv.o bf.c bf.o speck.c speck.o parser.c parser.o
 
## Running

	$ ./banhammer -[ht:f:ms]
	
## Cleaning

	$ rm -f banhammer.c banhammer ht.c ht ll.c ll node.c node bv.c bv bf.c bf speck.c speck parser.c parser

