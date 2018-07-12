#ifndef COMCHESS_H
#define COMCHESS_H
#include "chess.h"


class ComChess:public Chess
{
public:
    ComChess(int c, int x, int y);
    ~ComChess(){}
};

#endif // COMCHESS_H
