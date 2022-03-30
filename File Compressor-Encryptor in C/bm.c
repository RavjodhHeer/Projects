#include "bm.h"

#include "bv.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// BitMatrix code taking inspiration from Sahiti's Lab section explanation

// struct for BitMatrix containing rows, cols, and vector BitVector

struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    BitVector *vector;
};

// Bitmatrix constructor function based on my previous stack code from
// assignment 3

BitMatrix *bm_create(uint32_t rows,
    uint32_t cols) { // BitMatrix bm_create function which
    // takes in rows and cols as input
    BitMatrix *m = (BitMatrix *) malloc(sizeof(BitMatrix)); // Allocates memory for a bitmatrix m
    if (m == NULL) { // if memory allocation fails, return NULL
        return NULL;
    }
    m->rows = rows; // sets m->rows to rows function input
    m->cols = cols; // sets m->cols to cols function input
    m->vector = bv_create(rows * cols); // calls bv_create function to create a bitvector m->vector
    if (m->vector == NULL) { // if vector creation unsuccessful, free bitmatrix m
        // and return NULL
        free(m);
        return NULL;
    }
    return m; // returns bitmatrix m
}

// destructor function bm_delete based on my previous stack destructor code from
// assignment 3

void bm_delete(BitMatrix **m) { // bm_delete function deletes the created matrix
    bv_delete(&(*m)->vector); // calls bv_delete to delete the bitvector m->vector
    free(*m); // frees bitmatrix m and sets pointer to m to NULL
    *m = NULL;
    return;
}

uint32_t bm_rows(BitMatrix *m) { // bm_rows function returns bitmatrix m->rows
    return m->rows;
}

uint32_t bm_cols(BitMatrix *m) { // bm_cols function returns bitmatrix m->cols
    return m->cols;
}

void bm_set_bit(BitMatrix *m, uint32_t r,
    uint32_t c) { // bm_set_bit function calls bv_set_bit to set bit
    // at rows and columns position
    if (r < m->rows && c < m->cols) {
        bv_set_bit(m->vector, (r * (m->cols)) + c);
    }
}

void bm_clr_bit(BitMatrix *m, uint32_t r,
    uint32_t c) { // bm_clr_bit function calls bv_clr_bit to clear
    // bit at rows and columns position
    if (r < m->rows && c < m->cols) {
        bv_clr_bit(m->vector, (r * (m->cols)) + c);
    }
}

uint8_t bm_get_bit(BitMatrix *m, uint32_t r,
    uint32_t c) { // bm_get_bit function calls bv_get_bit to get
    // bit at rows and columns position
    return (bv_get_bit(m->vector, (r * (m->cols)) + c));
}

// bm_from_data code taking inspiration from sahiti's lab section

BitMatrix *bm_from_data(uint8_t byte,
    uint32_t length) { // bm_from_data turns a byte of given length to
    // a bitmatrix of 1 * length size
    if (length > 8) { // if the given length is greater than 8, return NULL
        return NULL;
    }
    BitMatrix *nm = bm_create(1, length); // creats an empty bitmatrix of 1 *
        // length size to store byte into
    for (uint32_t i = 0; i < length; i += 1) { // iterates over the length of the inputted byte
        if (byte & (1 << i)) { // masks the byte with 1 left shifted by i in order
            // to retrieve current bit value
            bm_set_bit(nm, 0, i); // if current bit value is 1, call bm_set_bit to set
                // the bit at current position to 1
        } else {
            bm_clr_bit(nm, 0,
                i); // otherwise calls bm_clr_bit to clear bit at current position
        }
    }
    return nm; // returns the resulting bitmatrix
}

// bm_to_data code taking inspiration from brian's lab section

uint8_t bm_to_data(BitMatrix *m) { // bm_to_data takes in a bitmatrix and turns it into a byte
    uint8_t val = 0; // introduces val variable to store returning byte
    for (uint8_t i = 0; i < m->cols;
         i += 1) { // for loop that iterates through columns of bitmatrix
        uint8_t new = bm_get_bit(m, 0,
            i); // gets bit at current i index and stores it into variable "new"
        val |= new << i; // iteratively OR's val by new left shifted by i
    }
    return val; // returns val variable containing new byte
}

BitMatrix *bm_multiply(BitMatrix *A,
    BitMatrix *B) { // bm_multiply function takes two
    // matrices and multiplies them
    if (A->cols != B->rows) { // checks if the columns of first matrix and rows of
        // second matrix are equal, if not, returns NULL
        return NULL;
    }
    BitMatrix *result = bm_create(A->rows, B->cols); // creates a result matrix to store result into
    for (uint32_t i = 0; i < A->rows;
         i += 1) { // for loop iterates through the rows of the first matrix
        for (uint32_t j = 0; j < B->cols;
             j += 1) { // for loop iterates through the columns of the second matrix
            uint32_t sum = 0; // creates a sum variable to store values in
            for (uint32_t k = 0; k < A->cols;
                 k += 1) { // iterates through joint column/row of both matrices
                sum ^= (bm_get_bit(A, i, k)
                        & bm_get_bit(B, k,
                            j)); // iteratively XOR's sum variable with first matrix's
                // current bit AND'ed with second matrix's current bit
            }
            sum = sum % 2; // mods sum by two to make sure result matrix only contains 0 and 1
            if (sum == 1) { // if the sum value is 1, set the bit at the current position
                bm_set_bit(result, i, j);
            }
            if (sum == 0) { // otherwise clear the bit at the current position
                bm_clr_bit(result, i, j);
            }
        }
    }
    return result; // return the resulting matrix
}

void bm_print(BitMatrix *m) { // bm_print function used for debugging reasons,
    // not important
    uint32_t counter = 0;
    for (uint32_t i = 0; i < m->rows; i += 1) {
        for (uint32_t j = 0; j < m->cols; j += 1) {
            printf("%hhu ", bm_get_bit(m, i, j));
            counter += 1;
            if (((counter) % m->cols) == 0) {
                printf("\n");
            }
        }
    }
    printf("\n");
}
