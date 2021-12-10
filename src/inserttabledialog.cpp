#include "ui_inserttabledialog.h"

#include "include/inserttabledialog.h"

insertTableDialog::insertTableDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::insertTableDialog)
{
    columnCount = 0;
    rowCount = 0;
    ui->setupUi(this);
}

insertTableDialog::~insertTableDialog()
{
    delete ui;
}

void insertTableDialog::on_buttonCancel_clicked()
{
    close();
}

void insertTableDialog::on_buttonOk_clicked()
{
    rowCount = ui->spinBoxRows->value();
    columnCount = ui->spinBoxColumns->value();
    close();
}
