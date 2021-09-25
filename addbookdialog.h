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
    void on_buttonCancel_clicked();

    void on_buttonClear_clicked();

    void on_buttonBrowse_clicked();

    void on_buttonAdd_clicked();

private:
    Ui::addBookDialog *ui;
};

#endif // ADDBOOKDIALOG_H
