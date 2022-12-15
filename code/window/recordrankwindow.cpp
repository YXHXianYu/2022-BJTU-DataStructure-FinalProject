#include "recordrankwindow.h"

#include <QFont>
#include <QFontDatabase>

#include "../database/rank.h"
#include "ui_recordrankwindow.h"
RecordRankWindow::RecordRankWindow(int score, QWidget *parent)
    : QWidget(parent), ui(new Ui::RecordRankWindow), score_(score) {
    ui->setupUi(this);
    int font_Id = QFontDatabase::addApplicationFont(":/font/SmileySans-Oblique.ttf");
    QStringList font_list = QFontDatabase::applicationFontFamilies(font_Id);
    if (!font_list.isEmpty()) {
        QFont f;
        f.setFamily(font_list[0]);
        f.setPointSize(18);
        ui->label->setFont(f);
        f.setPointSize(22);
        ui->label_2->setFont(f);
    }
    setFocus();
}

RecordRankWindow::~RecordRankWindow() { delete ui; }

void RecordRankWindow::on_skip_button_clicked() { this->close(); }

void RecordRankWindow::on_record_button_clicked() {
    int last_score = Rank::GetInstance()->Query(ui->lineEdit->text().toStdString());
    if (last_score < score_) {
        Rank::GetInstance()->Insert(ui->lineEdit->text().toStdString(), score_);
    }
    this->close();
}

int RecordRankWindow::score() const { return score_; }

void RecordRankWindow::set_score(int score) {
    score_ = score;
    ui->label->setText(QString::fromStdString("获得了" + std::to_string(score) + "分！谱写下你的ID吧！"));
}
