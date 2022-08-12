#include "include/common.h"
#include "include/queries.h"
#include "include/windows/mainwindow.h"
#include "include/dialogs/quotedialog.h"
#include "include/dialogs/yesnodialog.h"
#include "include/windows/summarywindow.h"
#include "include/dialogs/getnamedialog.h"
#include "include/dialogs/addbookdialog.h"
#include "include/dialogs/addbooksdialog.h"
#include "include/windows/dataviewerwindow.h"
#include "include/windows/bookmetadatawindow.h"
#include "include/dialogs/cleanbooksdialog.h"
#include "include/windows/linkmanagerwindow.h"
#include "include/dialogs/extselectiondialog.h"

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
	m_trayIcon->setIcon(QIcon(":/icons/icon.ico"));
	m_trayIcon->setVisible(true);
	m_trayIcon->setToolTip("Ebook Access\nClick to Open");
	m_trayIcon->show();

	QTimer::singleShot(1500, this, SLOT(showQuote()));

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
	m_splitter->setStyleSheet(QString("QFrame#splitter {border:0px;}"
									  "QSplitter::handle { background-color: #2D2D30;}"));
	m_splitter->setLineWidth(0);
	m_splitter->setOrientation(Qt::Horizontal);
	m_splitter->setHandleWidth(2);

	m_frameMainLeft = new QFrame(m_splitter);
	m_frameMainLeft->setObjectName("frameMainLeft");
	m_frameMainLeft->setStyleSheet(QString("QFrame#frameMainLeft { border: 0px;}"));

	m_vertLayMainLeft = new QVBoxLayout(m_frameMainLeft);
	m_vertLayMainLeft->setSizeConstraint(QLayout::SetDefaultConstraint);
	m_vertLayMainLeft->setContentsMargins(-1, 32, -1, 0);

	m_frameSearchCriteria = new QFrame(m_frameMainLeft);
	m_frameSearchCriteria->setStyleSheet(QString(""));
	m_frameSearchCriteria->setFrameShape(QFrame::StyledPanel);
	m_frameSearchCriteria->setFrameShadow(QFrame::Raised);

	m_horLayCriteria = new QHBoxLayout(m_frameSearchCriteria);
	m_horLayCriteria->setSizeConstraint(QLayout::SetDefaultConstraint);
	m_horLayCriteria->setContentsMargins(-1, 9, -1, 9);

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

	m_comboBoxExtensionsCriteria = new QComboBox(m_frameSearchCriteria);
	m_comboBoxExtensionsCriteria->setMinimumSize(QSize(0, 23));
	m_comboBoxExtensionsCriteria->setEditable(true);

	m_spinBoxFromSizeCriteria = new QSpinBox(m_frameSearchCriteria);
	m_spinBoxFromSizeCriteria->setMaximumSize(QSize(70, 16777215));
	m_spinBoxFromSizeCriteria->setMaximum(1023);

	m_labelTagsCriteria = new QLabel("Tags", m_frameSearchCriteria);

	m_labelFolderCriteria = new QLabel("Folder", m_frameSearchCriteria);

	m_comboBoxSearchLoad = new QComboBox(m_frameSearchCriteria);
	m_comboBoxSearchLoad->setContextMenuPolicy(Qt::CustomContextMenu);
	m_comboBoxSearchLoad->addItem(QString());
	m_comboBoxSearchLoad->setMinimumSize(QSize(0, 23));
	m_comboBoxSearchLoad->setEditable(false);

	m_comboBoxTagsCriteria = new QComboBox(m_frameSearchCriteria);
	m_comboBoxTagsCriteria->setMinimumSize(QSize(0, 23));
	m_comboBoxTagsCriteria->setEditable(true);

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

	m_vertLaySearchCriteria = new QVBoxLayout();
	m_vertLaySearchCriteria->setSpacing(25);
	m_vertLaySearchCriteria->setContentsMargins(0, 0, 0, 20);

	m_vertSpacerSearchCriteriaTop = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

	m_scrollAreaCriteria = new QScrollArea(m_frameSearchCriteria);
	m_scrollAreaCriteria->setObjectName("m_scrollAreaCriteria");
	m_scrollAreaCriteria->setStyleSheet("QFrame#m_scrollAreaCriteria { border: none; }");
	m_scrollAreaCriteria->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_frameGridLayCriteria = new QFrame(m_scrollAreaCriteria);
	m_frameGridLayCriteria->setObjectName("m_frameGridLayCriteria");
	m_frameGridLayCriteria->setStyleSheet("QFrame#m_frameGridLayCriteria { border: none; }");

	m_labelSeriesCriteria = new QLabel("Series", m_frameSearchCriteria);

	m_comboBoxSeriesCriteria = new QComboBox(m_frameSearchCriteria);
	m_comboBoxSeriesCriteria->setMinimumSize(QSize(0, 23));
	m_comboBoxSeriesCriteria->setEditable(true);

	m_labelPublisherCriteria = new QLabel("Publisher", m_frameSearchCriteria);

	m_comboBoxPublisherCriteria = new QComboBox(m_frameSearchCriteria);
	m_comboBoxPublisherCriteria->setMinimumSize(QSize(0, 23));
	m_comboBoxPublisherCriteria->setEditable(true);

	m_labelDatePublishedCriteria = new QLabel("Published", m_frameSearchCriteria);

	m_comboBoxDatePublishedCriteria = new QComboBox(m_frameSearchCriteria);
	m_comboBoxDatePublishedCriteria->setMinimumSize(QSize(0, 23));
	m_comboBoxDatePublishedCriteria->setEditable(true);

	m_labelRatingCriteria = new QLabel("Rating", m_frameSearchCriteria);

	m_comboBoxRatingCriteria = new QComboBox(m_frameSearchCriteria);
	m_comboBoxRatingCriteria->setMinimumSize(QSize(0, 23));
	m_comboBoxRatingCriteria->setEditable(true);

	m_labelStatusCriteria = new QLabel("Status", m_frameSearchCriteria);

	m_comboBoxStatusCriteria = new QComboBox(m_frameSearchCriteria);
	m_comboBoxStatusCriteria->setMinimumSize(QSize(0, 23));
	m_comboBoxStatusCriteria->setEditable(true);

	m_buttonPublisher = new QToolButton(m_frameSearchCriteria);
	m_buttonPublisher->setCursor(Qt::PointingHandCursor);
	m_buttonPublisher->setIcon(QIcon(":/icons/publisher.png"));
	m_buttonPublisher->setIconSize(QSize(20, 20));

	m_buttonDatePublished = new QToolButton(m_frameSearchCriteria);
	m_buttonDatePublished->setCursor(Qt::PointingHandCursor);
	m_buttonDatePublished->setIcon(QIcon(":/icons/calendar.png"));
	m_buttonDatePublished->setIconSize(QSize(20, 20));

	m_buttonSeries = new QToolButton(m_frameSearchCriteria);
	m_buttonSeries->setCursor(Qt::PointingHandCursor);
	m_buttonSeries->setIcon(QIcon(":/icons/series.png"));
	m_buttonSeries->setIconSize(QSize(20, 20));

	m_buttonRating = new QToolButton(m_frameSearchCriteria);
	m_buttonRating->setCursor(Qt::PointingHandCursor);
	m_buttonRating->setIcon(QIcon(":/icons/rating.png"));
	m_buttonRating->setIconSize(QSize(20, 20));

	m_buttonStatus = new QToolButton(m_frameSearchCriteria);
	m_buttonStatus->setCursor(Qt::PointingHandCursor);
	m_buttonStatus->setIcon(QIcon(":/icons/status.png"));
	m_buttonStatus->setIconSize(QSize(20, 20));

	// Grid Search Criteria
	m_vertLaySearchCriteria->addSpacerItem(m_vertSpacerSearchCriteriaTop);
	m_vertLaySearchCriteria->addWidget(m_buttonSearchCriteria);
	m_vertLaySearchCriteria->addWidget(m_buttonClearCriteria);
	m_vertLaySearchCriteria->addWidget(m_buttonSaveCriteria);

	m_gridLayCriteria->addWidget(m_labelFolderCriteria, 0, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxFolderCriteria, 0, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_buttonFolder, 0, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_labelAuthorCriteria, 1, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxAuthorCriteria, 1, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_buttonAuthor, 1, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_labelGenreCriteria, 2, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxGenreCriteria, 2, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_buttonGenre, 2, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_labelTagsCriteria, 3, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxTagsCriteria, 3, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_buttonTags, 3, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_labelExtensionCriteria, 4, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxExtensionsCriteria, 4, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_buttonExtensions, 4, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_labelPublisherCriteria, 5, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxPublisherCriteria, 5, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_buttonPublisher, 5, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_labelDatePublishedCriteria, 6, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxDatePublishedCriteria, 6, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_buttonDatePublished, 6, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_labelSeriesCriteria, 7, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxSeriesCriteria, 7, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_buttonSeries, 7, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_labelRatingCriteria, 8, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxRatingCriteria, 8, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_buttonRating, 8, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_labelStatusCriteria, 9, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxStatusCriteria, 9, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_buttonStatus, 9, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_labelSizeCriteria, 10, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_spinBoxFromSizeCriteria, 10, 1, 1, 1);
	m_gridLayCriteria->addWidget(m_labelSizeToCriteria, 10, 2, 1, 2);
	m_gridLayCriteria->addWidget(m_spinBoxToSizeCriteria, 10, 4, 1, 1);
	m_gridLayCriteria->addWidget(m_buttonSizeCriteria, 10, 5, 1, 1);
	m_gridLayCriteria->addWidget(m_labelPagesCriteria, 11, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_spinBoxFromPagesCriteria, 11, 1, 1, 1);
	m_gridLayCriteria->addWidget(m_labelPagesToCriteria, 11, 2, 1, 2);
	m_gridLayCriteria->addWidget(m_spinBoxToPagesCriteria, 11, 4, 1, 1);
	m_gridLayCriteria->addWidget(m_labelSearchCriteria, 12, 0, 1, 1);
	m_gridLayCriteria->addWidget(m_comboBoxSearchLoad, 12, 1, 1, 4);
	m_gridLayCriteria->addWidget(m_buttonSearchLoad, 12, 5, 1, 1);

	m_frameGridLayCriteria->setLayout(m_gridLayCriteria);
	m_scrollAreaCriteria->setWidget(m_frameGridLayCriteria);

	m_horLayCriteria->addWidget(m_scrollAreaCriteria);
	m_horLayCriteria->addLayout(m_vertLaySearchCriteria);

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
	m_frameMainRight->setStyleSheet(QString("QFrame#frameMainRight {border: 0px; }"));

	m_vertLayMainRight = new QVBoxLayout(m_frameMainRight);
	m_vertLayMainRight->setSpacing(6);
	m_vertLayMainRight->setContentsMargins(0, 0, 9, 0);

	m_frameSearchToolBar = new QFrame(m_frameMainRight);
	QSizePolicy retain = m_frameSearchToolBar->sizePolicy();
	retain.setRetainSizeWhenHidden(true);
	m_frameSearchToolBar->setSizePolicy(retain);
	m_frameSearchToolBar->setStyleSheet(QString("QFrame { border: 0px; }"));

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
	m_textDetailsGenre->setMinimumSize(QSize(0, 23));
	m_textDetailsGenre->setMaximumHeight(24);

	m_frameSizeDetails = new QFrame(m_frameDetails);
	m_frameSizeDetails->setMinimumSize(QSize(32, 0));
	m_frameSizeDetails->setMaximumHeight(24);
	m_frameSizeDetails->setStyleSheet(QString("QFrame { border: 0px; }"));

	m_horLayDetailsSize = new QHBoxLayout(m_frameSizeDetails);
	m_horLayDetailsSize->setSpacing(7);
	m_horLayDetailsSize->setContentsMargins(0, 0, 0, 0);

	m_textDetailsSize = new QLineEdit(m_frameSizeDetails);
	m_textDetailsSize->setEnabled(false);
	m_textDetailsSize->setMinimumSize(QSize(0, 23));
	m_textDetailsSize->setMaximumHeight(24);

	m_buttonSizeUnit = new QPushButton("MB", m_frameSizeDetails);
	m_buttonSizeUnit->setMinimumSize(QSize(21, 0));
	m_buttonSizeUnit->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonSizeUnit->setStyleSheet(QString("border: 0px;"));

	m_horLayDetailsSize->addWidget(m_textDetailsSize);
	m_horLayDetailsSize->addWidget(m_buttonSizeUnit);

	m_textDetailsTags = new QLineEdit(m_frameDetails);
	m_textDetailsTags->setMaximumHeight(24);
	m_textDetailsTags->setMinimumSize(QSize(0, 23));

	m_labelDetailsTags = new QLabel("Tags: ", m_frameDetails);
	m_labelDetailsTags->setMaximumHeight(24);

	m_labelDetailsName = new QLabel("Name: ", m_frameDetails);
	m_labelDetailsName->setMaximumHeight(24);

	m_textDetailsAuthor = new QLineEdit(m_frameDetails);
	m_textDetailsAuthor->setMaximumHeight(24);
	m_textDetailsAuthor->setMinimumSize(QSize(0, 23));

	m_textDetailsFolder = new QLineEdit(m_frameDetails);
	m_textDetailsFolder->setMaximumHeight(24);
	m_textDetailsFolder->setMinimumSize(QSize(0, 23));

	m_textDetailsExt = new QLineEdit(m_frameDetails);
	m_textDetailsExt->setEnabled(false);
	m_textDetailsExt->setMaximumHeight(24);
	m_textDetailsExt->setMinimumSize(QSize(0, 23));

	m_textDetailsPublisher = new QLineEdit(m_frameDetails);
	m_textDetailsPublisher->setMaximumHeight(24);
	m_textDetailsPublisher->setMinimumSize(QSize(0, 23));

	m_textDetailsDatePublished = new QLineEdit(m_frameDetails);
	m_textDetailsDatePublished->setMaximumHeight(24);
	m_textDetailsDatePublished->setMinimumSize(QSize(0, 23));

	m_textDetailsSeries = new QLineEdit(m_frameDetails);
	m_textDetailsSeries->setMaximumHeight(24);
	m_textDetailsSeries->setMinimumSize(QSize(0, 23));

	m_comboBoxDetailsRating = new RatingComboBox(m_frameDetails);
	m_comboBoxDetailsRating->setMaximumHeight(24);
	m_comboBoxDetailsRating->setMinimumSize(QSize(0, 23));

	m_comboBoxDetailsStatus = new StatusComboBox(m_frameDetails);
	m_comboBoxDetailsStatus->setMaximumHeight(24);
	m_comboBoxDetailsStatus->setMinimumSize(QSize(0, 23));

	m_labelDetailsStatus = new QLabel("Status: ", m_frameDetails);
	m_labelDetailsStatus->setMaximumHeight(24);

	m_labelDetailsRating = new QLabel("Rating: ", m_frameDetails);
	m_labelDetailsRating->setMaximumHeight(24);

	m_labelDetailsSeries = new QLabel("Series: ", m_frameDetails);
	m_labelDetailsSeries->setMaximumHeight(24);

	m_labelDetailsDatePublished = new QLabel("Published: ", m_frameDetails);
	m_labelDetailsDatePublished->setMaximumHeight(24);

	m_labelDetailsPublisher = new QLabel("Publisher: ", m_frameDetails);
	m_labelDetailsPublisher->setMaximumHeight(24);

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
	m_textDetailsName->setMinimumSize(QSize(0, 23));

	m_textDetailsPages = new QLineEdit(m_frameDetails);
	m_textDetailsPages->setMaximumHeight(24);
	m_textDetailsPages->setMinimumSize(QSize(0, 23));

	m_scrollAreaDetailsFields = new QScrollArea(m_frameDetails);
	m_scrollAreaDetailsFields->setObjectName("m_scrollAreaDetailsFields");
	m_scrollAreaDetailsFields->setStyleSheet("QScrollArea#m_scrollAreaDetailsFields { border: none; }");
	m_scrollAreaDetailsFields->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_frameDetailsFields = new QFrame(m_scrollAreaDetailsFields);
	m_frameDetailsFields->setObjectName("m_gridLayDetailsFields");
	m_frameDetailsFields->setContentsMargins(0, 0, 0, 0);
	m_frameDetailsFields->setStyleSheet("QFrame#m_gridLayDetailsFields { border: none ; }");

	m_gridLayDetailsFields->addWidget(m_labelDetailsName, 0, 0, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsName, 0, 1, 1, 3);
	m_gridLayDetailsFields->addWidget(m_labelDetailsAuthor, 1, 0, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsAuthor, 1, 1, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsExt, 1, 3, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsExt, 1, 2, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsGenre, 2, 0, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsGenre, 2, 1, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsPages, 2, 2, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsPages, 2, 3, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsFolder, 3, 0, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsFolder, 3, 1, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsSize, 3, 2, 1, 1);
	m_gridLayDetailsFields->addWidget(m_frameSizeDetails, 3, 3, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsTags, 4, 0, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsTags, 4, 1, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsPublisher, 4, 2, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsPublisher, 4, 3, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsDatePublished, 5, 0, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsDatePublished, 5, 1, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsSeries, 5, 2, 1, 1);
	m_gridLayDetailsFields->addWidget(m_textDetailsSeries, 5, 3, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsRating, 6, 0, 1, 1);
	m_gridLayDetailsFields->addWidget(m_comboBoxDetailsRating, 6, 1, 1, 1);
	m_gridLayDetailsFields->addWidget(m_labelDetailsStatus, 6, 2, 1, 1);
	m_gridLayDetailsFields->addWidget(m_comboBoxDetailsStatus, 6, 3, 1, 1);

	m_frameDetailsFields->setLayout(m_gridLayDetailsFields);
	m_scrollAreaDetailsFields->setWidget(m_frameDetailsFields);
	m_scrollAreaDetailsFields->setWidgetResizable(true);

	m_vertLayDetails->addWidget(m_scrollAreaDetailsFields);

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
	refreshAll();
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
	connect(m_buttonPublisher, &QToolButton::clicked, this, &MainWindow::selectPublishers);
	connect(m_buttonDatePublished, &QToolButton::clicked, this, &MainWindow::selectDatePublished);
	connect(m_buttonSeries, &QToolButton::clicked, this, &MainWindow::selectSeries);
	connect(m_buttonRating, &QToolButton::clicked, this, &MainWindow::selectRatings);
	connect(m_buttonStatus, &QToolButton::clicked, this, &MainWindow::selectStatus);

	connect(m_buttonSizeUnit, &QToolButton::clicked, this, &MainWindow::toggleSizeUnit);
	connect(m_buttonSummaries, &QToolButton::clicked, []
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
	connect(m_textSearchBar, &QLineEdit::returnPressed, this, &MainWindow::searchString);
	connect(m_textSearchBar, &QLineEdit::textChanged, this, &MainWindow::searchString);

	connect(m_textDetailsName, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
	connect(m_textDetailsAuthor, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
	connect(m_textDetailsGenre, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
	connect(m_textDetailsFolder, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
	connect(m_textDetailsPages, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
	connect(m_textDetailsTags, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);

	connect(m_actionBookDetails, &QAction::triggered, []
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
	connect(m_actionSummaries, &QAction::triggered, []
	{ showSummary(""); });
	connect(m_actionMinimizeTray, &QAction::triggered, [this]
	{
		QTimer::singleShot(0, this, SLOT(hide()));
	});
	connect(m_actionOpenDB, &QAction::triggered, []
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile("./database.db"));
	});
	connect(m_actionEbookReportsDir, &QAction::triggered, []
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile("./reports/ebooks/"));
	});
	connect(m_actionUsageReportsDir, &QAction::triggered, []
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile("./reports/usage/"));
	});
	connect(m_actionInstallationDir, &QAction::triggered, []
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile("./"));
	});
}

