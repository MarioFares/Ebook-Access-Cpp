#include "include/windows/mainwindow.h"
#include "include/common.h"

#include <QApplication>

#include "queries.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	queries::setupDatabase();
	MainWindow w;

	w.setStyleSheet(common::openSheet(":/styles/style.qss"));
	w.ensurePolished();
	w.setVisible(true);
	w.showMaximized();
	return a.exec();
}
