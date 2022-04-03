#ifndef INSERTLINKDIALOG_H
#define INSERTLINKDIALOG_H

#include <QDialog>

namespace Ui {
class insertLinkDialog;
}

class InsertLinkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertLinkDialog(QWidget *parent = nullptr);
    ~InsertLinkDialog();

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
