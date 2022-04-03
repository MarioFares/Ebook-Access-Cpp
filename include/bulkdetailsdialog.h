#ifndef BULKDETAILSDIALOG_H
#define BULKDETAILSDIALOG_H

#include <QDialog>

namespace Ui {
class bulkDetailsDialog;
}

class BulkDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BulkDetailsDialog(QWidget *parent = nullptr);

    QString author;
    QString genre;
    QString tags;

    ~BulkDetailsDialog();

private slots:
    void setupConnections();

    void applyFields();

private:
    Ui::bulkDetailsDialog *ui;
};

#endif // BULKDETAILSDIALOG_H
