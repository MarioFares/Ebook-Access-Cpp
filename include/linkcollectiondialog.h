#ifndef LINKCOLLECTIONDIALOG_H
#define LINKCOLLECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class linkCollectionDialog;
}

class linkCollectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit linkCollectionDialog(QWidget *parent = nullptr);

    QString collectionName;

    void setDataOnOpen();

    ~linkCollectionDialog();

private slots:
    void on_buttonCancel_clicked();

    void on_buttonOk_clicked();

private:
    Ui::linkCollectionDialog *ui;
};

#endif // LINKCOLLECTIONDIALOG_H
