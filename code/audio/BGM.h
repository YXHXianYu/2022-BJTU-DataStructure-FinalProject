#ifndef BGM_H
#define BGM_H
#include <QMediaPlayer>
#include <QSound>
class BGM {
   private:
    static BGM *instance_;
    QMediaPlayer *open;
    QMediaPlayer *open1;
    QMediaPlayer *open2;
    QMediaPlayer *open3;
    QMediaPlayer *open4;
    QMediaPlayer *open5;
    QMediaPlayer *close;
    QMediaPlayer *switch_task;
    QMediaPlayer *switch_type;
    QMediaPlayer *music;
    QMediaPlayer *player;
    QMediaPlayer *match1;
    QMediaPlayer *match2;
    QMediaPlayer *match3;
    QMediaPlayer *match4;
    QMediaPlayer *match5;
    QMediaPlayer *match6;
    QMediaPlayer *fall;
    QMediaPlayer *bgm1;
    QMediaPlayer *bgm2;

   public:
    BGM();
    bool stopAllMusic = false;
    bool stopAllSound = false;
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
    bool IsPlayingMatch1();
    void PlayMatch2();
    bool IsPlayingMatch2();
    void PlayMatch3();
    bool IsPlayingMatch3();
    void PlayMatch4();
    bool IsPlayingMatch4();
    void PlayMatch5();
    bool IsPlayingMatch5();
    void PlayMatch6();
    bool IsPlayingMatch6();
    void PlayFall();

    void PlayPlayer();

   private:
    QMediaPlayer *GetMediaPlayer(const QMediaContent &content, int volume);
};

#endif  // BGM_H
