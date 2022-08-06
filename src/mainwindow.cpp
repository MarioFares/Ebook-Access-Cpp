#include "include/common.h"
#include "include/queries.h"
#include "include/mainwindow.h"
#include "include/quotedialog.h"
#include "include/yesnodialog.h"
#include "include/summarywindow.h"
#include "include/getnamedialog.h"
#include "include/addbookdialog.h"
#include "include/addbooksdialog.h"
#include "include/dataviewerwindow.h"
#include "include/bookmetadatawindow.h"
#include "include/cleanbooksdialog.h"
#include "include/linkmanagerwindow.h"
#include "include/extselectiondialog.h"

#include <QDir>
#include <QFile>
#include <QTimer>
#include <QProcess>
#include <QSqlRecord>
#include <QSpacerItem>
#include <QToolButton>
#include <QSystemTrayIcon>
#include <QRandomGenerator>
#include <QDesktopServices>
#include <QOperatingSystemVersion>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	// Setup DB
	queries::connectToDatabase();
	queries::setupDb();
	queries::logSessionStart();

	// Create dirs
	QDir dir;
	dir.mkdir("./reports");
	dir.mkdir("./reports/ebooks");
	dir.mkdir("./reports/usage");

	// Setup Conversion
	m_sizeConvFactors["KB"] = 1024;
	m_sizeConvFactors["MB"] = 1024 * 1024;
	m_sizeConvFactors["GB"] = 1024 * 1024 * 1024;

	// Setup Size Factors to Toggle Sizes
	m_sizeUnits["KB"] = "MB";
	m_sizeUnits["MB"] = "GB";
	m_sizeUnits["GB"] = "KB";

	// Setup List Sorting
	m_SORT = Qt::AscendingOrder;

	// Setup Tray Icon
	m_trayIcon = new QSystemTrayIcon(this);
	m_trayIcon->setIcon(QIcon(":/icons/books_icon.png"));
	m_trayIcon->setVisible(true);
	m_trayIcon->setToolTip("Ebook Access\nClick to Open");
	m_trayIcon->show();

	// QTimer::singleShot(1500, this, SLOT(showQuote()));

	setupInterface();
	setupConnections();
	setupTabOrder();
	setupShortcuts();
}

