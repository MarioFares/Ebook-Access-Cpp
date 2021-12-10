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
    explicit extSelectionDialog(QWidget *parent = nullptr, QVector<QString> extVector = {},
                                QString title = "Preferences", QString label = "Select Preferences");
    ~extSelectionDialog();

    QVector<QString> getExtVector();

private slots:
    void fillListWidget(QVector<QString> extVector);

    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_buttonOk_clicked();

    void on_buttonSelectAll_clicked();

    void on_buttonDeselectAll_clicked();

    void on_textSearch_textChanged(const QString &arg1);

private:
    Ui::extSelectionDialog *ui;

    QVector<QString> outputVector;

    QVector<QString> inputVector;

    void setExtVector();
};

#endif // EXTSELECTIONDIALOG_H
