#include "board.h"
#include "selfchess.h"
#include "comchess.h"
#include "mainwindow.h"
#include <iostream>
#include <ctime>
#include <cassert>
using namespace std;

Board::Board(){         //c初始化
    for (int i=0;i<15;i++)
        for (int j=0;j<15;j++){
            this->data[i][j] = 0;
            this->value[i][j] = -1000000;
        }
    this->next_x = -1;
    this->next_y = -1;
}

void Board::show(QPainter *pa){
    vector<SelfChess>::iterator it;
    for(it=this->player.begin();it!=this->player.end();it++){   //画玩家的棋子
        (*it).show(pa );
    }

    vector<ComChess>::iterator iter;
    for(iter=this->computer.begin();iter!=this->computer.end();iter++){     //画电脑的棋子
        (*iter).show(pa);
    }
}

void Board::layChessman(int x, int y){
    SelfChess newchess(MainWindow::order, x, y);        //setplayerchess
    this->player.push_back(newchess);
    this->data[x][y] = -1;
}

void Board::layOtherChess(){
    this->setCompos();
    ComChess newchess((MainWindow::order+1)%2, this->next_x, this->next_y); //setcomchess
    this->computer.push_back(newchess);
    this->data[this->next_x][this->next_y] = 1;
}

void Board::setCompos(){    //确定next x，y
    for (int i=0;i<15;i++)          //这段为方案一中setvalue的基础value，感觉有必要又不能放在递归中，所以暂时在这里
        for (int j=0;j<15;j++){
            if(this->data[i][j] != 0)
                this->value[i][j] = -1000000;
            else
                this->value[i][j] = (abs(7-i)+abs(7-j))*(-200000);

            if (unneed(i,j)) continue;
            this->value[i][j] = this->Minimax(i, j, -1000000, 1000000, 3, 1);  //true棋放在i，j的value值
    }

    int maxvalue = -2000000;
    for (int i=0;i<15;i++){
        for (int j=0;j<15;j++)
            if(maxvalue < this->value[i][j]){
                this->next_x = i;
                this->next_y = j;
                maxvalue = this->value[i][j];
            }
        }
    return ;
}

int Board::Minimax(int x, int y, int alpha, int beta, int depth, int Max){
    this->data[x][y] = Max;
    int bestvalue, minimax;
    if (depth <= 0 || this->checkwin(-1) || this->checkwin(1)){
        bestvalue = this->getValue(x, y);
        this->data[x][y] = 0;
        return bestvalue;
    }

    if (Max == -1 ){      //设置电脑方落子时value
        bestvalue = -1000000;
        for (int i=0;i<15;i++)      //生成子节点：玩家落子
            for (int j=0;j<15;j++)
            {
                if (this->unneed(i,j)) continue;
                minimax = Minimax(i,j,alpha, beta, depth-1,1);

                if (bestvalue < minimax)    bestvalue = minimax;
                if (bestvalue>=beta)    {this->data[x][y] = 0; return bestvalue;}
                if (bestvalue>alpha)    alpha = bestvalue;
            }
    }
    else if (Max == 1){       //设置玩家落子时value
        bestvalue = 1000000;
        for (int i=0;i<15;i++)      //生成子节点：电脑方落子
            for (int j=0;j<15;j++)
            {
                if (this->unneed(i,j)) continue;
                minimax = Minimax(i,j,alpha, beta, depth-1,-1);
                if (bestvalue > minimax)     bestvalue = minimax;
                if (bestvalue<=alpha)   {this->data[x][y] = 0;return bestvalue;}
                if (bestvalue<beta)     beta = bestvalue;
            }
    }
    this->data[x][y] = 0;
    return bestvalue;
}

bool Board::unneed(int i, int j){
    if (this->data[i][j] != 0)
        return true;
    for (int a=-1;a<=1;a++)
        for (int b=-1;b<=1;b++){
            if (i+a>0 && i+a<15 && j+b>0 && j+b<15 && this->data[i+a][j+b] != 0) return false;
        }
    return true;
}

