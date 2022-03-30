#include "bm.h"
#include "bv.h"
#include "hamming.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define OPTIONS "hi:o:"

// lower/upper nibble and packing/unpacking code supplied in assignment document

uint8_t lowernibble(uint8_t val) {
    return val & 0xF;
}

uint8_t uppernibble(uint8_t val) {
    return val >> 4;
}

uint8_t packbyte(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0xF);
}

// Getopt copied from my assignment 4 tsp.c code

int main(int argc, char **argv) { // main function that takes in getopt
    // arguments
    FILE *infile = stdin; // sets infile path to stdin as default
    FILE *outfile = stdout; // sets outfile path to stdout as default
    int opt = 0; // sets opt to 0 for getopt function
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { // getopt function while statement
        switch (opt) { // switch statement for getopt function
        case 'h': // case h defined as help statement
            printf("SYNOPSIS\n");
            printf("  A Hamming(8, 4) systematic code generator.\n");
            printf("\n");
            printf("USAGE\n");
            printf("  ./encode [-h] [-i infile] [-o outfile]\n");
            printf("\n");
            printf("OPTIONS");
            printf("  -h             Program usage and help.\n");
            printf("  -i infile      Input data to encode.\n");
            printf("  -o outfile     Output of encoded data.\n");
            break;
        case 'i': // case i for infile
            infile = fopen(optarg,
                "r"); // sets infile to given optarg and opens for reading
            if (infile == NULL) { // if infile is NULL
                fprintf(stderr, "failed to open file!"); // prints error "failed to
                    // open"
                fclose(infile); // closes the file
                return 1; // returns 1 and terminates
            }
            break;
        case 'o': // case o for outfile
            outfile = fopen(optarg, "w"); // sets outfile to given optarg and opens for writing
            if (outfile == NULL) { // if outfile is NULL
                fprintf(stderr, "failed to open file!"); // prints error "failed to
                    // open"
                fclose(infile); // closes infile
                fclose(outfile); // closes outfile
                return 1; // returns 1 and terminates
            }
            break;
        default: // default case printing out error statement if user input is invalid
            printf("SYNOPSIS\n");
            printf("  A Hamming(8, 4) systematic code generator.\n");
            printf("\n");
            printf("USAGE\n");
            printf("  ./encode [-h] [-i infile] [-o outfile]\n");
            printf("\n");
            printf("OPTIONS");
            printf("  -h             Program usage and help.\n");
            printf("  -i infile      Input data to encode.\n");
            printf("  -o outfile     Output of encoded data.\n");
            return 0;
        }
    }

    // fchmod code given in assignment document to set file permissions;

    struct stat statbuf;
    fstat(fileno(infile), &statbuf);
    fchmod(fileno(outfile), statbuf.st_mode);

    // Generator Matrix creation:

    BitMatrix *G = bm_create(4, 8);
    bm_set_bit(G, 0, 0);
    bm_set_bit(G, 1, 1);
    bm_set_bit(G, 2, 2);
    bm_set_bit(G, 3, 3);
    bm_set_bit(G, 0, 5);
    bm_set_bit(G, 0, 6);
    bm_set_bit(G, 0, 7);
    bm_set_bit(G, 1, 4);
    bm_set_bit(G, 1, 6);
    bm_set_bit(G, 1, 7);
    bm_set_bit(G, 2, 4);
    bm_set_bit(G, 2, 5);
    bm_set_bit(G, 2, 7);
    bm_set_bit(G, 3, 4);
    bm_set_bit(G, 3, 5);
    bm_set_bit(G, 3, 6);

    int c; // creats c variable to store read bytes
    while (((c = fgetc(infile))
            != EOF)) { // while loop that iterates through each byte and stores it in
        // c variable until end of line
        uint8_t lower_nibble = lowernibble(c); // separates byte into lower and upper nibbles
        uint8_t upper_nibble = uppernibble(c);

        uint8_t lower_encoded
            = ham_encode(G, lower_nibble); // encodes lower byte and stores in 8 bit variable
        uint8_t upper_encoded
            = ham_encode(G, upper_nibble); // encodes upper byte and stores in 8 bit variable

        fputc(lower_encoded, outfile); // outputs lower encoded byte to outfile
        fputc(upper_encoded, outfile); // outputs upper encoded byte to outfile
    }

    bm_delete(&G); // deletes all used matrices to prevent memory leaks
    fclose(infile);
    fclose(outfile);

    return 0;
}
