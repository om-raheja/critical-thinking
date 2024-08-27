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


#endif
