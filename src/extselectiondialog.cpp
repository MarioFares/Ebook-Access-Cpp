#include "ui_extselectiondialog.h"

#include "include/extselectiondialog.h"

#include <QVector>
#include <QListWidgetItem>

extSelectionDialog::extSelectionDialog(QWidget *parent, QVector<QString> extVector, QString title, QString label):
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

extSelectionDialog::~extSelectionDialog()
{
    delete ui;
}

void extSelectionDialog::setupConnections()
{

    connect(ui->listWidget, &QListWidget::itemActivated, this, &extSelectionDialog::setItemState);
    connect(ui->textSearch, &QLineEdit::textChanged, this, &extSelectionDialog::searchText);

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

void extSelectionDialog::fillListWidget(QVector<QString> extVector)
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

QVector<QString> extSelectionDialog::getExtVector()
{
    return outputVector;
}

void extSelectionDialog::setExtVector()
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

void extSelectionDialog::setItemState(QListWidgetItem *item)
{
    item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
}

void extSelectionDialog::searchText(const QString &arg1)
{
    QList<QListWidgetItem*> items = ui->listWidget->findItems(arg1, Qt::MatchContains);
    ui->listWidget->setCurrentItem(items.count() > 0 ? items[0] : ui->listWidget->currentItem());
}


void extSelectionDialog::setItemsCheckState(Qt::CheckState state)
{
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        ui->listWidget->item(i)->setCheckState(state);
    }
}
