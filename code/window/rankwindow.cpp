#include "rankwindow.h"

#include "mainwindow.h"
#include "ui_rankwindow.h"

RankWindow::RankWindow(QWidget *parent)
    : FrameLessWindow(parent), ui(new Ui::RankWindow) {
  ui->setupUi(this);
  // 固定窗口大小
  this->setFixedSize(800, 600);
  // 去除自带的边框
  this->setWindowFlag(Qt::FramelessWindowHint);
}

RankWindow::~RankWindow() { delete ui; }

void RankWindow::on_btnReturn_clicked() {
  MainWindow *mw = dynamic_cast<MainWindow *>(this->parent());
  mw->show();
  delay(20);
  this->close();
}
