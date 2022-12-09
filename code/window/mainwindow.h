#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCoreApplication>
#include <QEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QScreen>
#include <QTime>

#include "framelesswindow.h"
#include "gamewindow.h"

// 主菜单类

namespace Ui {
class MainWindow;
}

class MainWindow : public FrameLessWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_btnGame_clicked();

  void on_btnRank_clicked();

  void on_btnConfig_clicked();

  void on_btnAbout_clicked();

  void on_btnQuit_clicked();

 signals:
  void sentDifficulty(QString data);

 private:
  Ui::MainWindow *ui;
};

// 延迟时间，为优化切换窗口体验
void delay(int x);

#endif  // MAINWINDOW_H
