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

    QString author;
    QString genre;
    QString tags;

    ~bulkDetailsDialog();

private slots:
    void setupConnections();

    void applyFields();

private:
    Ui::bulkDetailsDialog *ui;
};

#endif // BULKDETAILSDIALOG_H
