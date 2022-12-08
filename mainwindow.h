#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_btnGame_clicked();

  void on_btnRank_clicked();

  void on_btnConfig_clicked();

  void on_btnAbout_clicked();

 private:
  Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
