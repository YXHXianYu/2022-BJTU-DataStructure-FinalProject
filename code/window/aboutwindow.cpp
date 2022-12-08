#include "aboutwindow.h"

#include "mainwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AboutWindow) {
  ui->setupUi(this);
  // 固定窗口大小
  this->setFixedSize(800, 600);
  // 去除自带的边框
  this->setWindowFlag(Qt::FramelessWindowHint);
}

AboutWindow::~AboutWindow() { delete ui; }

void AboutWindow::on_btnReturn_clicked() {
  MainWindow *mw = dynamic_cast<MainWindow *>(this->parent());
  mw->show();
  delay(20);
  this->close();
}
