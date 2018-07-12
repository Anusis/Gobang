#include "chess.h"

Chess::Chess(int c, int x, int y): color(c), x(x), y(y){}

void Chess::show(QPainter *pa){
    if (this->color == 0)
        pa->drawPixmap(6+x*66,3+y*66,66,66,QPixmap("://c_black.png"));
    else
        pa->drawPixmap(6+x*66,3+y*66,66,66,QPixmap("://c_white.png"));
}
