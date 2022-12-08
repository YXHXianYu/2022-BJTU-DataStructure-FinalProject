#include "framelesswindow.h"

#include "ui_framelesswindow.h"

FrameLessWindow::FrameLessWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::FrameLessWindow) {
  ui->setupUi(this);
  this->setWindowFlag(Qt::FramelessWindowHint);
}

void FrameLessWindow::mousePressEvent(QMouseEvent *e) { last = e->globalPos(); }
void FrameLessWindow::mouseMoveEvent(QMouseEvent *e) {
  int dx = e->globalX() - last.x();
  int dy = e->globalY() - last.y();
  last = e->globalPos();
  move(x() + dx, y() + dy);
}
void FrameLessWindow::mouseReleaseEvent(QMouseEvent *e) {
  int dx = e->globalX() - last.x();
  int dy = e->globalY() - last.y();
  move(x() + dx, y() + dy);
}

FrameLessWindow::~FrameLessWindow() { delete ui; }
