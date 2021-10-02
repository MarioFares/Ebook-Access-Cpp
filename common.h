#ifndef COMMON_H
#define COMMON_H

#include <QDialog>
#include <QMessageBox>
#include <QMainWindow>

namespace common {

void openDialog(QDialog *dialog, QString stylesheetPath);

void showMsgBox(QString title, QString text, QString stylesheetPath, QMessageBox::Icon msgIcon, QString winIcon);

void showErrorMsg(QString title, QString message, QString stylesheetPath);

void openWindow(QMainWindow *window, QString stylesheetPath);

int getPageCount(QString path);

}
#endif // COMMON_H
