/**
 * @file patience.c
 * @brief Implementation of a patience card game
 * 
 * This program plays a game of patience with either a random seed
 * or a seed provided as a command line argument. The game output is saved to
 * either win.txt or lose.txt depending on the outcome
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "patience_utils.h"

/**
 * @brief Main function that runs the patience card game
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return int Exit status code
 * 
 * I feel like this comment isn't that helpful but I'll put it here anyway :)
 */
int main(int argc, char *argv[]) {
    int deck[DECK_SIZE];
    int seed = 0;
    FILE *output_file;
    
    // Check if a seed was provided as a command line argument
    if (argc > 1) {
        seed = atoi(argv[1]);
    } else {
        // Use current time as seed if none provided
        seed = time(NULL);
    }
    
    shuffle_deck(deck, seed);
    
    printf("Playing a game of patience with seed %u\n", seed);
    
    // Play the game and determine the outcome
    int cards_left = play(deck, 1); // Verbose mode
    
    // Determine which file to open based on the outcome
    if (cards_left == 0) {
        printf("Player wins\n");
        output_file = fopen("win.txt", "w");
    } else {
        printf("Player loses with %d cards left in the deck\n", cards_left);
        output_file = fopen("lose.txt", "w");
    }
    
    if (!output_file) {
        printf("Error: Could not open output file\n");
        return 1;
    }
    
    // Reshuffle the deck with the same seed
    shuffle_deck(deck, seed);
    
    // Redirect stdout to the file using freopen
    FILE *original_stdout = stdout;
    freopen(cards_left == 0 ? "win.txt" : "lose.txt", "w", stdout);
    
    // Print the header to the file
    printf("Game of patience with seed %u\n\n", seed);
    
    // Play the game again, this time output goes to the file, I don't like this method but it works nicely
    play(deck, 1);
    
    // Print the result to the file
    if (cards_left == 0) {
        printf("\nPlayer wins\n");
    } else {
        printf("\nPlayer loses with %d cards left in the deck\n", cards_left);
    }
    
    fclose(stdout);
    fclose(output_file);

    return 0;
}