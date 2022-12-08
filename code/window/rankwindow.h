#ifndef RANKWINDOW_H
#define RANKWINDOW_H

#include "framelesswindow.h"

// 排行榜窗口类

namespace Ui {
class RankWindow;
}

class RankWindow : public FrameLessWindow {
  Q_OBJECT

 public:
  explicit RankWindow(QWidget *parent = nullptr);
  ~RankWindow();

 private slots:

  void on_btnReturn_clicked();

 private:
  Ui::RankWindow *ui;
};

#endif  // RANKWINDOW_H
