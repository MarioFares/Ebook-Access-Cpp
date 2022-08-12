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

DataViewerWindow::DataViewerWindow(QWidget* parent) : QMainWindow(parent)
{
	m_columnsResizeMode = QHeaderView::Stretch;
	setupInterface();
	setupConnections();
	setupTabOrder();
}

void DataViewerWindow::setupInterface()
{
	// Window
	setWindowTitle("Data Viewer");
	resize(800, 600);

	// Widgets
	m_centralWidget = new QWidget(this);

	m_vertLayMain = new QVBoxLayout(m_centralWidget);
	m_frameSearch = new QFrame(m_centralWidget);
	m_frameSearch->setStyleSheet(QString("QFrame {border: none;}"));
	m_frameSearch->setFrameShape(QFrame::StyledPanel);
	m_frameSearch->setFrameShadow(QFrame::Raised);

	// ComboBox Layout
	m_horLayComboBox = new QHBoxLayout(m_frameSearch);
	m_horSpacerComboBoxLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_horSpacerComboBoxRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_comboBoxTables = new QComboBox(m_frameSearch);
	m_comboBoxTables->setMinimumSize(QSize(0, 25));
	m_comboBoxTables->setStyleSheet(QString("QFrame { border: 1px solid grey; } "));

	m_horLayComboBox->addItem(m_horSpacerComboBoxLeft);
	m_horLayComboBox->addWidget(m_comboBoxTables);
	m_horLayComboBox->addItem(m_horSpacerComboBoxRight);

	// Tool Buttons Layout
	m_horLayButtons = new QHBoxLayout();
	m_horSpacerButtonsLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_horSpacerButtonsRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_buttonRefresh = new QToolButton(m_centralWidget);
	m_buttonRefresh->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconRefresh(":/icons/refresh_icon.png");
	m_buttonRefresh->setIcon(iconRefresh);
	m_buttonRefresh->setAutoRaise(false);

	m_buttonToggleColors = new QToolButton(m_centralWidget);
	m_buttonToggleColors->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconToggleColors(":/icons/drop_icon.png");
	m_buttonToggleColors->setIcon(iconToggleColors);
	m_buttonToggleColors->setAutoRaise(false);

	m_buttonToggleGrid = new QToolButton(m_centralWidget);
	m_buttonToggleGrid->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconToggleGrid(":/icons/hide_grid_icon.png");
	m_buttonToggleGrid->setIcon(iconToggleGrid);
	m_buttonToggleGrid->setIconSize(QSize(22, 22));
	m_buttonToggleGrid->setPopupMode(QToolButton::InstantPopup);
	m_buttonToggleGrid->setAutoRaise(false);
	m_buttonToggleGrid->setArrowType(Qt::NoArrow);

	m_buttonToggleFitColumns = new QToolButton(m_centralWidget);
	m_buttonToggleFitColumns->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconToggleFitColumns(":/icons/expand_icon.png");
	m_buttonToggleFitColumns->setIcon(iconToggleFitColumns);
	m_buttonToggleFitColumns->setIconSize(QSize(22, 22));
	m_buttonToggleFitColumns->setAutoRaise(false);

	m_buttonFontColor = new QToolButton(m_centralWidget);
	m_buttonFontColor->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconFontColor(":/icons/font_color_icon.png");
	m_buttonFontColor->setIcon(iconFontColor);

	m_buttonBackColor = new QToolButton(m_centralWidget);
	m_buttonBackColor->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconBackColor(":/icons/background_fill_white.png");
	m_buttonBackColor->setIcon(iconBackColor);
	m_buttonBackColor->setPopupMode(QToolButton::InstantPopup);
	m_buttonBackColor->setAutoRaise(false);
	m_buttonBackColor->setArrowType(Qt::NoArrow);

	m_buttonGridStyle = new QToolButton(m_centralWidget);
	m_buttonGridStyle->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconGridStyle(":/icons/grid_icon.png");
	m_buttonGridStyle->setIcon(iconGridStyle);
	m_buttonGridStyle->setIconSize(QSize(22, 22));
	m_buttonGridStyle->setPopupMode(QToolButton::InstantPopup);

	m_horLayButtons->addSpacerItem(m_horSpacerButtonsLeft);
	m_horLayButtons->addWidget(m_buttonRefresh);
	m_horLayButtons->addWidget(m_buttonToggleColors);
	m_horLayButtons->addWidget(m_buttonToggleGrid);
	m_horLayButtons->addWidget(m_buttonToggleFitColumns);
	m_horLayButtons->addWidget(m_buttonFontColor);
	m_horLayButtons->addWidget(m_buttonBackColor);
	m_horLayButtons->addWidget(m_buttonGridStyle);
	m_horLayButtons->addSpacerItem(m_horSpacerButtonsRight);

	m_tableWidget = new QTableWidget(m_centralWidget);
	m_tableWidget->setShowGrid(true);
	m_tableWidget->setGridStyle(Qt::SolidLine);
	m_tableWidget->setSortingEnabled(false);
	m_tableWidget->setColumnCount(0);
	m_tableWidget->horizontalHeader()->setVisible(true);
	m_tableWidget->horizontalHeader()->setCascadingSectionResizes(true);
	m_tableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
	m_tableWidget->horizontalHeader()->setStretchLastSection(false);
	m_tableWidget->verticalHeader()->setVisible(false);
	m_tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	m_vertLayMain->addWidget(m_frameSearch);
	m_vertLayMain->addLayout(m_horLayButtons);
	m_vertLayMain->addWidget(m_tableWidget);

	// Status Bar
	m_statusbar = new QStatusBar(this);
	setStatusBar(m_statusbar);

	// Central widget
	setCentralWidget(m_centralWidget);

	// Load Data
	m_comboBoxTables->insertItem(0, "Ebooks");
	m_comboBoxTables->insertItem(1, "Links");
	m_comboBoxTables->insertItem(2, "Link Collections");
	m_comboBoxTables->insertItem(3, "Searches");
	m_comboBoxTables->insertItem(4, "Session Logs");
	m_comboBoxTables->insertItem(5, "Tags");
	tableSelected(0);
}

