#ifndef CHESS_H
#define CHESS_H
#include <QPainter>


class Chess
{
public:
    Chess(int c, int x, int y);
    ~Chess(){}
    void show(QPainter * pa);
private:
    int color;   //0-黑棋 ,1-白棋
    int x;      //坐标
    int y;
};

#endif // CHESS_H
