#include "mainwindow.h"

#include <QFile>
#include <iostream>

#include "aboutwindow.h"
#include "configwindow.h"
#include "gamewindow.h"
#include "rankwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : FrameLessWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(800, 600);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);

    msg = "easy";
    BGM::GetInstance()->PlayBgm1();
}

MainWindow::~MainWindow() { delete ui; }

void delay(int x) {
    QTime dieTime = QTime::currentTime().addMSecs(x);
    while (QTime::currentTime() < dieTime) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

// 游戏开始
void MainWindow::on_btnGame_clicked() {
    BGM::GetInstance()->StopBgm1();
    GameWindow *gw = new GameWindow();
    gw->move(this->pos().x(), this->pos().y());
    BGM::GetInstance()->PlayBgm2();

    connect(this, &MainWindow::sentDifficulty, gw, &GameWindow::getDifficulty);
    gw->show();
    std::cerr << "MainWindow::on_btnGame_clicked - GameWindow has showed." << std::endl;
    emit sentDifficulty(msg);
    delay(200);
    this->close();
}

// 排行榜
void MainWindow::on_btnRank_clicked() {
    BGM::GetInstance()->PlaySwitchType();
    RankWindow *rw = new RankWindow();
    rw->move(this->pos().x(), this->pos().y());
    rw->show();
    delay(200);
    this->close();
}

// 设置
void MainWindow::on_btnConfig_clicked() {
    BGM::GetInstance()->PlaySwitchType();
    ConfigWindow *cw = new ConfigWindow();
    cw->move(this->pos().x(), this->pos().y());
    cw->show();
    delay(200);
    this->close();
}

// 关于
void MainWindow::on_btnAbout_clicked() {
    BGM::GetInstance()->PlaySwitchType();
    AboutWindow *aw = new AboutWindow();
    aw->move(this->pos().x(), this->pos().y());
    aw->show();
    delay(200);
    this->close();
}

void MainWindow::on_btnQuit_clicked() {
    BGM::GetInstance()->PlayClose();
    this->close();
}

void MainWindow::on_rbtnEasy_clicked() {
    BGM::GetInstance()->PlayOpen();
    msg = "easy";
    ui->rbtnEasy->setIcon(QIcon(":/images/mainwindow/1-.png"));
    ui->rbtnNormal->setIcon(QIcon(":/images/mainwindow/2.png"));
    ui->rbtnHard->setIcon(QIcon(":/images/mainwindow/3.png"));
}

void MainWindow::on_rbtnNormal_clicked() {
    msg = "normal";
    ui->rbtnEasy->setIcon(QIcon(":/images/mainwindow/1.png"));
    ui->rbtnNormal->setIcon(QIcon(":/images/mainwindow/2-.png"));
    ui->rbtnHard->setIcon(QIcon(":/images/mainwindow/3.png"));
    BGM::GetInstance()->PlayOpen();
}

void MainWindow::on_rbtnHard_clicked() {
    msg = "hard";
    ui->rbtnEasy->setIcon(QIcon(":/images/mainwindow/1.png"));
    ui->rbtnNormal->setIcon(QIcon(":/images/mainwindow/2.png"));
    ui->rbtnHard->setIcon(QIcon(":/images/mainwindow/3-.png"));
    BGM::GetInstance()->PlayOpen();
}

void MainWindow::on_btnGame_pressed() { ui->btnGame->setIcon(QIcon(":/images/mainwindow/4-.png")); }

void MainWindow::on_btnGame_released() { ui->btnGame->setIcon(QIcon(":/images/mainwindow/4.png")); }

void MainWindow::on_btnRank_pressed() { ui->btnRank->setIcon(QIcon(":/images/mainwindow/5-.png")); }

void MainWindow::on_btnRank_released() { ui->btnRank->setIcon(QIcon(":/images/mainwindow/5.png")); }

void MainWindow::on_btnConfig_pressed() { ui->btnConfig->setIcon(QIcon(":/images/mainwindow/6-.png")); }

void MainWindow::on_btnConfig_released() { ui->btnConfig->setIcon(QIcon(":/images/mainwindow/6.png")); }
