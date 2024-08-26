/*
 * Ranking tool in "The Thinker's Toolkit" by Morgan Jones
 * Book copyright (c) 1995, 1998 Morgan Jones
 * This file is public domain.
 */

#include <stdio.h>
#include <string.h>

#include "color.h"

#define M_STR       255
#define M_STR_LEN   255

void isort(char [][M_STR], char [][M_STR_LEN], int *, int);

int
main(void)
{
    // maximum M_STR strings, 255 bytes each
    char strings[M_STR][M_STR_LEN] = { 0 };

    // read strings
    printf(BOLDWHITE "Enter a ranking (empty line when done) \n" RESET);

    int i;
    for (i = 0; i < M_STR; i++) {
        fgets(strings[i], M_STR_LEN, stdin);

        // if its an empty string, stop reading
        if (strings[i][0] == '\n') {
            strings[i][0] = '\0';
            break;
        } 

        // remove newline
        strings[i][strcspn(strings[i], "\n")] = '\0';
    }

    // init sorted strings
    char sorted_strings[i][M_STR_LEN];
    memcpy(sorted_strings, strings, sizeof(sorted_strings));

    // construct matrix of comparisons
    // NOTE: it doesn't actually store the "reason",
    // it just forces the user to type something out lol
    char matrix[i][i];

    // memcpy(3) with null bytes
    memset(matrix, 0, sizeof(matrix));

    // store the winners
    int winners[i];
    int sorted_winners[i];

    // memcpy(3) with null bytes
    memset(winners, 0, sizeof(winners));

    // compare all of the elements
    for (int j = 0; j < i; j++) {
        for (int k = j + 1; k < i; k++) {
            printf("%s vs %s (>/<): ", strings[j], strings[k]);
            // read one char, set matrix accordingly
            char c = getchar();
            if (c == '>') {
                matrix[j][k]++;
                winners[j]++;
            } else if (c == '<') {
                matrix[k][j]++;
                winners[k]++;
            } else {
                printf("Setting to " BOLDRED "<" RESET ": %c\n", c);
                matrix[k][j]++;
                winners[k]++;
            } 

            // clear input buffer
            while (getchar() != '\n');

            // ask the user to explain their choice; but we don't care what
            // they say
            printf(BOLDWHITE "Explain: " RESET);

            while (getchar() != '\n');
        }
    }

    memcpy(sorted_winners, winners, sizeof(sorted_winners));

    // insertion sort: the array **should be** nearly sorted
    isort(matrix, sorted_strings, sorted_winners, i);

    // print out the results
    for (int j = 0; j < i; j++) {
        printf("%s: %d\n", sorted_strings[j], sorted_winners[j]);
    }

    return 0;
} 

void
isort(char matrix[][M_STR], char name[][M_STR_LEN], int *value, int len) {
    // sort by value, modify key accordingly
    for (int i = 1; i < len; i++) {
        int key = value[i];

        char str_key[M_STR_LEN];
        strncpy(str_key, name[i], M_STR_LEN);

        int j = i - 1;

        // test
        printf("%s: %d %d\n", name[i], value[i], i);

        while (j >= 0 && value[j] < key ) {
            // test
            
            printf("key: %d, j: %d, value[j]: %d, value[j+1]: %d, name[j]: %s, name[j+1]: %s\n", key, j, value[j], value[j + 1], name[j], name[j + 1]);
            // matrix print at value
            printf("matrix[%d][%d]\n", j, i);

            value[j + 1] = value[j];
            strncpy(name[j + 1], name[j], M_STR_LEN);
            j--;
        }
        value[++j] = key;
        strncpy(name[j + 1], str_key, M_STR_LEN);
    }
     
    return;
} 
