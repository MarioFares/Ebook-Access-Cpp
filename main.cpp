#include "mainwindow.h"
#include "common.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setStyleSheet(common::openSheet(":/style.qss"));
    w.ensurePolished();
    w.show();
    return a.exec();
}