void MainWindow::setupInterface()
{
	// Window
	setWindowTitle("Ebook Access");
	resize(1190, 668);

	// Widgets
	m_centralWidget = new QWidget(this);

	m_horLayCentral = new QHBoxLayout(m_centralWidget);

	m_frameMain = new QFrame(m_centralWidget);
	m_frameMain->setObjectName("frameMain");
	m_frameMain->setStyleSheet(QString("QFrame#frameMain { border:0px; }"));

	m_horLayMain = new QHBoxLayout(m_frameMain);
	m_splitter = new QSplitter(m_frameMain);
	m_splitter->setObjectName("splitter");
	m_splitter->setStyleSheet(QString("QFrame#splitter\n"
									  "{border:0px;}\n"
									  "\n"
									  "QSplitter::handle {\n"
									  "         background-color: #2D2D30;\n"
									  "     }"));
	m_splitter->setLineWidth(0);
	m_splitter->setOrientation(Qt::Horizontal);
	m_splitter->setHandleWidth(2);

	m_frameMainLeft = new QFrame(m_splitter);
	m_frameMainLeft->setObjectName("frameMainLeft");
	m_frameMainLeft->setStyleSheet(QString("QFrame#frameMainLeft\n"
										   "{"
										   "border: 0px;\n"
										   "}"));

	m_vertLayMainLeft = new QVBoxLayout(m_frameMainLeft);
	m_vertLayMainLeft->setSizeConstraint(QLayout::SetDefaultConstraint);
	m_vertLayMainLeft->setContentsMargins(-1, 32, -1, 0);

	m_frameSearchCriteria = new QFrame(m_frameMainLeft);
	m_frameSearchCriteria->setStyleSheet(QString(""));
	m_frameSearchCriteria->setFrameShape(QFrame::StyledPanel);
	m_frameSearchCriteria->setFrameShadow(QFrame::Raised);

	m_gridLaySearch = new QGridLayout(m_frameSearchCriteria);
	m_gridLaySearch->setSizeConstraint(QLayout::SetDefaultConstraint);
	m_gridLaySearch->setContentsMargins(-1, 9, -1, 9);

	m_gridLayCriteria = new QGridLayout();
	m_gridLayCriteria->setHorizontalSpacing(17);
	m_gridLayCriteria->setVerticalSpacing(30);
	m_gridLayCriteria->setContentsMargins(20, 5, 20, 5);

	m_labelGenreCriteria = new QLabel("Genre", m_frameSearchCriteria);

	m_spinBoxToPagesCriteria = new QSpinBox(m_frameSearchCriteria);
	m_spinBoxToPagesCriteria->setMaximumSize(QSize(70, 16777215));
	m_spinBoxToPagesCriteria->setMinimum(0);
	m_spinBoxToPagesCriteria->setMaximum(100000);
	m_spinBoxToPagesCriteria->setValue(100000);

	m_labelPagesToCriteria = new QLabel(m_frameSearchCriteria);
	m_labelPagesToCriteria->setPixmap(QPixmap(QString(":/icons/right_arrow_icon.png")));
	m_labelPagesToCriteria->setAlignment(Qt::AlignCenter);

	m_labelSizeToCriteria = new QLabel(m_frameSearchCriteria);
	m_labelSizeToCriteria->setPixmap(QPixmap(QString(":/icons/right_arrow_icon.png")));
	m_labelSizeToCriteria->setScaledContents(false);
	m_labelSizeToCriteria->setAlignment(Qt::AlignCenter);

	m_labelAuthorCriteria = new QLabel("Author", m_frameSearchCriteria);

	m_labelSearchCriteria = new QLabel("Search", m_frameSearchCriteria);

	m_textExts = new QLineEdit(m_frameSearchCriteria);
	m_textExts->setMinimumSize(QSize(0, 23));

	m_spinBoxFromSizeCriteria = new QSpinBox(m_frameSearchCriteria);
	m_spinBoxFromSizeCriteria->setMaximumSize(QSize(70, 16777215));
	m_spinBoxFromSizeCriteria->setMaximum(1023);

	m_labelGroupsCriteria = new QLabel("Tags", m_frameSearchCriteria);

	m_labelFolderCriteria = new QLabel("Folder", m_frameSearchCriteria);

	m_comboBoxSearchLoad = new QComboBox(m_frameSearchCriteria);
	m_comboBoxSearchLoad->setContextMenuPolicy(Qt::CustomContextMenu);
	m_comboBoxSearchLoad->addItem(QString());
	m_comboBoxSearchLoad->setMinimumSize(QSize(0, 23));
	m_comboBoxSearchLoad->setEditable(false);

	m_textTagsCriteria = new QLineEdit(m_frameSearchCriteria);
	m_textTagsCriteria->setMinimumSize(QSize(0, 23));

	m_spinBoxToSizeCriteria = new QSpinBox(m_frameSearchCriteria);
	m_spinBoxToSizeCriteria->setMaximumSize(QSize(70, 16777215));
	m_spinBoxToSizeCriteria->setMaximum(1024);
	m_spinBoxToSizeCriteria->setValue(1024);

	m_spinBoxFromPagesCriteria = new QSpinBox(m_frameSearchCriteria);
	m_spinBoxFromPagesCriteria->setMaximumSize(QSize(70, 16777215));
	m_spinBoxFromPagesCriteria->setMaximum(100000);

	m_comboBoxFolderCriteria = new QComboBox(m_frameSearchCriteria);
	m_comboBoxFolderCriteria->addItem(QString());
	m_comboBoxFolderCriteria->setMinimumSize(QSize(0, 23));
	m_comboBoxFolderCriteria->setEditable(true);
	m_comboBoxFolderCriteria->setMinimumContentsLength(40);
	m_comboBoxFolderCriteria->setFrame(true);

	m_comboBoxGenreCriteria = new QComboBox(m_frameSearchCriteria);
	m_comboBoxGenreCriteria->addItem(QString());
	m_comboBoxGenreCriteria->setMinimumSize(QSize(0, 23));
	m_comboBoxGenreCriteria->setEditable(true);
	m_comboBoxGenreCriteria->setMinimumContentsLength(40);

	m_comboBoxAuthorCriteria = new QComboBox(m_frameSearchCriteria);
	m_comboBoxAuthorCriteria->addItem(QString());
	m_comboBoxAuthorCriteria->setMinimumSize(QSize(0, 23));
	m_comboBoxAuthorCriteria->setEditable(true);
	m_comboBoxAuthorCriteria->setMinimumContentsLength(40);

	m_labelSizeCriteria = new QLabel("Size", m_frameSearchCriteria);

	m_labelPagesCriteria = new QLabel("Pages", m_frameSearchCriteria);

	m_labelExtensionCriteria = new QLabel("Ext", m_frameSearchCriteria);

	m_buttonSearchCriteria = new QToolButton(m_frameSearchCriteria);
	m_buttonSearchCriteria->setMinimumSize(QSize(70, 30));
	m_buttonSearchCriteria->setMaximumSize(QSize(90, 16777215));
	m_buttonSearchCriteria->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonSearchCriteria->setIcon(QIcon(":/icons/search_icon.png"));
	m_buttonSearchCriteria->setIconSize(QSize(30, 30));
	m_buttonSearchCriteria->setAutoRaise(false);

	m_buttonAuthor = new QToolButton(m_frameSearchCriteria);
	m_buttonAuthor->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonAuthor->setIcon(QIcon(":/icons/author_icon.png"));
	m_buttonAuthor->setIconSize(QSize(22, 22));

	m_buttonClearCriteria = new QToolButton(m_frameSearchCriteria);
	m_buttonClearCriteria->setMinimumSize(QSize(70, 30));
	m_buttonClearCriteria->setMaximumSize(QSize(90, 16777215));
	m_buttonClearCriteria->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonClearCriteria->setIcon(QIcon(":/icons/clear_search_white_icon.png"));
	m_buttonClearCriteria->setIconSize(QSize(30, 30));

	m_buttonExtensions = new QToolButton(m_frameSearchCriteria);
	m_buttonExtensions->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonExtensions->setIcon(QIcon(":/icons/puzzle_icon.png"));
	m_buttonExtensions->setIconSize(QSize(20, 20));

	m_buttonFolder = new QToolButton(m_frameSearchCriteria);
	m_buttonFolder->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonFolder->setIcon(QIcon(":/icons/folder_icon.png"));
	m_buttonFolder->setIconSize(QSize(20, 20));

	m_buttonGenre = new QToolButton(m_frameSearchCriteria);
	m_buttonGenre->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonGenre->setIcon(QIcon(":/icons/add_new_icon.png"));
	m_buttonGenre->setIconSize(QSize(20, 20));

	m_buttonSaveCriteria = new QToolButton(m_frameSearchCriteria);
	m_buttonSaveCriteria->setMinimumSize(QSize(70, 30));
	m_buttonSaveCriteria->setMaximumSize(QSize(90, 16777215));
	m_buttonSaveCriteria->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonSaveCriteria->setIcon(QIcon(":/icons/save_icon.png"));
	m_buttonSaveCriteria->setIconSize(QSize(30, 30));

	m_buttonSearchLoad = new QToolButton(m_frameSearchCriteria);
	m_buttonSearchLoad->setMinimumSize(QSize(51, 25));
	m_buttonSearchLoad->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonSearchLoad->setIcon(QIcon(":/icons/upload_icon.png"));

	m_buttonSizeCriteria = new QToolButton(m_frameSearchCriteria);
	m_buttonSizeCriteria->setText("MB");
	m_buttonSizeCriteria->setMinimumSize(QSize(0, 0));
	m_buttonSizeCriteria->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonSizeCriteria->setIconSize(QSize(25, 20));

	m_buttonTags = new QToolButton(m_frameSearchCriteria);
	m_buttonTags->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonTags->setIcon(QIcon(":/icons/tags_icon.png"));
	m_buttonTags->setIconSize(QSize(20, 20));

	// Grid Search Criteria
	m_gridLayCriteria->addWidget(m_labelGenreCriteria, 2, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_spinBoxToPagesCriteria, 6, 4, 1, 1);
	m_gridLayCriteria->addWidget(m_labelPagesToCriteria, 6, 2, 1, 2);
	m_gridLayCriteria->addWidget(m_labelSizeToCriteria, 5, 2, 1, 2);
	m_gridLayCriteria->addWidget(m_labelAuthorCriteria, 1, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_labelSearchCriteria, 7, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_textExts, 4, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_spinBoxFromSizeCriteria, 5, 1, 1, 1);
	m_gridLayCriteria->addWidget(m_labelGroupsCriteria, 3, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_labelFolderCriteria, 0, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxSearchLoad, 7, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_textTagsCriteria, 3, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_spinBoxToSizeCriteria, 5, 4, 1, 1);
	m_gridLayCriteria->addWidget(m_spinBoxFromPagesCriteria, 6, 1, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxFolderCriteria, 0, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_comboBoxGenreCriteria, 2, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_comboBoxAuthorCriteria, 1, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_labelSizeCriteria, 5, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_labelPagesCriteria, 6, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_labelExtensionCriteria, 4, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_buttonSearchCriteria, 5, 6, 1, 1);
	m_gridLayCriteria->addWidget(m_buttonAuthor, 1, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_buttonClearCriteria, 6, 6, 1, 1);
	m_gridLayCriteria->addWidget(m_buttonExtensions, 4, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_buttonFolder, 0, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_buttonGenre, 2, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_buttonSaveCriteria, 7, 6, 1, 1);
	m_gridLayCriteria->addWidget(m_buttonSearchLoad, 7, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_buttonSizeCriteria, 5, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_buttonTags, 3, 5, 1, 1);
	m_gridLaySearch->addLayout(m_gridLayCriteria, 0, 0, 1, 1);

	m_vertLayMainLeft->addWidget(m_frameSearchCriteria);

	m_frameBottomButtons = new QFrame(m_frameMainLeft);
	m_frameBottomButtons->setObjectName("frameBottomButtons");
	m_frameBottomButtons->setStyleSheet(QString("QFrame#frameBottomButtons { border: none; }"));
	m_frameBottomButtons->setLineWidth(0);

	m_horLayBottomButtons = new QHBoxLayout(m_frameBottomButtons);
	m_horLayBottomButtons->setContentsMargins(0, 0, 0, 0);
	m_frameAddButtons = new QFrame(m_frameBottomButtons);
	m_frameAddButtons->setFrameShape(QFrame::StyledPanel);
	m_frameAddButtons->setFrameShadow(QFrame::Raised);

	m_horLayAddButtons = new QHBoxLayout(m_frameAddButtons);
	m_horLayAddButtons->setSpacing(0);

	m_buttonAddBook = new QToolButton(m_frameAddButtons);
	m_buttonAddBook->setMinimumSize(QSize(0, 50));
	m_buttonAddBook->setMaximumWidth(80);
	m_buttonAddBook->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonAddBook->setIcon(QIcon(":/icons/add_book_icon.png"));
	m_buttonAddBook->setIconSize(QSize(41, 37));

	m_buttonAddBooks = new QToolButton(m_frameAddButtons);
	m_buttonAddBooks->setMinimumSize(QSize(0, 50));
	m_buttonAddBooks->setMaximumWidth(80);
	m_buttonAddBooks->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonAddBooks->setIcon(QIcon(":/icons/add_books_icon.png"));
	m_buttonAddBooks->setIconSize(QSize(41, 44));

	m_frameInfoButtons = new QFrame(m_frameBottomButtons);
	m_frameInfoButtons->setFrameShape(QFrame::StyledPanel);
	m_frameInfoButtons->setFrameShadow(QFrame::Raised);

	m_horLayToolButtons = new QHBoxLayout(m_frameInfoButtons);

	m_buttonSummaries = new QToolButton(m_frameInfoButtons);
	m_buttonSummaries->setMinimumSize(QSize(0, 50));
	m_buttonSummaries->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonSummaries->setIcon(QIcon(":/icons/white_notebook_icon.png"));
	m_buttonSummaries->setIconSize(QSize(41, 44));

	m_buttonLinkManager = new QToolButton(m_frameInfoButtons);
	m_buttonLinkManager->setMinimumSize(QSize(0, 50));
	m_buttonLinkManager->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonLinkManager->setIcon(QIcon(":/icons/link_icon.png"));
	m_buttonLinkManager->setIconSize(QSize(41, 44));

	m_buttonDbViewer = new QToolButton(m_frameInfoButtons);
	m_buttonDbViewer->setMinimumSize(QSize(0, 50));
	m_buttonDbViewer->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonDbViewer->setIcon(QIcon(":/icons/dbviewer_icon.png"));
	m_buttonDbViewer->setIconSize(QSize(41, 34));

	m_horLayAddButtons->addWidget(m_buttonAddBook);
	m_horLayAddButtons->addWidget(m_buttonAddBooks);
	m_horLayBottomButtons->addWidget(m_frameAddButtons);

	m_horLayToolButtons->addWidget(m_buttonSummaries);
	m_horLayToolButtons->addWidget(m_buttonDbViewer);
	m_horLayToolButtons->addWidget(m_buttonLinkManager);

	m_horLayBottomButtons->addWidget(m_frameInfoButtons);

	m_vertLayMainLeft->addWidget(m_frameBottomButtons);

	m_vertLayMainLeft->setStretch(0, 6);
	m_splitter->addWidget(m_frameMainLeft);
	m_frameMainRight = new QFrame(m_splitter);
	m_frameMainRight->setObjectName("frameMainRight");
	QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy3.setHorizontalStretch(6);
	sizePolicy3.setVerticalStretch(0);
	sizePolicy3.setHeightForWidth(m_frameMainRight->sizePolicy().hasHeightForWidth());
	m_frameMainRight->setSizePolicy(sizePolicy3);
	m_frameMainRight->setBaseSize(QSize(0, 0));
	m_frameMainRight->setStyleSheet(QString("QFrame#frameMainRight\n"
											"{"
											"border: 0px;\n"
											"}"));

	m_vertLayMainRight = new QVBoxLayout(m_frameMainRight);
	m_vertLayMainRight->setSpacing(6);
	m_vertLayMainRight->setContentsMargins(0, 0, 9, 0);

	m_frameSearchToolBar = new QFrame(m_frameMainRight);
	QSizePolicy retain = m_frameSearchToolBar->sizePolicy();
	retain.setRetainSizeWhenHidden(true);
	m_frameSearchToolBar->setSizePolicy(retain);
	m_frameSearchToolBar->setStyleSheet(QString("QFrame {\n"
												"border: 0px;\n"
												"}"));

	m_horLaySearchBar = new QHBoxLayout(m_frameSearchToolBar);
	m_horLaySearchBar->setSpacing(6);
	m_horLaySearchBar->setSizeConstraint(QLayout::SetMinimumSize);
	m_horLaySearchBar->setContentsMargins(0, 0, 0, 2);
	m_labelSearchBar = new QLabel("Search: ", m_frameSearchToolBar);

	m_textSearchBar = new QLineEdit(m_frameSearchToolBar);
	m_textSearchBar->setMaximumSize(QSize(16777215, 26));
	m_textSearchBar->setContextMenuPolicy(Qt::DefaultContextMenu);

	m_buttonSearchString = new QToolButton(m_frameSearchToolBar);
	m_buttonSearchString->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonSearchString->setIcon(QIcon(":/icons/search_icon.png"));
	m_buttonSearchString->setIconSize(QSize(25, 20));

	m_buttonClearSearch = new QToolButton(m_frameSearchToolBar);
	m_buttonClearSearch->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonClearSearch->setIcon(QIcon(":/icons/clear_search_white_icon.png"));
	m_buttonClearSearch->setIconSize(QSize(25, 20));

	m_buttonSortSearch = new QToolButton(m_frameSearchToolBar);
	m_buttonSortSearch->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonSortSearch->setIcon(QIcon(":/icons/sort_icon.png"));
	m_buttonSortSearch->setIconSize(QSize(25, 20));

	m_horLaySearchBar->addWidget(m_labelSearchBar);
	m_horLaySearchBar->addWidget(m_textSearchBar);
	m_horLaySearchBar->addWidget(m_buttonSearchString);
	m_horLaySearchBar->addWidget(m_buttonClearSearch);
	m_horLaySearchBar->addWidget(m_buttonSortSearch);

	m_vertLayMainRight->addWidget(m_frameSearchToolBar);

	m_ebooksListWidget = new QListWidget(m_frameMainRight);
	m_ebooksListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	m_ebooksListWidget->setSortingEnabled(true);
	m_ebooksListWidget->setAlternatingRowColors(false);
	m_ebooksListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	m_ebooksListWidget->setModelColumn(0);
	m_ebooksListWidget->setSelectionRectVisible(false);

	m_vertLayMainRight->addWidget(m_ebooksListWidget);

	m_frameDetails = new QFrame(m_frameMainRight);
	m_frameDetails->setFrameShape(QFrame::StyledPanel);
	m_frameDetails->setFrameShadow(QFrame::Raised);

	m_gridLayDetails = new QGridLayout(m_frameDetails);
	m_vertLayDetails = new QVBoxLayout();
	m_vertLayDetails->setSpacing(30);
	m_vertLayDetails->setContentsMargins(-1, 6, -1, 9);

	m_gridLayDetailsFields = new QGridLayout();
	m_gridLayDetailsFields->setSizeConstraint(QLayout::SetNoConstraint);
	m_gridLayDetailsFields->setHorizontalSpacing(12);
	m_gridLayDetailsFields->setVerticalSpacing(20);

	m_labelDetailsSize = new QLabel("Size: ", m_frameDetails);
	m_labelDetailsSize->setMaximumHeight(24);

	m_textDetailsGenre = new QLineEdit(m_frameDetails);
	m_textDetailsGenre->setMaximumHeight(24);

	m_frameSizeDetails = new QFrame(m_frameDetails);
	m_frameSizeDetails->setMinimumSize(QSize(32, 0));
	m_frameSizeDetails->setMaximumHeight(24);
	m_frameSizeDetails->setStyleSheet(QString("QFrame {\n"
											  "border: 0px;\n"
											  "}"));

	m_horLayDetailsSize = new QHBoxLayout(m_frameSizeDetails);
	m_horLayDetailsSize->setSpacing(7);
	m_horLayDetailsSize->setContentsMargins(0, 0, 0, 0);

	m_textDetailsSize = new QLineEdit(m_frameSizeDetails);
	m_textDetailsSize->setEnabled(false);
	m_textDetailsSize->setMaximumHeight(24);

	m_buttonSizeUnit = new QPushButton("MB", m_frameSizeDetails);
	m_buttonSizeUnit->setMinimumSize(QSize(21, 0));
	m_buttonSizeUnit->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonSizeUnit->setStyleSheet(QString("border: 0px;"));

	m_horLayDetailsSize->addWidget(m_textDetailsSize);
	m_horLayDetailsSize->addWidget(m_buttonSizeUnit);

	m_textDetailsTags = new QLineEdit(m_frameDetails);
	m_textDetailsTags->setMaximumHeight(24);

	m_labelDetailsGroups = new QLabel("Tags: ", m_frameDetails);
	m_labelDetailsGroups->setMaximumHeight(24);

	m_labelDetailsName = new QLabel("Name: ", m_frameDetails);
	m_labelDetailsName->setMaximumHeight(24);

	m_textDetailsAuthor = new QLineEdit(m_frameDetails);
	m_textDetailsAuthor->setMaximumHeight(24);

	m_textDetailsFolder = new QLineEdit(m_frameDetails);
	m_textDetailsFolder->setMaximumHeight(24);

	m_textDetailsExt = new QLineEdit(m_frameDetails);
	m_textDetailsExt->setEnabled(false);
	m_textDetailsExt->setMaximumHeight(24);

	m_labelDetailsExt = new QLabel("Ext: ", m_frameDetails);
	m_labelDetailsExt->setMaximumHeight(24);

	m_labelDetailsPages = new QLabel("Pages: ", m_frameDetails);
	m_labelDetailsPages->setMaximumHeight(24);

	m_labelDetailsGenre = new QLabel("Genre: ", m_frameDetails);
	m_labelDetailsGenre->setMaximumHeight(24);

	m_labelDetailsFolder = new QLabel("Folder: ", m_frameDetails);
	m_labelDetailsFolder->setMaximumHeight(24);

	m_labelDetailsAuthor = new QLabel("Author: ", m_frameDetails);
	m_labelDetailsAuthor->setMaximumHeight(24);

	m_textDetailsName = new QLineEdit(m_frameDetails);
	m_textDetailsName->setMaximumHeight(24);
	m_textDetailsName->setClearButtonEnabled(false);

	m_textDetailsPages = new QLineEdit(m_frameDetails);
	m_textDetailsPages->setMaximumHeight(24);

	m_gridLayDetailsFields->addWidget(m_labelDetailsSize, 2, 2, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsGenre, 2, 1, 1, 1);
	m_gridLayDetailsFields->addWidget(m_frameSizeDetails, 2, 3, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsTags, 3, 3, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsGroups, 3, 2, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsName, 0, 0, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsAuthor, 1, 1, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsFolder, 3, 1, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsExt, 0, 3, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsExt, 0, 2, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsPages, 1, 2, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsGenre, 2, 0, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsFolder, 3, 0, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsAuthor, 1, 0, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsName, 0, 1, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsPages, 1, 3, 1, 1);

	m_gridLayDetailsFields->setRowStretch(0, 1);
	m_gridLayDetailsFields->setRowStretch(1, 1);
	m_gridLayDetailsFields->setRowStretch(2, 1);
	m_gridLayDetailsFields->setRowStretch(3, 1);
	m_gridLayDetailsFields->setRowMinimumHeight(0, 1);
	m_gridLayDetailsFields->setRowMinimumHeight(1, 1);
	m_gridLayDetailsFields->setRowMinimumHeight(2, 1);
	m_gridLayDetailsFields->setRowMinimumHeight(3, 1);

	m_vertLayDetails->addLayout(m_gridLayDetailsFields);

	m_horLayDetailsButtons = new QHBoxLayout();
	m_horSpacerButtonsLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_horSpacerButtonsRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_buttonDetailsRestore = new QPushButton("Restore", m_frameDetails);
	m_buttonDetailsRestore->setMinimumSize(QSize(70, 30));
	m_buttonDetailsRestore->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonDetailsRestore->setFlat(true);

	m_buttonDetailsUpdate = new QPushButton("Update", m_frameDetails);
	m_buttonDetailsUpdate->setMinimumSize(QSize(70, 30));
	m_buttonDetailsUpdate->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonDetailsUpdate->setFlat(true);

	m_buttonDetailsClear = new QPushButton("Clear", m_frameDetails);
	m_buttonDetailsClear->setMinimumSize(QSize(70, 30));
	m_buttonDetailsClear->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonDetailsClear->setFlat(true);

	m_horLayDetailsButtons->addItem(m_horSpacerButtonsLeft);
	m_horLayDetailsButtons->addWidget(m_buttonDetailsRestore);
	m_horLayDetailsButtons->addWidget(m_buttonDetailsUpdate);
	m_horLayDetailsButtons->addWidget(m_buttonDetailsClear);
	m_horLayDetailsButtons->addItem(m_horSpacerButtonsRight);

	m_vertLayDetails->addLayout(m_horLayDetailsButtons);
	m_vertLayDetails->setStretch(1, 1);

	m_gridLayDetails->addLayout(m_vertLayDetails, 0, 0, 1, 1);

	m_vertLayMainRight->addWidget(m_frameDetails);

	m_vertLayMainRight->setStretch(1, 30);
	m_vertLayMainRight->setStretch(2, 21);
	m_splitter->addWidget(m_frameMainRight);
	m_horLayMain->addWidget(m_splitter);
	m_horLayCentral->addWidget(m_frameMain);

	// Actions
	setupActions();

	// Menubar
	m_menubar = new QMenuBar(this);
	m_menubar->setGeometry(QRect(0, 0, 1190, 21));
	setMenuBar(m_menubar);

	// Statusbar
	m_statusBar = new QStatusBar(this);
	setStatusBar(m_statusBar);

	// Menus
	setupMenus();

	// Central Widget
	setCentralWidget(m_centralWidget);

	// Refresh
	refreshFolders();
	refreshAuthors();
	refreshGenres();
	refreshSearches();
}

