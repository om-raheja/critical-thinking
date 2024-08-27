#ifndef RANK_H
#define RANK_H

#define M_STR       255
#define M_STR_LEN   255

struct Rank
{ 
    char name[M_STR_LEN];
    int  score;
};

struct RankList
{
    struct Rank rank[M_STR];
    struct Rank rank_sorted[M_STR];
    int  rank_count;
};

struct Weighted
{
    char opt1[M_STR_LEN];
    float opt1_w;
    char opt2[M_STR_LEN];
    float opt2_w;
    char opt3[M_STR_LEN];
    float opt3_w;
};

void isort(char [][M_STR], struct Rank[], int);
void rank(struct RankList *);
void print_ranklist(struct RankList *);

#endif
