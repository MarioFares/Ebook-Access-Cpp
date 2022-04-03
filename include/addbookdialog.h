#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class addBookDialog;
}

class AddBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddBookDialog(QWidget *parent = nullptr);
    ~AddBookDialog();

private slots:
    void setupConnections();

    void clearInputs();

    void browseFile();

    void addFile();

private:
    Ui::addBookDialog *ui;
};

#endif // ADDBOOKDIALOG_H
