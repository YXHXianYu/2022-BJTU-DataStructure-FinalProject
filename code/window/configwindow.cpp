#include "configwindow.h"

#include "mainwindow.h"
#include "ui_configwindow.h"

int ConfigWindow::music_state = 1;
int ConfigWindow::sound_state = 1;

ConfigWindow::ConfigWindow(QWidget *parent)
    : FrameLessWindow(parent), ui(new Ui::ConfigWindow) {
  ui->setupUi(this);
  // 固定窗口大小
  this->setFixedSize(800, 600);
  // 去除自带的边框
  this->setWindowFlag(Qt::FramelessWindowHint);
}

ConfigWindow::~ConfigWindow() { delete ui; }

void ConfigWindow::on_btnReturn_clicked() {
  BGM::GetInstance()->PlayClose();
  MainWindow *mw = new MainWindow();
  mw->move(this->pos().x(), this->pos().y());
  mw->show();
  delay(20);
  this->close();
}

void ConfigWindow::on_btnMusic_clicked() {
  if (music_state == 1) {
    BGM::GetInstance()->stopAllMusic = true;
    BGM::GetInstance()->StopBgm1();
    BGM::GetInstance()->PlayOpen();
    ui->btnMusic->setStyleSheet(
        "QPushButton#btnMusic{border-image:url(:/images/configwindow/"
        "设置_音乐关闭.png)}"
        "QPushButton::pressed#btnMusic{border-image:url(:/images/configwindow/"
        "设置_音乐关闭-.png)}");
    music_state ^= 1;
  } else {
    BGM::GetInstance()->stopAllMusic = false;
    BGM::GetInstance()->PlayBgm1();
    BGM::GetInstance()->PlayOpen();
    ui->btnMusic->setStyleSheet(
        "QPushButton#btnMusic{border-image:url(:/images/configwindow/"
        "设置_音乐开启.png)}"
        "QPushButton::pressed#btnMusic{border-image:url(:/images/configwindow/"
        "设置_音乐开启-.png)}");
    music_state ^= 1;
  }
}

void ConfigWindow::on_btnSound_clicked() {
  if (sound_state == 1) {
    BGM::GetInstance()->PlayOpen();
    BGM::GetInstance()->stopAllSound = true;
    ui->btnSound->setStyleSheet(
        "QPushButton#btnSound{border-image:url(:/images/configwindow/"
        "设置_音效关闭.png)}"
        "QPushButton::pressed#btnSound{border-image:url(:/images/configwindow/"
        "设置_音效关闭-.png)}");
    sound_state ^= 1;
  } else {
    BGM::GetInstance()->stopAllSound = false;
    BGM::GetInstance()->PlayOpen();
    ui->btnSound->setStyleSheet(
        "QPushButton#btnSound{border-image:url(:/images/configwindow/"
        "设置_音效开启.png)}"
        "QPushButton::pressed#btnSound{border-image:url(:/images/configwindow/"
        "设置_音效开启-.png)}");
    sound_state ^= 1;
  }
}