int Board::getValue(int x, int y){
    int value = 0, count = 0;
    //分别向四个方向检查十二种情况；i：0-11
    for (int i=0;i<=12;i++){
        count = 0;
        if (this->judge(i,x,y,-1,1)) count++;
        if (this->judge(i,x,y,0,1)) count++;
        if (this->judge(i,x,y,1,1)) count++;
        if (this->judge(i,x,y,1,0)) count++;
        if (count == 1){
            switch (i) {
            case 0:     value += 100000;    break;
            case 1:     value += 70000;    break;
            case 2:     value += 2000;    break;
            case 3:     value += 3000;    break;
            case 4:     value += 800;    break;
            case 5:     value += 500;    break;
            case 6:     value += 200;    break;
            default:
                break;
            }
            break;
        }
        if (count > 1){
            switch (i) {
            case 0:     value += 200000;    break;
            case 1:     value += 14000;    break;
            case 2:     value += 4000;    break;
            case 3:     value += 10000;    break;
            case 4:     value += 1800;    break;
            case 5:     value += 6000;    break;
            case 6:     value += 800;    break;
            default:
                break;
            }
            break;
        }

    }
    if (this->data[x][y] == 1) return value;        //是电脑的棋子，返回正值，否则返回负值
    else return -value;
}

bool Board::judge(int num, int x, int y, int dx, int dy){
    switch (num) {
    case 0:{            //五子连珠
        if (this->join(x,y,dx,dy,5,0 || this->join(x,y,dx,dy,5,1)))
            return true;
        break;
    }
    case 1:       //活四
        return this->join(x,y,dx,dy,4,0);
        break;
    case 2:        //眠四
        return this->join(x,y,dx,dy,4,1);
        break;
    case 3:        //活三
        return this->join(x,y,dx,dy,3,0);
        break;
    case 4:        //眠三
        return this->join(x,y,dx,dy,3,1);
        break;
    case 5:        //活二
        return this->join(x,y,dx,dy,2,0);
        break;
    case 6:       //眠二
        return this->join(x,y,dx,dy,2,1);
        break;
    default:
        break;
    }
    return false;
}

bool Board::join(int x, int y, int dx, int dy, int num, bool barry){
    int side = data[x][y], count = 0, empty = 0, havebarry = 0;
    for (int i=1;i<=num;i++){
        if (this->data[x+i*dx][y+i*dy] == side) count++;
        else if (this->data[x+i*dx][y+i*dy] == 0){
            empty++;break;
        }
        else{
            havebarry++;break;
        }
    }
    for (int i=1;i<=num;i++){
        if (this->data[x-i*dx][y-i*dy] == side) count++;
        else if (this->data[x-i*dx][y-i*dy] == 0){
            empty++;break;
        }
        else {
            havebarry++;break;
        }

    }
    if ((count == num-1)&&(empty == 2)&&(havebarry == 0)&&(!barry)) return true;
    else if((count == num-1)&&(empty == 1)&&(havebarry == 1)&&(barry)) return true;
    return false;
}

bool Board::checkwin(int side){
    for(int i=0;i<15;++i)
        for(int j=0;j<15;++j)
            {
            if(data[i][j]!=side)continue;
            int dx=1,dy=0;
            for(int k=0;k<=5;++k)
                {
                if(k==5)return true;
                int tx=i+dx*k,ty=j+dy*k;
                if(tx<0||tx>=15||ty<0||ty>=15||data[tx][ty]!=side)break;
                }
            dx=1,dy=1;
            for(int k=0;k<=5;++k)
                {
                if(k==5)return true;
                int tx=i+dx*k,ty=j+dy*k;
                if(tx<0||tx>=15||ty<0||ty>=15||data[tx][ty]!=side)break;
                }
            dx=0,dy=1;
            for(int k=0;k<=5;++k)
                {
                if(k==5)return true;
                int tx=i+dx*k,ty=j+dy*k;
                if(tx<0||tx>=15||ty<0||ty>=15||data[tx][ty]!=side)break;
                }
            dx=-1,dy=1;
            for(int k=0;k<=5;++k)
                {
                if(k==5)return true;
                int tx=i+dx*k,ty=j+dy*k;
                if(tx<0||tx>=15||ty<0||ty>=15||data[tx][ty]!=side)break;
                }
            }
    return false;
}

bool Board::checktie()
{
    for(int i=0;i<15;++i)
        for(int j=0;j<15;++j)
            if(data[i][j]==0)return false;
    return true;
}
