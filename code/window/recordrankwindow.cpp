#include "recordrankwindow.h"

#include "../database/rank.h"
#include "ui_recordrankwindow.h"

RecordRankWindow::RecordRankWindow(int score, QWidget *parent)
    : QWidget(parent), ui(new Ui::RecordRankWindow), score_(score) {
  ui->setupUi(this);
  setFocus();
}

RecordRankWindow::~RecordRankWindow() { delete ui; }

void RecordRankWindow::on_skip_button_clicked() { this->close(); }

void RecordRankWindow::on_record_button_clicked() {
  Rank::GetInstance()->Insert(ui->lineEdit->text().toStdString(), score_);
  this->close();
}

int RecordRankWindow::score() const { return score_; }

void RecordRankWindow::set_score(int score) {
  score_ = score;
  ui->label->setText(QString::fromStdString("获得了" + std::to_string(score) +
                                            "分！谱写下你的ID吧！"));
}
