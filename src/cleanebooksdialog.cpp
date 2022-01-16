#include "ui_cleanebooksdialog.h"

#include "include/cleanebooksdialog.h"
#include "include/queries.h"

#include <QFileInfo>

cleanEbooksDialog::cleanEbooksDialog(QWidget *parent):
    QDialog(parent), ui(new Ui::cleanEbooksDialog)
{
    ui->setupUi(this);
}

cleanEbooksDialog::~cleanEbooksDialog()
{
    delete ui;
}

void cleanEbooksDialog::setupConnections()
{
    connect(ui->buttonBegin, &QPushButton::clicked, this, &cleanEbooksDialog::beginClean);
}

void cleanEbooksDialog::beginClean()
{
    queries::selectPath();
    while (queries::query.next())
    {
        QString path = queries::query.value(0).toString();
        if (!QFileInfo::exists(path))
        {
            ui->listWidget->addItem(path);
        }
    }
}
