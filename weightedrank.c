/*
 * (weighted) ranking tool in "The Thinker's Toolkit" by Morgan Jones
 * Book copyright (c) 1995, 1998 Morgan Jones
 * This file is public domain.
 */

#include <stdio.h>
#include <string.h>

#include "color.h"
#include "rank.h"

void isort(char [][M_STR], char [][M_STR_LEN], int *, int);

int
main(void)
{
    // allocate memory for Rank
    struct RankList *items = malloc(sizeof(struct RankList));
    memset(items, 0, sizeof(struct RankList));
    if (!rank) {
        printf(BOLDRED "Error: out of memory\n" RESET);
        return 1;
    }

    // read strings
    printf(BOLDWHITE "Enter a ranking (empty line when done) \n" RESET);

    int i;
    for (i = 0; i < M_STR; i++) {
        // local buffer for name
        char *name = items->rank[i]->name;

        fgets(name, M_STR_LEN, stdin);

        // if its an empty string, stop reading
        if (name[0] == '\n') {
            name[0] = '\0';
            break;
        } 

        // remove newline
        name[strcspn(name, "\n")] = '\0';
    }

    // init sorted strings
    memcpy(items->rank_sorted, items->rank, M_STR);

    // construct matrix of comparisons
    // NOTE: it doesn't actually store the "reason",
    // it just forces the user to type something out lol
    char matrix[i][i];

    // memcpy(3) with null bytes
    memset(matrix, 0, sizeof(matrix));

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

    printf(BOLDRED "Original: " RESET " | " BOLDGREEN "Sorted: " RESET "\n\n");
    for (int j = 0; j < i; j++) {
        printf("%s: %d | %s: %d\n", 
                strings[j], winners[j], 
                sorted_strings[j], sorted_winners[j]);
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

        while (j >= 0 && (value[j] < key || matrix[i][j]) ) {
            value[j + 1] = value[j];
            strncpy(name[j + 1], name[j], M_STR_LEN);

            j--;
        }
        value[++j] = key;
        strncpy(name[j], str_key, M_STR_LEN);
    }
     
    return;
} 
