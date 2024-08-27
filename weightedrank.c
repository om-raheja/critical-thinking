/*
 * (weighted) ranking tool in "The Thinker's Toolkit" by Morgan Jones
 * Book copyright (c) 1995, 1998 Morgan Jones
 * This file is public domain.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "color.h"
#include "rank.h"

void isort(char [][M_STR], struct Rank[], int);

int
main(void)
{
    // allocate memory for Rank
    struct RankList *items = malloc(sizeof(struct RankList));
    memset(items, 0, sizeof(struct RankList));
    if (!items) {
        printf(BOLDRED "Error: out of memory\n" RESET);
        return 1;
    }

    // read strings
    printf(BOLDWHITE "Enter a ranking (empty line when done) \n" RESET);

    int i;
    for (i = 0; i < M_STR; i++) {
        // local buffer for name
        char *name = items->rank[i].name;

        fgets(name, M_STR_LEN, stdin);

        // if its an empty string, stop reading
        if (name[0] == '\n') {
            name[0] = '\0';
            break;
        } 

        // remove newline
        name[strcspn(name, "\n")] = '\0';
    }

    // construct matrix of comparisons
    // NOTE: it doesn't actually store the "reason",
    // it just forces the user to type something out lol
    char matrix[i][i];

    // memcpy(3) with null bytes
    memset(matrix, 0, sizeof(matrix));

    // compare all of the elements
    for (int j = 0; j < i; j++) {
        for (int k = j + 1; k < i; k++) {
            printf("%s vs %s (>/<): ", items->rank[j].name, items->rank[k].name);
            // read one char, set matrix accordingly
            char c = getchar();
            if (c == '>') {
                matrix[j][k]++;
                items->rank[j].score++;
            } else if (c == '<') {
                matrix[k][j]++;
                items->rank[k].score++;
            } else {
                printf("Setting to " BOLDRED "<" RESET ": %c\n", c);
                matrix[k][j]++;
                items->rank[k].score++;
            } 

            // clear input buffer
            while (getchar() != '\n');

            // ask the user to explain their choice; but we don't care what
            // they say
            printf(BOLDWHITE "Explain: " RESET);

            while (getchar() != '\n');
        }
    }

    memcpy(items->rank_sorted, items->rank, sizeof(struct Rank) * M_STR);

    // insertion sort: the array **should be** nearly sorted
    isort(matrix, items->rank_sorted, i);

    printf(BOLDRED "Original: " RESET " | " BOLDGREEN "Sorted: " RESET "\n\n");
    for (int j = 0; j < i; j++) {
        printf("%s: %d | %s: %d\n", 
                items->rank[j].name, items->rank[j].score, 
                items->rank_sorted[j].name, items->rank_sorted[j].score);
    }

    free(items);
    return 0;
} 

void
isort(char matrix[][M_STR], struct Rank to_sort[], int len) {
    // sort by value, modify key accordingly
    for (int i = 1; i < len; i++) {
        int key = to_sort[i].score;

        char str_key[M_STR_LEN];
        strncpy(str_key, to_sort[i].name, M_STR_LEN);

        int j = i - 1;

        while (j >= 0 && (to_sort[j].score < key || matrix[i][j]) ) {
            to_sort[j + 1].score = to_sort[j].score;
            strncpy(to_sort[j + 1].name, to_sort[j].name, M_STR_LEN);

            j--;
        }
        to_sort[++j].score = key;
        strncpy(to_sort[j].name, str_key, M_STR_LEN);
    }
     
    return;
} 
