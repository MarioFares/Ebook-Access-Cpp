#include "include/mainwindow.h"
#include "include/common.h"

#include <QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	w.setStyleSheet(common::openSheet(":/styles/style.qss"));
	w.ensurePolished();
	w.setVisible(true);
	w.showMaximized();
	return a.exec();
}