void MainWindow::setupTabOrder()
{
	QWidget::setTabOrder(m_textSearchBar, m_ebooksListWidget);
	QWidget::setTabOrder(m_ebooksListWidget, m_textDetailsName);
	QWidget::setTabOrder(m_textDetailsName, m_textDetailsAuthor);
	QWidget::setTabOrder(m_textDetailsAuthor, m_textDetailsExt);
	QWidget::setTabOrder(m_textDetailsExt, m_textDetailsGenre);
	QWidget::setTabOrder(m_textDetailsGenre, m_textDetailsPages);
	QWidget::setTabOrder(m_textDetailsPages, m_textDetailsFolder);
	QWidget::setTabOrder(m_textDetailsFolder, m_textDetailsSize);
	QWidget::setTabOrder(m_textDetailsSize, m_buttonSizeUnit);
	QWidget::setTabOrder(m_buttonSizeUnit, m_textDetailsTags);
	QWidget::setTabOrder(m_textDetailsTags, m_textDetailsPublisher);
	QWidget::setTabOrder(m_textDetailsPublisher, m_textDetailsDatePublished);
	QWidget::setTabOrder(m_textDetailsDatePublished, m_textDetailsSeries);
	QWidget::setTabOrder(m_textDetailsSeries, m_comboBoxDetailsRating);
	QWidget::setTabOrder(m_comboBoxDetailsRating, m_comboBoxDetailsStatus);
	QWidget::setTabOrder(m_comboBoxDetailsStatus, m_buttonDetailsUpdate);
	QWidget::setTabOrder(m_buttonDetailsUpdate, m_buttonDetailsRestore);
	QWidget::setTabOrder(m_buttonDetailsRestore, m_buttonDetailsClear);
	QWidget::setTabOrder(m_buttonDetailsClear, m_comboBoxFolderCriteria);
	QWidget::setTabOrder(m_comboBoxFolderCriteria, m_buttonFolder);
	QWidget::setTabOrder(m_buttonFolder, m_comboBoxAuthorCriteria);
	QWidget::setTabOrder(m_comboBoxAuthorCriteria, m_buttonAuthor);
	QWidget::setTabOrder(m_buttonAuthor, m_comboBoxGenreCriteria);
	QWidget::setTabOrder(m_comboBoxGenreCriteria, m_buttonGenre);
	QWidget::setTabOrder(m_buttonGenre, m_comboBoxTagsCriteria);
	QWidget::setTabOrder(m_comboBoxTagsCriteria, m_buttonTags);
	QWidget::setTabOrder(m_buttonTags, m_comboBoxExtensionsCriteria);
	QWidget::setTabOrder(m_comboBoxExtensionsCriteria, m_buttonExtensions);
	QWidget::setTabOrder(m_buttonExtensions, m_comboBoxPublisherCriteria);
	QWidget::setTabOrder(m_comboBoxPublisherCriteria, m_buttonPublisher);
	QWidget::setTabOrder(m_buttonPublisher, m_comboBoxDatePublishedCriteria);
	QWidget::setTabOrder(m_comboBoxDatePublishedCriteria, m_buttonDatePublished);
	QWidget::setTabOrder(m_buttonDatePublished, m_comboBoxSeriesCriteria);
	QWidget::setTabOrder(m_comboBoxSeriesCriteria, m_buttonSeries);
	QWidget::setTabOrder(m_buttonSeries, m_comboBoxRatingCriteria);
	QWidget::setTabOrder(m_comboBoxRatingCriteria, m_buttonRating);
	QWidget::setTabOrder(m_buttonRating, m_comboBoxStatusCriteria);
	QWidget::setTabOrder(m_comboBoxStatusCriteria, m_buttonStatus);
	QWidget::setTabOrder(m_buttonStatus, m_spinBoxFromSizeCriteria);
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

void MainWindow::refreshTags()
{
	queries::selectTags();
	refreshComboBox(m_comboBoxTagsCriteria);
}

void MainWindow::refreshExtensions()
{
	queries::selectExt();
	refreshComboBox(m_comboBoxExtensionsCriteria);
}

void MainWindow::refreshPublishers()
{
	queries::selectPublisherQuery();
	refreshComboBox(m_comboBoxPublisherCriteria);
}

void MainWindow::refreshDatePublished()
{
	queries::selectDatePublishedQuery();
	refreshComboBox(m_comboBoxDatePublishedCriteria);
}

void MainWindow::refreshSeries()
{
	queries::selectSeriesQuery();
	refreshComboBox(m_comboBoxSeriesCriteria);
}

void MainWindow::refreshStatus()
{
	queries::selectStatusQuery();
	m_comboBoxStatusCriteria->clear();
	m_comboBoxStatusCriteria->addItem("");
	while (queries::query.next())
	{
		QString value = StatusComboBox::convertToStatus(queries::query.value(0).toInt());
		if (value.isEmpty())
		{
			continue;
		}
		m_comboBoxStatusCriteria->addItem(value);
	}
	m_comboBoxStatusCriteria->model()->sort(0, Qt::AscendingOrder);
}

void MainWindow::refreshRatings()
{
	queries::selectRatingsQuery();
	m_comboBoxRatingCriteria->clear();
	m_comboBoxRatingCriteria->addItem("");
	while (queries::query.next())
	{
		QString value = RatingComboBox::convertToStars(queries::query.value(0).toInt());
		if (value.isEmpty())
		{
			continue;
		}
		m_comboBoxRatingCriteria->addItem(value);
	}
	m_comboBoxRatingCriteria->model()->sort(0, Qt::AscendingOrder);
}

void MainWindow::refreshSearches()
{
	queries::selectSearchesQuery();
	refreshComboBox(m_comboBoxSearchLoad);
}

void MainWindow::refreshAll()
{
	refreshFolders();
	refreshAuthors();
	refreshGenres();
	refreshTags();
	refreshExtensions();
	refreshPublishers();
	refreshDatePublished();
	refreshSeries();
	refreshRatings();
	refreshStatus();
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

	refreshAll();
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
	m_comboBoxExtensionsCriteria->setCurrentIndex(0);
	m_comboBoxPublisherCriteria->setCurrentIndex(0);
	m_comboBoxDatePublishedCriteria->setCurrentIndex(0);
	m_comboBoxSeriesCriteria->setCurrentIndex(0);
	m_comboBoxRatingCriteria->setCurrentIndex(0);
	m_comboBoxStatusCriteria->setCurrentIndex(0);
	m_comboBoxTagsCriteria->setCurrentIndex(0);
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
	m_textDetailsPublisher->clear();
	m_textDetailsDatePublished->clear();
	m_textDetailsSeries->clear();
	m_comboBoxDetailsRating->setCurrentIndex(0);
	m_comboBoxDetailsStatus->setCurrentIndex(0);
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
	refreshAll();
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
	refreshAll();
}

void MainWindow::clearSearch()
{
	m_textSearchBar->clear();
	m_ebooksListWidget->clear();
	clearDetails();
	m_statusBar->clearMessage();
}

void MainWindow::searchCriteria()
{
	m_ebooksListWidget->clear();
	QString folder = m_comboBoxFolderCriteria->currentText();
	QString author = m_comboBoxAuthorCriteria->currentText();
	QString genre = m_comboBoxGenreCriteria->currentText();
	QString tags = m_comboBoxTagsCriteria->currentText();
	quint64 sizeTo = m_spinBoxToSizeCriteria->value();
	quint64 sizeFrom = m_spinBoxFromSizeCriteria->value();
	QString convUnit = m_buttonSizeCriteria->text();
	sizeTo = sizeTo * m_sizeConvFactors[convUnit];
	sizeFrom = sizeFrom * m_sizeConvFactors[convUnit];
	quint32 pagesTo = m_spinBoxToPagesCriteria->value();
	quint32 pagesFrom = m_spinBoxFromPagesCriteria->value();
	QString ext = m_comboBoxExtensionsCriteria->currentText();
	QString publisher = m_comboBoxPublisherCriteria->currentText();
	QString published = m_comboBoxDatePublishedCriteria->currentText();
	QString series = m_comboBoxSeriesCriteria->currentText();
	// Transform ratings from strings to indices
	QString ratings = m_comboBoxRatingCriteria->currentText();
	ratings = convertValuesToIndices(ratings, &RatingComboBox::convertToIndex);
	// Transform status from strings to indices
	QString status = m_comboBoxStatusCriteria->currentText();
	status = convertValuesToIndices(status, &StatusComboBox::convertToIndex);

	queries::selectNameBasedOnCriteria(folder, genre, author, tags, ext, pagesFrom, pagesTo, sizeFrom, sizeTo,
			publisher, published, series, ratings, status);
	quint32 count = 0;
	while (queries::query.next())
	{
		m_ebooksListWidget->addItem(queries::query.value(0).toString());
		count++;
	}

	m_statusBar->showMessage("Number of ebooks: " + QString::number(count));
}

QString MainWindow::convertValuesToIndices(QString values, std::function<QString(QString)> func)
{
	if (values.isEmpty())
	{
		return values;
	}
	QStringList valueList = values.split(common::SEP);
	std::transform(valueList.begin(), valueList.end(), valueList.begin(), func);
	return valueList.join(common::SEP);
}

void MainWindow::showEbookDetails(QListWidgetItem* item)
{
	QString fileName = item->text();
	queries::selectAllBasedOnName(fileName);
	queries::query.next();
	QString author = queries::query.value("author").toString();
	author = (author == "N/A" ? "" : author);
	QString genre = queries::query.value("genre").toString();
	genre = (genre == "N/A" ? "" : genre);
	QString ext = queries::query.value("ext").toString();
	QString pages = queries::query.value("pages").toString();
	double size = changeBookSizeUnit(queries::query.value("size").toDouble(), m_buttonSizeUnit->text());
	QString folder = queries::query.value("folder").toString();
	QString publisher = queries::query.value("publisher").toString();
	publisher = (publisher == "N/A" ? "" : publisher);
	QString datePublished = queries::query.value("date_published").toString();
	datePublished = (datePublished == "N/A" ? "" : datePublished);
	QString series = queries::query.value("series").toString();
	series = (series == "N/A" ? "" : series);
	quint32 rating = queries::query.value("rating").toInt();
	quint32 status = queries::query.value("status").toInt();
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
	m_textDetailsPublisher->setText(publisher);
	m_textDetailsDatePublished->setText(datePublished);
	m_textDetailsSeries->setText(series);
	m_comboBoxDetailsRating->setCurrentIndex(rating);
	m_comboBoxDetailsStatus->setCurrentIndex(status);
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
	if (m_ebooksListWidget->selectedItems().isEmpty())
	{
		return;
	}
	QString oldName = m_ebooksListWidget->currentItem()->text();
	QString newName = m_textDetailsName->text();
	QString author = m_textDetailsAuthor->text();
	QString folder = m_textDetailsFolder->text();
	QString genre = m_textDetailsGenre->text();
	QString publisher = m_textDetailsPublisher->text();
	QString datePublished = m_textDetailsDatePublished->text();
	QString series = m_textDetailsSeries->text();
	quint32 rating = m_comboBoxDetailsRating->currentIndex();
	quint32 status = m_comboBoxDetailsStatus->currentIndex();

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
	queries::updateBookQuery(oldName, newName, folder, genre, author, pages, tags, path, publisher, datePublished,
			series, rating, status);
	m_ebooksListWidget->currentItem()->setText(newName);

	refreshAll();
	m_statusBar->showMessage("Details updated successfully.");
}

void MainWindow::saveCriteria()
{
	QString folder = m_comboBoxFolderCriteria->currentText();
	QString author = m_comboBoxAuthorCriteria->currentText();
	QString genre = m_comboBoxGenreCriteria->currentText();
	QString tags = m_comboBoxTagsCriteria->currentText();
	QString ext = m_comboBoxExtensionsCriteria->currentText();
	quint32 sizeTo = m_spinBoxToSizeCriteria->value();
	quint32 sizeFrom = m_spinBoxFromSizeCriteria->value();
	QString sizeIn = m_buttonSizeCriteria->text();
	quint32 pagesTo = m_spinBoxToPagesCriteria->value();
	quint32 pagesFrom = m_spinBoxFromPagesCriteria->value();
	QString publisher = m_comboBoxPublisherCriteria->currentText();
	QString datePublished = m_comboBoxDatePublishedCriteria->currentText();
	QString series = m_comboBoxSeriesCriteria->currentText();
	QString rating = m_comboBoxRatingCriteria->currentText();
	QString status = m_comboBoxStatusCriteria->currentText();

	GetNameDialog dialog(this, "Search Name", "Please provide a name for the search you want to save:");
	common::openDialog(&dialog, ":/styles/style.qss");
	if (!dialog.m_name.isEmpty())
	{
		queries::insertSearchQuery(dialog.m_name, folder, author, genre, tags, ext,
				sizeFrom, sizeTo, sizeIn, pagesFrom, pagesTo,
				publisher, datePublished, series, rating, status);
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
	m_comboBoxTagsCriteria->setCurrentText(queries::query.value("tags").toString());
	m_comboBoxExtensionsCriteria->setCurrentText(queries::query.value("ext").toString());
	m_spinBoxFromSizeCriteria->setValue(queries::query.value("size_from").toUInt());
	m_spinBoxToSizeCriteria->setValue(queries::query.value("size_to").toUInt());
	m_buttonSizeCriteria->setText(queries::query.value("size_in").toString());
	m_spinBoxFromPagesCriteria->setValue(queries::query.value("pages_from").toUInt());
	m_spinBoxToPagesCriteria->setValue(queries::query.value("pages_to").toUInt());
	m_comboBoxPublisherCriteria->setCurrentText(queries::query.value("publisher").toString());
	m_comboBoxDatePublishedCriteria->setCurrentText(queries::query.value("date_published").toString());
	m_comboBoxSeriesCriteria->setCurrentText(queries::query.value("series").toString());
	m_comboBoxRatingCriteria->setCurrentText(queries::query.value("rating").toString());
	m_comboBoxStatusCriteria->setCurrentText(queries::query.value("status").toString());

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
	extSelectionSetup("Extensions", "Select Available Extensions", m_comboBoxExtensionsCriteria);
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

void MainWindow::selectPublishers()
{
	queries::selectPublisherQuery();
	extSelectionSetup("Publishers", "Select Available Publishers", m_comboBoxPublisherCriteria);
}

void MainWindow::selectDatePublished()
{
	queries::selectDatePublishedQuery();
	extSelectionSetup("Dates Published", "Select Available Dates", m_comboBoxDatePublishedCriteria);
}

void MainWindow::selectSeries()
{
	queries::selectSeriesQuery();
	extSelectionSetup("Series", "Select Available Series", m_comboBoxSeriesCriteria);
}

void MainWindow::selectStatus()
{
	queries::selectStatusQuery();
	QVector<QString> results;
	while (queries::query.next())
	{
		results.push_back(StatusComboBox::convertToStatus(queries::query.value(0).toInt()));
	}

	ExtSelectionDialog* dialog = new ExtSelectionDialog(this, results, "Status", "Select Available Status");
	common::openDialog(dialog, ":/styles/style.qss");

	results = dialog->getExtVector();
	QString resultString = results.join(common::SEP);

	m_comboBoxStatusCriteria->setCurrentText(resultString);
}

void MainWindow::selectRatings()
{
	queries::selectRatingsQuery();
	QVector<QString> results;
	while (queries::query.next())
	{
		results.push_back(RatingComboBox::convertToStars(queries::query.value(0).toInt()));
	}

	ExtSelectionDialog* dialog = new ExtSelectionDialog(this, results, "Ratings", "Select Available Ratings");
	common::openDialog(dialog, ":/styles/style.qss");

	results = dialog->getExtVector();
	QString resultString = results.join(common::SEP);

	m_comboBoxRatingCriteria->setCurrentText(resultString);
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
	m_comboBoxTagsCriteria->setCurrentText(tagString);
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
