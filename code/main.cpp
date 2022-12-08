#include <QApplication>
#include <ctime>
#include <iostream>
#include <random>

#include "./window/mainwindow.h"

int main(int argc, char *argv[]) {
    srand(time(0));

    QApplication a(argc, argv);
    MainWindow m;
    m.show();
    return a.exec();
}
