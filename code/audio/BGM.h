#ifndef BGM_H
#define BGM_H
#include <QSound>

class BGM {
   private:
    static BGM *instance_;
    QSound *open;
    QSound *close;
    QSound *switch_task;
    QSound *switch_type;
    QSound *music;

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

    void PlayMusic();
    void StopMusic();
};

#endif  // BGM_H
