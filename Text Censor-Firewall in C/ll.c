#include "ll.h"

#include "node.h"
#include "speck.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct provided in assignment doc
//implementations of some functions taking inspiration from Sabrina's lab section

uint64_t seeks; //declares external variable for seeks
uint64_t links; //declares external variable for links

struct LinkedList { //linkedlist struct given in assignment doc
    uint32_t length;
    Node *head;
    Node *tail;
    bool mtf;
};

LinkedList *ll_create(bool mtf) { //linked list ll create function takes in mtf boolean
    LinkedList *ll
        = (LinkedList *) malloc(sizeof(LinkedList)); //dynamically allocates memory for linked list
    if (ll) { //if memory allocation successful
        ll->length = 0; //set length pointer to 0
        ll->mtf = mtf; //set mtf boolean pointer to inputted mtf boolean value
        ll->head = node_create(NULL, NULL); //create a NULL node at head pointer
        ll->tail = node_create(NULL, NULL); //create a NULL node at tail pointer
        ll->head->next = ll->tail; //set head pointer's next pointer to tail pointer
        ll->tail->prev = ll->head; //set tail pointer's prev pointer to head pointer
    }
    return ll; //return linkedlist
}

void ll_delete(LinkedList **ll) { //ll delete function takes in a linked list and frees all memory
    if (*ll) { //if the linked list exists
        for (
            uint32_t i = 0; i < ll_length(*ll) + 2;
            i
            += 1) { //loop through length of linkedlist + 2 for head and tail node and call node delete on each node
            Node *temp = (*ll)->head->next;
            node_delete(&(*ll)->head);
            (*ll)->head = temp;
        }
        free(*ll); //free linked list pointer
        *ll = NULL; //set pointer to null
    }
    return;
}

uint32_t ll_length(LinkedList *ll) { //ll length function returns length pointer
    return ll->length;
}

Node *ll_lookup(
    LinkedList *ll, char *oldspeak) { //ll lookup function takes in oldspeak and searches for node
    seeks += 1; //increment seeks to signify function being called
    for (Node *n = ll->head->next; n != ll->tail;
         n = n->next) { //loops through each node between head and tail
        if (strcmp(n->oldspeak, oldspeak)
            == 0) { //string compare node's oldspeak to inputted nodespeak
            if (ll->mtf) { //if move to front is true then swaps pointers around in order to move the node to front
                n->prev->next = n->next;
                n->next->prev = n->prev;
                n->prev = ll->head;
                n->next = ll->head->next;
                ll->head->next->prev = n;
                ll->head->next = n;
            }
            return n; //returns the node after swapping
        }
        links += 1; //increments links outside string compare
    }
    return NULL; //if node not found, return null
}

void ll_insert(LinkedList *ll, char *oldspeak,
    char *
        newspeak) { //ll insert function takes in olspeak and newspeak and inserts created node into linked list
    if (ll_lookup(ll, oldspeak)
        != NULL) { //calls ll lookup to check if oldspeak already exists in linked list
        return;
    }
    Node *n = node_create(oldspeak, newspeak); //creates a new node with given oldspeak and newspeak
    n->prev
        = ll->head; //sets respective node pointers to head/tail and increments ll length pointer by 1
    n->next = ll->head->next;
    ll->head->next->prev = n;
    ll->head->next = n;
    ll->length += 1; //
}

void ll_print(LinkedList
        *ll) { //ll print function iterates through nodes between head and tail and calls node print
    for (Node *n = ll->head->next; n != ll->tail; n = n->next) {
        node_print(n);
    }
}
