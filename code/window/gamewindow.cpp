#include "gamewindow.h"

#include "mainwindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GameWindow) {
  ui->setupUi(this);
  // 固定窗口大小
  this->setFixedSize(800, 600);
}

GameWindow::~GameWindow() { delete ui; }

void GameWindow::on_btnReturn_clicked() {
  MainWindow *mw = dynamic_cast<MainWindow *>(this->parent());
  mw->show();
  this->close();
}
