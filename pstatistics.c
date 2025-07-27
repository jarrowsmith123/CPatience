/**
 * @file pstatistics.c
 * @brief Program to run multiple games of patience and generate statistics
 *
 * This program simulates multiple games of patience, collects statistics
 * about the number of cards remaining at the end of each game, and generates
 * a histogram of the results
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "patience_utils.h"
#include "histogram.h"

/**
 * @brief Main function that runs patience simulations and generates statistics
 *
 * The function performs the following operations:
 * 1. Seeds the random number generator
 * 2. Runs multiple games of patience
 * 3. Calculates statistics and percentages
 * 4. Displays a histogram on the console
 * 5. Writes histogram to a file
 *
 * @return 0 on successful execution, 1 if file operations fail
 */
int main() {
    // Seed the random number generator 
    srand(time(NULL));
    
    printf("Running %d games of patience...\n", NUM_GAMES);
    
    int max_cards_left = (DECK_SIZE - MAX_PILES) + 1; // We add 1 here to include 0 cards left
    printf("Maximum cards left: %d\n", max_cards_left);
    // Play many games and get statistics
    int* stats = many_plays(NUM_GAMES);
    
    // Calculate percentages
    double percentages[max_cards_left];
    for (int i = 0; i <= max_cards_left; i++) {
        percentages[i] = (double)stats[i] / NUM_GAMES * 100.0;
    }
    
    // Create array for histogram, 0 to max_cards_left
    int x[max_cards_left];
    for (int i = 0; i <= max_cards_left; i++) {
        x[i] = i;
    }
    
    // Print probability and histogram
    printf("Win probability: %.2f%%\n", percentages[0]);
    printf("\nHistogram of cards remaining:\n");
    histogram(x, percentages, max_cards_left, 100);
    
    // Write histogram to file
    FILE *file = fopen("phistogram.txt", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    
    fprintf(file, "Cards Remaining | Percentage\n");
    
    // Find max percentage to scale histogram
    double max_percentage = percentages[0];
    for (int i = 1; i < max_cards_left; i++) {
        if (percentages[i] > max_percentage) {
            max_percentage = percentages[i];
        }
    }
    
    // Write histogram to file in the same format as the histogram function
    for (int i = 0; i < max_cards_left; i++) {
        if (stats[i] > 0) {
            fprintf(file, "%d ", i);
            
            // Calculate stars by rounding the percentage
            int stars = ceil(percentages[i] * 100 / max_percentage);
            
            // Write the histogram bars
            for (int j = 0; j < stars; j++) {
                fprintf(file, "*");
            }
            
            fprintf(file, " %.3f (%d stars)\n", percentages[i], stars);
        }
    }
    
    fclose(file);
    
    printf("\nHistogram data saved to phistogram.txt\n");
    
    // Free allocated memory

    free(stats);
    
    return 0;
}