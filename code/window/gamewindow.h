#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QTime>

#include "../core/board.h"
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
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    ~GameWindow();

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
