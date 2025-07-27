#ifndef PATIENCE_UTILS_H
#define PATIENCE_UTILS_H

#include <stdbool.h>

#define DECK_SIZE 52 // Number of cards in a standard deck
#define MAX_PILES 9 // Maximum number of piles allowed before player loses
#define NUM_GAMES 10000 // Number of games to simulate in pstatistics

bool add_to_11(int visible[], int num_visible, int *pos1, int *pos2);

bool jqk(int visible[], int num_visible, int positions[]);

void shuffle_deck(int deck[], int seed);

void print_visible(int visible[], int num_visible);

int play(int deck[], int verbose);

int* many_plays(int N);

#endif