#include "ui_inserttabledialog.h"

#include "include/inserttabledialog.h"

InsertTableDialog::InsertTableDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::insertTableDialog)
{
    columnCount = 0;
    rowCount = 0;
    ui->setupUi(this);
    setupConnections();
}

InsertTableDialog::~InsertTableDialog()
{
    delete ui;
}

void InsertTableDialog::setupConnections()
{
    connect(ui->buttonCancel, &QPushButton::clicked, this, &InsertTableDialog::close);
    connect(ui->buttonOk, &QPushButton::clicked, this, &InsertTableDialog::getTableDims);
}

void InsertTableDialog::getTableDims()
{
    rowCount = ui->spinBoxRows->value();
    columnCount = ui->spinBoxColumns->value();
    close();
}
