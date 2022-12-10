#include "BGM.h"

#include <iostream>

BGM::BGM() {
    open = new QSound("../2022-BJTU-DataStructrue-FinalProject/resource/sounds/open_win.wav");
    close = new QSound("../2022-BJTU-DataStructrue-FinalProject/resource/sounds/close_win.wav");
    switch_task = new QSound("../2022-BJTU-DataStructrue-FinalProject/resource/sounds/switch_task.wav");
    switch_type = new QSound("../2022-BJTU-DataStructrue-FinalProject/resource/sounds/switch_type.wav");
    music = new QSound("../2022-BJTU-DataStructrue-FinalProject/resource/sounds/music.wav");
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
