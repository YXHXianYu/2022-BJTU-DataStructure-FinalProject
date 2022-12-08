#include "configwindow.h"

#include "mainwindow.h"
#include "ui_configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ConfigWindow) {
  ui->setupUi(this);
}

ConfigWindow::~ConfigWindow() { delete ui; }

void ConfigWindow::on_btnReturn_clicked() {
  MainWindow *mw = dynamic_cast<MainWindow *>(this->parent());
  this->close();
  mw->show();
}
