#include "ui_dataviewerwindow.h"

#include "include/queries.h"
#include "include/dataviewerwindow.h"

#include <QSqlRecord>
#include <QCloseEvent>
#include <QComboBox>

#include "include/common.h"

DataViewerWindow::DataViewerWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::DataViewerWindow)
{
    ui->setupUi(this);

    ui->comboBoxTables->insertItem(0, "Ebooks");
    ui->comboBoxTables->insertItem(1, "Links");
    ui->comboBoxTables->insertItem(2, "Link Collections");
    ui->comboBoxTables->insertItem(3, "Searches");
    ui->comboBoxTables->insertItem(4, "Session Logs");
    ui->comboBoxTables->insertItem(5, "Tags");

    this->columnsResizeMode = QHeaderView::Stretch;

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    setupConnections();
    tableSelected(0);
}

DataViewerWindow::~DataViewerWindow()
{
    delete ui;
}

void DataViewerWindow::setupConnections()
{
    connect(ui->comboBoxTables, &QComboBox::currentIndexChanged, this, &DataViewerWindow::tableSelected);
    connect(ui->buttonRefresh, &QToolButton::clicked, this, [this] {tableSelected(ui->comboBoxTables->currentIndex());});
    connect(ui->buttonToggleColors, &QToolButton::clicked, this, &DataViewerWindow::toggleColors);
    connect(ui->buttonToggleGrid, &QToolButton::clicked, this, &DataViewerWindow::toggleGrid);
    connect(ui->buttonToggleFitColumns, &QToolButton::clicked, this, &DataViewerWindow::toggleFitColumns);
    connect(ui->tableWidget, &QTableWidget::currentCellChanged, this, &DataViewerWindow::showCellText);
    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested, this, &DataViewerWindow::showTableContextMenu);
}

void DataViewerWindow::showTableContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->tableWidget->mapToGlobal(pos);

    QMenu mainMenu;
    mainMenu.setStyleSheet(common::openSheet(":/styles/style.qss"));
    mainMenu.addAction("Hide Column", this, [this]
    {
        hideColumn(ui->tableWidget->currentColumn());
    });

    QMenu *showMenu = mainMenu.addMenu("Show Column");
    showMenu->setStyleSheet(common::openSheet(":/styles/style.qss"));

    QHashIterator<int, QString> i(this->hiddenColumns);
    while (i.hasNext())
    {
        i.next();
        showMenu->addAction(i.value(), this, [this, i]
        {
            showColumn(i.key());
        });
    }
    mainMenu.exec(globalPos);
}

void DataViewerWindow::showColumn(int index)
{
    this->hiddenColumns.remove(index);
    ui->tableWidget->showColumn(index);
}

void DataViewerWindow::showAllColumns()
{
    QHashIterator<int, QString> i(this->hiddenColumns);
    while (i.hasNext())
    {
        i.next();
        showColumn(i.key());
    }
}

void DataViewerWindow::hideColumn(int index)
{
    this->hiddenColumns[index] = ui->tableWidget->horizontalHeaderItem(index)->text();
    ui->tableWidget->hideColumn(index);
}

void DataViewerWindow::populateTable()
{
    qint32 fieldCount = queries::query.record().count();
    QStringList fieldNames;
    for (int i = 0; i < fieldCount; i++)
    {
        QString fieldName = queries::query.record().fieldName(i);
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
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(queries::query.value(i).toString());
            ui->tableWidget->setItem(rowCount, i, item);
        }
    }

    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->statusbar->clearMessage();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(this->columnsResizeMode);
}

void DataViewerWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void DataViewerWindow::tableSelected(int index)
{
    showAllColumns();
    switch (index)
    {
        case 0:
            queries::selectEbooksTable();
            break;

        case 1:
            queries::selectLinksTable();
            break;

        case 2:
            queries::selectLinkCollectionsTable();
            break;

        case 3:
            queries::selectSearchesTable();
            break;

        case 4:
            queries::selectSessionLogTable();
            break;

        case 5:
            queries::selectTagsTable();
            break;
    }
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    populateTable();
}

void DataViewerWindow::toggleColors()
{
    if (ui->tableWidget->alternatingRowColors())
    {
        ui->tableWidget->setAlternatingRowColors(false);
        ui->buttonToggleColors->setIcon(QIcon(":/icons/drop_icon.png"));
    }
    else
    {
        ui->tableWidget->setAlternatingRowColors(true);
        ui->buttonToggleColors->setIcon(QIcon(":/icons/crossed_drop_icon.png"));
    }
}

void DataViewerWindow::toggleGrid()
{
    bool gridOn = ui->tableWidget->showGrid();
    if (gridOn)
    {
        ui->buttonToggleGrid->setIcon(QIcon(":/icons/grid_icon.png"));
    }
    else
    {
        ui->buttonToggleGrid->setIcon(QIcon(":/icons/hide_grid_icon.png"));
    }
    ui->tableWidget->setShowGrid(!gridOn);
}

void DataViewerWindow::toggleFitColumns()
{
    if (columnsResizeMode == QHeaderView::Stretch)
    {
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        columnsResizeMode = QHeaderView::ResizeToContents;
    }
    else
    {
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        columnsResizeMode = QHeaderView::Stretch;
    }
}

void DataViewerWindow::showCellText()
{
    if (ui->tableWidget->currentItem())
    {
        QString text = ui->tableWidget->currentItem()->text();
        ui->statusbar->showMessage(text);
    }
}