void DataViewerWindow::setupConnections()
{
	connect(m_comboBoxTables, &QComboBox::currentIndexChanged, this, &DataViewerWindow::tableSelected);
	connect(m_buttonRefresh, &QToolButton::clicked, this, [this]
	{ tableSelected(m_comboBoxTables->currentIndex()); });
	connect(m_buttonToggleColors, &QToolButton::clicked, this, &DataViewerWindow::toggleColors);
	connect(m_buttonToggleGrid, &QToolButton::clicked, this, &DataViewerWindow::toggleGrid);
	connect(m_buttonToggleFitColumns, &QToolButton::clicked, this, &DataViewerWindow::toggleFitColumns);
	connect(m_buttonFontColor, &QToolButton::clicked, this, &DataViewerWindow::setFontColor);
	connect(m_buttonBackColor, &QToolButton::clicked, this, &DataViewerWindow::setBackColor);
	connect(m_tableWidget, &QTableWidget::currentCellChanged, this, &DataViewerWindow::showCellText);
	connect(m_tableWidget, &QTableWidget::customContextMenuRequested, this, &DataViewerWindow::showTableContextMenu);
	connect(m_buttonGridStyle, &QToolButton::clicked, this, &DataViewerWindow::setupGridMenu);
}

void DataViewerWindow::setupTabOrder()
{
	QWidget::setTabOrder(m_comboBoxTables, m_buttonRefresh);
	QWidget::setTabOrder(m_buttonRefresh, m_buttonToggleColors);
	QWidget::setTabOrder(m_buttonToggleColors, m_buttonToggleGrid);
	QWidget::setTabOrder(m_buttonToggleGrid, m_buttonToggleFitColumns);
	QWidget::setTabOrder(m_buttonToggleFitColumns, m_buttonFontColor);
	QWidget::setTabOrder(m_buttonFontColor, m_buttonBackColor);
	QWidget::setTabOrder(m_buttonBackColor, m_buttonGridStyle);
	QWidget::setTabOrder(m_buttonGridStyle, m_tableWidget);
}

void DataViewerWindow::setupGridMenu()
{
	QMenu gridMenu;
	gridMenu.setStyleSheet(common::openSheet(":/styles/style.qss"));
	gridMenu.addAction("Solid Line", this, [this]
	{
		m_tableWidget->setGridStyle(Qt::SolidLine);
	});
	gridMenu.addAction("Dash Line", this, [this]
	{
		m_tableWidget->setGridStyle(Qt::DashLine);
	});
	gridMenu.addAction("Dot Line", this, [this]
	{
		m_tableWidget->setGridStyle(Qt::DotLine);
	});
	gridMenu.addAction("DashDot Line", this, [this]
	{
		m_tableWidget->setGridStyle(Qt::DashDotLine);
	});
	gridMenu.addAction("DashDotDot Line", this, [this]
	{
		m_tableWidget->setGridStyle(Qt::DashDotDotLine);
	});
	m_buttonGridStyle->setMenu(&gridMenu);
	m_buttonGridStyle->showMenu();
}

void DataViewerWindow::showTableContextMenu(const QPoint& pos)
{
	QPoint globalPos = m_tableWidget->mapToGlobal(pos);

	QMenu mainMenu;
	mainMenu.setStyleSheet(common::openSheet(":/styles/style.qss"));
	mainMenu.addAction("Edit Cell", this, [this]
	{
		m_tableWidget->editItem(m_tableWidget->currentItem());
	});
	mainMenu.addAction("Hide Column", this, [this]
	{
		hideColumn(m_tableWidget->currentColumn());
	});

	QMenu* showMenu = mainMenu.addMenu("Show Column");
	showMenu->setStyleSheet(common::openSheet(":/styles/style.qss"));

	QHashIterator<int, QString> i(m_hiddenColumns);
	while (i.hasNext())
	{
		i.next();
		showMenu->addAction(i.value(), this, [this, i]
		{
			showColumn(i.key());
		});
	}

	mainMenu.addAction("Show All Columns", this, [this]
	{
		showAllColumns();
	});
	mainMenu.exec(globalPos);
}

