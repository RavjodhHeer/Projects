#include "bf.h"
#include "bv.h"
#include "ht.h"
#include "ll.h"
#include "node.h"
#include "parser.h"
#include "speck.h"

#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#define OPTIONS "ht:f:ms"

#define KILOBYTE 1024 //for buffer to read in words

//regular expression implementation given by Eugene in 6/1/21 lab section

#define WORD "([a-zA-Z0-9_])+(('|-)([a-zA-Z0-9_])+)*"

uint64_t seeks; //declare seeks and links variables for variable tracking
uint64_t links;

//lowercase converter function idea given in Eugene's section 6/1/21

void make_lowercase(char *s) {
    for (int i = 0; s[i] != '\0'; i += 1) {
        s[i] = tolower(s[i]);
    }
}

//banhammer getopt taken from previous assignment encode file
//banhammer implementation taking inspiration from Eugene's lab section 6/1/21

int main(int argc, char **argv) { // main function that takes in getopt arguments
    uint32_t hashtable_size = 10000; //default hashtable size set to 10000
    uint32_t bloomfilter_size = 1048576; //default bloomfilter size set to 2^20 (1048576)
    bool mtf = false; //mtf boolean set to false as default
    bool statistics = false; //statistics boolean to enable printing of statistics (default false)
    int opt = 0; // sets opt to 0 for getopt function
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) { // getopt function while statement
        switch (opt) { // switch statement for getopt function
        case 'h': // case h defined as help statement
            printf("SYNOPSIS\n");
            printf("  A word filtering program for the GPRSC.\n");
            printf("  Filters out and reports bad words parsed from stdin.\n");
            printf("\n");
            printf("USAGE\n");
            printf("  ./banhammer [-hsm] [-t size] [-f size]\n");
            printf("\n");
            printf("OPTIONS");
            printf("  -h             Program usage and help.\n");
            printf("  -s             Print program statistics.\n");
            printf("  -m             Enable move-to-front rule.\n");
            printf("  -t size        Specify hash table size (default: 10000).\n");
            printf("  -f size        Specify Bloom filter size (default: 2^20).\n");
            break;
        case 't': //case t for hashtable size
            hashtable_size = atoi(optarg);
            break;
        case 'f': // case f for bloomfilter size
            bloomfilter_size = atoi(optarg);
            break;
        case 'm': // case m for move-to-front rule
            mtf = true;
            break;
        case 's': // case s for printing stats to stderr
            statistics = true;
            break;
        default: // default case printing out error statement if user input is invalid
            printf("SYNOPSIS\n");
            printf("  A word filtering program for the GPRSC.\n");
            printf("  Filters out and reports bad words parsed from stdin.\n");
            printf("\n");
            printf("USAGE\n");
            printf("  ./banhammer [-hsm] [-t size] [-f size]\n");
            printf("\n");
            printf("OPTIONS");
            printf("  -h             Program usage and help.\n");
            printf("  -s             Print program statistics.\n");
            printf("  -m             Enable move-to-front rule.\n");
            printf("  -t size        Specify hash table size (default: 10000).\n");
            printf("  -f size        Specify Bloom filter size (default: 2^20).\n");
            return 0;
        }
    }
    //continue banhammer implementation

    //create bloom filter and hashtable
    BloomFilter *bf = bf_create(bloomfilter_size);
    HashTable *ht = ht_create(hashtable_size, mtf);

    //regex creation taken from assignment doc example of parsing module
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }
    //read in badspeak file
    char badspeak_buffer[KILOBYTE];
    FILE *badspeak = fopen("badspeak.txt", "r");
    if (badspeak == NULL) {
        fprintf(stderr, "Failed to open file!");
        fclose(badspeak);
        return 1;
    }
    //while loop with fscanf to scan in each bad word and add to bloomfilter and hashtable
    while (fscanf(badspeak, "%s", badspeak_buffer) != EOF) {
        bf_insert(bf, badspeak_buffer);
        ht_insert(ht, badspeak_buffer, NULL);
    }

    //read in newspeak file
    char newspeak_buffer[KILOBYTE];
    FILE *newspeak = fopen("newspeak.txt", "r");
    if (newspeak == NULL) {
        fprintf(stderr, "Failed to open file!");
        fclose(badspeak);
        fclose(newspeak);
        return 1;
    }
    //while loop with fscanf to scan in each new word and add to bloomfilter and hashtable
    while (fscanf(newspeak, "%s %s", badspeak_buffer, newspeak_buffer) != EOF) {
        bf_insert(bf, badspeak_buffer);
        ht_insert(ht, badspeak_buffer, newspeak_buffer);
    }

    LinkedList *badspeak_ll = ll_create(mtf); //create a linked list for badspeak
    LinkedList *newspeak_ll = ll_create(mtf); //create a linked list for newspeak

    //word read-in taken from assignment doc parsing example
    //pseudo implementation taking inspiration from Eugene's 6/1/21 section
    //fscanf input
    char *word = NULL;
    while ((word = next_word(stdin, &re)) != NULL) {
        //change word to lowercase
        make_lowercase(word);
        //check bloom filter for words
        if (!bf_probe(bf, word)) {
            continue;
        }
        //check hashtable
        Node *n = ht_lookup(ht, word);
        //if hashtable
        if (n == NULL) {
            continue;
        }
        //record words
        if (n->newspeak == NULL) {
            ll_insert(badspeak_ll, n->oldspeak, n->newspeak);
        }
        if (n->oldspeak && n->newspeak) {
            ll_insert(newspeak_ll, n->oldspeak, n->newspeak);
        }
    }
    //for recorded words
    //print respective message
    //print words
    //if statistics isn't set to true, print out messages if badspeak or newspeak linked lists contain any values
    if (!statistics) {
        if ((ll_length(badspeak_ll) > 0) && (ll_length(newspeak_ll) > 0)) {
            printf("Dear beloved citizen of the GPRSC,\n");
            printf("\n");
            printf("We have some good news, and we have some bad news.\n");
            printf("The good news is that there is bad news. The bad news is that you will\n");
            printf("be sent to joycamp and subjected to a week-long destitute existence.\n");
            printf("This is the penalty for using degenerate words, as well as using\n");
            printf("oldspeak in place of newspeak. We hope you can correct your behavior.\n");
            printf("\n");
            printf("Your transgressions, followed by the words you must think on:\n");
            printf("\n");
            ll_print(badspeak_ll);
            ll_print(newspeak_ll);
        } else if (ll_length(badspeak_ll) > 0) {
            printf("Dear beloved citizen of the GPRSC,\n");
            printf("\n");
            printf("You have been caught using degenerate words that may cause\n");
            printf("distress among the moral and upstanding citizens of the GPSRC.\n");
            printf("As such, you will be sent to joycamp. It is there where you will\n");
            printf("sit and reflect on the consequences of your choice in language.\n");
            printf("\n");
            printf("Your transgressions:\n");
            printf("\n");
            ll_print(badspeak_ll);
        } else if (ll_length(newspeak_ll) > 0) {
            printf("Dear beloved citizen of the GPRSC,\n");
            printf("\n");
            printf("We recognize your efforts in conforming to the language standards\n");
            printf("of the GPSRC. Alas, you have been caught uttering questionable words\n");
            printf("and thinking unpleasant thoughts. You must correct your wrongspeak\n");
            printf("and badthink at once. Failure to do so will result in your deliverance\n");
            printf("to joycamp.\n");
            printf("\n");
            printf("Words that you must think on:\n");
            printf("\n");
            ll_print(newspeak_ll);
        }
    }
    //print statistics to stdout
    if (statistics) {
        double average_seek_length = (double) links / seeks;
        double hash_table_load = 100 * ((double) ht_count(ht) / ht_size(ht));
        double bloom_filter_load = 100 * ((double) bf_count(bf) / bf_size(bf));
        fprintf(stderr, "Seeks: %lu\n", seeks);
        fprintf(stderr, "Average seek length: %f\n", average_seek_length);
        fprintf(stderr, "Hash table load: %f%%\n", hash_table_load);
        fprintf(stderr, "Bloom filter load; %f%%\n", bloom_filter_load);
    }

    //free and close files, delete ADTs, and clear regex then return 0 and end program
    fclose(newspeak);
    fclose(badspeak);
    bf_delete(&bf);
    ht_delete(&ht);
    clear_words();
    regfree(&re);
    ll_delete(&badspeak_ll);
    ll_delete(&newspeak_ll);
    return 0;
}
