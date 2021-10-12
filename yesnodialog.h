#ifndef YESNODIALOG_H
#define YESNODIALOG_H

#include <QDialog>

namespace Ui {
class yesNoDialog;
}

class yesNoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit yesNoDialog(QWidget *parent = nullptr, QString windowTitle = "Select",
                         QString title = "Select", QString prompt = "Select");
    ~yesNoDialog();

    bool getResult();

private slots:
    void on_buttonYes_clicked();

    void on_buttonNo_clicked();

private:
    Ui::yesNoDialog *ui;

    bool result;
};

#endif // YESNODIALOG_H
