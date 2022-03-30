#include "bv.h"

#include "bm.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// code taking inspiration from professor Long's code walkthrough in lecture and
// Sahiti's lab section

// struct for BitVector given in assignment document

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// Code taking inspiration from my previous stack code

BitVector *bv_create(uint32_t length) { // BitVector bv_create function with
    // length as input parameter
    BitVector *v = (BitVector *) calloc(1, sizeof(BitVector)); // Allocates memory for BitVector
    if (v == NULL) { // if memory allocation fails, return NULL
        return NULL;
    }
    v->length = length; // set v->length to length
    int val = length % 8 == 0 ? length / 8
                              : (length / 8) + 1; // equation to calculate byte allocation for
    // v->vector allocation
    v->vector = (uint8_t *) calloc(val, sizeof(uint8_t)); // allocates memory for v->vector
    if (v->vector == NULL) { // if memory allocation fails, free v and return NULL
        free(v);
        return NULL;
    }
    return v; // returns vector v
}

// bv delete taking inspiration from my previous stack code

void bv_delete(BitVector **v) { // bv_delete function deletes the bitvector
    free((*v)->vector); // frees the pointer to v->vector and frees v and sets v
        // to NULL
    free(*v);
    *v = NULL;
    return;
}

uint32_t bv_length(BitVector *v) { // bv_length returns length pointer of v
    return v->length;
}

// set bit and clear bit implementation taking inspiration from Sahiti's lab
// section walkthrough

void bv_set_bit(BitVector *v,
    uint32_t i) { // bv_set_bit function takes in vector and index i as input
    uint32_t val = (0x1 << (i % 8)); // creates a value to store result of masking
        // index value by 1
    v->vector[i / 8] |= val; // i/8 index of vector is OR'd with mask value at
        // index, OR preserves the value (sets bits to 1)
}

void bv_clr_bit(BitVector *v,
    uint32_t i) { // bv_clr_bit function takes in vector and index i as input
    uint32_t val = ~(0x1 << (i % 8)); // creates a value to store the result of
        // inverting the mask of index i
    v->vector[i / 8] &= val; // i/8 index of vector is AND'ed with mask value, AND
        // sets previous values to 0 (clears bits to 0)
}

uint8_t bv_get_bit(BitVector *v,
    uint32_t i) { // bv_get_bit function takes in vector and index i as input
    uint32_t mod = (i % 8); // modulus value storing index position is created
    uint32_t mask = 0x1; // mask value of 0x1 is stored in mask variable
    return (
        v->vector[i / 8] >> mod & mask); // i/8 index of vector is right shifted by modulus & mask
}

void bv_xor_bit(BitVector *v, uint32_t i,
    uint8_t bit) { // bv_xor_bit function takes in vector, index i and a bit
    uint32_t val = (bit << (i % 8)); // creates a value to store the result of
        // right shifting index i
    v->vector[i / 8] ^= val; // i/8 index of vector is XOR'ed with mask value, XOR
        // preserves previous value if 0, sets to 0 if same
}

void bv_print(BitVector *v) { // bv_print function for debugging reasons, not important
    for (uint32_t i = 0; i < v->length; i += 1) {
        printf("%hhu ", bv_get_bit(v, i));
    }
    printf("\n");
}
