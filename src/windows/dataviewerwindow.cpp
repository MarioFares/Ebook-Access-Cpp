#include "include/common.h"
#include "include/queries.h"
#include "include/windows/dataviewerwindow.h"
#include "include/widgets/colorpickerwidget.h"

#include <QMenu>
#include <QComboBox>
#include <QStatusBar>
#include <QBoxLayout>
#include <QSqlRecord>
#include <QCloseEvent>
#include <QSpacerItem>

DataViewerWindow::DataViewerWindow(QWidget *parent) : QMainWindow(parent) {
    _columnsResizeMode = QHeaderView::Stretch;
    setupInterface();
    setupConnections();
    setupTabOrder();
}

void DataViewerWindow::setupInterface() {
    // Window
    setWindowTitle("Data Viewer");
    resize(800, 600);

    // Widgets
    _centralWidget = new QWidget(this);

    _vertLayMain = new QVBoxLayout(_centralWidget);
    _frameSearch = new QFrame(_centralWidget);
    _frameSearch->setStyleSheet(QString("QFrame {border: none;}"));
    _frameSearch->setFrameShape(QFrame::StyledPanel);
    _frameSearch->setFrameShadow(QFrame::Raised);

    // ComboBox Layout
    _horLayComboBox = new QHBoxLayout(_frameSearch);
    _horSpacerComboBoxLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horSpacerComboBoxRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    _comboBoxTables = new QComboBox(_frameSearch);
    _comboBoxTables->setMinimumSize(QSize(0, 25));
    _comboBoxTables->setStyleSheet(QString("QFrame { border: 1px solid grey; } "));

    _horLayComboBox->addItem(_horSpacerComboBoxLeft);
    _horLayComboBox->addWidget(_comboBoxTables);
    _horLayComboBox->addItem(_horSpacerComboBoxRight);

    // Tool Buttons Layout
    _horLayButtons = new QHBoxLayout();
    _horSpacerButtonsLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horSpacerButtonsRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    _buttonRefresh = new QToolButton(_centralWidget);
    _buttonRefresh->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconRefresh(":/icons/refresh_icon.png");
    _buttonRefresh->setIcon(iconRefresh);
    _buttonRefresh->setAutoRaise(false);

    _buttonToggleColors = new QToolButton(_centralWidget);
    _buttonToggleColors->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconToggleColors(":/icons/drop_icon.png");
    _buttonToggleColors->setIcon(iconToggleColors);
    _buttonToggleColors->setAutoRaise(false);

    _buttonToggleGrid = new QToolButton(_centralWidget);
    _buttonToggleGrid->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconToggleGrid(":/icons/hide_grid_icon.png");
    _buttonToggleGrid->setIcon(iconToggleGrid);
    _buttonToggleGrid->setIconSize(QSize(22, 22));
    _buttonToggleGrid->setPopupMode(QToolButton::InstantPopup);
    _buttonToggleGrid->setAutoRaise(false);
    _buttonToggleGrid->setArrowType(Qt::NoArrow);

    _buttonToggleFitColumns = new QToolButton(_centralWidget);
    _buttonToggleFitColumns->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconToggleFitColumns(":/icons/expand_icon.png");
    _buttonToggleFitColumns->setIcon(iconToggleFitColumns);
    _buttonToggleFitColumns->setIconSize(QSize(22, 22));
    _buttonToggleFitColumns->setAutoRaise(false);

    _buttonFontColor = new QToolButton(_centralWidget);
    _buttonFontColor->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconFontColor(":/icons/font_color_icon.png");
    _buttonFontColor->setIcon(iconFontColor);

    _buttonBackColor = new QToolButton(_centralWidget);
    _buttonBackColor->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconBackColor(":/icons/background_fill_white.png");
    _buttonBackColor->setIcon(iconBackColor);
    _buttonBackColor->setPopupMode(QToolButton::InstantPopup);
    _buttonBackColor->setAutoRaise(false);
    _buttonBackColor->setArrowType(Qt::NoArrow);

    _buttonGridStyle = new QToolButton(_centralWidget);
    _buttonGridStyle->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconGridStyle(":/icons/grid_icon.png");
    _buttonGridStyle->setIcon(iconGridStyle);
    _buttonGridStyle->setIconSize(QSize(22, 22));
    _buttonGridStyle->setPopupMode(QToolButton::InstantPopup);

    _horLayButtons->addSpacerItem(_horSpacerButtonsLeft);
    _horLayButtons->addWidget(_buttonRefresh);
    _horLayButtons->addWidget(_buttonToggleColors);
    _horLayButtons->addWidget(_buttonToggleGrid);
    _horLayButtons->addWidget(_buttonToggleFitColumns);
    _horLayButtons->addWidget(_buttonFontColor);
    _horLayButtons->addWidget(_buttonBackColor);
    _horLayButtons->addWidget(_buttonGridStyle);
    _horLayButtons->addSpacerItem(_horSpacerButtonsRight);

    _tableWidget = new QTableWidget(_centralWidget);
    _tableWidget->setShowGrid(true);
    _tableWidget->setGridStyle(Qt::SolidLine);
    _tableWidget->setSortingEnabled(false);
    _tableWidget->setColumnCount(0);
    _tableWidget->horizontalHeader()->setVisible(true);
    _tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
    _tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
    _tableWidget->horizontalHeader()->setStretchLastSection(false);
    _tableWidget->verticalHeader()->setVisible(false);
    _tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    _vertLayMain->addWidget(_frameSearch);
    _vertLayMain->addLayout(_horLayButtons);
    _vertLayMain->addWidget(_tableWidget);

    // Status Bar
    _statusbar = new QStatusBar(this);
    setStatusBar(_statusbar);

    // Central widget
    setCentralWidget(_centralWidget);

    // Load Data
    _comboBoxTables->insertItem(0, "Ebooks");
    _comboBoxTables->insertItem(1, "Links");
    _comboBoxTables->insertItem(2, "Link Collections");
    _comboBoxTables->insertItem(3, "Searches");
    _comboBoxTables->insertItem(4, "Session Logs");
    _comboBoxTables->insertItem(5, "Tags");
    tableSelected(0);
}

