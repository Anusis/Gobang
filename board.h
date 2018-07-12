#ifndef BOARD_H
#define BOARD_H
#include <QPainter>
#include <vector>
#include "selfchess.h"
#include "comchess.h"
using namespace std;


class Board
{
public:
    Board();
    ~Board(){}
    void show(QPainter *pa);    //画出棋盘上的棋子
    void layChessman(int x, int y);     //玩家下棋
    void layOtherChess();    //评估得到最优点->电脑下棋
    int getdata(int x, int y) { return this->data[x][y];}

    void setCompos();    //设置电脑下棋的位置

    int Minimax(int x, int y, int alpha, int beta, int depth, int Max);
    int getValue(int x, int y);       //x,y位置上的棋子是不是我想要的，对我而言的价值是多少
    //int getCount(int num, int side);
    bool judge(int num, int x, int y, int dx, int dy);  //判断十二种可能的情况
    bool join(int x, int y, int dx, int dy, int num, bool barry);   //连珠
    bool stem(int x, int y, int dx, int dy, int num, bool barry);   //堵住

    bool unneed(int i, int j);

    bool checkwin(int side);
    bool checktie();
private:
    vector<SelfChess> player;             //构造函数传(MainWindow::order+1)%2,表示棋子种类
    vector<ComChess> computer;            //构造函数传MainWindow::order%2
    int data[15][15];       //标志点上的棋子-1:player, 1:computer
    int value[15][15];      //各个点评估值
    int next_x;
    int next_y;

};

#endif // BOARD_H
