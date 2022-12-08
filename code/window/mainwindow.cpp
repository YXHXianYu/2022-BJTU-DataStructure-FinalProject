#include "mainwindow.h"

#include "aboutwindow.h"
#include "configwindow.h"
#include "gamewindow.h"
#include "rankwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_btnGame_clicked() {
  GameWindow *gw = new GameWindow(this);
  gw->show();
  this->hide();
}

void MainWindow::on_btnRank_clicked() {
  RankWindow *gw = new RankWindow(this);
  gw->show();
  this->hide();
}

void MainWindow::on_btnConfig_clicked() {
  ConfigWindow *cw = new ConfigWindow(this);
  cw->show();
  this->hide();
}

void MainWindow::on_btnAbout_clicked() {
  AboutWindow *aw = new AboutWindow(this);
  aw->show();
  this->hide();
}
