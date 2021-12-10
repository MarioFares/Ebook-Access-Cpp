#include "ui_dataviewerwindow.h"

#include "include/queries.h"
#include "include/dataviewerwindow.h"

#include <QSqlRecord>
#include <QCloseEvent>
#include <QComboBox>

DataViewerWindow::DataViewerWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::DataViewerWindow)
{
    ui->setupUi(this);

    QStringList tables = queries::db.tables();
    ui->comboBoxTables->insertItems(0, tables);

    populateTable(ui->comboBoxTables->currentText());

}

DataViewerWindow::~DataViewerWindow()
{
    delete ui;
}

void DataViewerWindow::populateTable(const QString &tableName)
{
    queries::selectAllTable(tableName);

    qint32 fieldCount = queries::query.record().count();
    QStringList fieldNames;
    for (int i = 0; i < fieldCount; i++)
    {
        QString fieldName = queries::query.record().fieldName(i);
        fieldName = fieldName.at(0).toUpper() + fieldName.mid(1).toLower();
        fieldNames.append(fieldName);
    }

    ui->tableWidget->setColumnCount(fieldCount);
    ui->tableWidget->setHorizontalHeaderLabels(fieldNames);
    while (queries::query.next())
    {
        qint32 rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount);
        ui->tableWidget->insertRow(rowCount);
        for (int i = 0; i < fieldCount; i++)
        {
            auto *item = new QTableWidgetItem();
            item->setText(queries::query.value(i).toString());
            ui->tableWidget->setItem(rowCount, i, item);
        }
    }

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void DataViewerWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    delete this;
}

void DataViewerWindow::on_comboBoxTables_currentTextChanged(const QString &arg1)
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    populateTable(arg1);
}
