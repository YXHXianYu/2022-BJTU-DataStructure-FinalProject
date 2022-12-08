#include "gamewindow.h"

#include "mainwindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GameWindow) {
  ui->setupUi(this);
  // 固定窗口大小
  this->setFixedSize(800, 600);
  // 去除自带的边框
  this->setWindowFlag(Qt::FramelessWindowHint);
}

GameWindow::~GameWindow() { delete ui; }

void GameWindow::on_btnReturn_clicked() {
  MainWindow *mw = dynamic_cast<MainWindow *>(this->parent());
  mw->show();
  delay(20);
  this->close();
}
