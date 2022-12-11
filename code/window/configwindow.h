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

  void on_btn_stopMusic_clicked();


  void on_btn_playMusic_clicked();

  void on_btn_stopSound_clicked();

  void on_btn_openSound_clicked();

private:
  Ui::ConfigWindow *ui;
};

#endif  // CONFIGWINDOW_H
