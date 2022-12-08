#include "configwindow.h"

#include "mainwindow.h"
#include "ui_configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ConfigWindow) {
  ui->setupUi(this);
  // 固定窗口大小
  this->setFixedSize(800, 600);
  // 去除自带的边框
  this->setWindowFlag(Qt::FramelessWindowHint);
}

ConfigWindow::~ConfigWindow() { delete ui; }

void ConfigWindow::on_btnReturn_clicked() {
  MainWindow *mw = dynamic_cast<MainWindow *>(this->parent());
  mw->show();
  delay(20);
  this->close();
}
