#include "mainwindow.h"

#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::cout << u8"SharkWrite 3\n\u00a9 Sharkbyteprojects" << std::endl;
    w.show();
    return a.exec();
}
