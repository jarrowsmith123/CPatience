/**
 * @file histogram.c
 * @brief Functions for creating and displaying histograms
 * @details Provides utilities for generating histograms from data arrays
 *          and visualizing distributions using ASCII characters
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Displays a histogram of data using ASCII characters
 *
 * @param x Array of x-axis values (labels)
 * @param y Array of y-axis values (frequencies/values to plot)
 * @param n Number of elements in the arrays
 * @param width Maximum width of the histogram in characters
 * @details Scales the histogram bars proportionally to the maximum y value
 *          and displays the actual value and star count for each bar
 */
void histogram(int *x, double *y, int n, int width) {

    double max_y = y[0];
    for (int i = 1; i < n; i++) {
        if (y[i] > max_y) {
            max_y = y[i];
        }
    }

    // Print histogram: x value, stars, percentage and star count
    for (int i = 0; i < n; i++) {

        printf("%d ", x[i]);
        // Calculate stars by rounding the percentage
        int stars = ceil(y[i] * width / max_y);
    
        for (int j = 0; j < stars; j++) {
            printf("*");
        }

        printf(" %.3f (%d stars)\n", y[i], stars);
    }
}

/**
 * @brief Creates a histogram of string lengths from an array of strings
 *
 * @param strings Array of strings to analyze
 * @param n Number of strings in the array
 * @return int* Dynamically allocated array where index i contains the count of strings with length i
 */
int *histogram_lengths(char **strings, int n) {
    // Find the maximum string length
    int max_length = 0;
    for (int i = 0; i < n; i++) {
        int len = strlen(strings[i]);
        if (len > max_length) {
            max_length = len;
        }
    }

    // Create the histogram array
    int *H = calloc(max_length, sizeof(int));

    // Count occurrences of each length
    for (int i = 0; i < n; i++) {
        int len = strlen(strings[i]);
        H[len] += 1;
    }

    return H;
}
