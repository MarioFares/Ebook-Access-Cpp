#ifndef GETNAMEDIALOG_H
#define GETNAMEDIALOG_H

#include <QDialog>

namespace Ui {
class getNameDialog;
}

class GetNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GetNameDialog(QWidget *parent = nullptr,
                           QString title = "Input Name",
                           QString prompt = "Please input a name");

    ~GetNameDialog();

    QString name;

    void setDataOnOpen();

private slots:
    void setupConnections();

    void setName();

private:
    Ui::getNameDialog *ui;
};

#endif // GETNAMEDIALOG_H
