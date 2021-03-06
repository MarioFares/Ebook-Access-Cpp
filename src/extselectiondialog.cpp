#include "ui_extselectiondialog.h"

#include "include/extselectiondialog.h"

#include <QVector>
#include <QListWidgetItem>

ExtSelectionDialog::ExtSelectionDialog(QWidget *parent, QVector<QString> extVector, QString title, QString label):
    QDialog(parent), ui(new Ui::extSelectionDialog)
{
    ui->setupUi(this);
    setupConnections();
    inputVector = extVector;
    setWindowTitle(title);
    ui->labelTitle->setText(label);
    ui->listWidget->setSortingEnabled(true);
    fillListWidget(extVector);
}

ExtSelectionDialog::~ExtSelectionDialog()
{
    delete ui;
}

void ExtSelectionDialog::setupConnections()
{

    connect(ui->listWidget, &QListWidget::itemActivated, this, &ExtSelectionDialog::setItemState);
    connect(ui->textSearch, &QLineEdit::textChanged, this, &ExtSelectionDialog::searchText);

    connect(ui->buttonDeselectAll, &QPushButton::clicked, [this]
    {
        setItemsCheckState(Qt::Unchecked);
    });
    connect(ui->buttonSelectAll, &QPushButton::clicked, [this]
    {
        setItemsCheckState(Qt::Checked);
    });
    connect(ui->buttonContinue, &QPushButton::clicked, [this]
    {
        setExtVector();
        close();
    });
}

void ExtSelectionDialog::fillListWidget(QVector<QString> extVector)
{
    ui->listWidget->clear();
    for (QString &entry: extVector)
    {
        if (entry.isEmpty())
        {
            continue;
        }
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setCheckState(Qt::Checked);
        item->setText(entry);
        ui->listWidget->addItem(item);
    }
}

QVector<QString> ExtSelectionDialog::getExtVector()
{
    return outputVector;
}

void ExtSelectionDialog::setExtVector()
{
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        if (item->checkState() == Qt::Checked)
        {
            outputVector.push_back(item->text());
        }
    }
}

void ExtSelectionDialog::setItemState(QListWidgetItem *item)
{
    item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
}

void ExtSelectionDialog::searchText(const QString &arg1)
{
    QList<QListWidgetItem*> items = ui->listWidget->findItems(arg1, Qt::MatchContains);
    ui->listWidget->setCurrentItem(items.count() > 0 ? items[0] : ui->listWidget->currentItem());
}


void ExtSelectionDialog::setItemsCheckState(Qt::CheckState state)
{
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        ui->listWidget->item(i)->setCheckState(state);
    }
}