void MainWindow::setupActions()
{
	// File Menu
	m_actionClose = new QAction("Close", this);
	// --> Goto Menu
	m_actionOpenDB = new QAction("Open Database", this);
	m_actionEbookReportsDir = new QAction("Ebook Reports", this);
	m_actionUsageReportsDir = new QAction("Usage Reports", this);
	m_actionInstallationDir = new QAction("Installation", this);

	// View Menu
	m_actionFullscreen = new QAction("Fullscreen", this);
	m_actionFullscreen->setIcon(QIcon(":/icons/fullscreen_icon.png"));
	m_actionMaximize = new QAction("Maximize", this);
	m_actionMinimize = new QAction("Minimize", this);
	m_actionMinimize->setIcon(QIcon(":/icons/minimize_icon.png"));
	m_actionWindowTop = new QAction("Window on Top", this);
	m_actionMinimizeTray = new QAction("Minimize to Tray", this);
	m_actionHideSearchBar = new QAction("Hide Search Bar", this);
	m_actionHideDetailsSection = new QAction("Hide Details Section", this);
	m_actionHideRightFrame = new QAction("Hide Right Frame", this);
	m_actionHideUtilities = new QAction("Hide Utilities Section", this);
	m_actionHideLeftFrame = new QAction("Hide Left Frame", this);
	m_actionHideStatusBar = new QAction("Hide Status Strip", this);

	// Add Menu
	m_actionAddBook = new QAction("Add Books", this);
	m_actionAddBook->setIcon(QIcon(":/icons/add_book_icon.png"));
	m_actionAddBooks = new QAction("Add Books", this);
	m_actionAddBooks->setIcon(QIcon(":/icons/add_books_icon.png"));

	// Reset Menu
	m_actionResetEbooks = new QAction("Reset Ebooks", this);
	m_actionResetTags = new QAction("Reset Tags", this);
	m_actionResetSearches = new QAction("Reset Searches", this);
	m_actionResetSummaries = new QAction("Reset Summaries", this);
	m_actionResetDatabase = new QAction("Reset Database", this);
	m_actionResetDatabase->setIcon(QIcon(":/icons/reset_database_icon.png"));
	m_actionClearEntries = new QAction("Clear Criteria", this);
	m_actionClearEntries->setIcon(QIcon(":/icons/clear_icon.png"));

	// Clean Menu
	m_actionCleanEbooks = new QAction("Clean Ebooks", this);
	m_actionCleanEbooks->setIcon(QIcon(":/icons/clean_icon.png"));

	// Search Menu
	m_actionSearchFiles = new QAction("Search Criteria", this);
	m_actionSearchFiles->setIcon(QIcon(":/icons/search_icon.png"));
	m_actionSearchText = new QAction("Search Text", this);
	m_actionSearchText->setIcon(QIcon(":/icons/searchbar_icon.png"));
	m_actionSortSearch = new QAction("Sort Entries", this);
	m_actionSortSearch->setIcon(QIcon(":/icons/sort_icon.png"));
	m_actionClearSearch = new QAction("Clear Search", this);
	m_actionClearSearch->setIcon(QIcon(":/icons/clear_search_white_icon.png"));

	// Tools Menu
	m_actionSummaries = new QAction("Summaries", this);
	m_actionSummaries->setIcon(QIcon(":/icons/white_notebook_icon.png"));
	m_actionLinkManager = new QAction("Link Manager", this);
	m_actionLinkManager->setIcon(QIcon(":/icons/link_icon.png"));
	m_actionDataViewer = new QAction("Data Viewer", this);
	m_actionDataViewer->setIcon(QIcon(":/icons/dbviewer_icon.png"));
	m_actionBookDetails = new QAction("MetaData", this);
	m_actionBookDetails->setIcon(QIcon(":/icons/stack_icon.png"));

	// Generate Menu
	m_actionEbooksReport = new QAction("Ebooks Report", this);
	m_actionEbooksReport->setIcon(QIcon(":/icons/ebooks_report_icon.png"));
	m_actionUsageReport = new QAction("Usage Report", this);
	m_actionUsageReport->setIcon(QIcon(":/icons/metrics_icon.png"));
	m_actionChooseRandomBook = new QAction("Random Book", this);

	// Settings Menu
	m_actionApplicationSettings = new QAction("Application Settings", this);
	m_actionApplicationSettings->setIcon(QIcon(":/icons/settings_icon.png"));
}

