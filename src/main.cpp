#include "include/mainwindow.h"
#include <QIcon>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Quiz game");
    w.show();
    return a.exec();
}
