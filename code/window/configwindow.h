#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include "framelesswindow.h"

// 设置窗口类

namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public FrameLessWindow {
  Q_OBJECT

 public:
  explicit ConfigWindow(QWidget *parent = nullptr);
  ~ConfigWindow();

 private slots:
  void on_btnReturn_clicked();

  void on_btnMusic_clicked();

  void on_btnSound_clicked();

 private:
  Ui::ConfigWindow *ui;

  static int music_state;
  static int sound_state;
};

#endif  // CONFIGWINDOW_H
