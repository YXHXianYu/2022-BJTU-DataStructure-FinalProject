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
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

   private:
    QPoint last;
    int difficulty_ = 1;

   private slots:
    void on_btnReturn_clicked();

    void on_skill1_button_clicked();

    void on_skill2_button_clicked();

    void on_skill3_button_clicked();

    void on_pause_button_clicked();

    void on_hint_button_clicked();

   private:
    Ui::GameWindow *ui;

    QTimer *timer_flush_score_and_left_time_bar_;
    int left_time_cnt_;

    Board *board;
    Hypercube::Hypercube *hypercube_;

    bool is_pausing_;  // 这个变量记录当前是否为暂停状态。游戏是否暂停，在项目中有三个不同变量记录它，注意数据同步问题！！！

   public:
    void InitBoard();
};

#endif  // GAMEWINDOW_H
