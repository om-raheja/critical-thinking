/*
 * Ranking tool in "The Thinker's Toolkit" by Morgan Jones
 * Book copyright (c) 1995, 1998 Morgan Jones
 * This file is public domain.
 */

#include <stdio.h>
#include <string.h>

#include "color.h"

#define M_STR 255

int
main(void)
{
    // maximum M_STR strings, 255 bytes each
    char strings[M_STR][255] = { 0 };

    // read strings
    printf(BOLDWHITE "Enter strings: (empty line when done) \n" RESET);

    int i;
    for (i = 0; i < M_STR; i++) {
        fgets(strings[i], 255, stdin);
        // if its an empty string, stop reading
        if (strings[i][0] == '\n') {
            strings[i][0] = '\0';
            break;
        } 

        // remove newline
        strings[i][strcspn(strings[i], "\n")] = '\0';
    }

    // construct matrix of comparisons
    // NOTE: it doesn't actually store the "reason",
    // it just forces the user to type something out lol
    char matrix[i][i];

    // memcpy(3) with null bytes
    memset(matrix, 0, sizeof(matrix));

    // store the winners
    int winners[i];

    printf("%d: %d\n", i, sizeof(winners));

    // memcpy(3) with null bytes
    memset(winners, 0, sizeof(winners));

    // compare all of the elements
    for (int j = 0; j < i; j++) {
        for (int k = j + 1; k < i; k++) {
            printf("%s vs %s (>/<): ", strings[j], strings[k]);
            // read one char, set matrix accordingly
            char c = getchar();
            if (c == '>') {
                matrix[j][k] = 1;
                winners[j]++;
            } else if (c == '<') {
                matrix[j][k] = 0;
                winners[k]++;
            } else {
                printf("Setting to " BOLDRED "<" RESET ": %c\n", c);
                matrix[j][k] = 0;
            } 

        }
    }

    // print out the results
    for (int j = 0; j < i; j++) {
        printf("%s: %d\n", strings[j], winners[j]);
    }

    return 0;
} 
