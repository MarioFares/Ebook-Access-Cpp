#ifndef COMMON_H
#define COMMON_H

#include <QDialog>
#include <QMessageBox>
#include <QMainWindow>

namespace common {

extern QString SEP;

void openDialog(QDialog *dialog, QString stylesheetPath);

void showMsgBox(QString title, QString text, QString stylesheetPath, QMessageBox::Icon msgIcon, QString winIcon);

void showErrorMsg(QString title, QString message, QString stylesheetPath);

void openWindow(QMainWindow *window, QString stylesheetPath);

int getPageCount(QString path);

QString openSheet(QString sheetUrl);

}
#endif // COMMON_H