void MainWindow::setupMenus()
{
	m_menuFile = m_menubar->addMenu(tr("&File"));
	m_menuAdd = m_menubar->addMenu(tr("&Add"));
	m_menuReset = m_menubar->addMenu(tr("&Reset"));
	m_menuClean = m_menubar->addMenu(tr("&Clean"));
	m_menuSearch = m_menubar->addMenu(tr("&Search"));
	m_menuGenerate = m_menubar->addMenu(tr("&Generate"));
	m_menuSettings = m_menubar->addMenu(tr("&Settings"));
	m_menuView = m_menubar->addMenu(tr("&View"));
	m_menuTools = m_menubar->addMenu(tr("&Tools"));

	m_menuGoTo = new QMenu(tr("Go To"), m_menuFile);

	m_menubar->addAction(m_menuFile->menuAction());
	m_menubar->addAction(m_menuView->menuAction());
	m_menubar->addAction(m_menuAdd->menuAction());
	m_menubar->addAction(m_menuReset->menuAction());
	m_menubar->addAction(m_menuClean->menuAction());
	m_menubar->addAction(m_menuSearch->menuAction());
	m_menubar->addAction(m_menuTools->menuAction());
	m_menubar->addAction(m_menuGenerate->menuAction());
	m_menubar->addAction(m_menuSettings->menuAction());
	m_menuFile->addSeparator();
	m_menuFile->addAction(m_menuGoTo->menuAction());
	m_menuFile->addAction(m_actionOpenDB);
	m_menuFile->addSeparator();
	m_menuFile->addAction(m_actionClose);
	m_menuGoTo->addAction(m_actionInstallationDir);
	m_menuGoTo->addAction(m_actionEbookReportsDir);
	m_menuGoTo->addAction(m_actionUsageReportsDir);
	m_menuAdd->addAction(m_actionAddBook);
	m_menuAdd->addAction(m_actionAddBooks);
	m_menuReset->addAction(m_actionResetEbooks);
	m_menuReset->addAction(m_actionResetTags);
	m_menuReset->addAction(m_actionResetSearches);
	m_menuReset->addAction(m_actionResetSummaries);
	m_menuReset->addAction(m_actionResetDatabase);
	m_menuReset->addSeparator();
	m_menuReset->addAction(m_actionClearEntries);
	m_menuClean->addAction(m_actionCleanEbooks);
	m_menuSearch->addAction(m_actionSearchFiles);
	m_menuSearch->addAction(m_actionSearchText);
	m_menuSearch->addSeparator();
	m_menuSearch->addAction(m_actionSortSearch);
	m_menuSearch->addAction(m_actionClearSearch);
	m_menuGenerate->addAction(m_actionEbooksReport);
	m_menuGenerate->addAction(m_actionUsageReport);
	m_menuGenerate->addSeparator();
	m_menuGenerate->addAction(m_actionChooseRandomBook);
	m_menuSettings->addAction(m_actionApplicationSettings);
	m_menuView->addAction(m_actionFullscreen);
	m_menuView->addAction(m_actionMaximize);
	m_menuView->addAction(m_actionMinimize);
	m_menuView->addAction(m_actionWindowTop);
	m_menuView->addAction(m_actionMinimizeTray);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionHideSearchBar);
	m_menuView->addAction(m_actionHideDetailsSection);
	m_menuView->addAction(m_actionHideRightFrame);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionHideUtilities);
	m_menuView->addAction(m_actionHideLeftFrame);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionHideStatusBar);
	m_menuTools->addAction(m_actionSummaries);
	m_menuTools->addAction(m_actionLinkManager);
	m_menuTools->addAction(m_actionDataViewer);
	m_menuTools->addAction(m_actionBookDetails);
}

