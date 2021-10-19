#ifndef COMMON_H
#define COMMON_H

#include <QDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QCompleter>
#include <QLineEdit>

namespace common {

const extern QString SEP;

void openDialog(QDialog *dialog, const QString &stylesheetPath);

void showMsgBox(const QString &title, const QString &text,
                const QString &stylesheetPath, const QMessageBox::Icon &msgIcon, const QString &winIcon);

void showErrorMsg(const QString &title, const QString &message, const QString &stylesheetPath);

void openWindow(QMainWindow *window, const QString &stylesheetPath);

quint32 getPageCount(const QString &path);

QString openSheet(const QString &sheetUrl);

QCompleter *dirCompleter(QWidget *parent);

} // Namespace common
#endif // COMMON_H
