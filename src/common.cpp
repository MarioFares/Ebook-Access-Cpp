#include <include/common.h>

#include <QFile>
#include <QDialog>
#include <QAction>
#include <QProcess>
#include <QFileInfo>
#include <QCompleter>
#include <QMessageBox>
#include <QMainWindow>
#include <QErrorMessage>
#include <QAbstractItemView>
#include <QFileSystemModel>

namespace common
{
QString SEP = "|";

quint32 getPageCount(const QString &path)
{
    QFileInfo file(path);
    quint32 pages = 0;
    if (file.suffix() == "pdf")
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
    file.close();
    return styleSheet;
}

void openDialog(QDialog *dialog, const QString &stylesheetPath)
{
    dialog->setStyleSheet(common::openSheet(stylesheetPath));
    dialog->ensurePolished();
    dialog->setFixedSize(dialog->size());
    dialog->exec();
}

void showMsgBox(const QString &title, const QString &text, const QString &stylesheetPath, const QMessageBox::Icon &msgIcon, const QString &winIcon)
{
    QMessageBox box;
    box.setText(text);
    box.setWindowTitle(title);
    box.setStyleSheet(common::openSheet(stylesheetPath));
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
    msg.setStyleSheet(common::openSheet(stylesheetPath));
    msg.ensurePolished();
    msg.exec();
}

void openWindow(QMainWindow *window, const QString &stylesheetPath)
{
    window->setStyleSheet(common::openSheet(stylesheetPath));
    window->ensurePolished();
    window->show();
}

QCompleter *dirCompleter(QWidget *parent)
{
    QCompleter *dirCompleter = new QCompleter(parent);
    dirCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    dirCompleter->setCompletionMode(QCompleter::PopupCompletion);
    QAbstractItemView *popup = dirCompleter->popup();
    popup->setStyleSheet(common::openSheet(":/styles/style.qss"));
    QFileSystemModel *model = new QFileSystemModel(dirCompleter);
    model->setRootPath("/");
    model->sort(0, Qt::DescendingOrder);
    dirCompleter->setModel(model);

    return dirCompleter;
}

void changeWidgetVisibility(QWidget *widget, QAction *action)
{
    bool isHidden = widget->isHidden();
    widget->setHidden(!isHidden);

    QString actionText = action->text();
    actionText.replace(isHidden ? "Show" : "Hide", isHidden ? "Hide" : "Show", Qt::CaseInsensitive);
    action->setText(actionText);
}

void toggleFullscreen(QMainWindow *window)
{
    window->isFullScreen() ? window->showMaximized() : window->showFullScreen();
}

void toggleMaximized(QMainWindow *window)
{
    window->isMaximized() ? window->showNormal() : window->showMaximized();
}


} // Namespace common