void MainWindow::setupConnections()
{
	connect(m_ebooksListWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
	connect(m_trayIcon, &QSystemTrayIcon::activated, this, &QMainWindow::show);
	connect(m_comboBoxSearchLoad, &QComboBox::customContextMenuRequested, this, &MainWindow::showSearchBoxContextMenu);

	connect(m_buttonDetailsClear, &QPushButton::clicked, this, &MainWindow::clearDetails);
	connect(m_buttonDetailsRestore, &QPushButton::clicked, this, &MainWindow::restoreDetails);
	connect(m_buttonDetailsUpdate, &QPushButton::clicked, this, &MainWindow::updateDetails);

	connect(m_buttonAddBooks, &QToolButton::clicked, this, &MainWindow::showAddBooksDialog);
	connect(m_buttonSearchString, &QToolButton::clicked, this, &MainWindow::searchString);
	connect(m_buttonClearCriteria, &QToolButton::clicked, this, &MainWindow::clearCriteria);
	connect(m_buttonSortSearch, &QToolButton::clicked, this, &MainWindow::sortSearch);
	connect(m_buttonAddBook, &QToolButton::clicked, this, &MainWindow::showAddBookDialog);
	connect(m_buttonClearSearch, &QToolButton::clicked, this, &MainWindow::clearSearch);
	connect(m_buttonSearchCriteria, &QToolButton::clicked, this, &MainWindow::searchCriteria);
	connect(m_buttonSaveCriteria, &QToolButton::clicked, this, &MainWindow::saveCriteria);
	connect(m_buttonSearchLoad, &QToolButton::clicked, this, &MainWindow::loadSearch);
	connect(m_buttonLinkManager, &QToolButton::clicked, this, &MainWindow::showLinkManager);
	connect(m_buttonDbViewer, &QToolButton::clicked, this, &MainWindow::showDbViewer);
	connect(m_buttonSizeCriteria, &QToolButton::clicked, this, &MainWindow::toggleSizeCriteria);
	connect(m_buttonExtensions, &QToolButton::clicked, this, &MainWindow::selectExtensions);
	connect(m_buttonFolder, &QToolButton::clicked, this, &MainWindow::selectFolders);
	connect(m_buttonAuthor, &QToolButton::clicked, this, &MainWindow::selectAuthors);
	connect(m_buttonGenre, &QToolButton::clicked, this, &MainWindow::selectGenres);
	connect(m_buttonTags, &QToolButton::clicked, this, &MainWindow::selectTags);
	connect(m_buttonSizeUnit, &QToolButton::clicked, this, &MainWindow::toggleSizeUnit);
	connect(m_buttonSummaries, &QToolButton::clicked, [this]
	{ showSummary(""); });

	connect(m_ebooksListWidget, &QListWidget::itemClicked, this, &MainWindow::showEbookDetails);
	connect(m_ebooksListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::openEbook);
	connect(m_ebooksListWidget, &QListWidget::itemActivated, this, &MainWindow::openEbook);
	connect(m_ebooksListWidget, &QListWidget::itemSelectionChanged, [this]
	{
		if (m_ebooksListWidget->count() > 0)
		{
			showEbookDetails(m_ebooksListWidget->currentItem());
		}
	});

	connect(m_comboBoxSearchLoad, &QComboBox::currentTextChanged, this, &MainWindow::loadSearch);
	connect(m_textTagsCriteria, &QLineEdit::returnPressed, this, &MainWindow::searchCriteria);
	connect(m_textExts, &QLineEdit::returnPressed, this, &MainWindow::searchCriteria);
	connect(m_textSearchBar, &QLineEdit::returnPressed, this, &MainWindow::searchString);
	connect(m_textSearchBar, &QLineEdit::textChanged, this, &MainWindow::searchString);

	connect(m_textDetailsName, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
	connect(m_textDetailsAuthor, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
	connect(m_textDetailsGenre, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
	connect(m_textDetailsFolder, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
	connect(m_textDetailsPages, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
	connect(m_textDetailsTags, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);

	connect(m_actionBookDetails, &QAction::triggered, [this]
	{ showBookDetailsWindow(""); });
	connect(m_actionClearEntries, &QAction::triggered, this, &MainWindow::clearCriteria);
	connect(m_actionAddBook, &QAction::triggered, this, &MainWindow::showAddBookDialog);
	connect(m_actionAddBooks, &QAction::triggered, this, &MainWindow::showAddBooksDialog);
	connect(m_actionSearchFiles, &QAction::triggered, this, &MainWindow::searchCriteria);
	connect(m_actionSearchText, &QAction::triggered, this, &MainWindow::searchString);
	connect(m_actionSortSearch, &QAction::triggered, this, &MainWindow::sortSearch);
	connect(m_actionResetEbooks, &QAction::triggered, this, &MainWindow::resetEbooks);
	connect(m_actionHideSearchBar, &QAction::triggered, this, &MainWindow::hideSearchBar);
	connect(m_actionHideDetailsSection, &QAction::triggered, this, &MainWindow::hideDetailsSection);
	connect(m_actionHideRightFrame, &QAction::triggered, this, &MainWindow::hideRightFrame);
	connect(m_actionHideUtilities, &QAction::triggered, this, &MainWindow::hideUtilities);
	connect(m_actionHideLeftFrame, &QAction::triggered, this, &MainWindow::hideLeftFrame);
	connect(m_actionCleanEbooks, &QAction::triggered, this, &MainWindow::showCleanEbooksDialog);
	connect(m_actionChooseRandomBook, &QAction::triggered, this, &MainWindow::chooseRandomBook);
	connect(m_actionFullscreen, &QAction::triggered, [this]
	{ common::toggleFullscreen(this); });
	connect(m_actionResetSummaries, &QAction::triggered, this, &MainWindow::resetSummaries);
	connect(m_actionResetTags, &QAction::triggered, this, &MainWindow::resetTags);
	connect(m_actionResetSearches, &QAction::triggered, this, &MainWindow::resetSearches);
	connect(m_actionResetDatabase, &QAction::triggered, this, &MainWindow::resetDb);
	connect(m_actionWindowTop, &QAction::triggered, this, &MainWindow::toggleWindowOnTop);
	connect(m_actionLinkManager, &QAction::triggered, this, &MainWindow::showLinkManager);
	connect(m_actionDataViewer, &QAction::triggered, this, &MainWindow::showDbViewer);
	connect(m_actionApplicationSettings, &QAction::triggered, this, &MainWindow::showSettings);
	connect(m_actionEbooksReport, &QAction::triggered, this, &MainWindow::genEbooksReport);
	connect(m_actionHideStatusBar, &QAction::triggered, this, &MainWindow::hideStatusBar);
	connect(m_actionClearSearch, &QAction::triggered, this, &MainWindow::clearSearch);
	connect(m_actionMinimize, &QAction::triggered, this, &MainWindow::showMinimized);
	connect(m_actionMaximize, &QAction::triggered, [this]
	{ common::toggleMaximized(this); });
	connect(m_actionClose, &QAction::triggered, this, &MainWindow::close);
	connect(m_actionSummaries, &QAction::triggered, [this]
	{ showSummary(""); });
	connect(m_actionMinimizeTray, &QAction::triggered, [this]
	{
		QTimer::singleShot(0, this, SLOT(hide()));
	});
	connect(m_actionOpenDB, &QAction::triggered, [this]
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile("./database.db"));
	});
	connect(m_actionEbookReportsDir, &QAction::triggered, [this]
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile("./reports/ebooks/"));
	});
	connect(m_actionUsageReportsDir, &QAction::triggered, [this]
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile("./reports/usage/"));
	});
	connect(m_actionInstallationDir, &QAction::triggered, [this]
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile("./"));
	});
}

void MainWindow::setupTabOrder()
{
	QWidget::setTabOrder(m_textSearchBar, m_ebooksListWidget);
	QWidget::setTabOrder(m_ebooksListWidget, m_textDetailsName);
	QWidget::setTabOrder(m_textDetailsName, m_textDetailsAuthor);
	QWidget::setTabOrder(m_textDetailsAuthor, m_textDetailsGenre);
	QWidget::setTabOrder(m_textDetailsGenre, m_textDetailsFolder);
	QWidget::setTabOrder(m_textDetailsFolder, m_textDetailsExt);
	QWidget::setTabOrder(m_textDetailsExt, m_textDetailsPages);
	QWidget::setTabOrder(m_textDetailsPages, m_textDetailsSize);
	QWidget::setTabOrder(m_textDetailsSize, m_buttonSizeUnit);
	QWidget::setTabOrder(m_buttonSizeUnit, m_textDetailsTags);
	QWidget::setTabOrder(m_textDetailsTags, m_buttonDetailsUpdate);
	QWidget::setTabOrder(m_buttonDetailsUpdate, m_buttonDetailsRestore);
	QWidget::setTabOrder(m_buttonDetailsRestore, m_buttonDetailsClear);
	QWidget::setTabOrder(m_buttonDetailsClear, m_comboBoxFolderCriteria);
	QWidget::setTabOrder(m_comboBoxFolderCriteria, m_buttonFolder);
	QWidget::setTabOrder(m_buttonFolder, m_comboBoxAuthorCriteria);
	QWidget::setTabOrder(m_comboBoxAuthorCriteria, m_buttonAuthor);
	QWidget::setTabOrder(m_buttonAuthor, m_comboBoxGenreCriteria);
	QWidget::setTabOrder(m_comboBoxGenreCriteria, m_buttonGenre);
	QWidget::setTabOrder(m_buttonGenre, m_textTagsCriteria);
	QWidget::setTabOrder(m_textTagsCriteria, m_buttonTags);
	QWidget::setTabOrder(m_buttonTags, m_textExts);
	QWidget::setTabOrder(m_textExts, m_buttonExtensions);
	QWidget::setTabOrder(m_buttonExtensions, m_spinBoxFromSizeCriteria);
	QWidget::setTabOrder(m_spinBoxFromSizeCriteria, m_spinBoxToSizeCriteria);
	QWidget::setTabOrder(m_spinBoxToSizeCriteria, m_buttonSizeCriteria);
	QWidget::setTabOrder(m_buttonSizeCriteria, m_spinBoxFromPagesCriteria);
	QWidget::setTabOrder(m_spinBoxFromPagesCriteria, m_spinBoxToPagesCriteria);
	QWidget::setTabOrder(m_spinBoxToPagesCriteria, m_buttonSearchCriteria);
	QWidget::setTabOrder(m_buttonSearchCriteria, m_buttonClearCriteria);
	QWidget::setTabOrder(m_buttonClearCriteria, m_buttonSaveCriteria);
	QWidget::setTabOrder(m_buttonSaveCriteria, m_comboBoxSearchLoad);
	QWidget::setTabOrder(m_comboBoxSearchLoad, m_buttonSearchLoad);
	QWidget::setTabOrder(m_buttonSearchLoad, m_buttonAddBook);
	QWidget::setTabOrder(m_buttonAddBook, m_buttonAddBooks);
	QWidget::setTabOrder(m_buttonAddBooks, m_buttonSummaries);
	QWidget::setTabOrder(m_buttonSummaries, m_buttonDbViewer);
	QWidget::setTabOrder(m_buttonDbViewer, m_buttonLinkManager);
}

