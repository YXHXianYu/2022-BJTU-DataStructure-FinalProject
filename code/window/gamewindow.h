#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QTime>

#include "../hypercube/hypercube.h"
#include "framelesswindow.h"

// 游戏窗口类

namespace Ui {
class GameWindow;
}

class GameWindow : public FrameLessWindow {
    Q_OBJECT

   public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

   private slots:
    void on_btnReturn_clicked();

   private:
    Ui::GameWindow *ui;

    Hypercube::Hypercube *hypercube_;
};

#endif  // GAMEWINDOW_H
