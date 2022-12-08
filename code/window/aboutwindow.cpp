#include "aboutwindow.h"

#include "mainwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AboutWindow) {
  ui->setupUi(this);
}

AboutWindow::~AboutWindow() { delete ui; }

void AboutWindow::on_btnReturn_clicked() {
  MainWindow *mw = dynamic_cast<MainWindow *>(this->parent());
  this->close();
  mw->show();
}
