#ifndef INSERTTABLEDIALOG_H
#define INSERTTABLEDIALOG_H

#include <QDialog>

namespace Ui {
class insertTableDialog;
}

class insertTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit insertTableDialog(QWidget *parent = nullptr);

    int columnCount;

    int rowCount;

    ~insertTableDialog();

private slots:
    void setupConnections();

    void getTableDims();

private:
    Ui::insertTableDialog *ui;
};

#endif // INSERTTABLEDIALOG_H