void DataViewerWindow::showColumn(int index)
{
	m_hiddenColumns.remove(index);
	m_tableWidget->showColumn(index);
}

void DataViewerWindow::showAllColumns()
{
	QHashIterator<int, QString> i(m_hiddenColumns);
	while (i.hasNext())
	{
		i.next();
		showColumn(i.key());
	}
}

void DataViewerWindow::hideColumn(int index)
{
	m_hiddenColumns[index] = m_tableWidget->horizontalHeaderItem(index)->text();
	m_tableWidget->hideColumn(index);
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

	m_tableWidget->setColumnCount(fieldCount);
	m_tableWidget->setHorizontalHeaderLabels(fieldNames);
	while (queries::query.next())
	{
		qint32 rowCount = m_tableWidget->rowCount();
		m_tableWidget->setRowCount(rowCount);
		m_tableWidget->insertRow(rowCount);
		for (int i = 0; i < fieldCount; i++)
		{
			QTableWidgetItem* item = new QTableWidgetItem();
			item->setText(queries::query.value(i).toString());
			m_tableWidget->setItem(rowCount, i, item);
		}
	}

	m_tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	m_statusbar->clearMessage();
	m_tableWidget->horizontalHeader()->setSectionResizeMode(m_columnsResizeMode);
}

void DataViewerWindow::closeEvent(QCloseEvent* event)
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
		default:
			queries::selectEbooksTable();
			break;
	}
	m_tableWidget->clear();
	m_tableWidget->setRowCount(0);
	populateTable();
}

void DataViewerWindow::toggleColors()
{
	if (m_tableWidget->alternatingRowColors())
	{
		m_tableWidget->setAlternatingRowColors(false);
		m_buttonToggleColors->setIcon(QIcon(":/icons/drop_icon.png"));
	}
	else
	{
		m_tableWidget->setAlternatingRowColors(true);
		m_buttonToggleColors->setIcon(QIcon(":/icons/crossed_drop_icon.png"));
	}
}

void DataViewerWindow::toggleGrid()
{
	bool gridOn = m_tableWidget->showGrid();
	if (gridOn)
	{
		m_buttonToggleGrid->setIcon(QIcon(":/icons/grid_icon.png"));
	}
	else
	{
		m_buttonToggleGrid->setIcon(QIcon(":/icons/hide_grid_icon.png"));
	}
	m_tableWidget->setShowGrid(!gridOn);
}

void DataViewerWindow::toggleFitColumns()
{
	if (m_columnsResizeMode == QHeaderView::Stretch)
	{
		m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		m_columnsResizeMode = QHeaderView::ResizeToContents;
	}
	else
	{
		m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		m_columnsResizeMode = QHeaderView::Stretch;
	}
}

void DataViewerWindow::showCellText()
{
	if (m_tableWidget->currentItem())
	{
		QString text = m_tableWidget->currentItem()->text();
		m_statusbar->showMessage(text);
	}
}

void DataViewerWindow::setFontColor()
{
	QPoint bottom = m_buttonFontColor->rect().bottomLeft();
	QPoint globalPos = m_buttonFontColor->mapToGlobal(bottom);
	globalPos.setY(globalPos.y() + 2);

	ColorPickerWidget* widget = new ColorPickerWidget(this, Qt::white);
	widget->move(globalPos);
	common::openDialog(widget, ":/styles/colorpickerstyle.qss");

	if (!widget->colorSelected())
	{
		return;
	}

	QString colorSheet = "background-color: " + widget->getCurrentColor().name();
	m_buttonFontColor->setStyleSheet(colorSheet);
	m_buttonFontColor->update();
	colorSheet = "color: " + widget->getCurrentColor().name();
	m_tableWidget->setStyleSheet(m_tableWidget->styleSheet() + ";" + colorSheet);
}

void DataViewerWindow::setBackColor()
{
	QPoint bottom = m_buttonBackColor->rect().bottomLeft();
	QPoint globalPos = m_buttonBackColor->mapToGlobal(bottom);
	globalPos.setY(globalPos.y() + 2);

	ColorPickerWidget* widget = new ColorPickerWidget(this, QColor("#2D2D30"));
	widget->move(globalPos);
	common::openDialog(widget, ":/styles/colorpickerstyle.qss");

	if (!widget->colorSelected())
	{
		return;
	}

	QString colorSheet = "background-color: " + widget->getCurrentColor().name();
	m_buttonBackColor->setStyleSheet(colorSheet);
	m_buttonBackColor->update();
	m_tableWidget->setStyleSheet(m_tableWidget->styleSheet() + ";" + colorSheet);

	if (widget->getCurrentColor() != Qt::white)
	{
		m_buttonBackColor->setIcon(QIcon(":/icons/background_fill_white.png"));
	}
	else
	{
		m_buttonBackColor->setIcon(QIcon(":/icons/background_fill_black.png"));
	}
}
