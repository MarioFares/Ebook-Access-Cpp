#ifndef GETNAMEDIALOG_H
#define GETNAMEDIALOG_H

#include <QDialog>

namespace Ui {
class getNameDialog;
}

class getNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit getNameDialog(QWidget *parent = nullptr,
                           QString title = "Input Name",
                           QString prompt = "Please input a name");

    ~getNameDialog();

    QString name;

    void setDataOnOpen();

private slots:
    void setupConnections();

    void setName();

private:
    Ui::getNameDialog *ui;
};

#endif // GETNAMEDIALOG_H
