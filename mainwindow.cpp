#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QApplication>


#include <iostream>
using namespace std;

int MainWindow::order = -1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1000,1000);
    ui->pushButton_3->hide();
    ui->pushButton_4->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *p){
    QPainter *pa;
    pa = new QPainter();
    pa->begin(this);

    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":\\c_board.jpg"));      //画出棋盘

    if (this->order == 0 || this->order == 1){
        if (_game.checkwin(1))      //判断输赢
            this->order = 2;
        else if (_game.checkwin(-1))
            this->order = 3;
        else if (_game.checktie())
            this->order = 4;
    }

    _game.show(pa);     //开始游戏，画出游戏中的棋子

    switch (this->order) {
    case -1:
        pa->drawPixmap(300,320,440,200,QPixmap(":\\c_tip.png"));    //提示选择黑白棋
        break;
    case 2:{
        ui->pushButton_3->show();
        ui->pushButton_4->show();
        pa->drawPixmap(300, 320, 440, 200, QPixmap(":\\c_lose.png"));   //玩家失败
        break;
    }
    case 3:{
        ui->pushButton_3->show();
        ui->pushButton_4->show();
        pa->drawPixmap(300, 320, 440, 200, QPixmap(":\\c_win.png"));    //玩家胜利
        break;
    }
    case 4:{
        ui->pushButton_3->show();
        ui->pushButton_4->show();
        pa->drawPixmap(300, 320, 440, 200, QPixmap(":\\c_tie.png"));   //平局——画图
        break;
    }
    case 5:{
        ui->pushButton_3->hide();
        ui->pushButton_4->hide();
        break;
    }
    default:
        break;
    }

    pa->end();
    delete pa;
}

void MainWindow::mousePressEvent(QMouseEvent *m){
    if((this->order < 0) ||(this->order > 1))
        return ;
    float _x = (m->x()-39.0)/66.0;      //获取鼠标点击位置
    float _y = (m->y()-37.0)/66.0;
    int x = (int)(_x+0.5);
    int y = (int)(_y+0.5);
    if (this->_game.getdata(x,y)!=0)
        return ;
    _game.layChessman(x,y);     //setplayerchess
    this->repaint();
    if((this->order < 0) ||(this->order > 1))
        return ;
    _game.layOtherChess();          //setcomputerchess
    this->repaint();
}

void MainWindow::on_pushButton_clicked()
{
    this->order = 0;              //self(1)玩家选择黑棋
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    this->repaint();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->order = 1;              //self(2)玩家选择白棋
    _game.layOtherChess();         //电脑为黑棋，先下
    ui->pushButton->hide();
    ui->pushButton_2->hide();
    this->repaint();
}

void MainWindow::on_pushButton_3_clicked()
{
    exit(1);
}


void MainWindow::on_pushButton_4_clicked()
{
    this->order = 5;
    this->repaint();
}
