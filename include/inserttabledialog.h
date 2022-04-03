#ifndef INSERTTABLEDIALOG_H
#define INSERTTABLEDIALOG_H

#include <QDialog>

namespace Ui {
class insertTableDialog;
}

class InsertTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InsertTableDialog(QWidget *parent = nullptr);

    int columnCount;

    int rowCount;

    ~InsertTableDialog();

private slots:
    void setupConnections();

    void getTableDims();

private:
    Ui::insertTableDialog *ui;
};

#endif // INSERTTABLEDIALOG_H
