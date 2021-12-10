#ifndef SEARCHNAMEDIALOG_H
#define SEARCHNAMEDIALOG_H

#include <QDialog>

namespace Ui {
class searchNameDialog;
}

class searchNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit searchNameDialog(QWidget *parent = nullptr);
    QString searchName;
    ~searchNameDialog();

private slots:
    void on_buttonCancel_clicked();

    void on_buttonOk_clicked();

private:
    Ui::searchNameDialog *ui;
};

#endif // SEARCHNAMEDIALOG_H
