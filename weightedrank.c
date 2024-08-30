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

    items->rank_count = rsstdin(items->rank);
    for (int i = 0; i < items->rank_count; i++) {
        strncpy(items->srank[i].name, items->rank[i], M_STR_LEN);
        items->srank[i].score = 0;
    }
    rank(items->srank, items->rank_count);

    print_ranklist(items);
    if (weighted) {
        // weighted ranking
        puts(BOLDWHITE "Choosing top 3 weights." RESET);

        // leave if any are null
        if (items->rank_count < 3) {
            puts(BOLDRED "Error: not enough weights.\n" RESET);
            goto w_end;
        }

        /* we are now going to rank in a weighted manner for all 3 */

        // init
        struct WAlloc *mem = malloc(sizeof(struct WAlloc));
        struct Weighted weighted = mem->w;

        // move the top 3 items from the ranklist here
        strncpy(weighted.opt1, items->srank[0].name, M_STR);
        strncpy(weighted.opt2, items->srank[1].name, M_STR);
        strncpy(weighted.opt3, items->srank[2].name, M_STR);

        char buf[16]; //enough to hold weight
        float left = 1.0; // 100%
        char *ptr;

        /* weight 1 */
        printf(BOLDWHITE "Weight for %s\n" BOLDGREEN "(1.0) > " RESET, weighted.opt1);
        fgets(buf, 16, stdin);
        weighted.opt1_w = strtof(buf, &ptr);
        if (weighted.opt1_w >= 1.0 || weighted.opt1_w <= 0.0) {
            printf(BOLDRED "Error: weight must be between 0 and 1\n" RESET);
            goto w_end;
        }

        printf(BOLDWHITE "Using %g for %s" RESET "\n", 
                weighted.opt1_w, weighted.opt1);

        left -= weighted.opt1_w;

        /* weight 2 */
        printf(BOLDWHITE "Weight for %s\n" BOLDGREEN "(%g) > " RESET, weighted.opt2, left);
        fgets(buf, 16, stdin);
        weighted.opt2_w = strtof(buf, &ptr);
        if (weighted.opt2_w >= 1.0 || weighted.opt2_w <= 0.0) {
            printf(BOLDRED "Error: weight must be between 0 and 1\n" RESET);
            goto w_end;
        }

        printf(BOLDWHITE "Using %g for %s" RESET "\n", 
                weighted.opt2_w, weighted.opt2);

        left -= weighted.opt2_w;

        /* weight 3 */
        weighted.opt3_w = left;
        printf(BOLDWHITE "Using %g for %s" RESET "\n", 
                weighted.opt3_w, weighted.opt3);

        // copy memory before ranking
        int space = sizeof(struct Rank) * items->rank_count;

        printf("\n\n" BOLDWHITE "**Choose items to rank!**" "\n");
        mem->rl.rank_count = rsstdin(mem->rl.rank);

        // manual copy with for loop
        for (int i = 0; i < items->rank_count; i++) {
            strncpy(mem->rl.srank[0][i].name, mem->rl.rank[i], M_STR_LEN);
        }

        memcpy(mem->rl.srank[1], mem->rl.srank[0], space);
        memcpy(mem->rl.srank[2], mem->rl.srank[1], space);


        printf(BOLDRED "\nRank according to %s\n\n" RESET, weighted.opt1);
        rank(mem->rl.srank[0], mem->rl.rank_count);
        printf(BOLDRED "\nRank according to %s\n\n" RESET, weighted.opt2);
        rank(mem->rl.srank[1], mem->rl.rank_count);
        printf(BOLDRED "\nRank according to %s\n\n" RESET, weighted.opt3);
        rank(mem->rl.srank[2], mem->rl.rank_count);

        printf("\n\n" BOLDWHITE "**FINAL RANKING**" "\n"
                BOLDYELLOW "name | "
                "%s " BOLDGREEN "(%g)" BOLDWHITE " | " 
                "%s " BOLDGREEN "(%g)" BOLDWHITE " | " 
                "%s " BOLDGREEN "(%g)" BOLDWHITE " | "
                "total " RESET "\n",
                weighted.opt1, weighted.opt1_w, 
                weighted.opt2, weighted.opt2_w, 
                weighted.opt3, weighted.opt3_w);

        for (int i = 0; i < mem->rl.rank_count; i++) {
            // garbage dot product T_T
            float total = 
                ((double) mem->rl.srank[0][i].score) * weighted.opt1_w +
                ((double) mem->rl.srank[1][i].score) * weighted.opt2_w +
                ((double) mem->rl.srank[2][i].score) * weighted.opt3_w;

            printf(BOLDWHITE "%s | " 
                    BOLDGREEN "%d*%g " BOLDWHITE "| " 
                    BOLDGREEN "%d*%g " BOLDWHITE "| " 
                    BOLDGREEN "%d*%g " BOLDWHITE " |" 
                    BOLDGREEN "%g " RESET "\n", 
                    mem->rl.srank[0][i].name, 
                    mem->rl.srank[0][i].score, weighted.opt1_w, 
                    mem->rl.srank[1][i].score, weighted.opt2_w, 
                    mem->rl.srank[2][i].score, weighted.opt3_w, 
                    total);
        }
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

int
rsstdin(char items[M_STR][M_STR_LEN]) 
{
    puts(BOLDWHITE "Enter items..." RESET);
    int i;
    for (i = 0; i < M_STR; i++) {
        // local buffer for name
        printf(BOLDGREEN "> " RESET);
        fgets(items[i], M_STR_LEN, stdin);

        // if its an empty string, stop reading
        if (items[i][0] == '\n') {
            items[i][0] = '\0';
            break;
        } 

        // remove newline
        char nl = strcspn(items[i], "\n");
        if (items[i][nl] != '\n' && items[i][nl] != 0) {
            // it was cut off
            puts(BOLDRED "Truncated to 255 chars." RESET);
            items[i][nl] = '\0';
        } 
        items[i][nl] = '\0';
    }
    
    return i;
}

void
rank(struct Rank *items, int i) 
{
    // construct matrix of comparisons
    // NOTE: it doesn't actually store the "reason",
    // it just forces the user to type something out lol
    char matrix[M_STR][M_STR];

    // memset(3) with null bytes
    memset(matrix, 0, sizeof(matrix));

    // compare all of the elements
    for (int j = 0; j < i; j++) {
        for (int k = j + 1; k < i; k++) {
            printf("%s vs %s (>/<): ", items[j].name, items[k].name);
            // read one char, set matrix accordingly
            char c = getchar();
            if (c == '>') {
                matrix[j][k]++;
                items[j].score++;
            } else if (c == '<') {
                matrix[k][j]++;
                items[k].score++;
            } else {
                printf("Setting to " BOLDRED "<" RESET ": %c\n", c);
                matrix[k][j]++;
                items[k].score++;
            } 

            // clear input buffer
            while (getchar() != '\n');

            // ask the user to explain their choice; but we don't care what
            // they say
            printf(BOLDWHITE "Explain: " RESET);

            while (getchar() != '\n');
        }
    }

    // insertion sort: the array **should be** nearly sorted
    isort(matrix, items, i);
}

void
print_ranklist(struct RankList *items) {
    printf(BOLDRED "Original: " RESET " | " BOLDGREEN "Sorted: " RESET "\n\n");
    for (int j = 0; j < items->rank_count; j++) {
        printf("%s: %d | %s: %d\n", 
                items->rank[j], items->rank_count - j - 1, 
                items->srank[j].name, items->srank[j].score);
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

    exit(0);
} 
