#ifndef RANK_H
#define RANK_H

#define M_STR       255
#define M_STR_LEN   255

struct Rank
{ 
    int     score; 
    char    name[M_STR_LEN];
};

struct RankList
{
    int         rank_count;
    char        rank[M_STR][M_STR_LEN];
    struct Rank srank[M_STR];
};

struct WRankList
{
    int             rank_count;
    char            rank[M_STR][M_STR_LEN];
    struct Rank     srank[3][M_STR];
};

struct Weighted
{
    float opt1_w;
    float opt2_w;
    float opt3_w;

    char opt1[M_STR_LEN];
    char opt2[M_STR_LEN];
    char opt3[M_STR_LEN];
};

/* for one big alloc operation instead of multiple */
struct WAlloc 
{
    struct Weighted     w;
    struct WRankList    rl;
};

void isort(char [][M_STR], struct Rank[], int);
void rank(struct Rank *, int);
void print_ranklist(struct RankList *);

/* read string from stdin */
int  rsstdin(char [][M_STR_LEN]);

#endif
