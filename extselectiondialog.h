#ifndef EXTSELECTIONDIALOG_H
#define EXTSELECTIONDIALOG_H

#include <QDialog>
#include <QVector>
#include <QListWidgetItem>

namespace Ui {
class extSelectionDialog;
}

class extSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit extSelectionDialog(QWidget *parent = nullptr, QVector<QString> extVector = {});
    ~extSelectionDialog();

    QVector<QString> getExtVector();

private slots:
    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_buttonOk_clicked();

    void on_buttonSelectAll_clicked();

    void on_buttonDeselectAll_clicked();

private:
    Ui::extSelectionDialog *ui;

    QVector<QString> extVector;

    void setExtVector();
};

#endif // EXTSELECTIONDIALOG_H
