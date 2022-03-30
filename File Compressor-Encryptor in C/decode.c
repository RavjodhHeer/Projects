#include "bm.h"
#include "bv.h"
#include "externvar.h"
#include "hamming.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define OPTIONS "hvi:o:"

// defines external stat tracking variables

int total_bytes_processed = 0;
int uncorrected_errors = 0;
int corrected_errors = 0;

// lower/upper nibble and packing/unpacking code supplied in assignment document

uint8_t lowernib(uint8_t val) {
    return val & 0xF;
}

uint8_t uppernib(uint8_t val) {
    return val >> 4;
}

uint8_t bytepack(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0xF);
}

// Getopt copied from my assignment 4 tsp.c code and my assignment 5 encode.c
// code

int main(int argc, char **argv) { // main function that takes in getopt
    // arguments
    FILE *infile = stdin; // sets infile path to stdin as default
    FILE *outfile = stdout; // sets outfile path to stdout as default
    bool verbose = false;
    int opt = 0; // sets opt to 0 for getopt function
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { // getopt function while statement
        switch (opt) { // switch statement for getopt function
        case 'h': // case h defined as help statement
            printf("SYNOPSIS\n");
            printf("  A Hamming(8, 4) systematic code decoder.\n");
            printf("\n");
            printf("USAGE\n");
            printf("  ./decode [-h] [-v] [-i infile] [-o outfile]\n");
            printf("\n");
            printf("OPTIONS");
            printf("  -h             Program usage and help.\n");
            printf("  -v             Print statistics of decoding to stderr.\n");
            printf("  -i infile      Input data to decode.\n");
            printf("  -o outfile     Output of decoded data.\n");
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
        case 'v': // case v for verbose
            verbose = true; // sets verbose to true
            break;
        default: // default case printing out error statement if user input is invalid
            printf("SYNOPSIS\n");
            printf("  A Hamming(8, 4) systematic code decoder.\n");
            printf("\n");
            printf("USAGE\n");
            printf("  ./decode [-h] [-v] [-i infile] [-o outfile]\n");
            printf("\n");
            printf("OPTIONS");
            printf("  -h             Program usage and help.\n");
            printf("  -v             Print statistics of decoding to stderr.\n");
            printf("  -i infile      Input data to decode.\n");
            printf("  -o outfile     Output of decoded data.\n");
            return 0;
        }
    }

    // fchmod code given in assignment document to set file permissions;

    struct stat statbuf;
    fstat(fileno(infile), &statbuf);
    fchmod(fileno(outfile), statbuf.st_mode);

    // H-transpose Matrix creation:

    BitMatrix *Ht = bm_create(8, 4);
    bm_set_bit(Ht, 4, 0);
    bm_set_bit(Ht, 5, 1);
    bm_set_bit(Ht, 6, 2);
    bm_set_bit(Ht, 7, 3);
    bm_set_bit(Ht, 0, 1);
    bm_set_bit(Ht, 0, 2);
    bm_set_bit(Ht, 0, 3);
    bm_set_bit(Ht, 1, 0);
    bm_set_bit(Ht, 1, 2);
    bm_set_bit(Ht, 1, 3);
    bm_set_bit(Ht, 2, 1);
    bm_set_bit(Ht, 2, 0);
    bm_set_bit(Ht, 2, 3);
    bm_set_bit(Ht, 3, 1);
    bm_set_bit(Ht, 3, 0);
    bm_set_bit(Ht, 3, 2);

    int a; // creates a variable a to store first byte read
    int b; // creates a variable b to store second byte read
    while (((a = fgetc(infile)) != EOF)
           && ((b = fgetc(infile)) != EOF)) { // while loop to read in and store 2
        // bytes at a time to a and b variable
        total_bytes_processed += 2; // increments total_bytes_processed variable by
            // 1

        uint8_t msg1; // creates 8 bit variables msg1 and msg2 to store first
            // decoded result and second decoded result
        uint8_t msg2;

        ham_decode(Ht, a, &msg1); // ham_decodes using h-transpose matrix, first
            // byte and msg1 as inputs
        ham_decode(Ht, b, &msg2); // ham_decodes using h-transpose matrix, second
            // byte and msg2 as inputs

        uint8_t decoded_text
            = bytepack(msg2, msg1); // packs upper and lower bytes together and stores
        // them in 8 bit variable

        fputc(decoded_text, outfile); // fputs the packed resulting byte to outfile
    }

    if (verbose) { // if verbose is true, prints out statistics to stderror
        fprintf(stderr, "Total bytes processed: %d\n", total_bytes_processed);
        fprintf(stderr, "Uncorrected errors: %d\n", uncorrected_errors);
        fprintf(stderr, "Corrected errors: %d\n", corrected_errors);
        fprintf(stderr, "Error rate: %f\n", (float) (uncorrected_errors) / total_bytes_processed);
    }

    bm_delete(&Ht); // deletes created matrices and closes infile and outfile,
        // terminates program
    fclose(infile);
    fclose(outfile);

    return 0;
}
