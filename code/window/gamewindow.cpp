#include "gamewindow.h"

#include <ctime>
#include <iostream>
#include <random>

#include "mainwindow.h"
#include "ui_gamewindow.h"

const QPoint hypercube_size(562, 562);
const QPoint opengl_up_left(20, 20);
const QPoint opengl_down_right = opengl_up_left + QPoint(hypercube_size.x(), hypercube_size.y());
const int TITLE_HEIGHT = 30;

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(800, 600);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    // close时析构成员变量
    // setAttribute(Qt::WA_DeleteOnClose);
    // 创建窗口
    int DEBUG = true;
    if (DEBUG) std::cerr << "GameWindow::GameWindow - 0" << std::endl;

    hypercube_ = new Hypercube::Hypercube(this);
    hypercube_->setFixedSize(hypercube_size.x(), hypercube_size.y());
    hypercube_->setGeometry(opengl_up_left.x(), opengl_up_left.y(), hypercube_->width(), hypercube_->height());

    if (DEBUG) std::cerr << "GameWindow::GameWindow - 1" << std::endl;

    if (DEBUG) std::cerr << "GameWindow::GameWindow - 2" << std::endl;

    if (DEBUG) std::cerr << "GameWindow::GameWindow - 3" << std::endl;

    if (DEBUG) std::cerr << "GameWindow::GameWindow - 4 END" << std::endl;
}

void GameWindow::InitBoard() {
    board = new Board(difficulty_);
    board->SetHypercube(hypercube_);
    // timer_init_hypercube_ = new QTimer(this);
    // connect(timer_init_hypercube_, &QTimer::timeout, [&]() {
    board->InitHypercube();
    std::cerr << "GameWindow::GameWindow InitHypercube." << std::endl;
    //});
    /*
        timer_init_hypercube_->setSingleShot(true);
        timer_init_hypercube_->setInterval(500);
        timer_init_hypercube_->start();*/
}

void GameWindow::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
    std::cout << "mouse cliked on:" << x << " " << y << std::endl;
    if (event->y() < TITLE_HEIGHT) {
        last = event->globalPos();
    }
    board->Clicked(x, y);
}
/*
 * 鼠标移动函数
 * 这里实时修改窗口的坐标
 */
void GameWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->y() < TITLE_HEIGHT) {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        this->move(this->x() + dx, this->y() + dy);
    }
}
/*
 * 鼠标释放函数
 */
void GameWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->y() < TITLE_HEIGHT) {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        this->move(this->x() + dx, this->y() + dy);
    }
}

GameWindow::~GameWindow() {
    // ui
    delete ui;
    // timer_init_hypercube_ (no need)
    // hypercube_ (no need)
    // board
    delete board;
}

void GameWindow::getDifficulty(QString data) {
    if (data == "easy") difficulty_ = 1;
    if (data == "normal") difficulty_ = 2;
    if (data == "hard") difficulty_ = 3;
    std::cout << "difficulty is " << difficulty_ << std::endl;
    InitBoard();
    QMessageBox mes(this);
    mes.setText(data);
    mes.exec();
}

void GameWindow::on_btnReturn_clicked() {
    MainWindow *mw = new MainWindow();
    mw->move(this->pos().x(), this->pos().y());
    mw->show();
    delay(20);
    this->close();
}