void DataViewerWindow::setupConnections() {
    connect(_comboBoxTables, &QComboBox::currentIndexChanged, this, &DataViewerWindow::tableSelected);
    connect(_buttonRefresh, &QToolButton::clicked, this, [this] { tableSelected(_comboBoxTables->currentIndex()); });
    connect(_buttonToggleColors, &QToolButton::clicked, this, &DataViewerWindow::toggleColors);
    connect(_buttonToggleGrid, &QToolButton::clicked, this, &DataViewerWindow::toggleGrid);
    connect(_buttonToggleFitColumns, &QToolButton::clicked, this, &DataViewerWindow::toggleFitColumns);
    connect(_buttonFontColor, &QToolButton::clicked, this, &DataViewerWindow::setFontColor);
    connect(_buttonBackColor, &QToolButton::clicked, this, &DataViewerWindow::setBackColor);
    connect(_tableWidget, &QTableWidget::currentCellChanged, this, &DataViewerWindow::showCellText);
    connect(_tableWidget, &QTableWidget::customContextMenuRequested, this, &DataViewerWindow::showTableContextMenu);
    connect(_buttonGridStyle, &QToolButton::clicked, this, &DataViewerWindow::setupGridMenu);
}

void DataViewerWindow::setupTabOrder() {
    QWidget::setTabOrder(_comboBoxTables, _buttonRefresh);
    QWidget::setTabOrder(_buttonRefresh, _buttonToggleColors);
    QWidget::setTabOrder(_buttonToggleColors, _buttonToggleGrid);
    QWidget::setTabOrder(_buttonToggleGrid, _buttonToggleFitColumns);
    QWidget::setTabOrder(_buttonToggleFitColumns, _buttonFontColor);
    QWidget::setTabOrder(_buttonFontColor, _buttonBackColor);
    QWidget::setTabOrder(_buttonBackColor, _buttonGridStyle);
    QWidget::setTabOrder(_buttonGridStyle, _tableWidget);
}

void DataViewerWindow::setupGridMenu() {
    QMenu gridMenu;
    gridMenu.setStyleSheet(common::openSheet(":/styles/style.qss"));
    gridMenu.addAction("Solid Line", this, [this] {
        _tableWidget->setGridStyle(Qt::SolidLine);
    });
    gridMenu.addAction("Dash Line", this, [this] {
        _tableWidget->setGridStyle(Qt::DashLine);
    });
    gridMenu.addAction("Dot Line", this, [this] {
        _tableWidget->setGridStyle(Qt::DotLine);
    });
    gridMenu.addAction("DashDot Line", this, [this] {
        _tableWidget->setGridStyle(Qt::DashDotLine);
    });
    gridMenu.addAction("DashDotDot Line", this, [this] {
        _tableWidget->setGridStyle(Qt::DashDotDotLine);
    });
    _buttonGridStyle->setMenu(&gridMenu);
    _buttonGridStyle->showMenu();
}

void DataViewerWindow::showTableContextMenu(const QPoint &pos) {
    QPoint globalPos = _tableWidget->mapToGlobal(pos);

    QMenu mainMenu;
    mainMenu.setStyleSheet(common::openSheet(":/styles/style.qss"));
    mainMenu.addAction("Edit Cell", this, [this] {
        _tableWidget->editItem(_tableWidget->currentItem());
    });
    mainMenu.addAction("Hide Column", this, [this] {
        hideColumn(_tableWidget->currentColumn());
    });

    QMenu *showMenu = mainMenu.addMenu("Show Column");
    showMenu->setStyleSheet(common::openSheet(":/styles/style.qss"));

    QHashIterator<int, QString> i(_hiddenColumns);
    while (i.hasNext()) {
        i.next();
        showMenu->addAction(i.value(), this, [this, i] {
            showColumn(i.key());
        });
    }

    mainMenu.addAction("Show All Columns", this, [this] {
        showAllColumns();
    });
    mainMenu.exec(globalPos);
}

