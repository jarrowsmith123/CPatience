/**
 * @file patience_utils.c
 * @brief Utility functions for playing the patience game
 *          
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "patience_utils.h"

/**
 * @brief Checks if any two visible cards sum to 11
 *
 * Iterates through all pairs of visible cards to find any two that sum to 11
 * If found, the positions of these cards are stored in the provided pointers
 *
 * @param visible Array of visible card values
 * @param num_visible Number of visible cards in the array
 * @param pos1 Pointer to store the position of the first card
 * @param pos2 Pointer to store the position of the second card
 * @return true if a pair summing to 11 is found, false otherwise
 */
bool add_to_11(int visible[], int num_visible, int *pos1, int *pos2) {
    for (int i = 0; i < num_visible; i++) {
        for (int j = i + 1; j < num_visible; j++) {
            if (visible[i] + visible[j] == 11) {
                *pos1 = i;
                *pos2 = j;
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Checks if Jack Queen and King are all visible
 *
 * Searches through the visible cards to determine if Jack 11 Queen 12
 * and King 13 are all present. If found, their positions are stored in the
 * provided array
 *
 * @param visible Array of visible card values
 * @param num_visible Number of visible cards in the array
 * @param positions Array to store which pile the Jack Queen and King are in
 * @return true if Jack Queen and King are all visible, false otherwise
 */
bool jqk(int visible[], int num_visible, int positions[]) {
    bool jack = false, queen = false, king = false;
    int jack_pos, queen_pos, king_pos;
    // check each visible card for Jack Queen and King
    for (int i = 0; i < num_visible; i++) {
        if (visible[i] == 11) {
            jack = true;
            jack_pos = i;
        } else if (visible[i] == 12) {
            queen = true;
            queen_pos = i;
        } else if (visible[i] == 13) {
            king = true;
            king_pos = i;
        }
    }
    // If all found, update positions
    if (jack && queen && king) {
        positions[0] = jack_pos;
        positions[1] = queen_pos;
        positions[2] = king_pos;
        return true;
    }
    
    return false;
}

/**
 * @brief Shuffles a deck of cards
 * 
 * Initializes a deck with 4 copies of each card value (1-13) and
 * shuffles them using the Fisher-Yates algorithm, using a seed provided or using the current seed if not
 * Since the shuffle function in the CA leaked memory, I decided to use a simple Fisher-Yates shuffle
 *
 * @param deck Array to store the shuffled deck
 * @param seed Random seed value, 0 means use current seed
 */
void shuffle_deck(int deck[], int seed) {
    // Initialize random seed if provided
    if (seed != 0) {
        srand(seed);
    }
    
    for (int i = 0; i < DECK_SIZE; i++) {
        // Fill deck with 4 copies of each card value (1-13)
        deck[i] = (i % 13) + 1;
    }
    
    // Fisher-Yates shuffle
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

/**
 * @brief Prints the visible cards
 *
 * Displays all visible cards followed by a newline.
 *
 * @param visible Array of visible card values
 * @param num_visible Number of visible cards in the array
 */
void print_visible(int visible[], int num_visible) {
    for (int i = 0; i < num_visible; i++) {
        printf("%d ", visible[i]);
    }
    printf("\n");
}

/**
 * @brief Plays a single game of patience
 *
 * Implements the game logic for a patience card game where:
 * - Cards that sum to 11 draw new cards
 * - Jack Queen and King when all visible draw new cards
 * - If neither condition is met, a new pile is started
 * - Game ends when all cards are used (win) or more than MAX_PILES piles exist (lose)
 *
 * @param deck Array containing the shuffled deck of cards
 * @param verbose Flag to enable printing the game, 1 to print, 0 to not print
 * @return Number of cards left in the deck after game has ended
 */
int play(int deck[], int verbose) {
    int visible[MAX_PILES]; // At most we'll have MAX_PILES visible cards
    int num_visible = 0;
    int num_piles = 0;
    int deck_pos = 0;
    
    // Place the first two cards
    visible[0] = deck[deck_pos++];
    visible[1] = deck[deck_pos++];
    num_visible = 2;
    num_piles = 1;
    
    if (verbose) {
        print_visible(visible, num_visible);
    }
    
    while (deck_pos < DECK_SIZE) {
        // Check if any two cards add to 11
        int pos1, pos2;
        if (add_to_11(visible, num_visible, &pos1, &pos2)) {
            // Replace cards that sum to 11
            if (deck_pos < DECK_SIZE){
                visible[pos1] = deck[deck_pos++];
            } 
            if (deck_pos < DECK_SIZE){
                visible[pos2] = deck[deck_pos++];
            }
            if (verbose) {
                print_visible(visible, num_visible);
            }
            
        } 
        else {
            // Check for Jack, Queen, King
            int positions[3];
            if (jqk(visible, num_visible, positions)) {
                // Replace JQK with new cards
                for (int i = 0; i < 3; i++) {
                    if (deck_pos < DECK_SIZE) visible[positions[i]] = deck[deck_pos++];
                    else visible[positions[i]] = 0; // No more cards in deck
                }
                
                if (verbose) {
                    print_visible(visible, num_visible);
                }
            } 
            else {
                // Start a new pile
                num_piles++;
                if (num_piles > MAX_PILES - 1) {
                    // More than 9 piles, we lose, we need to -1 from max_piles since array starts at 0
                    return DECK_SIZE - deck_pos; // Return number of cards left
                }
                
                visible[num_visible++] = deck[deck_pos++];
                
                if (verbose) {
                    print_visible(visible, num_visible);
                }
                
            }
        }
    }
    
    // If we've used all cards in the deck, we win
    return 0;
}

/**
 * @brief Plays multiple games and collects statistics
 *
 * Simulates N games of patience and counts how many cards remain in the deck
 * for each game. Returns an array where index i contains the count of games
 * that ended with i cards remaining.
 *
 * @param N Number of games to simulate
 * @return Dynamically allocated array containing counts of remaining cards after games played
 */
int* many_plays(int N) {
    // Allocate array to hold counts of remaining cards (0 to 52)
    int* remaining = (int*)calloc(DECK_SIZE, sizeof(int));
    int deck[DECK_SIZE];
    
    for (int i = 0; i < N; i++) {
        // Generate a new shuffled deck for each game
        shuffle_deck(deck, 0); // 0 means use the current random seed
        
        // Play the game and count remaining cards
        int cards_left = play(deck, 0); // Non-verbose mode
        remaining[cards_left]++;
    }
    
    return remaining;
}