#ifndef BGM_H
#define BGM_H
#include <QSound>

class BGM{
public:
    QSound *open=new QSound("../2022-BJTU-DataStructrue-FinalProject/resource/sounds/open_win.wav");
    QSound *close=new QSound("../2022-BJTU-DataStructrue-FinalProject/resource/sounds/close_win.wav");
    QSound *switch_task=new QSound("../2022-BJTU-DataStructrue-FinalProject/resource/sounds/switch_task.wav");
    QSound *switch_type=new QSound("../2022-BJTU-DataStructrue-FinalProject/resource/sounds/switch_type.wav");
    QSound *music=new QSound("../2022-BJTU-DataStructrue-FinalProject/resource/sounds/music.wav");
private:

};

#endif // BGM_H
