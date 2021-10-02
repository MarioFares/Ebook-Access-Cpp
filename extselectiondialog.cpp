#include "extselectiondialog.h"
#include "ui_extselectiondialog.h"

#include <QVector>
#include <QListWidgetItem>

extSelectionDialog::extSelectionDialog(QWidget *parent, QVector<QString> extVector, QString title, QString label) :
    QDialog(parent),
    ui(new Ui::extSelectionDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(title);
    ui->labelTitle->setText(label);

    ui->listWidget->setSortingEnabled(true);
    for (QString &entry : extVector)
    {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setCheckState(Qt::Checked);
        item->setText(entry);
        ui->listWidget->addItem(item);
    }
}

QVector<QString> extSelectionDialog::getExtVector()
{
    return extVector;
}

void extSelectionDialog::setExtVector()
{
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        if(item->checkState() == Qt::Checked)
            extVector.push_back(item->text());
    }
}

extSelectionDialog::~extSelectionDialog()
{
    delete ui;
}

void extSelectionDialog::on_listWidget_itemActivated(QListWidgetItem *item)
{
    item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
}

void extSelectionDialog::on_buttonOk_clicked()
{
    setExtVector();
    close();
}

void extSelectionDialog::on_buttonSelectAll_clicked()
{
    for (int i = 0; i < ui->listWidget->count(); i++)
        ui->listWidget->item(i)->setCheckState(Qt::Checked);
}


void extSelectionDialog::on_buttonDeselectAll_clicked()
{
    for (int i = 0; i < ui->listWidget->count(); i++)
        ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
}

