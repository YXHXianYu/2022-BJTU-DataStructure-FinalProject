#include "BGM.h"

#include <iostream>

BGM::BGM() {
    open = new QMediaPlayer();
    open->setMedia(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/open_win.wav"));
    open->setVolume(50);

    close = new QMediaPlayer();
    close->setMedia(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/close_win.wav"));
    close->setVolume(50);

    switch_task = new QMediaPlayer();
    switch_task->setMedia(
        QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/switch_task.wav"));
    switch_task->setVolume(50);

    switch_type = new QMediaPlayer();
    switch_type->setMedia(
        QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/switch_type.wav"));
    switch_type->setVolume(50);

    music = new QMediaPlayer();
    music->setMedia(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/music.wav"));
    music->setVolume(50);

    match1 = new QMediaPlayer();
    match1->setMedia(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/match1.wav"));
    match1->setVolume(50);

    match2 = new QMediaPlayer();
    match2->setMedia(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/match2.wav"));
    match2->setVolume(50);

    match3 = new QMediaPlayer();
    match3->setMedia(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/match3.wav"));
    match3->setVolume(50);

    fall = new QMediaPlayer();
    fall->setMedia(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/fall.wav"));
    fall->setVolume(50);

    /*
    player = new QMediaPlayer();
    player->setMedia(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/switch_task.wav"));
    player->setVolume(50);*/
}

BGM* BGM::instance_ = nullptr;

BGM* BGM::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new BGM();
    }
    return instance_;
}

void BGM::PlayOpen() { open->play(); }
void BGM::StopOpen() { open->stop(); }

void BGM::PlayClose() { close->play(); }
void BGM::StopClose() { close->stop(); }

void BGM::PlaySwitchTask() { switch_task->play(); }
void BGM::StopSwitchTask() { switch_task->stop(); }

void BGM::PlaySwitchType() { switch_type->play(); }
void BGM::StopSwitchType() { switch_type->stop(); }

void BGM::PlayMusic() { music->play(); }
void BGM::StopMusic() { music->stop(); }

void BGM::PlayMatch1() { match1->play(); }
void BGM::PlayMatch2() { match2->play(); }
void BGM::PlayMatch3() { match3->play(); }
void BGM::PlayFall() { fall->play(); }

void BGM::PlayPlayer() { player->play(); }
