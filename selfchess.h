#ifndef SELFCHESS_H
#define SELFCHESS_H
#include "chess.h"


class SelfChess:public Chess
{
public:
    SelfChess(int c, int x, int y);
    ~SelfChess(){}
};

#endif // SELFCHESS_H
