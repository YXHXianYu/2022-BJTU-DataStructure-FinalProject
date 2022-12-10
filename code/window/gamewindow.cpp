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

    // 创建Hypercube窗口
    hypercube_ = new Hypercube::Hypercube(this);
    hypercube_->setFixedSize(hypercube_size.x(), hypercube_size.y());
    hypercube_->setGeometry(opengl_up_left.x(), opengl_up_left.y(), hypercube_->width(), hypercube_->height());

    // 初始化进度条
    is_pausing_ = false;
    ui->left_time_bar->setRange(0, 600);  // 1min
    // 创建timer
    timer_flush_score_and_left_time_bar_ = new QTimer(this);
    left_time_cnt_ = -10;
    connect(timer_flush_score_and_left_time_bar_, &QTimer::timeout, [&]() {
        // score_bar
        ui->score_bar->setText(QString::fromStdString(std::to_string(board->GetScore())));
        // left_time_bar
        if (is_pausing_ == false) left_time_cnt_++;
        ui->left_time_bar->setValue(
            std::min(std::max(left_time_cnt_, ui->left_time_bar->minimum()), ui->left_time_bar->maximum()));

        if (left_time_cnt_ > ui->left_time_bar->maximum()) {
            on_btnReturn_clicked();  // 时间到，直接退出游戏（
            timer_flush_score_and_left_time_bar_->stop();
        }
    });
    timer_flush_score_and_left_time_bar_->setInterval(100);  // 0.1s
    timer_flush_score_and_left_time_bar_->start();
}

GameWindow::~GameWindow() {
    // ui
    delete ui;
    // board
    delete board;
}

void GameWindow::InitBoard() {
    // board
    board = new Board(difficulty_);
    board->SetHypercube(hypercube_);
    board->InitHypercube();
    // timer etc
    left_time_cnt_ = -10;

    std::cerr << "GameWindow::GameWindow InitHypercube." << std::endl;
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
    BGM::GetInstance()->StopBgm2();
    BGM::GetInstance()->PlayBgm1();
    delay(20);
    this->close();
}

void GameWindow::on_skill1_button_clicked() {
    board->ClickedOnDiamond();  // 道具1
}

void GameWindow::on_skill2_button_clicked() {
    board->ClickedOnLightning();  // 道具2
}

void GameWindow::on_skill3_button_clicked() {
    board->ClickedOnShuffle();  // 道具3
}

void GameWindow::on_pause_button_clicked() {
    board->ClickedOnStop();  // 暂停
}

void GameWindow::on_hint_button_clicked() {
    board->ShowHint(true);  // 提示
}
