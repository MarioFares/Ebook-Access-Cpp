#ifndef COMMON_H
#define COMMON_H

#include <QDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QCompleter>
#include <QLineEdit>

namespace common {

extern QString SEP;

void openDialog(QDialog *dialog, QString stylesheetPath);

void showMsgBox(QString title, QString text, QString stylesheetPath, QMessageBox::Icon msgIcon, QString winIcon);

void showErrorMsg(QString title, QString message, QString stylesheetPath);

void openWindow(QMainWindow *window, QString stylesheetPath);

quint32 getPageCount(QString path);

QString openSheet(QString sheetUrl);

QCompleter *dirCompleter(QWidget *parent);

}
#endif // COMMON_H
