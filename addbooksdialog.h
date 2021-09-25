#ifndef ADDBOOKSDIALOG_H
#define ADDBOOKSDIALOG_H

#include <QDialog>

namespace Ui {
class addBooksDialog;
}

class addBooksDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addBooksDialog(QWidget *parent = nullptr);
    ~addBooksDialog();

private slots:
    void on_buttonClose_clicked();

    void on_buttonBrowseFolders_clicked();

    void on_buttonAdd_clicked();

private:
    Ui::addBooksDialog *ui;

};

#endif // ADDBOOKSDIALOG_H
