#include "gamewindow.h"

#include <ctime>
#include <iostream>
#include <random>

#include "mainwindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GameWindow) {
  ui->setupUi(this);
  // 固定窗口大小
  this->setFixedSize(800, 600);
  // 去除自带的边框
  this->setWindowFlag(Qt::FramelessWindowHint);
  // 创建窗口
  hypercube_ = new Hypercube::Hypercube(this);
  hypercube_->setFixedSize(562, 562);
  hypercube_->setGeometry(20, 20, hypercube_->width(), hypercube_->height());

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

const QPoint opengl_up_left(19, 19);
const QPoint opengl_down_right(19 + 562, 19 + 562);
const int TITLE_HEIGHT = 30;

void GameWindow::mousePressEvent(QMouseEvent *event) {
  int x = event->x();
  int y = event->y();
  std::cout << "mouse cliked on:" << x << " " << y << std::endl;
  if (event->y() < TITLE_HEIGHT) {
    last = event->globalPos();
  }
  if (x >= opengl_up_left.x() && x <= opengl_down_right.x() &&
      y >= opengl_up_left.y() && y <= opengl_down_right.y())
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
