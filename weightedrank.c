/*
 * (weighted) ranking tool in "The Thinker's Toolkit" by Morgan Jones
 * Book copyright (c) 1995, 1998 Morgan Jones
 * This file is public domain.
 */

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "color.h"
#include "rank.h"

void usage(void);

static struct option long_options[] = {
    {"help",        no_argument,        NULL, 'h'},
    {"no-color",    no_argument,        NULL, 'n'},
    {"weighted",    no_argument,        NULL, 'w'},
    {"log-file",    required_argument,  NULL, 'l'},
    {NULL,          0,                  NULL,  0 }
};

int
main(int argc, char *argv[])
{
    // allocate memory for Rank
    struct RankList *items = malloc(sizeof(struct RankList));
    memset(items, 0, sizeof(struct RankList));
    if (!items) {
        printf(BOLDRED "Error: out of memory\n" RESET);
        return 1;
    }
    
    // get options
    char ch;
    char weighted = 0;
    while ((ch = getopt_long(argc, argv, "hnwl:", long_options, NULL)) != -1) {
        switch (ch) {
            case 'h':
                usage();
                break;
            case 'n':
                //TODO: disable_color();
                puts("Not implemented yet");
                break;
            case 'w':
                weighted = 1;
                break;
            case 'l':
                //TODO: set_log_file(optarg);
                puts("Not implemented yet");
                break;
            default:
                usage();
                break;
        }
    }

    rank(items);

    if (!weighted) {
        // print the rank list
        print_ranklist(items);
    } else {

        // leave if any are null
        if (items->rank_sorted[0].name[0] == 0 || 
            items->rank_sorted[1].name[0] == 0 ||
            items->rank_sorted[2].name[0] == 0) {
                
            puts(BOLDRED "Error: not enough weights.\n" RESET);
            goto w_end;
        }

        /* we are now going to rank in a weighted manner for all 3 */

        // init
        struct WAlloc *mem = malloc(sizeof(struct WAlloc));
        struct Weighted weighted = mem->weighted;

        // move the top 3 items from the ranklist here
        weighted.opt1 = items->rank_sorted[0].name;
        weighted.opt2 = items->rank_sorted[1].name;
        weighted.opt3 = items->rank_sorted[2].name;

        char buf[16]; //enough to hold weight
        char *ptr;
        // ask for weights
        printf(BOLDGREEN "Enter weight for %s\n" RESET);
        fgets(buf, 16, stdin);
        weighted.opt1_w = strtof(buf, &ptr, 10);
        if (ptr != NULL) {
            printf(BOLDWHITE "Using %f" RESET "\n", weighted.opt1_w);
        }

        printf(BOLDGREEN "Enter weight for %s\n" RESET);
        fgets(buf, 16, stdin);
        weighted.opt2_w = strtof(buf, &ptr, 10);
            
        if (ptr != NULL) {
            printf(BOLDWHITE "Using %f" RESET "\n", weighted.opt2_w);
        }

        printf(BOLDGREEN "Enter weight for %s\n" RESET);
        fgets(buf, 16, stdin);
        weighted.opt3_w = strtof(buf, &ptr, 10);
        
        if (ptr != NULL) {
            printf(BOLDWHITE "Using %f" RESET "\n", weighted.opt3_w);
        }

        printf(BOLDRED "\nRank according to %s\n\n" RESET, weighted.opt1);
        rank(&mem->r[0]);
        printf(BOLDRED "\nRank according to %s\n\n" RESET, weighted.opt2);
        rank(&mem->r[1]);
        printf(BOLDRED "\nRank according to %s\n\n" RESET, weighted.opt3);
        rank(&mem->r[2]);

        
w_end:
        free(mem);
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

void
rank(struct RankList *items) {
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
    char matrix[M_STR][M_STR];

    // memset(3) with null bytes
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

    items->rank_count = i;

    // insertion sort: the array **should be** nearly sorted
    isort(matrix, items->rank_sorted, i);
}

void
print_ranklist(struct RankList *items) {
    printf(BOLDRED "Original: " RESET " | " BOLDGREEN "Sorted: " RESET "\n\n");
    for (int j = 0; j < items->rank_count; j++) {
        printf("%s: %d | %s: %d\n", 
                items->rank[j].name, items->rank[j].score, 
                items->rank_sorted[j].name, items->rank_sorted[j].score);
    }
}

void 
usage(void) {
    puts(BOLDWHITE "Usage: weightedrank [options]\n" 
                   "Options:\n" RESET
         "  -h, --help            display this help and exit\n"
         "  -n, --no-color        disable colors\n"
         "  -w, --weighted        use weighted rankings\n"
         "  -l, --log-file FILE   write log to FILE\n");
} 
