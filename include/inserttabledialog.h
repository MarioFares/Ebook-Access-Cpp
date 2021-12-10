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
    int columnCount = 0;
    int rowCount = 0;
    ~insertTableDialog();

private slots:
    void on_buttonCancel_clicked();

    void on_buttonOk_clicked();

private:
    Ui::insertTableDialog *ui;
};

#endif // INSERTTABLEDIALOG_H
