#ifndef YESNODIALOG_H
#define YESNODIALOG_H

#include <QDialog>

namespace Ui {
class yesNoDialog;
}

class YesNoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit YesNoDialog(QWidget *parent = nullptr, QString windowTitle = "Select",
                         QString title = "Select", QString prompt = "Select");
    ~YesNoDialog();

    bool getResult();

private slots:
    void setupConnections();

    void setResult();

private:
    Ui::yesNoDialog *ui;

    bool result;
};

#endif // YESNODIALOG_H
