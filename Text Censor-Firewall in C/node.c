#include "node.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//implementation of node ADT taken from my asgn 6 code
//node and strcopy implementation taking inspiration from Sabrina's lab section

char *stringdup(const char *s) { //stringdup function takes in a const char string as input
    return strcpy(malloc(strlen(s) + 1),
        s); //returns strcpy of string with dynamically allocated space for new string copy
}

Node *node_create(
    char *oldspeak, char *newspeak) { //node_create function takes in symbol and frequency as input
    Node *n = (Node *) malloc(sizeof(Node)); //dynamically allocates memory for Node
    if (n) { //if dynamic memory allocation is successful
        if (oldspeak == NULL) { //if oldspeak is null, sets node's oldspeak to null
            n->oldspeak = NULL;
        } else { //if oldspeak isn't null, sets node's oldspeak to stringdup function of oldspeak
            n->oldspeak = stringdup(oldspeak);
        }
        if (newspeak == NULL) { //if newspeak is null, sets node's newspeak to null
            n->newspeak = NULL;
        } else { //if newspeak isn't null, sets node's newspeak to stringdup function of newspeak
            n->newspeak = stringdup(newspeak);
        }
        n->next = NULL; //node's next child pointer set to NULL
        n->prev = NULL; //node's prev child pointer set to NULL
    }
    return n; //returns created node
}

void node_delete(Node **n) { //node_delete takes in a node and frees all memory
    if (*n) { //if the node exists
        free((*n)->oldspeak); //frees oldspeak
        free((*n)->newspeak); //frees newspeak
        free(*n); //frees the node and sets pointer to NULL
        *n = NULL; //sets node pointer to null
    }
    return;
}

void node_print(Node *n) { //node_print function for debugging purposes, given in assignment doc
    if (n->oldspeak && n->newspeak) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    }
    if (n->oldspeak && (n->newspeak == NULL)) {
        printf("%s\n", n->oldspeak);
    }
}
