#include "gamewindow.h"

#include <ctime>
#include <iostream>
#include <random>

#include "mainwindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) : FrameLessWindow(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(800, 600);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    // 创建窗口
    hypercube_ = new Hypercube::Hypercube(this);
    hypercube_->setFixedSize(562, 562);
    hypercube_->setGeometry(19, 19, hypercube_->width(), hypercube_->height());

    board = new Board();
    board->SetHypercube(hypercube_);

    timer_init_hypercube_ = new QTimer(this);
    connect(timer_init_hypercube_, &QTimer::timeout, [&]() {
        board->InitHypercube();
        std::cerr << "GameWindow::GameWindow InitHypercube." << std::endl;
    });
    timer_init_hypercube_->setSingleShot(true);
    timer_init_hypercube_->start(500);
}

void GameWindow::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
    std::cout << "mouse cliked on:" << x << " " << y << std::endl;
    board->Clicked(x, y);
}

GameWindow::~GameWindow() {
    delete ui;
    // timer_init_hypercube_;
    delete timer_init_hypercube_;
    timer_init_hypercube_ = nullptr;
}

void GameWindow::on_btnReturn_clicked() {
    MainWindow *mw = dynamic_cast<MainWindow *>(this->parent());
    mw->show();
    delay(20);
    this->close();
}
