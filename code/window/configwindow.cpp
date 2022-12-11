#include "configwindow.h"

#include "mainwindow.h"
#include "ui_configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent) : FrameLessWindow(parent), ui(new Ui::ConfigWindow) {
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


void ConfigWindow::on_btn_stopMusic_clicked()
{
    BGM::GetInstance()->stopAllMusic = true;
    BGM::GetInstance()->StopBgm1();
    BGM::GetInstance()->PlayOpen();
}

void ConfigWindow::on_btn_playMusic_clicked()
{
    BGM::GetInstance()->stopAllMusic = false;
    BGM::GetInstance()->PlayBgm1();
    BGM::GetInstance()->PlayOpen();
}

void ConfigWindow::on_btn_stopSound_clicked()
{
    BGM::GetInstance()->PlayOpen();
    BGM::GetInstance()->stopAllSound = true;
}

void ConfigWindow::on_btn_openSound_clicked()
{
    BGM::GetInstance()->stopAllSound = false;
    BGM::GetInstance()->PlayOpen();
}
