#ifndef RECORDRANKWINDOW_H
#define RECORDRANKWINDOW_H

#include <QWidget>

namespace Ui {
class RecordRankWindow;
}

class RecordRankWindow : public QWidget {
    Q_OBJECT

   public:
    explicit RecordRankWindow(int score, QWidget *parent = nullptr);
    ~RecordRankWindow();

    int score() const;
    void set_score(int score);

   private slots:
    void on_skip_button_clicked();

    void on_record_button_clicked();

   private:
    Ui::RecordRankWindow *ui;

    int score_;
};

#endif  // RECORDRANKWINDOW_H
