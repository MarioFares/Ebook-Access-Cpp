#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class addBookDialog;
}

class addBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addBookDialog(QWidget *parent = nullptr);
    ~addBookDialog();

private slots:
    void setupConnections();

    void clearInputs();

    void browseFile();

    void addFile();

private:
    Ui::addBookDialog *ui;
};

#endif // ADDBOOKDIALOG_H
