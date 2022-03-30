#include "hamming.h"

#include "bm.h"
#include "bv.h"
#include "externvar.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

int uncorrected_errors; // initializes external variable uncorrected errors
    // which keeps track of uncorrected errors
int corrected_errors; // initializes external variable corrected errors which
    // keeps track of corrected errors

// Code taking inspiration from Sahiti's encode and decode explanation in lab
// section

int bits[16] = {
    [0] = -1,
    [1] = 4,
    [2] = 5,
    [3] = -1,
    [4] = 6,
    [5] = -1,
    [6] = -1,
    [7] = 3,
    [8] = 7,
    [9] = -1,
    [10] = -1,
    [11] = 2,
    [12] = -1,
    [13] = 1,
    [14] = 0,
    [15] = -1,
};

// creates array of bits containing ham_status codes

// lower and upper nibble and byte packing code given in assignment document

uint8_t lower_nibble(uint8_t val) {
    return val & 0xF;
}

uint8_t upper_nibble(uint8_t val) {
    return val >> 4;
}

uint8_t pack_byte(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0xF);
}

uint8_t ham_encode(BitMatrix *G,
    uint8_t msg) { // ham_encode function taking in generator
    // matrix and message as input
    BitMatrix *message = bm_from_data(msg, 4); // creates a bitmatrix of the given message
    BitMatrix *matrixresult = bm_multiply(message, G); // creates a resulting matrix of bm_multiply
        // result between generator and message matrices
    uint8_t result = (bm_to_data(matrixresult)); // creates a 8 bit result variable to store
        // bm_to_data of result matrix
    bm_delete(&message); // deletes message matrix and result matrix to prevent
        // memory leaks
    bm_delete(&matrixresult);
    return result; // returns the 8 bit result
}

HAM_STATUS
ham_decode(BitMatrix *Ht, uint8_t code,
    uint8_t *msg) { // ham_decode function with ham_status return values,
    // h-transpose, code input and msg output
    BitMatrix *ham_code
        = bm_from_data(code, 8); // creates bitmatrices for code bm_from_data and matrix multiply
    // result between h-transpose and code
    BitMatrix *result = (bm_multiply(ham_code, Ht));
    uint8_t error = bm_to_data(result); // stores resulting matrix converted to
        // byte into 8 bit error variable
    uint8_t flip = bits[error]; // stores the ham_status code at error index to 8 bit value
    if (error == 0) { // if the error is 0, deletes matrices to prevent memory
        // leaks, sets msg to lower nibble of code and returns HAM_OK
        *msg = lower_nibble(code);
        bm_delete(&ham_code);
        bm_delete(&result);
        return HAM_OK;
    }
    if (bits[error] == -1) { // if the error is -1, sets msg to original msg input, increments
        // uncorrected erros by 1, deletes matrices and returns HAM_ERR
        *msg = *msg;
        uncorrected_errors += 1;
        bm_delete(&ham_code);
        bm_delete(&result);
        return HAM_ERR;
    } else { // otherwise checks bit value of error bit and clears or sets it
        // based off current value
        if (bm_get_bit(ham_code, 0, flip) == 1) {
            bm_clr_bit(ham_code, 0, flip);
        } else {
            bm_set_bit(ham_code, 0, flip);
        }
        uint8_t decoded
            = bm_to_data(ham_code); // stores changed matrix to 8 bit value using bm_to_data
        uint8_t lownib_decoded
            = lower_nibble(decoded); // takes lower nibble of decoded changed value
        // using helper function
        *msg = lownib_decoded; // return lower nibble of changed code through msg
        corrected_errors += 1; // increments corrected errors external variable by 1
        bm_delete(&ham_code); // deletes matrices to prevent memory leaks and
            // returms HAM_CORRECT
        bm_delete(&result);
        return HAM_CORRECT;
    }
}