void MainWindow::setupShortcuts()
{
	// File Menu
	m_actionClose->setShortcut(QKeySequence("Ctrl+Q"));

	// View Menu
	m_actionFullscreen->setShortcut(QKeySequence("Ctrl+F, Ctrl+S"));
	m_actionMaximize->setShortcut(QKeySequence("Ctrl+M, Ctrl+X"));
	m_actionMinimize->setShortcut(QKeySequence("Ctrl+M, Ctrl+N"));
	m_actionHideSearchBar->setShortcut(QKeySequence("Ctrl+S, Ctrl+B"));
	m_actionHideDetailsSection->setShortcut(QKeySequence("Ctrl+D, Ctrl+S"));
	m_actionHideRightFrame->setShortcut(QKeySequence("Ctrl+R, Ctrl+F"));
	m_actionHideUtilities->setShortcut(QKeySequence("Ctrl+U, Ctrl+S"));
	m_actionHideLeftFrame->setShortcut(QKeySequence("Ctrl+L, Ctrl+F"));
	m_actionHideStatusBar->setShortcut(QKeySequence("Ctrl+S, Ctrl+S"));

	// Add Menu
	m_actionAddBook->setShortcut(QKeySequence("Ctrl+A, Ctrl+B"));
	m_actionAddBooks->setShortcut(QKeySequence("Ctrl+A, Ctrl+M"));

	// Reset Menu
	m_actionClearEntries->setShortcut(QKeySequence("Ctrl+Shift+Del"));

	// Search Menu
	m_actionSearchText->setShortcut(QKeySequence("Ctrl+S, Ctrl+T"));
	m_actionSearchFiles->setShortcut(QKeySequence("Ctrl+S, Ctrl+C"));

	m_actionSortSearch->setShortcut(QKeySequence("Ctrl+S, Ctrl+E"));
	m_actionClearSearch->setShortcut(QKeySequence("Shift+Alt+Del"));

	// Tools Menu
	m_actionSummaries->setShortcut(QKeySequence("Ctrl+O, Ctrl+S"));
	m_actionLinkManager->setShortcut(QKeySequence("Ctrl+O, Ctrl+L"));
	m_actionDataViewer->setShortcut(QKeySequence("Ctrl+O, Ctrl+D"));
	m_actionBookDetails->setShortcut(QKeySequence("Ctrl+O, Ctrl+M"));

	// Generate Menu
	m_actionEbooksReport->setShortcut(QKeySequence("Ctrl+E, Ctrl+R"));
	m_actionUsageReport->setShortcut(QKeySequence("Ctrl+U, Ctrl+R"));
	m_actionChooseRandomBook->setShortcut(QKeySequence("Ctrl+Shift+R"));

	// Settings Menu
	m_actionApplicationSettings->setShortcut(QKeySequence("Ctrl+A, Ctrl+S"));
}

void MainWindow::showQuote()
{
	QuoteDialog dialog(this);
	common::openDialog(&dialog, ":styles/quote.qss");
}

void MainWindow::trayClicked(QSystemTrayIcon::ActivationReason r)
{
	if (r == QSystemTrayIcon::Trigger)
	{
		show();
	}
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	queries::logSessionEnd();
	queries::query.clear();
	queries::db.close();
	event->accept();
}

