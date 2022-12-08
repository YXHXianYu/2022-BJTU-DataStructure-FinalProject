#include "mainwindow.h"

#include "aboutwindow.h"
#include "configwindow.h"
#include "gamewindow.h"
#include "rankwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  // 固定窗口大小
  this->setFixedSize(800, 600);
  // 去除自带的边框
  // this->setWindowFlag(Qt::FramelessWindowHint);
}

MainWindow::~MainWindow() { delete ui; }

void delay(int x) {
  QTime dieTime = QTime::currentTime().addMSecs(x);
  while (QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::on_btnGame_clicked() {
  GameWindow *gw = new GameWindow(this);
  gw->show();
  delay(200);
  this->hide();
}

void MainWindow::on_btnRank_clicked() {
  RankWindow *gw = new RankWindow(this);
  gw->show();
  delay(200);
  this->hide();
}

void MainWindow::on_btnConfig_clicked() {
  ConfigWindow *cw = new ConfigWindow(this);
  cw->show();
  delay(200);
  this->hide();
}

void MainWindow::on_btnAbout_clicked() {
  AboutWindow *aw = new AboutWindow(this);
  aw->show();
  delay(200);
  this->hide();
}
