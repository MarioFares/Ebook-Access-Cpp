#ifndef CLEANEBOOKSDIALOG_H
#define CLEANEBOOKSDIALOG_H

#include <QDialog>

namespace Ui {
class cleanEbooksDialog;
}

class cleanEbooksDialog : public QDialog
{
    Q_OBJECT

public:
    explicit cleanEbooksDialog(QWidget *parent = nullptr);
    ~cleanEbooksDialog();

private slots:
    void on_buttonBegin_clicked();

private:
    Ui::cleanEbooksDialog *ui;
};

#endif // CLEANEBOOKSDIALOG_H
