#ifndef HYPERCUBETHREAD_H
#define HYPERCUBETHREAD_H

#include <QThread>

class HypercubeThread : public QThread {
    Q_OBJECT
   public:
    explicit HypercubeThread(QObject *parent = nullptr);

   private:
    void run() override;

   signals:
    void timeout();
};

#endif  // HYPERCUBETHREAD_H
