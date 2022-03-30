#include "ht.h"

#include "ll.h"
#include "node.h"
#include "speck.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct and constructor provided in assignment doc
//some hastable function implementations taking inspiration from Sahiti and Sabrina's lab sections

struct HashTable { //struct for hashtable given in assignment document
    uint64_t salt[2];
    uint32_t size;
    bool mtf;
    LinkedList **lists;
};

HashTable *ht_create(uint32_t size,
    bool
        mtf) { //constructor for hash table given in assignment doc with ht size, ht mtf and ht lists pointers
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        //Leviathan
        ht->salt[0] = 0x9846e4f157fe8840;
        ht->salt[1] = 0xc5f318d7e055afb8;
        ht->size = size;
        ht->mtf = mtf;
        ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

void ht_delete(HashTable **ht) { //ht delete takes in a hash table and frees all memory
    if (*ht) { //if the hash table exists
        for (
            uint32_t i = 0; i < ht_size(*ht);
            i
            += 1) { //loop through the hash table and delete each non-NULL linked list using ll delete
            if ((*ht)->lists[i] != NULL) {
                ll_delete(&(*ht)->lists[i]);
            }
        }
        free((*ht)->lists); //free ht lists pointer
        free(*ht); //free ht pointer
        *ht = NULL; //set pointer to null
    }
    return;
}

uint32_t ht_size(HashTable *ht) { //ht size function returns hash table size pointer
    return ht->size;
}

Node *ht_lookup(HashTable *ht,
    char *
        oldspeak) { //ht lookup function takes in oldspeak and searches hash table for given oldspeak
    uint32_t index = hash(ht->salt, oldspeak)
                     % ht_size(ht); //creates an index variable to store hashed salt value
    if (!ht->lists[index]) { //if hashtable at stored index doesn't exist, return null
        return NULL;
    }
    return ll_lookup(ht->lists[index],
        oldspeak); //otherwise call ll lookup on hashtable index and given oldspeak
}

void ht_insert(HashTable *ht, char *oldspeak,
    char *newspeak) { //ht insert function takes in oldspeak and newspeak and inserts into hashtable
    uint32_t index = hash(ht->salt, oldspeak)
                     % ht_size(ht); //creates index variable to store hashed salt value
    if (!ht->lists[index]) { //if hashtable at stored index doesn't exist
        ht->lists[index] = ll_create(ht->mtf); //call ll create to create a linked list at the index
    }
    ll_insert(ht->lists[index], oldspeak,
        newspeak); //call ll insert to insert given oldspeak and newspeak at hash table index
}

uint32_t ht_count(
    HashTable *ht) { //ht count returns the number of non-NULL linked lists in the hash table
    uint32_t counter = 0;
    for (
        uint32_t i = 0; i < ht_size(ht);
        i
        += 1) { //creates counter variable and loops through size of hashtable and increments counter when a null list is found
        if (ht->lists[i] == NULL) {
            counter += 1;
        }
    }
    return (ht_size(ht) - counter); //returns hashtable size minus null counter
}

void ht_print(HashTable *
        ht) { //ht print function iterates through ht size, creates a linked list at index and prints NULL or calls ll print based on index value
    for (uint32_t i = 0; i < ht_size(ht); i += 1) {
        LinkedList *ll = ht->lists[i];
        if (ll == NULL) {
            printf("NULL\n");
        } else {
            ll_print(ll);
        }
    }
}
