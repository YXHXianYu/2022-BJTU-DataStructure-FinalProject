#include "BGM.h"

#include <iostream>

BGM::BGM() {
    open = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/open_win.wav"), 50);
    open1 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/open_win.wav"), 50);
    open2 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/open_win.wav"), 50);
    open3 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/open_win.wav"), 50);
    open4 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/open_win.wav"), 50);
    open5 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/open_win.wav"), 50);

    close = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/close_win.wav"), 50);

    switch_task =
        GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/switch_task.wav"), 50);

    switch_type =
        GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/switch_type.wav"), 50);

    music = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/music.wav"), 50);
    match1 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/match1.wav"), 20);

    match2 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/match2.wav"), 20);

    match3 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/match3.wav"), 20);

    match4 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/match3.wav"), 20);

    match5 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/match3.wav"), 20);

    match6 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/match3.wav"), 20);

    fall = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/fall.wav"), 50);

    bgm1 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/bgm1.wav"), 8);

    bgm2 = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/bgm2.wav"), 8);

    fall = GetMediaPlayer(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/fall.wav"), 50);

    /*
    player = new QMediaPlayer();
    player->setMedia(QUrl::fromLocalFile("../2022-BJTU-DataStructure-FinalProject/resource/sounds/switch_task.wav"));
    player->setVolume(50);*/
}

BGM *BGM::instance_ = nullptr;

BGM *BGM::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new BGM();
    }
    return instance_;
}

void BGM::PlayOpen() {
    if (!BGM::GetInstance()->stopAllSound) {
        if (open->state() != QMediaPlayer::PlayingState) {
            open->play();
        } else if (open1->state() != QMediaPlayer::PlayingState) {
            open1->play();
        } else if (open2->state() != QMediaPlayer::PlayingState) {
            open2->play();
        } else if (open3->state() != QMediaPlayer::PlayingState) {
            open3->play();
        } else if (open4->state() != QMediaPlayer::PlayingState) {
            open4->play();
        } else if (open5->state() != QMediaPlayer::PlayingState) {
            open5->play();
        }
    }
}

void BGM::StopOpen() { open->stop(); }

void BGM::PlayClose() {
    if (!BGM::GetInstance()->stopAllSound) {
        close->play();
    }
}

void BGM::StopClose() { close->stop(); }

void BGM::PlaySwitchTask() {
    if (!BGM::GetInstance()->stopAllSound) {
        switch_task->play();
    }
}
void BGM::StopSwitchTask() { switch_task->stop(); }

void BGM::PlaySwitchType() {
    if (!BGM::GetInstance()->stopAllSound) {
        switch_type->play();
    }
}
void BGM::StopSwitchType() { switch_type->stop(); }

void BGM::PlayMusic() {
    if (!BGM::GetInstance()->stopAllMusic) {
        music->play();
    }
}
void BGM::StopMusic() { music->stop(); }

void BGM::PlayMatch1() {
    if (!BGM::GetInstance()->stopAllSound) {
        match1->play();
    }
}
void BGM::PlayMatch2() {
    if (!BGM::GetInstance()->stopAllSound) {
        match2->play();
    }
}
void BGM::PlayMatch3() {
    if (!BGM::GetInstance()->stopAllSound) {
        match3->play();
    }
}
void BGM::PlayMatch4() {
    if (!BGM::GetInstance()->stopAllSound) {
        match4->play();
    }
}
void BGM::PlayMatch5() {
    if (!BGM::GetInstance()->stopAllSound) {
        match5->play();
    }
}
void BGM::PlayMatch6() {
    if (!BGM::GetInstance()->stopAllSound) {
        match6->play();
    }
}
void BGM::PlayFall() {
    if (!BGM::GetInstance()->stopAllSound) {
        fall->play();
    }
}

bool BGM::IsPlayingMatch1() { return match1->state() == QMediaPlayer::PlayingState; }
bool BGM::IsPlayingMatch2() { return match2->state() == QMediaPlayer::PlayingState; }
bool BGM::IsPlayingMatch3() { return match3->state() == QMediaPlayer::PlayingState; }
bool BGM::IsPlayingMatch4() { return match4->state() == QMediaPlayer::PlayingState; }
bool BGM::IsPlayingMatch5() { return match5->state() == QMediaPlayer::PlayingState; }
bool BGM::IsPlayingMatch6() { return match6->state() == QMediaPlayer::PlayingState; }

void BGM::PlayPlayer() {
    if (!BGM::GetInstance()->stopAllSound) {
        player->play();
    }
}

QMediaPlayer *BGM::GetMediaPlayer(const QMediaContent &content, int volume) {
    QMediaPlayer *player = new QMediaPlayer();
    player->setMedia(content);
    player->setVolume(volume);

    player->play();
    player->stop();

    return player;
}

void BGM::PlayBgm1() {
    if (!BGM::GetInstance()->stopAllMusic) {
        bgm1->play();
    }
}
void BGM::StopBgm1() { bgm1->stop(); }

void BGM::PlayBgm2() {
    if (!BGM::GetInstance()->stopAllMusic) {
        bgm2->play();
    }
}
void BGM::StopBgm2() { bgm2->stop(); }
