#include "ui_inserttabledialog.h"

#include "include/inserttabledialog.h"

insertTableDialog::insertTableDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::insertTableDialog)
{
    columnCount = 0;
    rowCount = 0;
    ui->setupUi(this);
    setupConnections();
}

insertTableDialog::~insertTableDialog()
{
    delete ui;
}

void insertTableDialog::setupConnections()
{
    connect(ui->buttonCancel, &QPushButton::clicked, this, &insertTableDialog::close);
    connect(ui->buttonOk, &QPushButton::clicked, this, &insertTableDialog::getTableDims);
}

void insertTableDialog::getTableDims()
{
    rowCount = ui->spinBoxRows->value();
    columnCount = ui->spinBoxColumns->value();
    close();
}
