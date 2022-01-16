#ifndef INSERTLINKDIALOG_H
#define INSERTLINKDIALOG_H

#include <QDialog>

namespace Ui {
class insertLinkDialog;
}

class insertLinkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit insertLinkDialog(QWidget *parent = nullptr);
    ~insertLinkDialog();

    QString title;

    QString  link;

    void setDataOnOpen();

private slots:
    void setupConnections();

    void getInput();

private:
    Ui::insertLinkDialog *ui;
};

#endif // INSERTLINKDIALOG_H
