#ifndef BGM_H
#define BGM_H
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QtMultimedia>
#include <qmediaplaylist.h>
#include <qmediaplayer.h>
//播放背景音乐（初始化、切换、暂停、播放）
//游戏内音效（初始化、触发）


class BGM{
public:
    //添加音频文件到播放列表里,dir是路径，比如qrc:/new/sound/sounds/di.wav,   recurrent是否循环播放
    //返回player
    QMediaPlayer* init(QString dir,bool recurrent);

    QMediaPlayer* add(QString dir,QMediaPlayer* player);//增加播放列表中的曲目或音效
    QMediaPlayer* remove(int location,QMediaPlayer* player);//按添加顺序的position移除播放列表的曲目
    void play(QMediaPlayer *player);//播放
    void pause(QMediaPlayer* player);//暂停
private:

};

#endif // BGM_H
