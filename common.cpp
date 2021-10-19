#include <QFile>
#include <QDialog>
#include <QProcess>
#include <QFileInfo>
#include <QMessageBox>
#include <QMainWindow>
#include <QErrorMessage>
#include <QCompleter>
#include <QAbstractItemView>
#include <QFileSystemModel>

namespace common {

const QString SEP = "|";

void openDialog(QDialog *dialog, const QString &stylesheetPath)
{
    QFile file(stylesheetPath);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    dialog->setStyleSheet(styleSheet);
    dialog->ensurePolished();
    dialog->setFixedSize(dialog->size());
    dialog->exec();
}

void showMsgBox(const QString &title, const QString &text, const QString &stylesheetPath,
                const QMessageBox::Icon &msgIcon, const QString &winIcon)
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

void showErrorMsg(const QString &title, const QString &message, const QString &stylesheetPath)
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

void openWindow(QMainWindow *window, const QString &stylesheetPath)
{
    QFile file(stylesheetPath);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    window->setStyleSheet(styleSheet);
    window->ensurePolished();
    window->show();
}

quint32 getPageCount(const QString &path)
{
    QFileInfo file(path);
    quint32 pages = 0;
    if(file.suffix() == "pdf")
    {
        QProcess process;
        process.start("./xpdf/bin64/pdfinfo.exe ", QStringList() << path);
        process.waitForReadyRead();
        QList output = process.readAllStandardOutput().simplified().split(' ');
        pages = output[output.indexOf("Pages:") + 1].toInt();
    }
    return pages;
}

QString openSheet(const QString &sheetUrl)
{
    QFile file(sheetUrl);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    return styleSheet;
}

QCompleter *dirCompleter(QWidget *parent)
{
    auto *dirCompleter = new QCompleter(parent);
    dirCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    dirCompleter->setCompletionMode(QCompleter::PopupCompletion);
    QAbstractItemView *popup = dirCompleter->popup();
    popup->setStyleSheet(common::openSheet(":/style.qss"));
    auto *model = new QFileSystemModel(dirCompleter);
    model->setRootPath("/");
    model->sort(0, Qt::DescendingOrder);
    dirCompleter->setModel(model);

    return dirCompleter;
}

} // Namespace common
