#include <QFile>
#include <QDialog>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>
#include <QMainWindow>
#include <QErrorMessage>
#include <QList>

#include "common.h"

namespace common {

void openDialog(QDialog *dialog, QString stylesheetPath)
{
    QFile file(stylesheetPath);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    dialog->setStyleSheet(styleSheet);
    dialog->ensurePolished();
    dialog->setFixedSize(dialog->size());
    dialog->exec();
}

void showMsgBox(QString title, QString text, QString stylesheetPath, QMessageBox::Icon msgIcon, QString winIcon)
{
    QMessageBox box;
    box.setText(text);
    box.setWindowTitle(title);

    QFile file(stylesheetPath);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    box.setStyleSheet(styleSheet);
    box.setIcon(msgIcon);
    box.setWindowIcon(QIcon(winIcon));

    box.ensurePolished();
    box.exec();
}

void showErrorMsg(QString title, QString message, QString stylesheetPath)
{
    QErrorMessage msg;

    msg.showMessage(message);
    msg.setWindowTitle(title);

    QFile file(stylesheetPath);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    msg.setStyleSheet(styleSheet);

    msg.ensurePolished();
    msg.exec();
}

void openWindow(QMainWindow *window, QString stylesheetPath)
{
    QFile file(stylesheetPath);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    window->setStyleSheet(styleSheet);
    window->ensurePolished();
    window->show();
}

int getPageCount(QString path)
{
    QFileInfo file(path);
    int pages = 0;
    if(file.suffix() == "pdf")
    {
        QProcess process;
        process.start("./xpdf/bin64/pdfinfo.exe ", QStringList() << path);
        process.waitForFinished(-1);
        QList output = process.readAllStandardOutput().simplified().split(' ');
        pages = output[output.indexOf("Pages:") + 1].toInt();
    }
    return pages;
}

}
