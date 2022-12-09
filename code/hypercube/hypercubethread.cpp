#include "hypercubethread.h"

#include <iostream>

HypercubeThread::HypercubeThread(QObject *parent) : QThread(parent) {}

HypercubeThread::~HypercubeThread() {
    wait();
    quit();
}

void HypercubeThread::run() {
    // int i = 0;
    while (true) {
        // std::cerr << "signal: " << i++ << std::endl;
        timeout();
        QThread::msleep(10);
    }
}