void DataViewerWindow::showColumn(int index) {
    _hiddenColumns.remove(index);
    _tableWidget->showColumn(index);
}

void DataViewerWindow::showAllColumns() {
    QHashIterator<int, QString> i(_hiddenColumns);
    while (i.hasNext()) {
        i.next();
        showColumn(i.key());
    }
}

void DataViewerWindow::hideColumn(int index) {
    _hiddenColumns[index] = _tableWidget->horizontalHeaderItem(index)->text();
    _tableWidget->hideColumn(index);
}

void DataViewerWindow::populateTable() {
    qint32 fieldCount = queries::query.record().count();
    QStringList fieldNames;
    for (int i = 0; i < fieldCount; i++) {
        QString fieldName = queries::query.record().fieldName(i);
        fieldNames.append(fieldName);
    }

    _tableWidget->setColumnCount(fieldCount);
    _tableWidget->setHorizontalHeaderLabels(fieldNames);
    while (queries::query.next()) {
        qint32 rowCount = _tableWidget->rowCount();
        _tableWidget->setRowCount(rowCount);
        _tableWidget->insertRow(rowCount);
        for (int i = 0; i < fieldCount; i++) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(queries::query.value(i).toString());
            _tableWidget->setItem(rowCount, i, item);
        }
    }

    _tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _statusbar->clearMessage();
    _tableWidget->horizontalHeader()->setSectionResizeMode(_columnsResizeMode);
}

void DataViewerWindow::closeEvent(QCloseEvent *event) {
    event->accept();
}

void DataViewerWindow::tableSelected(int index) {
    showAllColumns();
    switch (index) {
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
        default:
            queries::selectEbooksTable();
            break;
    }
    _tableWidget->clear();
    _tableWidget->setRowCount(0);
    populateTable();
}

void DataViewerWindow::toggleColors() {
    if (_tableWidget->alternatingRowColors()) {
        _tableWidget->setAlternatingRowColors(false);
        _buttonToggleColors->setIcon(QIcon(":/icons/drop_icon.png"));
    } else {
        _tableWidget->setAlternatingRowColors(true);
        _buttonToggleColors->setIcon(QIcon(":/icons/crossed_drop_icon.png"));
    }
}

void DataViewerWindow::toggleGrid() {
    bool gridOn = _tableWidget->showGrid();
    if (gridOn) {
        _buttonToggleGrid->setIcon(QIcon(":/icons/grid_icon.png"));
    } else {
        _buttonToggleGrid->setIcon(QIcon(":/icons/hide_grid_icon.png"));
    }
    _tableWidget->setShowGrid(!gridOn);
}

void DataViewerWindow::toggleFitColumns() {
    if (_columnsResizeMode == QHeaderView::Stretch) {
        _tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        _columnsResizeMode = QHeaderView::ResizeToContents;
    } else {
        _tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        _columnsResizeMode = QHeaderView::Stretch;
    }
}

void DataViewerWindow::showCellText() {
    if (_tableWidget->currentItem()) {
        QString text = _tableWidget->currentItem()->text();
        _statusbar->showMessage(text);
    }
}

void DataViewerWindow::setFontColor() {
    QPoint bottom = _buttonFontColor->rect().bottomLeft();
    QPoint globalPos = _buttonFontColor->mapToGlobal(bottom);
    globalPos.setY(globalPos.y() + 2);

    ColorPickerWidget *widget = new ColorPickerWidget(this, Qt::white);
    widget->move(globalPos);
    common::openDialog(widget, ":/styles/colorpickerstyle.qss");

    if (!widget->colorSelected()) {
        return;
    }

    QString colorSheet = "background-color: " + widget->getCurrentColor().name();
    _buttonFontColor->setStyleSheet(colorSheet);
    _buttonFontColor->update();
    colorSheet = "color: " + widget->getCurrentColor().name();
    _tableWidget->setStyleSheet(_tableWidget->styleSheet() + ";" + colorSheet);
}

void DataViewerWindow::setBackColor() {
    QPoint bottom = _buttonBackColor->rect().bottomLeft();
    QPoint globalPos = _buttonBackColor->mapToGlobal(bottom);
    globalPos.setY(globalPos.y() + 2);

    ColorPickerWidget *widget = new ColorPickerWidget(this, QColor("#2D2D30"));
    widget->move(globalPos);
    common::openDialog(widget, ":/styles/colorpickerstyle.qss");

    if (!widget->colorSelected()) {
        return;
    }

    QString colorSheet = "background-color: " + widget->getCurrentColor().name();
    _buttonBackColor->setStyleSheet(colorSheet);
    _buttonBackColor->update();
    _tableWidget->setStyleSheet(_tableWidget->styleSheet() + ";" + colorSheet);

    if (widget->getCurrentColor() != Qt::white) {
        _buttonBackColor->setIcon(QIcon(":/icons/background_fill_white.png"));
    } else {
        _buttonBackColor->setIcon(QIcon(":/icons/background_fill_black.png"));
    }
}
