#ifndef BGM_H
#define BGM_H
#include <QMediaPlayer>
#include <QSound>
class BGM {
   private:
    static BGM *instance_;
    QMediaPlayer *open;
    QMediaPlayer *close;
    QMediaPlayer *switch_task;
    QMediaPlayer *switch_type;
    QMediaPlayer *music;
    QMediaPlayer *player;
    QMediaPlayer *match1;
    QMediaPlayer *match2;
    QMediaPlayer *match3;
    QMediaPlayer *fall;
    QMediaPlayer *bgm1;
    QMediaPlayer *bgm2;

   public:
    BGM();
    static BGM *GetInstance();
    void PlayOpen();
    void StopOpen();

    void PlayClose();
    void StopClose();

    void PlaySwitchTask();
    void StopSwitchTask();

    void PlaySwitchType();
    void StopSwitchType();

    void PlayBgm1();
    void StopBgm1();

    void PlayBgm2();
    void StopBgm2();

    void PlayMusic();
    void StopMusic();

    void PlayMatch1();
    void PlayMatch2();
    void PlayMatch3();
    void PlayFall();

    void PlayPlayer();
};

#endif  // BGM_H
