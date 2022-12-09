#include "hypercubethread.h"

#include <iostream>

HypercubeThread::HypercubeThread(QObject *parent) : QThread(parent) {}

void HypercubeThread::run() {
    while (true) {
        // std::cerr << "signal" << std::endl;
        timeout();
        QThread::msleep(10);
    }
}
