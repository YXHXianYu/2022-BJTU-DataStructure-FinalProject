#ifndef FRAMELESSWINDOW_H
#define FRAMELESSWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

// 无边窗口类，为其他窗口的父类

namespace Ui {
class FrameLessWindow;
}

class FrameLessWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit FrameLessWindow(QWidget *parent = nullptr);
  ~FrameLessWindow();

 protected:
  void mousePressEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);

 private:
  QPoint last;

 private:
  Ui::FrameLessWindow *ui;
};

#endif  // FRAMELESSWINDOW_H
