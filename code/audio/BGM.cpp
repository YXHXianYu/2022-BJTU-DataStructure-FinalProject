#include "BGM.h"
QMediaPlayer* BGM::init(QString dir,bool recurrent){
    QMediaPlaylist *playList = new QMediaPlaylist();  //添加音乐列表
    playList->addMedia(QUrl(dir));

    QMediaPlayer *player = new QMediaPlayer();  //创建音乐播放器
    player->setPlaylist(playList);  //设置音乐列表

    //player->play();

    if (recurrent){
        playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);  //循环播放
    }

    return player;
}


QMediaPlayer* add(QString dir,QMediaPlayer* player){
    QMediaPlaylist* templist = player->playlist();
    templist->addMedia(QUrl(dir));
    player->setMedia(templist);
    return player;
}

QMediaPlayer* remove(int position,QMediaPlayer* player){
    QMediaPlaylist* templist = player->playlist();
    templist->removeMedia(position);
    player->setMedia(templist);
    return player;
}

void BGM::play(QMediaPlayer *player){
    //播放某特定音效或音乐
    player->play();
}

void BGM::pause(QMediaPlayer *player){
    //暂停
    player->pause();
}

