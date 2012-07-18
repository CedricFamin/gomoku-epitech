#ifndef HISTORY_H
#define HISTORY_H

#include <queue>
#include "Goban.h"

class History
{
public:
    History();
    ~History();
    struct Turn
    {
        int x;
        int y;
        Goban::PION_TYPE pion;
        bool capture;
        int heuristicScore;
    };
    std::queue<Turn*> turnList;
};

#endif // HISTORY_H
