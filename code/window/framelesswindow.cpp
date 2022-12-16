#include "framelesswindow.h"

#include "ui_framelesswindow.h"

FrameLessWindow::FrameLessWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::FrameLessWindow) {
  ui->setupUi(this);
  this->setWindowFlag(Qt::FramelessWindowHint);
  QIcon icon = QIcon(":/images/windowicon.png");
  this->setWindowIcon(icon);
  QPixmap pix(":/images/mouse.png");
  QSize size(24, 24);
  // 设置图片大小
  pix = pix.scaled(size, Qt::KeepAspectRatio);
  this->setCursor(QCursor(pix, -1, -1));
}

const int TITLE_HEIGHT = 30;

void FrameLessWindow::mousePressEvent(QMouseEvent *e) {
  if (e->y() < TITLE_HEIGHT) {
    last = e->globalPos();
  }
}

/*
 * 鼠标移动函数
 * 这里实时修改窗口的坐标
 */
void FrameLessWindow::mouseMoveEvent(QMouseEvent *event) {
  if (event->y() < TITLE_HEIGHT) {
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    last = event->globalPos();
    this->move(this->x() + dx, this->y() + dy);
  }
}
/*
 * 鼠标释放函数
 */
void FrameLessWindow::mouseReleaseEvent(QMouseEvent *event) {
  if (event->y() < TITLE_HEIGHT) {
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    this->move(this->x() + dx, this->y() + dy);
  }
}

FrameLessWindow::~FrameLessWindow() { delete ui; }
