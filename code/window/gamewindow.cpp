#include "gamewindow.h"

#include <QAction>
#include <QBitmap>
#include <QFont>
#include <QFontDatabase>
#include <ctime>
#include <iostream>
#include <random>

#include "mainwindow.h"
#include "ui_gamewindow.h"

const QPoint hypercube_size(550, 550);
const QPoint opengl_up_left(25, 25);
const QPoint opengl_down_right = opengl_up_left + QPoint(hypercube_size.x(), hypercube_size.y());
const int TITLE_HEIGHT = 30;

GameWindow::GameWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(800, 600);
    // 去除自带的边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    // close时析构成员变量
    // setAttribute(Qt::WA_DeleteOnClose);
    /*
    QPixmap pix;
    pix.load(":/images/gamewindow/1.png");
    ui->skill1_button->setFixedSize(pix.size());
    ui->skill1_button->setMask(pix.mask());
    */
    ui->skill1_button->setStyleSheet("background-color:rgba(0,0,0,0)");

    // 创建Hypercube窗口
    hypercube_ = new Hypercube::Hypercube(ui->centralwidget);
    hypercube_->setFixedSize(hypercube_size.x(), hypercube_size.y());
    hypercube_->setGeometry(opengl_up_left.x(), opengl_up_left.y(), hypercube_->width(), hypercube_->height());

    // 初始化进度条
    is_pausing_ = false;
    // 创建timer
    timer_flush_score_and_left_time_bar_ = new QTimer(this);
    left_time_cnt_ = 12100;  // 1200* 0.1s
    connect(timer_flush_score_and_left_time_bar_, &QTimer::timeout, [&]() {
        // score_bar
        ui->score_bar->setText(QString::fromStdString(std::to_string(board->GetScore())));
        // left_time_bar
        if (is_pausing_ == false) left_time_cnt_ -= 10;
        RefreshTimeLabel();
        // set button state
        ui->skill1_button->setEnabled(board->GetRest1() > 0);
        ui->skill2_button->setEnabled(board->GetRest2() > 0);
        ui->skill3_button->setEnabled(board->GetRest3() > 0);
        // stop
        if (left_time_cnt_ <= 0 || board->IsGameOver()) {
            timer_flush_score_and_left_time_bar_->stop();
            left_time_cnt_ = -10;
            record_rank_window->set_score(board->GetScore());
            on_btnReturn_clicked();  // 时间到，直接退出游戏（
        }
    });
    timer_flush_score_and_left_time_bar_->start(100);

    // Record Rank Window
    record_rank_window = new RecordRankWindow(-1);

    int font_Id = QFontDatabase::addApplicationFont(":/font/SmileySans-Oblique.ttf");
    QStringList font_list = QFontDatabase::applicationFontFamilies(font_Id);
    qDebug() << font_Id;
    qDebug() << font_list;
    if (!font_list.isEmpty()) {
        QFont f;
        f.setFamily(font_list[0]);
        f.setPointSize(17);
        ui->score_bar->setFont(f);
        f.setPointSize(20);
        ui->minute_label->setFont(f);
        ui->minute_label->setStyleSheet("color:white;");
        ui->second_label->setFont(f);
        ui->second_label->setStyleSheet("color:white;");
    }
}

GameWindow::~GameWindow() {
    // timer
    timer_flush_score_and_left_time_bar_->stop();
    // ui
    delete ui;
    // board
    delete board;
}

void GameWindow::RefreshTimeLabel() {
    int seconds = left_time_cnt_ / 100;
    int minutes = seconds / 60;
    seconds %= 60;
    ui->minute_label->setText(QString::number(minutes));
    QString s = "";
    if (seconds < 10) s += "0";
    s += QString::number(seconds);
    ui->second_label->setText(s);
}

void GameWindow::InitBoard() {
    // board
    board = new Board(difficulty_);
    board->SetHypercube(hypercube_);
    board->InitHypercube();
    // timer etc
    left_time_cnt_ = 12100;
    // connect
    connect(board, SIGNAL(Release1()), this, SLOT(Release1()));
    connect(board, SIGNAL(Release2()), this, SLOT(Release2()));
    connect(board, SIGNAL(Release3()), this, SLOT(Release3()));
    std::cerr << "GameWindow::GameWindow InitHypercube." << std::endl;
}

void GameWindow::Release1() { ui->skill1_button->setIcon(QIcon(":/images/gamewindow/1.png")); }
void GameWindow::Release2() { ui->skill2_button->setIcon(QIcon(":/images/gamewindow/2.png")); }
void GameWindow::Release3() { ui->skill3_button->setIcon(QIcon(":/images/gamewindow/3.png")); }

void GameWindow::mousePressEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
    std::cout << "mouse cliked on:" << x << " " << y << std::endl;
    if (event->y() < TITLE_HEIGHT) {
        last = event->globalPos();
    }
    board->Clicked(x, y);
}
/*
 * 鼠标移动函数
 * 这里实时修改窗口的坐标
 */
void GameWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->y() < TITLE_HEIGHT) {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        this->move(this->x() + dx, this->y() + dy);
    }
}
/*
 * 鼠标释放函数
 */
void GameWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->y() < TITLE_HEIGHT) {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        this->move(this->x() + dx, this->y() + dy);
    }
}

/*
 * 键盘监听函数
 */
void GameWindow::keyPressEvent(QKeyEvent *e) {
    static bool shader_blinn_phong = true;
    static int shader_render_mode = 0;
    static int shader_light_source = 0;
    static float shader_hdr_exposure = 0.0;

    if (e->key() == Qt::Key_B) {  // B键，是否启用BlinnPhong光照模型
        shader_blinn_phong ^= 1;
        std::cout << "Blinn Phong: " << (shader_blinn_phong ? "true" : "false") << std::endl;
        hypercube_->SetBlinnPhong(shader_blinn_phong);
    }
    if (e->key() == Qt::Key_E || e->key() == Qt::Key_R) {
        if (e->key() == Qt::Key_E)
            shader_render_mode = (shader_render_mode + 4) % 5;  // E键，切换渲染模式，前翻
        else
            shader_render_mode = (shader_render_mode + 1) % 5;  // R键，切换渲染模式，后翻

        std::cout << "Render Mode: " << shader_render_mode << std::endl;
        hypercube_->SetRenderMode(shader_render_mode);
    }
    if (e->key() == Qt::Key_L) {  // L键，切换渲染光源
        shader_light_source = (shader_light_source + 1) % 3;
        std::cout << "Light Source: " << shader_light_source << std::endl;
        hypercube_->SetLightSource(shader_light_source);
    }
    if (e->key() == Qt::Key_G || e->key() == Qt::Key_H) {  // H键，切换HDR曝光等级
        shader_hdr_exposure += (e->key() == Qt::Key_H ? 1.f : -1.f) * 0.1f;
        std::cout << "HDR Exposure: " << shader_hdr_exposure << std::endl;
        hypercube_->SetHDRExposure(shader_hdr_exposure);
    }
    if (e->key() == Qt::Key_Shift) {
        on_pause_button_clicked();
    }
    if (e->key() == Qt::Key_A) {
        on_skill1_button_clicked();
    }
    if (e->key() == Qt::Key_S) {
        on_skill2_button_clicked();
    }
    if (e->key() == Qt::Key_D) {
        on_skill3_button_clicked();
    }
}

void GameWindow::getDifficulty(QString data) {
    if (data == "easy") difficulty_ = 1;
    if (data == "normal") difficulty_ = 2;
    if (data == "hard") difficulty_ = 3;
    std::cout << "difficulty is " << difficulty_ << std::endl;
    InitBoard();
    // QMessageBox mes(this);
    // mes.setText(data);
    // mes.exec();
}

void GameWindow::on_btnReturn_clicked() {
    MainWindow *mw = new MainWindow();
    mw->move(this->pos().x(), this->pos().y());
    mw->show();

    record_rank_window->set_score(board->GetScore());
    if (record_rank_window->score() > 0) {  // 分数>0，才显示排行榜窗口
        record_rank_window->show();
        record_rank_window->setFocus();
    }

    BGM::GetInstance()->PlayClose();
    BGM::GetInstance()->StopBgm2();
    BGM::GetInstance()->PlayBgm1();
    delay(20);
    timer_flush_score_and_left_time_bar_->stop();
    this->close();
}

void GameWindow::on_skill1_button_clicked() {
    BGM::GetInstance()->PlayOpen();
    board->ClickedOnDiamond();  // 道具1
    if (board->GetMouseOnDiamond())
        ui->skill1_button->setIcon(QIcon(":/images/gamewindow/1-.png"));
    else
        ui->skill1_button->setIcon(QIcon(":/images/gamewindow/1.png"));
}

void GameWindow::on_skill2_button_clicked() {
    BGM::GetInstance()->PlayOpen();

    board->ClickedOnLightning();  // 道具2
    if (board->GetMouseOnLightning())
        ui->skill2_button->setIcon(QIcon(":/images/gamewindow/2-.png"));
    else
        ui->skill2_button->setIcon(QIcon(":/images/gamewindow/2.png"));
}

void GameWindow::on_skill3_button_clicked() {
    BGM::GetInstance()->PlayOpen();

    board->ClickedOnShuffle();  // 道具3
    if (board->GetMouseOnShuffle())
        ui->skill3_button->setIcon(QIcon(":/images/gamewindow/3-.png"));
    else
        ui->skill3_button->setIcon(QIcon(":/images/gamewindow/3.png"));
}

void GameWindow::on_pause_button_clicked() {
    BGM::GetInstance()->PlayOpen();

    board->ClickedOnStop();  // 暂停
    is_pausing_ ^= 1;
    if (is_pausing_) {
        ui->pause_button->setIcon(QIcon(":/images/gamewindow/4-.png"));
        std::cerr << "Clicked!\n";
    } else {
        ui->pause_button->setIcon(QIcon(":/images/gamewindow/4.png"));
    }
}

void GameWindow::on_hint_button_clicked() {
    BGM::GetInstance()->PlayOpen();
    board->ShowHint(true);  // 提示
}