void MainWindow::showBookDetailsWindow(const QString& name)
{
	BookMetadataWindow* bookDetailsWindow = new BookMetadataWindow();
	common::openWindow(bookDetailsWindow, ":/styles/style.qss");
	bookDetailsWindow->showBookDetails(name);
	bookDetailsWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showSummary(const QString& name)
{
	SummaryWindow* summaryWindow = new SummaryWindow();
	common::openWindow(summaryWindow, ":/styles/summarystyle.qss");
	summaryWindow->selectEbookSummary(name);
	summaryWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showLinkManager()
{
	LinkManagerWindow* linkManagerWindow = new LinkManagerWindow();
	common::openWindow(linkManagerWindow, ":/styles/style.qss");
	linkManagerWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showDbViewer()
{
	DataViewerWindow* dataViewerWindow = new DataViewerWindow();
	common::openWindow(dataViewerWindow, ":/styles/style.qss");
	dataViewerWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showSettings()
{
//    SettingsWindow *settingsWindow = new SettingsWindow();
//    common::openWindow(settingsWindow, ":/styles/style.qss");
//    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::refreshComboBox(QComboBox* comboBox)
{
	comboBox->clear();
	comboBox->addItem("");
	while (queries::query.next())
	{
		QString value = queries::query.value(0).toString();
		if (value.isEmpty())
		{
			continue;
		}
		comboBox->addItem(value);
	}

	comboBox->model()->sort(0, Qt::AscendingOrder);
}

void MainWindow::refreshFolders()
{
	queries::selectFoldersQuery();
	refreshComboBox(m_comboBoxFolderCriteria);
}

void MainWindow::refreshAuthors()
{
	queries::selectAuthorsQuery();
	refreshComboBox(m_comboBoxAuthorCriteria);
}

void MainWindow::refreshGenres()
{
	queries::selectGenreQuery();
	refreshComboBox(m_comboBoxGenreCriteria);
}

void MainWindow::refreshSearches()
{
	queries::selectSearchesQuery();
	refreshComboBox(m_comboBoxSearchLoad);
}

double MainWindow::changeBookSizeUnit(double size, const QString& unit)
{
	size /= m_sizeConvFactors[unit];
	return size;
}

void MainWindow::showContextMenu(const QPoint& pos)
{
	QPoint globalPos = m_ebooksListWidget->mapToGlobal(pos);

	QMenu menu;
	menu.setStyleSheet(common::openSheet(":/styles/style.qss"));

	menu.addAction("Rename", this, [this]
	{
		editListItem(m_ebooksListWidget->currentItem());
	});
	menu.addAction("Open Ebook", this, [this]
	{
		openEbook(m_ebooksListWidget->currentItem());
	});
	menu.addAction("Open Summary", this, SLOT(openSummaryWindow()));
	menu.addAction("Open MetaData", this, [this]
	{
		showBookDetailsWindow(m_ebooksListWidget->currentItem()->text());
	});
	menu.addAction("Show in Folder", this, SLOT(openFolder()));
	menu.addAction("Delete Ebook", this, SLOT(deleteListItem()));

	if (!m_ebooksListWidget->selectedItems().isEmpty())
	{
		menu.exec(globalPos);
	}
}

void MainWindow::showSearchBoxContextMenu(const QPoint& pos)
{
	QPoint globalPos = m_comboBoxSearchLoad->mapToGlobal(pos);

	QMenu menu;
	menu.setStyleSheet(common::openSheet(":/styles/style.qss"));

	menu.addAction("Delete Search", this, [this]
	{
		QString item = m_comboBoxSearchLoad->currentText();
		queries::deleteSearch(item);
		refreshSearches();
	});

	menu.exec(globalPos);
}

void MainWindow::openSummaryWindow()
{
	showSummary(m_ebooksListWidget->currentItem()->text());
}

void MainWindow::deleteListItem()
{
	QString itemName = m_ebooksListWidget->currentItem()->text();
	QListWidgetItem* currentItem = m_ebooksListWidget->currentItem();

	YesNoDialog dialog(this, "Delete File", "Delete File",
			"Do you wish to delete the file on your hard drive as well?");
	common::openDialog(&dialog, ":/styles/style.qss");
	bool result = dialog.getResult();

	if (result)
	{
		queries::selectPathBasedonName(itemName);
		queries::query.next();
		QString path = queries::query.value(0).toString();
		QFile file(path);
		file.remove();
	}

	queries::deleteBook(itemName);
	delete (currentItem);

	m_statusBar->showMessage(itemName + " deleted.");
}

void MainWindow::openFolder()
{
	QString fileName = m_ebooksListWidget->currentItem()->text();
	queries::selectPathBasedonName(fileName);
	queries::query.next();
	QString filePath = queries::query.value(0).toString();
	QFileInfo file(filePath);

	if (QOperatingSystemVersion::current().type() == 1)
	{
		QStringList args;
		args << "/select," << QDir::toNativeSeparators(queries::query.value(0).toString());
		QProcess* process = new QProcess(this);
		process->start("explorer.exe", args);
	}
	else
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile((file.dir().path())));
	}
}

void MainWindow::showAddBooksDialog()
{
	AddBooksDialog dialog(this);
	common::openDialog(&dialog, ":/styles/style.qss");

	refreshFolders();
	refreshAuthors();
	refreshGenres();
}

void MainWindow::searchString()
{
	m_ebooksListWidget->clear();
	QString stringToSearch = m_textSearchBar->text();
	queries::selectNameBasedOnString(stringToSearch);
	quint32 count = 0;
	while (queries::query.next())
	{
		QListWidgetItem* item = new QListWidgetItem(m_ebooksListWidget);
		item->setText(queries::query.value(0).toString());
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		m_ebooksListWidget->addItem(item);
		count++;
	}

	m_statusBar->showMessage("Number of ebooks: " + QString::number(count));
}

void MainWindow::clearCriteria()
{
	m_spinBoxFromSizeCriteria->setValue(0);
	m_spinBoxFromPagesCriteria->setValue(0);
	m_spinBoxToSizeCriteria->setValue(1024);
	m_spinBoxToPagesCriteria->setValue(100000);
	m_comboBoxGenreCriteria->setCurrentIndex(0);
	m_comboBoxAuthorCriteria->setCurrentIndex(0);
	m_comboBoxFolderCriteria->setCurrentIndex(0);
	m_textExts->clear();
	m_textTagsCriteria->clear();
	m_ebooksListWidget->clear();
	m_statusBar->clearMessage();
}

void MainWindow::clearDetails()
{
	m_textDetailsAuthor->clear();
	m_textDetailsName->clear();
	m_textDetailsGenre->clear();
	m_textDetailsTags->clear();
	m_textDetailsExt->clear();
	m_textDetailsSize->clear();
	m_textDetailsFolder->clear();
	m_textDetailsPages->clear();
	m_statusBar->clearMessage();
}

void MainWindow::sortSearch()
{
	m_SORT = (m_SORT == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
	m_ebooksListWidget->sortItems(m_SORT);
}

void MainWindow::resetEbooks()
{
	queries::resetEbooksTableQuery();
	refreshFolders();
	refreshAuthors();
	refreshGenres();
	m_ebooksListWidget->clear();
	m_statusBar->showMessage("Ebooks, summaries, and tags have been deleted.");
}

void MainWindow::hideSearchBar()
{
	common::changeWidgetVisibility(m_frameSearchToolBar, m_actionHideSearchBar);
}

void MainWindow::hideDetailsSection()
{
	m_frameDetails->setHidden(!m_frameDetails->isHidden());
	m_actionHideDetailsSection->setText(m_frameDetails->isHidden() ? "Show Details Section" : "Hide Details Section");
	m_frameMainRight->setHidden(m_ebooksListWidget->isHidden() && m_frameDetails->isHidden());
}

void MainWindow::hideRightFrame()
{
	common::changeWidgetVisibility(m_frameMainRight, m_actionHideRightFrame);
}

void MainWindow::hideUtilities()
{
	common::changeWidgetVisibility(m_frameBottomButtons, m_actionHideUtilities);
}

void MainWindow::hideLeftFrame()
{
	common::changeWidgetVisibility(m_frameMainLeft, m_actionHideLeftFrame);
}

void MainWindow::showAddBookDialog()
{
	AddBookDialog dialog(this);
	common::openDialog(&dialog, ":/styles/style.qss");
	refreshFolders();
	refreshAuthors();
	refreshGenres();
}

void MainWindow::clearSearch()
{
	m_textSearchBar->clear();
	m_ebooksListWidget->clear();
	m_textDetailsName->clear();
	m_textDetailsFolder->clear();
	m_textDetailsGenre->clear();
	m_textDetailsAuthor->clear();
	m_textDetailsExt->clear();
	m_textDetailsPages->clear();
	m_textDetailsSize->clear();
	m_textDetailsTags->clear();
	m_statusBar->clearMessage();
}

void MainWindow::searchCriteria()
{
	m_ebooksListWidget->clear();
	QString folder = m_comboBoxFolderCriteria->currentText();
	QString author = m_comboBoxAuthorCriteria->currentText();
	QString genre = m_comboBoxGenreCriteria->currentText();
	QString tags = m_textTagsCriteria->text();
	quint64 sizeTo = m_spinBoxToSizeCriteria->value();
	quint64 sizeFrom = m_spinBoxFromSizeCriteria->value();
	QString convUnit = m_buttonSizeCriteria->text();
	sizeTo = sizeTo * m_sizeConvFactors[convUnit];
	sizeFrom = sizeFrom * m_sizeConvFactors[convUnit];
	quint32 pagesTo = m_spinBoxToPagesCriteria->value();
	quint32 pagesFrom = m_spinBoxFromPagesCriteria->value();
	QString ext = m_textExts->text();
	queries::selectNameBasedOnCriteria(folder, genre, author, tags, ext, pagesFrom, pagesTo, sizeFrom, sizeTo);
	quint32 count = 0;
	while (queries::query.next())
	{
		m_ebooksListWidget->addItem(queries::query.value(0).toString());
		count++;
	}

	m_statusBar->showMessage("Number of ebooks: " + QString::number(count));
}

void MainWindow::showEbookDetails(QListWidgetItem* item)
{
	QString fileName = item->text();
	queries::selectAllBasedonName(fileName);
	queries::query.next();
	QString author = queries::query.value("author").toString();
	author = (author == "N/A" ? "" : author);
	QString genre = queries::query.value("genre").toString();
	genre = (genre == "N/A" ? "" : genre);
	QString ext = queries::query.value("ext").toString();
	QString pages = queries::query.value("pages").toString();
	double size = changeBookSizeUnit(queries::query.value("size").toDouble(), m_buttonSizeUnit->text());
	QString folder = queries::query.value("folder").toString();
	QString tags = queries::selectTagsBasedOnName(fileName);

	m_textDetailsName->setText(fileName);
	m_textDetailsName->setCursorPosition(0);
	m_textDetailsAuthor->setText(author);
	m_textDetailsGenre->setText(genre);
	m_textDetailsPages->setText(pages);
	m_textDetailsExt->setText(ext);
	m_textDetailsSize->setText(QString::number(size));
	m_textDetailsFolder->setText(folder);
	m_textDetailsFolder->setCursorPosition(0);
	m_textDetailsTags->setText(tags);
	m_textDetailsTags->setCursorPosition(0);
	m_statusBar->showMessage("Current selected ebook: " + fileName);
}

void MainWindow::restoreDetails()
{
	if (!m_ebooksListWidget->selectedItems().isEmpty())
	{
		showEbookDetails(m_ebooksListWidget->currentItem());
		m_statusBar->showMessage("Details restored successfully.");
	}
}

void MainWindow::updateDetails()
{
	if (!m_ebooksListWidget->selectedItems().isEmpty())
	{
		QString oldName = m_ebooksListWidget->currentItem()->text();
		QString newName = m_textDetailsName->text();
		QString author = m_textDetailsAuthor->text();
		QString folder = m_textDetailsFolder->text();
		QString genre = m_textDetailsGenre->text();
		QString tags = m_textDetailsTags->text().trimmed();
		quint32 pages = m_textDetailsPages->text().toUInt();

		queries::selectPathBasedonName(oldName);
		queries::query.next();
		QString path = queries::query.value(0).toString();

		if (newName != oldName)
		{
			YesNoDialog dialog(this, "Rename File", "Rename File",
					"Do you wish to rename the file on your hard drive as well?");
			common::openDialog(&dialog, ":/styles/style.qss");
			bool result = dialog.getResult();
			if (result)
			{
				QFile file(path);
				QFileInfo info(file);
				path = info.absolutePath() + "/" + newName + "." + info.suffix();
				file.rename(path);
			}
		}
		queries::updateBookQuery(oldName, newName, folder, genre, author, pages, tags, path);
		m_ebooksListWidget->currentItem()->setText(newName);

		refreshAuthors();
		refreshFolders();
		refreshGenres();
		m_statusBar->showMessage("Details updated successfully.");
	}
}

void MainWindow::saveCriteria()
{
	QString folder = m_comboBoxFolderCriteria->currentText();
	QString author = m_comboBoxAuthorCriteria->currentText();
	QString genre = m_comboBoxGenreCriteria->currentText();
	QString tags = m_textTagsCriteria->text();
	QString ext = m_textExts->text();
	quint32 sizeTo = m_spinBoxToSizeCriteria->value();
	quint32 sizeFrom = m_spinBoxFromSizeCriteria->value();
	QString sizeIn = m_buttonSizeCriteria->text();
	quint32 pagesTo = m_spinBoxToPagesCriteria->value();
	quint32 pagesFrom = m_spinBoxFromPagesCriteria->value();

	GetNameDialog dialog(this, "Search Name", "Please provide a name for the search you want to save:");
	common::openDialog(&dialog, ":/styles/style.qss");
	if (!dialog.m_name.isEmpty())
	{
		queries::insertSearchQuery(dialog.m_name, folder, author, genre, tags, ext,
				sizeFrom, sizeTo, sizeIn, pagesFrom, pagesTo);
		m_statusBar->showMessage("Search saved successfully.");
	}

	refreshSearches();
}

void MainWindow::loadSearch()
{
	if (m_comboBoxSearchLoad->currentText().isEmpty())
	{
		clearCriteria();
		return;
	}

	QString searchName = m_comboBoxSearchLoad->currentText();
	queries::selectSearchCriteriaQuery(searchName);
	queries::query.next();
	m_comboBoxFolderCriteria->setCurrentText(queries::query.value("folder").toString());
	m_comboBoxAuthorCriteria->setCurrentText(queries::query.value("author").toString());
	m_comboBoxGenreCriteria->setCurrentText(queries::query.value("genre").toString());
	m_textTagsCriteria->setText(queries::query.value("tags").toString());
	m_textExts->setText(queries::query.value("ext").toString());
	m_spinBoxFromSizeCriteria->setValue(queries::query.value("size_from").toUInt());
	m_spinBoxToSizeCriteria->setValue(queries::query.value("size_to").toUInt());
	m_buttonSizeCriteria->setText(queries::query.value("size_in").toString());
	m_spinBoxFromPagesCriteria->setValue(queries::query.value("pages_from").toUInt());
	m_spinBoxToPagesCriteria->setValue(queries::query.value("pages_to").toUInt());

	m_statusBar->showMessage("Search loaded.");
}

void MainWindow::showCleanEbooksDialog()
{
	CleanBooksDialog dialog(this);
	common::openDialog(&dialog, ":/styles/style.qss");
}

void MainWindow::chooseRandomBook()
{
	queries::selectCountEbooks();
	queries::query.next();
	quint32 numberOfEbooks = queries::query.value(0).toUInt();
	if (numberOfEbooks != 0)
	{
		quint32 randomNumber = QRandomGenerator::global()->bounded(numberOfEbooks);
		queries::selectNameBasedOnRowid(randomNumber);
		queries::query.next();
		QString randomName = queries::query.value(0).toString();
		m_textSearchBar->setText(randomName);
		m_ebooksListWidget->setCurrentRow(0);
		m_statusBar->showMessage("Random Book: " + randomName);
	}
}

void MainWindow::openEbook(QListWidgetItem* item)
{
	QString ebookName = item->text();
	queries::selectPathBasedonName(ebookName);
	queries::query.next();
	QString path = queries::query.value(0).toString();
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
	m_statusBar->showMessage(ebookName + " ebook opened.");
}

void MainWindow::toggleSizeCriteria()
{
	QString currentText = m_buttonSizeCriteria->text();
	m_buttonSizeCriteria->setText(m_sizeUnits[currentText]);
}

void MainWindow::extSelectionSetup(const QString& title, const QString& prompt, QWidget* widget)
{
	QVector<QString> results;
	while (queries::query.next())
	{
		QString value = queries::query.value(0).toString();
		if (!results.contains(value))
		{
			results.push_back(value);
		}
	}

	ExtSelectionDialog* dialog = new ExtSelectionDialog(this, results, title, prompt);
	common::openDialog(dialog, ":/styles/style.qss");

	results = dialog->getExtVector();
	QString resultString = results.join(common::SEP);

	if (QComboBox* cb = qobject_cast<QComboBox*>(widget))
	{
		cb->setCurrentText(resultString);
	}
	else if (QLineEdit* le = qobject_cast<QLineEdit*>(widget))
	{
		le->setText(resultString);
	}
}

void MainWindow::selectExtensions()
{
	queries::selectExt();
	extSelectionSetup("Extensions", "Select Available Extensions", m_textExts);
}

void MainWindow::selectFolders()
{
	queries::selectFoldersQuery();
	extSelectionSetup("Folders", "Select Available Folders", m_comboBoxFolderCriteria);
}

void MainWindow::selectAuthors()
{
	queries::selectAuthorsQuery();
	extSelectionSetup("Authors", "Select Available Authors", m_comboBoxAuthorCriteria);
}

void MainWindow::selectGenres()
{
	queries::selectGenreQuery();
	extSelectionSetup("Genres", "Select Available Genres", m_comboBoxGenreCriteria);
}

void MainWindow::selectTags()
{
	QVector<QString> tags;
	queries::selectTags();
	while (queries::query.next())
	{
		QString tagString = queries::query.value(0).toString();
		QStringList tagList = tagString.split(common::SEP);
		for (QString& tag: tagList)
		{
			if (!tags.contains(tag))
			{
				tags.push_back(tag);
			}
		}
	}

	ExtSelectionDialog* dialog = new ExtSelectionDialog(this, tags, "Tags", "Select Available Tags");
	common::openDialog(dialog, ":/styles/style.qss");

	tags = dialog->getExtVector();
	QString tagString = tags.join(common::SEP);
	m_textTagsCriteria->setText(tagString);
}

void MainWindow::toggleSizeUnit()
{
	QString currentText = m_buttonSizeUnit->text();
	m_buttonSizeUnit->setText(m_sizeUnits[currentText]);
	if (!m_ebooksListWidget->selectedItems().isEmpty())
	{
		showEbookDetails(m_ebooksListWidget->currentItem());
	}
}

void MainWindow::resetSummaries()
{
	queries::resetSummaries();
	m_statusBar->showMessage("All summaries have been deleted.");
}

void MainWindow::resetTags()
{
	queries::resetTags();
	m_statusBar->showMessage("All tags have been deleted.");

}

void MainWindow::resetSearches()
{
	queries::resetSearchesTable();
	refreshSearches();
	m_statusBar->showMessage("All searches have been deleted.");
}

void MainWindow::resetDb()
{
	resetEbooks();
	resetSearches();
	m_statusBar->showMessage("All data has been deleted.");
}

void MainWindow::toggleWindowOnTop()
{
	Qt::WindowFlags flags = windowFlags();
	flags ^= Qt::WindowStaysOnTopHint;
	setWindowFlags(flags);
	show();
}

void MainWindow::genEbooksReport()
{
	QString fileName = QDateTime::currentDateTime().toString("yyyy.MM.dd hh.mm.ss") + ".html";
	QFile file = QFile("./reports/ebooks/" + fileName);
	file.open(QIODevice::WriteOnly);
	file.write(
			R"(<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css" integrity="sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm" crossorigin="anonymous">)");
	file.write(
			R"(<table style="table-layout:fixed; width: 100vw; height: 100vh;" id="table" class="table table-responsive table-striped table-bordered table-hover table-sm table-dark">)");
	file.write(R"(  <thead class="bg-primary">)");
	file.write("    <tr>");
	file.write("        <th>Name</th>");
	file.write("        <th>Author</th>");
	file.write("        <th>Genre</th>");
	file.write("        <th>Path</th>");
	file.write("        <th>Extension</th>");
	file.write("        <th>Pages</th>");
	file.write("        <th>Folder</th>");
	file.write("        <th>Tags</th>");
	file.write("        <th>Timestamp</th>");
	file.write("    </tr>");
	file.write("  </thead>");
	file.write("  <tbody>");

	queries::query.exec("SELECT name, author, genre, path, ext, pages, folder, tags, time_added FROM ebooks");
	qint32 fieldCount = queries::query.record().count();
	while (queries::query.next())
	{
		file.write(R"(<tr style="overflow: hidden; text-overflow: ellipsis; word-wrap: break-word;">)");
		for (int i = 0; i < fieldCount; i++)
		{
			file.write(R"(<td style="overflow: hidden; text-overflow: ellipsis; word-wrap: break-word;">)");
			file.write(queries::query.value(i).toString().toStdString().c_str());
			file.write("</td>");
		}
		file.write("</tr>");
	}
	file.write("  </tbody>");
	file.write("</table>");
	file.close();
	QDesktopServices::openUrl(QUrl::fromLocalFile("./reports/ebooks/" + fileName));
}

void MainWindow::hideStatusBar()
{
	common::changeWidgetVisibility(m_statusBar, m_actionHideStatusBar);
}

void MainWindow::editListItem(QListWidgetItem* item)
{
	QString oldName = item->text();
	m_ebooksListWidget->editItem(item);
	QEventLoop loop;
	connect(m_ebooksListWidget->itemDelegate(), &QAbstractItemDelegate::closeEditor, &loop, &QEventLoop::quit);
	loop.exec();
	QString newName = item->text();
	if (newName == oldName)
	{
		return;
	}
	queries::selectPathBasedonName(oldName);
	queries::query.next();
	QString oldPath = queries::query.value(0).toString();
	YesNoDialog dialog(this, "Rename File", "Rename File",
			"Do you wish to rename the file on your hard drive as well?");
	common::openDialog(&dialog, ":/styles/style.qss");
	bool result = dialog.getResult();
	if (result)
	{
		QFile file(oldPath);
		QFileInfo info(file);
		QString newPath = info.absolutePath() + "/" + newName + "." + info.suffix();
		file.rename(newPath);
		queries::updateBookPath(oldPath, newPath);
	}
	queries::updateBookName(oldName, newName);
	m_textDetailsName->setText(newName);
}
