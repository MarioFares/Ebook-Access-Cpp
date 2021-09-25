#ifndef BULKDETAILSDIALOG_H
#define BULKDETAILSDIALOG_H

#include <QDialog>

namespace Ui {
class bulkDetailsDialog;
}

class bulkDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit bulkDetailsDialog(QWidget *parent = nullptr);

    QString author = "";
    QString genre = "";
    QString tags = "";

    ~bulkDetailsDialog();

private slots:
    void on_checkBoxAuthor_stateChanged(int arg1);

    void on_checkBoxGenre_stateChanged(int arg1);

    void on_checkBoxTags_stateChanged(int arg1);

    void on_buttonCancel_clicked();

    void on_buttonApply_clicked();

private:
    Ui::bulkDetailsDialog *ui;
};

#endif // BULKDETAILSDIALOG_H
