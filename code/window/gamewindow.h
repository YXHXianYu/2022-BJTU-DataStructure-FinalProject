#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QTime>

#include "../core/board.h"
#include "../hypercube/hypercube.h"

// 游戏窗口类

namespace Ui {
class GameWindow;
}

class GameWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit GameWindow(QWidget *parent = nullptr);
  ~GameWindow();
 public slots:
  void getDifficulty(QString data);

 protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);

 private:
  QPoint last;

 private slots:
  void on_btnReturn_clicked();

 private:
  Ui::GameWindow *ui;

  QTimer *timer_init_hypercube_;
  int timer_init_hypercube_cnt_;

  Board *board;
  Hypercube::Hypercube *hypercube_;
};

#endif  // GAMEWINDOW_H
