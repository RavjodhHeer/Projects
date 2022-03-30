#include "bf.h"

#include "bv.h"
#include "speck.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

//struct and constructor provided from assignment doc

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

//Bloom Filter constructor given in assignment document with provided salt values and bf->filter bitvector

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        //Grimm's Fairy Tales
        bf->primary[0] = 0x5adf08ae86d36f21;
        bf->primary[1] = 0xa267bbd3116f3957;
        // The Adventures of Sherlock Holmes
        bf->secondary[0] = 0x419d292ea2ffd49e;
        bf->secondary[1] = 0x09601433057d5786;
        // The Strange Case of Dr. Jekyll and Mr. Hyde
        bf->tertiary[0] = 0x50d8bb08de3818df;
        bf->tertiary[1] = 0x4deaae187c16ae1d;
        bf->filter = bv_create(size);
        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

void bf_delete(BloomFilter **
        bf) { //bf delete function takes in bloom filter, calls bitvector delete on bf filter bitvector and sets pointer to null
    if (*bf) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
    return;
}

uint32_t bf_size(BloomFilter *bf) { //bf size function returns bitvector length given bloomfilter
    return bv_length(bf->filter);
}

void bf_insert(BloomFilter *bf,
    char *
        oldspeak) { //bf insert takes in bloomfilter and oldspeak and inserts oldspeak's hash values at respective indices
    uint32_t first = hash(bf->primary,
        oldspeak); //creates variables to store the 3 hash values of hashing inputted oldspeak
    uint32_t second = hash(bf->secondary, oldspeak);
    uint32_t third = hash(bf->tertiary, oldspeak);
    first = first % bf_size(bf); //mods returned hash values by bloomfilter size
    second = second % bf_size(bf);
    third = third % bf_size(bf);
    bv_set_bit(
        bf->filter, first); //sets the bits at each of those indices in the bloomfilter's bitvector
    bv_set_bit(bf->filter, second);
    bv_set_bit(bf->filter, third);
}

bool bf_probe(BloomFilter *bf,
    char *
        oldspeak) { //bf probe checks to see if a given	olspeak hash value has indices already set in the bitvector
    uint32_t first = hash(bf->primary,
        oldspeak); //creates variables to store the 3 hash value of hashing inputted oldspeak
    uint32_t second = hash(bf->secondary, oldspeak);
    uint32_t third = hash(bf->tertiary, oldspeak);
    first = first % bf_size(bf); //mods returned hash values by bloomfilter size
    second = second % bf_size(bf);
    third = third % bf_size(bf);
    if ((bv_get_bit(bf->filter, first) == 1) && (bv_get_bit(bf->filter, second) == 1)
        && (bv_get_bit(bf->filter, third)
            == 1)) { //if the bits at the 3 hash value indices are set to 1, returns true
        return true;
    }
    return false; //otherwise returns false
}

uint32_t bf_count(BloomFilter
        *bf) { //bf count function counts the number of set bits in the bloom filter's bitvector
    uint32_t counter = 0;
    for (
        uint32_t i = 0; i < bf_size(bf);
        i
        += 1) { //creates counter variable, loops through the bitvector and increments counter every time a bit set to 1 is detected
        if (bv_get_bit(bf->filter, i) == 1) {
            counter += 1;
        }
    }
    return counter;
}

void bf_print(BloomFilter *bf) { //bf print function calls bv print
    bv_print(bf->filter);
}
