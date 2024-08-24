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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
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
    _sizeConvFactors["KB"] = 1024;
    _sizeConvFactors["MB"] = 1024 * 1024;
    _sizeConvFactors["GB"] = 1024 * 1024 * 1024;

    // Setup Size Factors to Toggle Sizes
    _sizeUnits["KB"] = "MB";
    _sizeUnits["MB"] = "GB";
    _sizeUnits["GB"] = "KB";

    // Setup List Sorting
    _SORT = Qt::AscendingOrder;

    // Setup Tray Icon
    _trayIcon = new QSystemTrayIcon(this);
    _trayIcon->setIcon(QIcon(":/icons/icon.ico"));
    _trayIcon->setVisible(true);
    _trayIcon->setToolTip("Ebook Access\nClick to Open");
    _trayIcon->show();

    QTimer::singleShot(1500, this, SLOT(showQuote()));

    setupInterface();
    setupConnections();
    setupTabOrder();
    setupShortcuts();
}

void MainWindow::setupInterface() {
    // Window
    setWindowTitle("Ebook Access");
    resize(1190, 668);

    // Widgets
    _centralWidget = new QWidget(this);

    _horLayCentral = new QHBoxLayout(_centralWidget);

    _frameMain = new QFrame(_centralWidget);
    _frameMain->setObjectName("frameMain");
    _frameMain->setStyleSheet(QString("QFrame#frameMain { border:0px; }"));

    _horLayMain = new QHBoxLayout(_frameMain);
    _splitter = new QSplitter(_frameMain);
    _splitter->setObjectName("splitter");
    _splitter->setStyleSheet(QString("QFrame#splitter {border:0px;}"
        "QSplitter::handle { background-color: #2D2D30;}"));
    _splitter->setLineWidth(0);
    _splitter->setOrientation(Qt::Horizontal);
    _splitter->setHandleWidth(2);

    _frameMainLeft = new QFrame(_splitter);
    _frameMainLeft->setObjectName("frameMainLeft");
    _frameMainLeft->setStyleSheet(QString("QFrame#frameMainLeft { border: 0px;}"));

    _vertLayMainLeft = new QVBoxLayout(_frameMainLeft);
    _vertLayMainLeft->setSizeConstraint(QLayout::SetDefaultConstraint);
    _vertLayMainLeft->setContentsMargins(-1, 32, -1, 0);

    _frameSearchCriteria = new QFrame(_frameMainLeft);
    _frameSearchCriteria->setStyleSheet(QString(""));
    _frameSearchCriteria->setFrameShape(QFrame::StyledPanel);
    _frameSearchCriteria->setFrameShadow(QFrame::Raised);

    _horLayCriteria = new QHBoxLayout(_frameSearchCriteria);
    _horLayCriteria->setSizeConstraint(QLayout::SetDefaultConstraint);
    _horLayCriteria->setContentsMargins(-1, 9, -1, 9);

    _gridLayCriteria = new QGridLayout();
    _gridLayCriteria->setHorizontalSpacing(17);
    _gridLayCriteria->setVerticalSpacing(30);
    _gridLayCriteria->setContentsMargins(20, 15, 20, 5);

    _labelGenreCriteria = new QLabel("Genre", _frameSearchCriteria);

    _spinBoxToPagesCriteria = new QSpinBox(_frameSearchCriteria);
    _spinBoxToPagesCriteria->setMaximumSize(QSize(70, 16777215));
    _spinBoxToPagesCriteria->setMinimum(0);
    _spinBoxToPagesCriteria->setMaximum(100000);
    _spinBoxToPagesCriteria->setValue(100000);

    _labelPagesToCriteria = new QLabel(_frameSearchCriteria);
    _labelPagesToCriteria->setPixmap(QPixmap(QString(":/icons/right_arrow_icon.png")));
    _labelPagesToCriteria->setAlignment(Qt::AlignCenter);

    _labelSizeToCriteria = new QLabel(_frameSearchCriteria);
    _labelSizeToCriteria->setPixmap(QPixmap(QString(":/icons/right_arrow_icon.png")));
    _labelSizeToCriteria->setScaledContents(false);
    _labelSizeToCriteria->setAlignment(Qt::AlignCenter);

    _labelAuthorCriteria = new QLabel("Author", _frameSearchCriteria);

    _labelSearchCriteria = new QLabel("Search", _frameSearchCriteria);

    _comboBoxExtensionsCriteria = new QComboBox(_frameSearchCriteria);
    _comboBoxExtensionsCriteria->setMinimumSize(QSize(0, 23));
    _comboBoxExtensionsCriteria->setEditable(true);

    _spinBoxFromSizeCriteria = new QSpinBox(_frameSearchCriteria);
    _spinBoxFromSizeCriteria->setMaximumSize(QSize(70, 16777215));
    _spinBoxFromSizeCriteria->setMaximum(1023);

    _labelTagsCriteria = new QLabel("Tags", _frameSearchCriteria);

    _labelFolderCriteria = new QLabel("Folder", _frameSearchCriteria);

    _comboBoxSearchLoad = new QComboBox(_frameSearchCriteria);
    _comboBoxSearchLoad->setContextMenuPolicy(Qt::CustomContextMenu);
    _comboBoxSearchLoad->addItem(QString());
    _comboBoxSearchLoad->setMinimumSize(QSize(0, 23));
    _comboBoxSearchLoad->setEditable(false);

    _comboBoxTagsCriteria = new QComboBox(_frameSearchCriteria);
    _comboBoxTagsCriteria->setMinimumSize(QSize(0, 23));
    _comboBoxTagsCriteria->setEditable(true);

    _spinBoxToSizeCriteria = new QSpinBox(_frameSearchCriteria);
    _spinBoxToSizeCriteria->setMaximumSize(QSize(70, 16777215));
    _spinBoxToSizeCriteria->setMaximum(1024);
    _spinBoxToSizeCriteria->setValue(1024);

    _spinBoxFromPagesCriteria = new QSpinBox(_frameSearchCriteria);
    _spinBoxFromPagesCriteria->setMaximumSize(QSize(70, 16777215));
    _spinBoxFromPagesCriteria->setMaximum(100000);

    _comboBoxFolderCriteria = new QComboBox(_frameSearchCriteria);
    _comboBoxFolderCriteria->addItem(QString());
    _comboBoxFolderCriteria->setMinimumSize(QSize(0, 23));
    _comboBoxFolderCriteria->setEditable(true);
    _comboBoxFolderCriteria->setMinimumContentsLength(40);
    _comboBoxFolderCriteria->setFrame(true);

    _comboBoxGenreCriteria = new QComboBox(_frameSearchCriteria);
    _comboBoxGenreCriteria->addItem(QString());
    _comboBoxGenreCriteria->setMinimumSize(QSize(0, 23));
    _comboBoxGenreCriteria->setEditable(true);
    _comboBoxGenreCriteria->setMinimumContentsLength(40);

    _comboBoxAuthorCriteria = new QComboBox(_frameSearchCriteria);
    _comboBoxAuthorCriteria->addItem(QString());
    _comboBoxAuthorCriteria->setMinimumSize(QSize(0, 23));
    _comboBoxAuthorCriteria->setEditable(true);
    _comboBoxAuthorCriteria->setMinimumContentsLength(40);

    _labelSizeCriteria = new QLabel("Size", _frameSearchCriteria);

    _labelPagesCriteria = new QLabel("Pages", _frameSearchCriteria);

    _labelExtensionCriteria = new QLabel("Ext", _frameSearchCriteria);

    _buttonSearchCriteria = new QToolButton(_frameSearchCriteria);
    _buttonSearchCriteria->setMinimumSize(QSize(70, 30));
    _buttonSearchCriteria->setMaximumSize(QSize(90, 16777215));
    _buttonSearchCriteria->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonSearchCriteria->setIcon(QIcon(":/icons/search_icon.png"));
    _buttonSearchCriteria->setIconSize(QSize(30, 30));
    _buttonSearchCriteria->setAutoRaise(false);

    _buttonAuthor = new QToolButton(_frameSearchCriteria);
    _buttonAuthor->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonAuthor->setIcon(QIcon(":/icons/author_icon.png"));
    _buttonAuthor->setIconSize(QSize(22, 22));

    _buttonClearCriteria = new QToolButton(_frameSearchCriteria);
    _buttonClearCriteria->setMinimumSize(QSize(70, 30));
    _buttonClearCriteria->setMaximumSize(QSize(90, 16777215));
    _buttonClearCriteria->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonClearCriteria->setIcon(QIcon(":/icons/clear_search_white_icon.png"));
    _buttonClearCriteria->setIconSize(QSize(30, 30));

    _buttonExtensions = new QToolButton(_frameSearchCriteria);
    _buttonExtensions->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonExtensions->setIcon(QIcon(":/icons/puzzle_icon.png"));
    _buttonExtensions->setIconSize(QSize(20, 20));

    _buttonFolder = new QToolButton(_frameSearchCriteria);
    _buttonFolder->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonFolder->setIcon(QIcon(":/icons/folder_icon.png"));
    _buttonFolder->setIconSize(QSize(20, 20));

    _buttonGenre = new QToolButton(_frameSearchCriteria);
    _buttonGenre->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonGenre->setIcon(QIcon(":/icons/add_new_icon.png"));
    _buttonGenre->setIconSize(QSize(20, 20));

    _buttonSaveCriteria = new QToolButton(_frameSearchCriteria);
    _buttonSaveCriteria->setMinimumSize(QSize(70, 30));
    _buttonSaveCriteria->setMaximumSize(QSize(90, 16777215));
    _buttonSaveCriteria->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonSaveCriteria->setIcon(QIcon(":/icons/save_icon.png"));
    _buttonSaveCriteria->setIconSize(QSize(30, 30));

    _buttonSearchLoad = new QToolButton(_frameSearchCriteria);
    _buttonSearchLoad->setMinimumSize(QSize(51, 25));
    _buttonSearchLoad->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonSearchLoad->setIcon(QIcon(":/icons/upload_icon.png"));

    _buttonSizeCriteria = new QToolButton(_frameSearchCriteria);
    _buttonSizeCriteria->setText("MB");
    _buttonSizeCriteria->setMinimumSize(QSize(0, 0));
    _buttonSizeCriteria->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonSizeCriteria->setIconSize(QSize(25, 20));

    _buttonTags = new QToolButton(_frameSearchCriteria);
    _buttonTags->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonTags->setIcon(QIcon(":/icons/tags_icon.png"));
    _buttonTags->setIconSize(QSize(20, 20));

    _vertLaySearchCriteria = new QVBoxLayout();
    _vertLaySearchCriteria->setSpacing(25);
    _vertLaySearchCriteria->setContentsMargins(0, 0, 0, 20);

    _vertSpacerSearchCriteriaTop = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    _scrollAreaCriteria = new QScrollArea(_frameSearchCriteria);
    _scrollAreaCriteria->setObjectName("_scrollAreaCriteria");
    _scrollAreaCriteria->setStyleSheet("QFrame#_scrollAreaCriteria { border: none; }");
    _scrollAreaCriteria->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    _frameGridLayCriteria = new QFrame(_scrollAreaCriteria);
    _frameGridLayCriteria->setObjectName("_frameGridLayCriteria");
    _frameGridLayCriteria->setStyleSheet("QFrame#_frameGridLayCriteria { border: none; }");

    _labelSeriesCriteria = new QLabel("Series", _frameSearchCriteria);

    _comboBoxSeriesCriteria = new QComboBox(_frameSearchCriteria);
    _comboBoxSeriesCriteria->setMinimumSize(QSize(0, 23));
    _comboBoxSeriesCriteria->setEditable(true);

    _labelPublisherCriteria = new QLabel("Publisher", _frameSearchCriteria);

    _comboBoxPublisherCriteria = new QComboBox(_frameSearchCriteria);
    _comboBoxPublisherCriteria->setMinimumSize(QSize(0, 23));
    _comboBoxPublisherCriteria->setEditable(true);

    _labelDatePublishedCriteria = new QLabel("Published", _frameSearchCriteria);

    _comboBoxDatePublishedCriteria = new QComboBox(_frameSearchCriteria);
    _comboBoxDatePublishedCriteria->setMinimumSize(QSize(0, 23));
    _comboBoxDatePublishedCriteria->setEditable(true);

    _labelRatingCriteria = new QLabel("Rating", _frameSearchCriteria);

    _comboBoxRatingCriteria = new QComboBox(_frameSearchCriteria);
    _comboBoxRatingCriteria->setMinimumSize(QSize(0, 23));
    _comboBoxRatingCriteria->setEditable(true);

    _labelStatusCriteria = new QLabel("Status", _frameSearchCriteria);

    _comboBoxStatusCriteria = new QComboBox(_frameSearchCriteria);
    _comboBoxStatusCriteria->setMinimumSize(QSize(0, 23));
    _comboBoxStatusCriteria->setEditable(true);

    _buttonPublisher = new QToolButton(_frameSearchCriteria);
    _buttonPublisher->setCursor(Qt::PointingHandCursor);
    _buttonPublisher->setIcon(QIcon(":/icons/publisher.png"));
    _buttonPublisher->setIconSize(QSize(20, 20));

    _buttonDatePublished = new QToolButton(_frameSearchCriteria);
    _buttonDatePublished->setCursor(Qt::PointingHandCursor);
    _buttonDatePublished->setIcon(QIcon(":/icons/calendar.png"));
    _buttonDatePublished->setIconSize(QSize(20, 20));

    _buttonSeries = new QToolButton(_frameSearchCriteria);
    _buttonSeries->setCursor(Qt::PointingHandCursor);
    _buttonSeries->setIcon(QIcon(":/icons/series.png"));
    _buttonSeries->setIconSize(QSize(20, 20));

    _buttonRating = new QToolButton(_frameSearchCriteria);
    _buttonRating->setCursor(Qt::PointingHandCursor);
    _buttonRating->setIcon(QIcon(":/icons/rating.png"));
    _buttonRating->setIconSize(QSize(20, 20));

    _buttonStatus = new QToolButton(_frameSearchCriteria);
    _buttonStatus->setCursor(Qt::PointingHandCursor);
    _buttonStatus->setIcon(QIcon(":/icons/status.png"));
    _buttonStatus->setIconSize(QSize(20, 20));

    // Grid Search Criteria
    _vertLaySearchCriteria->addSpacerItem(_vertSpacerSearchCriteriaTop);
    _vertLaySearchCriteria->addWidget(_buttonSearchCriteria);
    _vertLaySearchCriteria->addWidget(_buttonClearCriteria);
    _vertLaySearchCriteria->addWidget(_buttonSaveCriteria);

    _gridLayCriteria->addWidget(_labelFolderCriteria, 0, 0, 1, 1);
    _gridLayCriteria->addWidget(_comboBoxFolderCriteria, 0, 1, 1, 4);
    _gridLayCriteria->addWidget(_buttonFolder, 0, 5, 1, 1);
    _gridLayCriteria->addWidget(_labelAuthorCriteria, 1, 0, 1, 1);
    _gridLayCriteria->addWidget(_comboBoxAuthorCriteria, 1, 1, 1, 4);
    _gridLayCriteria->addWidget(_buttonAuthor, 1, 5, 1, 1);
    _gridLayCriteria->addWidget(_labelGenreCriteria, 2, 0, 1, 1);
    _gridLayCriteria->addWidget(_comboBoxGenreCriteria, 2, 1, 1, 4);
    _gridLayCriteria->addWidget(_buttonGenre, 2, 5, 1, 1);
    _gridLayCriteria->addWidget(_labelTagsCriteria, 3, 0, 1, 1);
    _gridLayCriteria->addWidget(_comboBoxTagsCriteria, 3, 1, 1, 4);
    _gridLayCriteria->addWidget(_buttonTags, 3, 5, 1, 1);
    _gridLayCriteria->addWidget(_labelExtensionCriteria, 4, 0, 1, 1);
    _gridLayCriteria->addWidget(_comboBoxExtensionsCriteria, 4, 1, 1, 4);
    _gridLayCriteria->addWidget(_buttonExtensions, 4, 5, 1, 1);
    _gridLayCriteria->addWidget(_labelPublisherCriteria, 5, 0, 1, 1);
    _gridLayCriteria->addWidget(_comboBoxPublisherCriteria, 5, 1, 1, 4);
    _gridLayCriteria->addWidget(_buttonPublisher, 5, 5, 1, 1);
    _gridLayCriteria->addWidget(_labelDatePublishedCriteria, 6, 0, 1, 1);
    _gridLayCriteria->addWidget(_comboBoxDatePublishedCriteria, 6, 1, 1, 4);
    _gridLayCriteria->addWidget(_buttonDatePublished, 6, 5, 1, 1);
    _gridLayCriteria->addWidget(_labelSeriesCriteria, 7, 0, 1, 1);
    _gridLayCriteria->addWidget(_comboBoxSeriesCriteria, 7, 1, 1, 4);
    _gridLayCriteria->addWidget(_buttonSeries, 7, 5, 1, 1);
    _gridLayCriteria->addWidget(_labelRatingCriteria, 8, 0, 1, 1);
    _gridLayCriteria->addWidget(_comboBoxRatingCriteria, 8, 1, 1, 4);
    _gridLayCriteria->addWidget(_buttonRating, 8, 5, 1, 1);
    _gridLayCriteria->addWidget(_labelStatusCriteria, 9, 0, 1, 1);
    _gridLayCriteria->addWidget(_comboBoxStatusCriteria, 9, 1, 1, 4);
    _gridLayCriteria->addWidget(_buttonStatus, 9, 5, 1, 1);
    _gridLayCriteria->addWidget(_labelSizeCriteria, 10, 0, 1, 1);
    _gridLayCriteria->addWidget(_spinBoxFromSizeCriteria, 10, 1, 1, 1);
    _gridLayCriteria->addWidget(_labelSizeToCriteria, 10, 2, 1, 2);
    _gridLayCriteria->addWidget(_spinBoxToSizeCriteria, 10, 4, 1, 1);
    _gridLayCriteria->addWidget(_buttonSizeCriteria, 10, 5, 1, 1);
    _gridLayCriteria->addWidget(_labelPagesCriteria, 11, 0, 1, 1);
    _gridLayCriteria->addWidget(_spinBoxFromPagesCriteria, 11, 1, 1, 1);
    _gridLayCriteria->addWidget(_labelPagesToCriteria, 11, 2, 1, 2);
    _gridLayCriteria->addWidget(_spinBoxToPagesCriteria, 11, 4, 1, 1);
    _gridLayCriteria->addWidget(_labelSearchCriteria, 12, 0, 1, 1);
    _gridLayCriteria->addWidget(_comboBoxSearchLoad, 12, 1, 1, 4);
    _gridLayCriteria->addWidget(_buttonSearchLoad, 12, 5, 1, 1);

    _frameGridLayCriteria->setLayout(_gridLayCriteria);
    _scrollAreaCriteria->setWidget(_frameGridLayCriteria);

    _horLayCriteria->addWidget(_scrollAreaCriteria);
    _horLayCriteria->addLayout(_vertLaySearchCriteria);

    _vertLayMainLeft->addWidget(_frameSearchCriteria);

    _frameBottomButtons = new QFrame(_frameMainLeft);
    _frameBottomButtons->setObjectName("frameBottomButtons");
    _frameBottomButtons->setStyleSheet(QString("QFrame#frameBottomButtons { border: none; }"));
    _frameBottomButtons->setLineWidth(0);

    _horLayBottomButtons = new QHBoxLayout(_frameBottomButtons);
    _horLayBottomButtons->setContentsMargins(0, 0, 0, 0);
    _frameAddButtons = new QFrame(_frameBottomButtons);
    _frameAddButtons->setFrameShape(QFrame::StyledPanel);
    _frameAddButtons->setFrameShadow(QFrame::Raised);

    _horLayAddButtons = new QHBoxLayout(_frameAddButtons);
    _horLayAddButtons->setSpacing(0);

    _buttonAddBook = new QToolButton(_frameAddButtons);
    _buttonAddBook->setMinimumSize(QSize(0, 50));
    _buttonAddBook->setMaximumWidth(80);
    _buttonAddBook->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonAddBook->setIcon(QIcon(":/icons/add_book_icon.png"));
    _buttonAddBook->setIconSize(QSize(41, 37));

    _buttonAddBooks = new QToolButton(_frameAddButtons);
    _buttonAddBooks->setMinimumSize(QSize(0, 50));
    _buttonAddBooks->setMaximumWidth(80);
    _buttonAddBooks->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonAddBooks->setIcon(QIcon(":/icons/add_books_icon.png"));
    _buttonAddBooks->setIconSize(QSize(41, 44));

    _frameInfoButtons = new QFrame(_frameBottomButtons);
    _frameInfoButtons->setFrameShape(QFrame::StyledPanel);
    _frameInfoButtons->setFrameShadow(QFrame::Raised);

    _horLayToolButtons = new QHBoxLayout(_frameInfoButtons);

    _buttonSummaries = new QToolButton(_frameInfoButtons);
    _buttonSummaries->setMinimumSize(QSize(0, 50));
    _buttonSummaries->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonSummaries->setIcon(QIcon(":/icons/white_notebook_icon.png"));
    _buttonSummaries->setIconSize(QSize(41, 44));

    _buttonLinkManager = new QToolButton(_frameInfoButtons);
    _buttonLinkManager->setMinimumSize(QSize(0, 50));
    _buttonLinkManager->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonLinkManager->setIcon(QIcon(":/icons/link_icon.png"));
    _buttonLinkManager->setIconSize(QSize(41, 44));

    _buttonDbViewer = new QToolButton(_frameInfoButtons);
    _buttonDbViewer->setMinimumSize(QSize(0, 50));
    _buttonDbViewer->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonDbViewer->setIcon(QIcon(":/icons/dbviewer_icon.png"));
    _buttonDbViewer->setIconSize(QSize(41, 34));

    _horLayAddButtons->addWidget(_buttonAddBook);
    _horLayAddButtons->addWidget(_buttonAddBooks);
    _horLayBottomButtons->addWidget(_frameAddButtons);

    _horLayToolButtons->addWidget(_buttonSummaries);
    _horLayToolButtons->addWidget(_buttonDbViewer);
    _horLayToolButtons->addWidget(_buttonLinkManager);

    _horLayBottomButtons->addWidget(_frameInfoButtons);

    _vertLayMainLeft->addWidget(_frameBottomButtons);

    _vertLayMainLeft->setStretch(0, 6);
    _splitter->addWidget(_frameMainLeft);
    _frameMainRight = new QFrame(_splitter);
    _frameMainRight->setObjectName("frameMainRight");
    QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy3.setHorizontalStretch(6);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(_frameMainRight->sizePolicy().hasHeightForWidth());
    _frameMainRight->setSizePolicy(sizePolicy3);
    _frameMainRight->setBaseSize(QSize(0, 0));
    _frameMainRight->setStyleSheet(QString("QFrame#frameMainRight {border: 0px; }"));

    _vertLayMainRight = new QVBoxLayout(_frameMainRight);
    _vertLayMainRight->setSpacing(6);
    _vertLayMainRight->setContentsMargins(0, 0, 9, 0);

    _frameSearchToolBar = new QFrame(_frameMainRight);
    QSizePolicy retain = _frameSearchToolBar->sizePolicy();
    retain.setRetainSizeWhenHidden(true);
    _frameSearchToolBar->setSizePolicy(retain);
    _frameSearchToolBar->setStyleSheet(QString("QFrame { border: 0px; }"));

    _horLaySearchBar = new QHBoxLayout(_frameSearchToolBar);
    _horLaySearchBar->setSpacing(6);
    _horLaySearchBar->setSizeConstraint(QLayout::SetMinimumSize);
    _horLaySearchBar->setContentsMargins(0, 0, 0, 2);
    _labelSearchBar = new QLabel("Search: ", _frameSearchToolBar);

    _textSearchBar = new QLineEdit(_frameSearchToolBar);
    _textSearchBar->setMaximumSize(QSize(16777215, 26));
    _textSearchBar->setContextMenuPolicy(Qt::DefaultContextMenu);

    _buttonSearchString = new QToolButton(_frameSearchToolBar);
    _buttonSearchString->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonSearchString->setIcon(QIcon(":/icons/search_icon.png"));
    _buttonSearchString->setIconSize(QSize(25, 20));

    _buttonClearSearch = new QToolButton(_frameSearchToolBar);
    _buttonClearSearch->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonClearSearch->setIcon(QIcon(":/icons/clear_search_white_icon.png"));
    _buttonClearSearch->setIconSize(QSize(25, 20));

    _buttonSortSearch = new QToolButton(_frameSearchToolBar);
    _buttonSortSearch->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonSortSearch->setIcon(QIcon(":/icons/sort_icon.png"));
    _buttonSortSearch->setIconSize(QSize(25, 20));

    _horLaySearchBar->addWidget(_labelSearchBar);
    _horLaySearchBar->addWidget(_textSearchBar);
    _horLaySearchBar->addWidget(_buttonSearchString);
    _horLaySearchBar->addWidget(_buttonClearSearch);
    _horLaySearchBar->addWidget(_buttonSortSearch);

    _vertLayMainRight->addWidget(_frameSearchToolBar);

    _ebooksListWidget = new QListWidget(_frameMainRight);
    _ebooksListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    _ebooksListWidget->setSortingEnabled(true);
    _ebooksListWidget->setAlternatingRowColors(false);
    _ebooksListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    _ebooksListWidget->setModelColumn(0);
    _ebooksListWidget->setSelectionRectVisible(false);

    _vertLayMainRight->addWidget(_ebooksListWidget);

    _frameDetails = new QFrame(_frameMainRight);
    _frameDetails->setFrameShape(QFrame::StyledPanel);
    _frameDetails->setFrameShadow(QFrame::Raised);

    _gridLayDetails = new QGridLayout(_frameDetails);
    _vertLayDetails = new QVBoxLayout();
    _vertLayDetails->setSpacing(30);
    _vertLayDetails->setContentsMargins(-1, 6, -1, 9);

    _gridLayDetailsFields = new QGridLayout();
    _gridLayDetailsFields->setSizeConstraint(QLayout::SetNoConstraint);
    _gridLayDetailsFields->setHorizontalSpacing(12);
    _gridLayDetailsFields->setVerticalSpacing(20);

    _labelDetailsSize = new QLabel("Size: ", _frameDetails);
    _labelDetailsSize->setMaximumHeight(24);

    _textDetailsGenre = new QLineEdit(_frameDetails);
    _textDetailsGenre->setMinimumSize(QSize(0, 23));
    _textDetailsGenre->setMaximumHeight(24);

    _frameSizeDetails = new QFrame(_frameDetails);
    _frameSizeDetails->setMinimumSize(QSize(32, 0));
    _frameSizeDetails->setMaximumHeight(24);
    _frameSizeDetails->setStyleSheet(QString("QFrame { border: 0px; }"));

    _horLayDetailsSize = new QHBoxLayout(_frameSizeDetails);
    _horLayDetailsSize->setSpacing(7);
    _horLayDetailsSize->setContentsMargins(0, 0, 0, 0);

    _textDetailsSize = new QLineEdit(_frameSizeDetails);
    _textDetailsSize->setEnabled(false);
    _textDetailsSize->setMinimumSize(QSize(0, 23));
    _textDetailsSize->setMaximumHeight(24);

    _buttonSizeUnit = new QPushButton("MB", _frameSizeDetails);
    _buttonSizeUnit->setMinimumSize(QSize(21, 0));
    _buttonSizeUnit->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonSizeUnit->setStyleSheet(QString("border: 0px;"));

    _horLayDetailsSize->addWidget(_textDetailsSize);
    _horLayDetailsSize->addWidget(_buttonSizeUnit);

    _textDetailsTags = new QLineEdit(_frameDetails);
    _textDetailsTags->setMaximumHeight(24);
    _textDetailsTags->setMinimumSize(QSize(0, 23));

    _labelDetailsTags = new QLabel("Tags: ", _frameDetails);
    _labelDetailsTags->setMaximumHeight(24);

    _labelDetailsName = new QLabel("Name: ", _frameDetails);
    _labelDetailsName->setMaximumHeight(24);

    _labelDetailsComments = new QLabel("Comments: ", _frameDetails);
    _labelDetailsComments->setMaximumHeight(24);

    _textEditDetailsComments = new QPlainTextEdit(_frameDetails);
    QPalette p = _textEditDetailsComments->palette();
    p.setColor(QPalette::Text, Qt::white);
    _textEditDetailsComments->setPalette(p);

    _textDetailsAuthor = new QLineEdit(_frameDetails);
    _textDetailsAuthor->setMaximumHeight(24);
    _textDetailsAuthor->setMinimumSize(QSize(0, 23));

    _textDetailsFolder = new QLineEdit(_frameDetails);
    _textDetailsFolder->setMaximumHeight(24);
    _textDetailsFolder->setMinimumSize(QSize(0, 23));

    _textDetailsExt = new QLineEdit(_frameDetails);
    _textDetailsExt->setEnabled(false);
    _textDetailsExt->setMaximumHeight(24);
    _textDetailsExt->setMinimumSize(QSize(0, 23));

    _textDetailsPublisher = new QLineEdit(_frameDetails);
    _textDetailsPublisher->setMaximumHeight(24);
    _textDetailsPublisher->setMinimumSize(QSize(0, 23));

    _textDetailsDatePublished = new QLineEdit(_frameDetails);
    _textDetailsDatePublished->setMaximumHeight(24);
    _textDetailsDatePublished->setMinimumSize(QSize(0, 23));

    _textDetailsSeries = new QLineEdit(_frameDetails);
    _textDetailsSeries->setMaximumHeight(24);
    _textDetailsSeries->setMinimumSize(QSize(0, 23));

    _comboBoxDetailsRating = new RatingComboBox(_frameDetails);
    _comboBoxDetailsRating->setMaximumHeight(24);
    _comboBoxDetailsRating->setMinimumSize(QSize(0, 23));

    _comboBoxDetailsStatus = new StatusComboBox(_frameDetails);
    _comboBoxDetailsStatus->setMaximumHeight(24);
    _comboBoxDetailsStatus->setMinimumSize(QSize(0, 23));

    _labelDetailsStatus = new QLabel("Status: ", _frameDetails);
    _labelDetailsStatus->setMaximumHeight(24);

    _labelDetailsRating = new QLabel("Rating: ", _frameDetails);
    _labelDetailsRating->setMaximumHeight(24);

    _labelDetailsSeries = new QLabel("Series: ", _frameDetails);
    _labelDetailsSeries->setMaximumHeight(24);

    _labelDetailsDatePublished = new QLabel("Published: ", _frameDetails);
    _labelDetailsDatePublished->setMaximumHeight(24);

    _labelDetailsPublisher = new QLabel("Publisher: ", _frameDetails);
    _labelDetailsPublisher->setMaximumHeight(24);

    _labelDetailsExt = new QLabel("Ext: ", _frameDetails);
    _labelDetailsExt->setMaximumHeight(24);

    _labelDetailsPages = new QLabel("Pages: ", _frameDetails);
    _labelDetailsPages->setMaximumHeight(24);

    _labelDetailsGenre = new QLabel("Genre: ", _frameDetails);
    _labelDetailsGenre->setMaximumHeight(24);

    _labelDetailsFolder = new QLabel("Folder: ", _frameDetails);
    _labelDetailsFolder->setMaximumHeight(24);

    _labelDetailsAuthor = new QLabel("Author: ", _frameDetails);
    _labelDetailsAuthor->setMaximumHeight(24);

    _textDetailsName = new QLineEdit(_frameDetails);
    _textDetailsName->setMaximumHeight(24);
    _textDetailsName->setClearButtonEnabled(false);
    _textDetailsName->setMinimumSize(QSize(0, 23));

    _textDetailsPages = new QLineEdit(_frameDetails);
    _textDetailsPages->setMaximumHeight(24);
    _textDetailsPages->setMinimumSize(QSize(0, 23));

    _scrollAreaDetailsFields = new QScrollArea(_frameDetails);
    _scrollAreaDetailsFields->setObjectName("_scrollAreaDetailsFields");
    _scrollAreaDetailsFields->setStyleSheet("QScrollArea#_scrollAreaDetailsFields { border: none; }");
    _scrollAreaDetailsFields->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    _frameDetailsFields = new QFrame(_scrollAreaDetailsFields);
    _frameDetailsFields->setObjectName("_gridLayDetailsFields");
    _frameDetailsFields->setContentsMargins(0, 0, 0, 0);
    _frameDetailsFields->setStyleSheet("QFrame#_gridLayDetailsFields { border: none ; }");

    _gridLayDetailsFields->addWidget(_labelDetailsName, 0, 0, 1, 1);
    _gridLayDetailsFields->addWidget(_textDetailsName, 0, 1, 1, 3);
    _gridLayDetailsFields->addWidget(_labelDetailsAuthor, 1, 0, 1, 1);
    _gridLayDetailsFields->addWidget(_textDetailsAuthor, 1, 1, 1, 1);
    _gridLayDetailsFields->addWidget(_textDetailsExt, 1, 3, 1, 1);
    _gridLayDetailsFields->addWidget(_labelDetailsExt, 1, 2, 1, 1);
    _gridLayDetailsFields->addWidget(_labelDetailsGenre, 2, 0, 1, 1);
    _gridLayDetailsFields->addWidget(_textDetailsGenre, 2, 1, 1, 1);
    _gridLayDetailsFields->addWidget(_labelDetailsPages, 2, 2, 1, 1);
    _gridLayDetailsFields->addWidget(_textDetailsPages, 2, 3, 1, 1);
    _gridLayDetailsFields->addWidget(_labelDetailsFolder, 3, 0, 1, 1);
    _gridLayDetailsFields->addWidget(_textDetailsFolder, 3, 1, 1, 1);
    _gridLayDetailsFields->addWidget(_labelDetailsSize, 3, 2, 1, 1);
    _gridLayDetailsFields->addWidget(_frameSizeDetails, 3, 3, 1, 1);
    _gridLayDetailsFields->addWidget(_labelDetailsTags, 4, 0, 1, 1);
    _gridLayDetailsFields->addWidget(_textDetailsTags, 4, 1, 1, 1);
    _gridLayDetailsFields->addWidget(_labelDetailsPublisher, 4, 2, 1, 1);
    _gridLayDetailsFields->addWidget(_textDetailsPublisher, 4, 3, 1, 1);
    _gridLayDetailsFields->addWidget(_labelDetailsDatePublished, 5, 0, 1, 1);
    _gridLayDetailsFields->addWidget(_textDetailsDatePublished, 5, 1, 1, 1);
    _gridLayDetailsFields->addWidget(_labelDetailsSeries, 5, 2, 1, 1);
    _gridLayDetailsFields->addWidget(_textDetailsSeries, 5, 3, 1, 1);
    _gridLayDetailsFields->addWidget(_labelDetailsRating, 6, 0, 1, 1);
    _gridLayDetailsFields->addWidget(_comboBoxDetailsRating, 6, 1, 1, 1);
    _gridLayDetailsFields->addWidget(_labelDetailsStatus, 6, 2, 1, 1);
    _gridLayDetailsFields->addWidget(_comboBoxDetailsStatus, 6, 3, 1, 1);
    _gridLayDetailsFields->addWidget(_labelDetailsComments, 7, 0, 1, 1);
    _gridLayDetailsFields->addWidget(_textEditDetailsComments, 7, 1, 3, 3);

    _frameDetailsFields->setLayout(_gridLayDetailsFields);
    _scrollAreaDetailsFields->setWidget(_frameDetailsFields);
    _scrollAreaDetailsFields->setWidgetResizable(true);

    _vertLayDetails->addWidget(_scrollAreaDetailsFields);

    _horLayDetailsButtons = new QHBoxLayout();
    _horSpacerButtonsLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horSpacerButtonsRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    _buttonDetailsRestore = new QPushButton("Restore", _frameDetails);
    _buttonDetailsRestore->setMinimumSize(QSize(70, 30));
    _buttonDetailsRestore->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonDetailsRestore->setFlat(true);

    _buttonDetailsUpdate = new QPushButton("Update", _frameDetails);
    _buttonDetailsUpdate->setMinimumSize(QSize(70, 30));
    _buttonDetailsUpdate->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonDetailsUpdate->setFlat(true);

    _buttonDetailsClear = new QPushButton("Clear", _frameDetails);
    _buttonDetailsClear->setMinimumSize(QSize(70, 30));
    _buttonDetailsClear->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonDetailsClear->setFlat(true);

    _horLayDetailsButtons->addItem(_horSpacerButtonsLeft);
    _horLayDetailsButtons->addWidget(_buttonDetailsRestore);
    _horLayDetailsButtons->addWidget(_buttonDetailsUpdate);
    _horLayDetailsButtons->addWidget(_buttonDetailsClear);
    _horLayDetailsButtons->addItem(_horSpacerButtonsRight);

    _vertLayDetails->addLayout(_horLayDetailsButtons);
    _vertLayDetails->setStretch(1, 1);

    _gridLayDetails->addLayout(_vertLayDetails, 0, 0, 1, 1);

    _vertLayMainRight->addWidget(_frameDetails);

    _vertLayMainRight->setStretch(1, 30);
    _vertLayMainRight->setStretch(2, 21);
    _splitter->addWidget(_frameMainRight);
    _horLayMain->addWidget(_splitter);
    _horLayCentral->addWidget(_frameMain);

    // Actions
    setupActions();

    // Menubar
    _menubar = new QMenuBar(this);
    _menubar->setGeometry(QRect(0, 0, 1190, 21));
    setMenuBar(_menubar);

    // Statusbar
    _statusBar = new QStatusBar(this);
    setStatusBar(_statusBar);

    // Menus
    setupMenus();

    // Central Widget
    setCentralWidget(_centralWidget);

    // Refresh
    refreshAll();
    refreshSearches();
}

void MainWindow::setupActions() {
    // File Menu
    _actionClose = new QAction("Close", this);
    // --> Goto Menu
    _actionOpenDB = new QAction("Open Database", this);
    _actionEbookReportsDir = new QAction("Ebook Reports", this);
    _actionUsageReportsDir = new QAction("Usage Reports", this);
    _actionInstallationDir = new QAction("Installation", this);

    // View Menu
    _actionFullscreen = new QAction("Fullscreen", this);
    _actionFullscreen->setIcon(QIcon(":/icons/fullscreen_icon.png"));
    _actionMaximize = new QAction("Maximize", this);
    _actionMinimize = new QAction("Minimize", this);
    _actionMinimize->setIcon(QIcon(":/icons/minimize_icon.png"));
    _actionWindowTop = new QAction("Window on Top", this);
    _actionMinimizeTray = new QAction("Minimize to Tray", this);
    _actionHideSearchBar = new QAction("Hide Search Bar", this);
    _actionHideListWidget = new QAction("Hide List Widget", this);
    _actionHideDetailsSection = new QAction("Hide Details Section", this);
    _actionHideRightFrame = new QAction("Hide Right Frame", this);
    _actionHideUtilities = new QAction("Hide Utilities Section", this);
    _actionHideLeftFrame = new QAction("Hide Left Frame", this);
    _actionHideStatusBar = new QAction("Hide Status Strip", this);

    // Add Menu
    _actionAddBook = new QAction("Add Books", this);
    _actionAddBook->setIcon(QIcon(":/icons/add_book_icon.png"));
    _actionAddBooks = new QAction("Add Books", this);
    _actionAddBooks->setIcon(QIcon(":/icons/add_books_icon.png"));

    // Reset Menu
    _actionResetEbooks = new QAction("Reset Ebooks", this);
    _actionResetTags = new QAction("Reset Tags", this);
    _actionResetSearches = new QAction("Reset Searches", this);
    _actionResetSummaries = new QAction("Reset Summaries", this);
    _actionResetDatabase = new QAction("Reset Database", this);
    _actionResetDatabase->setIcon(QIcon(":/icons/reset_database_icon.png"));
    _actionClearEntries = new QAction("Clear Criteria", this);
    _actionClearEntries->setIcon(QIcon(":/icons/clear_icon.png"));

    // Clean Menu
    _actionCleanEbooks = new QAction("Clean Ebooks", this);
    _actionCleanEbooks->setIcon(QIcon(":/icons/clean_icon.png"));

    // Search Menu
    _actionSearchFiles = new QAction("Search Criteria", this);
    _actionSearchFiles->setIcon(QIcon(":/icons/search_icon.png"));
    _actionSearchText = new QAction("Search Text", this);
    _actionSearchText->setIcon(QIcon(":/icons/searchbar_icon.png"));
    _actionSortSearch = new QAction("Sort Entries", this);
    _actionSortSearch->setIcon(QIcon(":/icons/sort_icon.png"));
    _actionClearSearch = new QAction("Clear Search", this);
    _actionClearSearch->setIcon(QIcon(":/icons/clear_search_white_icon.png"));

    // Tools Menu
    _actionSummaries = new QAction("Summaries", this);
    _actionSummaries->setIcon(QIcon(":/icons/white_notebook_icon.png"));
    _actionLinkManager = new QAction("Link Manager", this);
    _actionLinkManager->setIcon(QIcon(":/icons/link_icon.png"));
    _actionDataViewer = new QAction("Data Viewer", this);
    _actionDataViewer->setIcon(QIcon(":/icons/dbviewer_icon.png"));
    _actionBookDetails = new QAction("MetaData", this);
    _actionBookDetails->setIcon(QIcon(":/icons/stack_icon.png"));

    // Generate Menu
    _actionEbooksReport = new QAction("Ebooks Report", this);
    _actionEbooksReport->setIcon(QIcon(":/icons/ebooks_report_icon.png"));
    _actionUsageReport = new QAction("Usage Report", this);
    _actionUsageReport->setIcon(QIcon(":/icons/metrics_icon.png"));
    _actionChooseRandomBook = new QAction("Random Book", this);

    // Settings Menu
    _actionApplicationSettings = new QAction("Application Settings", this);
    _actionApplicationSettings->setIcon(QIcon(":/icons/settings_icon.png"));
}

void MainWindow::setupMenus() {
    _menuFile = _menubar->addMenu(tr("&File"));
    _menuAdd = _menubar->addMenu(tr("&Add"));
    _menuReset = _menubar->addMenu(tr("&Reset"));
    _menuClean = _menubar->addMenu(tr("&Clean"));
    _menuSearch = _menubar->addMenu(tr("&Search"));
    _menuGenerate = _menubar->addMenu(tr("&Generate"));
    _menuSettings = _menubar->addMenu(tr("&Settings"));
    _menuView = _menubar->addMenu(tr("&View"));
    _menuTools = _menubar->addMenu(tr("&Tools"));

    _menuGoTo = new QMenu(tr("Go To"), _menuFile);

    _menubar->addAction(_menuFile->menuAction());
    _menubar->addAction(_menuView->menuAction());
    _menubar->addAction(_menuAdd->menuAction());
    _menubar->addAction(_menuReset->menuAction());
    _menubar->addAction(_menuClean->menuAction());
    _menubar->addAction(_menuSearch->menuAction());
    _menubar->addAction(_menuTools->menuAction());
    _menubar->addAction(_menuGenerate->menuAction());
    _menubar->addAction(_menuSettings->menuAction());
    _menuFile->addSeparator();
    _menuFile->addAction(_menuGoTo->menuAction());
    _menuFile->addAction(_actionOpenDB);
    _menuFile->addSeparator();
    _menuFile->addAction(_actionClose);
    _menuGoTo->addAction(_actionInstallationDir);
    _menuGoTo->addAction(_actionEbookReportsDir);
    _menuGoTo->addAction(_actionUsageReportsDir);
    _menuAdd->addAction(_actionAddBook);
    _menuAdd->addAction(_actionAddBooks);
    _menuReset->addAction(_actionResetEbooks);
    _menuReset->addAction(_actionResetTags);
    _menuReset->addAction(_actionResetSearches);
    _menuReset->addAction(_actionResetSummaries);
    _menuReset->addAction(_actionResetDatabase);
    _menuReset->addSeparator();
    _menuReset->addAction(_actionClearEntries);
    _menuClean->addAction(_actionCleanEbooks);
    _menuSearch->addAction(_actionSearchFiles);
    _menuSearch->addAction(_actionSearchText);
    _menuSearch->addSeparator();
    _menuSearch->addAction(_actionSortSearch);
    _menuSearch->addAction(_actionClearSearch);
    _menuGenerate->addAction(_actionEbooksReport);
    _menuGenerate->addAction(_actionUsageReport);
    _menuGenerate->addSeparator();
    _menuGenerate->addAction(_actionChooseRandomBook);
    _menuSettings->addAction(_actionApplicationSettings);
    _menuView->addAction(_actionFullscreen);
    _menuView->addAction(_actionMaximize);
    _menuView->addAction(_actionMinimize);
    _menuView->addAction(_actionWindowTop);
    _menuView->addAction(_actionMinimizeTray);
    _menuView->addSeparator();
    _menuView->addAction(_actionHideSearchBar);
    _menuView->addAction(_actionHideListWidget);
    _menuView->addAction(_actionHideDetailsSection);
    _menuView->addAction(_actionHideRightFrame);
    _menuView->addSeparator();
    _menuView->addAction(_actionHideUtilities);
    _menuView->addAction(_actionHideLeftFrame);
    _menuView->addSeparator();
    _menuView->addAction(_actionHideStatusBar);
    _menuTools->addAction(_actionSummaries);
    _menuTools->addAction(_actionLinkManager);
    _menuTools->addAction(_actionDataViewer);
    _menuTools->addAction(_actionBookDetails);
}

void MainWindow::setupConnections() {
    connect(_ebooksListWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    connect(_trayIcon, &QSystemTrayIcon::activated, this, &QMainWindow::show);
    connect(_comboBoxSearchLoad, &QComboBox::customContextMenuRequested, this, &MainWindow::showSearchBoxContextMenu);

    connect(_buttonDetailsClear, &QPushButton::clicked, this, &MainWindow::clearDetails);
    connect(_buttonDetailsRestore, &QPushButton::clicked, this, &MainWindow::restoreDetails);
    connect(_buttonDetailsUpdate, &QPushButton::clicked, this, &MainWindow::updateDetails);

    connect(_buttonAddBooks, &QToolButton::clicked, this, &MainWindow::showAddBooksDialog);
    connect(_buttonSearchString, &QToolButton::clicked, this, &MainWindow::searchString);
    connect(_buttonClearCriteria, &QToolButton::clicked, this, &MainWindow::clearCriteria);
    connect(_buttonSortSearch, &QToolButton::clicked, this, &MainWindow::sortSearch);
    connect(_buttonAddBook, &QToolButton::clicked, this, &MainWindow::showAddBookDialog);
    connect(_buttonClearSearch, &QToolButton::clicked, this, &MainWindow::clearSearch);
    connect(_buttonSearchCriteria, &QToolButton::clicked, this, &MainWindow::searchCriteria);
    connect(_buttonSaveCriteria, &QToolButton::clicked, this, &MainWindow::saveCriteria);
    connect(_buttonSearchLoad, &QToolButton::clicked, this, &MainWindow::loadSearch);
    connect(_buttonLinkManager, &QToolButton::clicked, this, &MainWindow::showLinkManager);
    connect(_buttonDbViewer, &QToolButton::clicked, this, &MainWindow::showDbViewer);
    connect(_buttonSizeCriteria, &QToolButton::clicked, this, &MainWindow::toggleSizeCriteria);
    connect(_buttonExtensions, &QToolButton::clicked, this, &MainWindow::selectExtensions);
    connect(_buttonFolder, &QToolButton::clicked, this, &MainWindow::selectFolders);
    connect(_buttonAuthor, &QToolButton::clicked, this, &MainWindow::selectAuthors);
    connect(_buttonGenre, &QToolButton::clicked, this, &MainWindow::selectGenres);
    connect(_buttonTags, &QToolButton::clicked, this, &MainWindow::selectTags);
    connect(_buttonPublisher, &QToolButton::clicked, this, &MainWindow::selectPublishers);
    connect(_buttonDatePublished, &QToolButton::clicked, this, &MainWindow::selectDatePublished);
    connect(_buttonSeries, &QToolButton::clicked, this, &MainWindow::selectSeries);
    connect(_buttonRating, &QToolButton::clicked, this, &MainWindow::selectRatings);
    connect(_buttonStatus, &QToolButton::clicked, this, &MainWindow::selectStatus);

    connect(_buttonSizeUnit, &QToolButton::clicked, this, &MainWindow::toggleSizeUnit);
    connect(_buttonSummaries, &QToolButton::clicked, [] { showSummary(""); });

    connect(_ebooksListWidget, &QListWidget::itemClicked, this, &MainWindow::showEbookDetails);
    connect(_ebooksListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::openEbook);
    connect(_ebooksListWidget, &QListWidget::itemActivated, this, &MainWindow::openEbook);
    connect(_ebooksListWidget, &QListWidget::itemSelectionChanged, [this] {
        if (_ebooksListWidget->count() > 0) {
            showEbookDetails(_ebooksListWidget->currentItem());
        }
    });

    connect(_comboBoxSearchLoad, &QComboBox::currentTextChanged, this, &MainWindow::loadSearch);
    connect(_textSearchBar, &QLineEdit::returnPressed, this, &MainWindow::searchString);
    connect(_textSearchBar, &QLineEdit::textChanged, this, &MainWindow::searchString);

    connect(_textDetailsName, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
    connect(_textDetailsAuthor, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
    connect(_textDetailsGenre, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
    connect(_textDetailsFolder, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
    connect(_textDetailsPages, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);
    connect(_textDetailsTags, &QLineEdit::returnPressed, this, &MainWindow::updateDetails);

    connect(_actionBookDetails, &QAction::triggered, [] { showBookDetailsWindow(""); });
    connect(_actionClearEntries, &QAction::triggered, this, &MainWindow::clearCriteria);
    connect(_actionAddBook, &QAction::triggered, this, &MainWindow::showAddBookDialog);
    connect(_actionAddBooks, &QAction::triggered, this, &MainWindow::showAddBooksDialog);
    connect(_actionSearchFiles, &QAction::triggered, this, &MainWindow::searchCriteria);
    connect(_actionSearchText, &QAction::triggered, this, &MainWindow::searchString);
    connect(_actionSortSearch, &QAction::triggered, this, &MainWindow::sortSearch);
    connect(_actionResetEbooks, &QAction::triggered, this, &MainWindow::resetEbooks);
    connect(_actionHideSearchBar, &QAction::triggered, this, &MainWindow::hideSearchBar);
    connect(_actionHideListWidget, &QAction::triggered, this, &MainWindow::hideListWidget);
    connect(_actionHideDetailsSection, &QAction::triggered, this, &MainWindow::hideDetailsSection);
    connect(_actionHideRightFrame, &QAction::triggered, this, &MainWindow::hideRightFrame);
    connect(_actionHideUtilities, &QAction::triggered, this, &MainWindow::hideUtilities);
    connect(_actionHideLeftFrame, &QAction::triggered, this, &MainWindow::hideLeftFrame);
    connect(_actionCleanEbooks, &QAction::triggered, this, &MainWindow::showCleanEbooksDialog);
    connect(_actionChooseRandomBook, &QAction::triggered, this, &MainWindow::chooseRandomBook);
    connect(_actionFullscreen, &QAction::triggered, [this] { common::toggleFullscreen(this); });
    connect(_actionResetSummaries, &QAction::triggered, this, &MainWindow::resetSummaries);
    connect(_actionResetTags, &QAction::triggered, this, &MainWindow::resetTags);
    connect(_actionResetSearches, &QAction::triggered, this, &MainWindow::resetSearches);
    connect(_actionResetDatabase, &QAction::triggered, this, &MainWindow::resetDb);
    connect(_actionWindowTop, &QAction::triggered, this, &MainWindow::toggleWindowOnTop);
    connect(_actionLinkManager, &QAction::triggered, this, &MainWindow::showLinkManager);
    connect(_actionDataViewer, &QAction::triggered, this, &MainWindow::showDbViewer);
    connect(_actionApplicationSettings, &QAction::triggered, this, &MainWindow::showSettings);
    connect(_actionEbooksReport, &QAction::triggered, this, &MainWindow::genEbooksReport);
    connect(_actionHideStatusBar, &QAction::triggered, this, &MainWindow::hideStatusBar);
    connect(_actionClearSearch, &QAction::triggered, this, &MainWindow::clearSearch);
    connect(_actionMinimize, &QAction::triggered, this, &MainWindow::showMinimized);
    connect(_actionMaximize, &QAction::triggered, [this] { common::toggleMaximized(this); });
    connect(_actionClose, &QAction::triggered, this, &MainWindow::close);
    connect(_actionSummaries, &QAction::triggered, [] { showSummary(""); });
    connect(_actionMinimizeTray, &QAction::triggered, [this] {
        QTimer::singleShot(0, this, SLOT(hide()));
    });
    connect(_actionOpenDB, &QAction::triggered, [] {
        QDesktopServices::openUrl(QUrl::fromLocalFile("./database.db"));
    });
    connect(_actionEbookReportsDir, &QAction::triggered, [] {
        QDesktopServices::openUrl(QUrl::fromLocalFile("./reports/ebooks/"));
    });
    connect(_actionUsageReportsDir, &QAction::triggered, [] {
        QDesktopServices::openUrl(QUrl::fromLocalFile("./reports/usage/"));
    });
    connect(_actionInstallationDir, &QAction::triggered, [] {
        QDesktopServices::openUrl(QUrl::fromLocalFile("./"));
    });
}

void MainWindow::setupTabOrder() {
    QWidget::setTabOrder(_textSearchBar, _ebooksListWidget);
    QWidget::setTabOrder(_ebooksListWidget, _textDetailsName);
    QWidget::setTabOrder(_textDetailsName, _textDetailsAuthor);
    QWidget::setTabOrder(_textDetailsAuthor, _textDetailsExt);
    QWidget::setTabOrder(_textDetailsExt, _textDetailsGenre);
    QWidget::setTabOrder(_textDetailsGenre, _textDetailsPages);
    QWidget::setTabOrder(_textDetailsPages, _textDetailsFolder);
    QWidget::setTabOrder(_textDetailsFolder, _textDetailsSize);
    QWidget::setTabOrder(_textDetailsSize, _buttonSizeUnit);
    QWidget::setTabOrder(_buttonSizeUnit, _textDetailsTags);
    QWidget::setTabOrder(_textDetailsTags, _textDetailsPublisher);
    QWidget::setTabOrder(_textDetailsPublisher, _textDetailsDatePublished);
    QWidget::setTabOrder(_textDetailsDatePublished, _textDetailsSeries);
    QWidget::setTabOrder(_textDetailsSeries, _comboBoxDetailsRating);
    QWidget::setTabOrder(_comboBoxDetailsRating, _comboBoxDetailsStatus);
    QWidget::setTabOrder(_comboBoxDetailsStatus, _buttonDetailsUpdate);
    QWidget::setTabOrder(_buttonDetailsUpdate, _buttonDetailsRestore);
    QWidget::setTabOrder(_buttonDetailsRestore, _buttonDetailsClear);
    QWidget::setTabOrder(_buttonDetailsClear, _comboBoxFolderCriteria);
    QWidget::setTabOrder(_comboBoxFolderCriteria, _buttonFolder);
    QWidget::setTabOrder(_buttonFolder, _comboBoxAuthorCriteria);
    QWidget::setTabOrder(_comboBoxAuthorCriteria, _buttonAuthor);
    QWidget::setTabOrder(_buttonAuthor, _comboBoxGenreCriteria);
    QWidget::setTabOrder(_comboBoxGenreCriteria, _buttonGenre);
    QWidget::setTabOrder(_buttonGenre, _comboBoxTagsCriteria);
    QWidget::setTabOrder(_comboBoxTagsCriteria, _buttonTags);
    QWidget::setTabOrder(_buttonTags, _comboBoxExtensionsCriteria);
    QWidget::setTabOrder(_comboBoxExtensionsCriteria, _buttonExtensions);
    QWidget::setTabOrder(_buttonExtensions, _comboBoxPublisherCriteria);
    QWidget::setTabOrder(_comboBoxPublisherCriteria, _buttonPublisher);
    QWidget::setTabOrder(_buttonPublisher, _comboBoxDatePublishedCriteria);
    QWidget::setTabOrder(_comboBoxDatePublishedCriteria, _buttonDatePublished);
    QWidget::setTabOrder(_buttonDatePublished, _comboBoxSeriesCriteria);
    QWidget::setTabOrder(_comboBoxSeriesCriteria, _buttonSeries);
    QWidget::setTabOrder(_buttonSeries, _comboBoxRatingCriteria);
    QWidget::setTabOrder(_comboBoxRatingCriteria, _buttonRating);
    QWidget::setTabOrder(_buttonRating, _comboBoxStatusCriteria);
    QWidget::setTabOrder(_comboBoxStatusCriteria, _buttonStatus);
    QWidget::setTabOrder(_buttonStatus, _spinBoxFromSizeCriteria);
    QWidget::setTabOrder(_spinBoxFromSizeCriteria, _spinBoxToSizeCriteria);
    QWidget::setTabOrder(_spinBoxToSizeCriteria, _buttonSizeCriteria);
    QWidget::setTabOrder(_buttonSizeCriteria, _spinBoxFromPagesCriteria);
    QWidget::setTabOrder(_spinBoxFromPagesCriteria, _spinBoxToPagesCriteria);
    QWidget::setTabOrder(_spinBoxToPagesCriteria, _buttonSearchCriteria);
    QWidget::setTabOrder(_buttonSearchCriteria, _buttonClearCriteria);
    QWidget::setTabOrder(_buttonClearCriteria, _buttonSaveCriteria);
    QWidget::setTabOrder(_buttonSaveCriteria, _comboBoxSearchLoad);
    QWidget::setTabOrder(_comboBoxSearchLoad, _buttonSearchLoad);
    QWidget::setTabOrder(_buttonSearchLoad, _buttonAddBook);
    QWidget::setTabOrder(_buttonAddBook, _buttonAddBooks);
    QWidget::setTabOrder(_buttonAddBooks, _buttonSummaries);
    QWidget::setTabOrder(_buttonSummaries, _buttonDbViewer);
    QWidget::setTabOrder(_buttonDbViewer, _buttonLinkManager);
}

void MainWindow::setupShortcuts() {
    // File Menu
    _actionClose->setShortcut(QKeySequence("Ctrl+Q"));

    // View Menu
    _actionFullscreen->setShortcut(QKeySequence("Ctrl+F, Ctrl+S"));
    _actionMaximize->setShortcut(QKeySequence("Ctrl+M, Ctrl+X"));
    _actionMinimize->setShortcut(QKeySequence("Ctrl+M, Ctrl+N"));
    _actionHideSearchBar->setShortcut(QKeySequence("Ctrl+S, Ctrl+B"));
    _actionHideListWidget->setShortcut(QKeySequence("Ctrl+L, Ctrl+W"));
    _actionHideDetailsSection->setShortcut(QKeySequence("Ctrl+D, Ctrl+S"));
    _actionHideRightFrame->setShortcut(QKeySequence("Ctrl+R, Ctrl+F"));
    _actionHideUtilities->setShortcut(QKeySequence("Ctrl+U, Ctrl+S"));
    _actionHideLeftFrame->setShortcut(QKeySequence("Ctrl+L, Ctrl+F"));
    _actionHideStatusBar->setShortcut(QKeySequence("Ctrl+S, Ctrl+S"));

    // Add Menu
    _actionAddBook->setShortcut(QKeySequence("Ctrl+A, Ctrl+B"));
    _actionAddBooks->setShortcut(QKeySequence("Ctrl+A, Ctrl+M"));

    // Reset Menu
    _actionClearEntries->setShortcut(QKeySequence("Ctrl+Shift+Del"));

    // Search Menu
    _actionSearchText->setShortcut(QKeySequence("Ctrl+S, Ctrl+T"));
    _actionSearchFiles->setShortcut(QKeySequence("Ctrl+S, Ctrl+C"));

    _actionSortSearch->setShortcut(QKeySequence("Ctrl+S, Ctrl+E"));
    _actionClearSearch->setShortcut(QKeySequence("Shift+Alt+Del"));

    // Tools Menu
    _actionSummaries->setShortcut(QKeySequence("Ctrl+O, Ctrl+S"));
    _actionLinkManager->setShortcut(QKeySequence("Ctrl+O, Ctrl+L"));
    _actionDataViewer->setShortcut(QKeySequence("Ctrl+O, Ctrl+D"));
    _actionBookDetails->setShortcut(QKeySequence("Ctrl+O, Ctrl+M"));

    // Generate Menu
    _actionEbooksReport->setShortcut(QKeySequence("Ctrl+E, Ctrl+R"));
    _actionUsageReport->setShortcut(QKeySequence("Ctrl+U, Ctrl+R"));
    _actionChooseRandomBook->setShortcut(QKeySequence("Ctrl+Shift+R"));

    // Settings Menu
    _actionApplicationSettings->setShortcut(QKeySequence("Ctrl+A, Ctrl+S"));
}

void MainWindow::showQuote() {
    QuoteDialog dialog(this);
    common::openDialog(&dialog, ":styles/quote.qss");
}

void MainWindow::closeEvent(QCloseEvent *event) {
    queries::logSessionEnd();
    queries::query.clear();
    queries::db.close();
    event->accept();
}

void MainWindow::showBookDetailsWindow(const QString &name) {
    BookMetadataWindow *bookDetailsWindow = new BookMetadataWindow();
    common::openWindow(bookDetailsWindow, ":/styles/style.qss");
    bookDetailsWindow->showBookDetails(name);
    bookDetailsWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showSummary(const QString &name) {
    SummaryWindow *summaryWindow = new SummaryWindow();
    common::openWindow(summaryWindow, ":/styles/summarystyle.qss");
    summaryWindow->selectEbookSummary(name);
    summaryWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showLinkManager() {
    LinkManagerWindow *linkManagerWindow = new LinkManagerWindow();
    common::openWindow(linkManagerWindow, ":/styles/style.qss");
    linkManagerWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showDbViewer() {
    DataViewerWindow *dataViewerWindow = new DataViewerWindow();
    common::openWindow(dataViewerWindow, ":/styles/style.qss");
    dataViewerWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showSettings() {
    //    SettingsWindow *settingsWindow = new SettingsWindow();
    //    common::openWindow(settingsWindow, ":/styles/style.qss");
    //    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::refreshComboBox(QComboBox *comboBox) {
    comboBox->clear();
    comboBox->addItem("");
    while (queries::query.next()) {
        QString value = queries::query.value(0).toString();
        if (value.isEmpty()) {
            continue;
        }
        comboBox->addItem(value);
    }

    comboBox->model()->sort(0, Qt::AscendingOrder);
}

void MainWindow::refreshFolders() {
    queries::selectFoldersQuery();
    refreshComboBox(_comboBoxFolderCriteria);
}

void MainWindow::refreshAuthors() {
    queries::selectAuthorsQuery();
    refreshComboBox(_comboBoxAuthorCriteria);
}

void MainWindow::refreshGenres() {
    queries::selectGenreQuery();
    refreshComboBox(_comboBoxGenreCriteria);
}

void MainWindow::refreshTags() {
    queries::selectTags();
    refreshComboBox(_comboBoxTagsCriteria);
}

void MainWindow::refreshExtensions() {
    queries::selectExt();
    refreshComboBox(_comboBoxExtensionsCriteria);
}

void MainWindow::refreshPublishers() {
    queries::selectPublisherQuery();
    refreshComboBox(_comboBoxPublisherCriteria);
}

void MainWindow::refreshDatePublished() {
    queries::selectDatePublishedQuery();
    refreshComboBox(_comboBoxDatePublishedCriteria);
}

void MainWindow::refreshSeries() {
    queries::selectSeriesQuery();
    refreshComboBox(_comboBoxSeriesCriteria);
}

void MainWindow::refreshStatus() {
    queries::selectStatusQuery();
    _comboBoxStatusCriteria->clear();
    _comboBoxStatusCriteria->addItem("");
    while (queries::query.next()) {
        QString value = StatusComboBox::convertToStatus(queries::query.value(0).toInt());
        if (value.isEmpty()) {
            continue;
        }
        _comboBoxStatusCriteria->addItem(value);
    }
    _comboBoxStatusCriteria->model()->sort(0, Qt::AscendingOrder);
}

void MainWindow::refreshRatings() {
    queries::selectRatingsQuery();
    _comboBoxRatingCriteria->clear();
    _comboBoxRatingCriteria->addItem("");
    while (queries::query.next()) {
        QString value = RatingComboBox::convertToStars(queries::query.value(0).toInt());
        if (value.isEmpty()) {
            continue;
        }
        _comboBoxRatingCriteria->addItem(value);
    }
    _comboBoxRatingCriteria->model()->sort(0, Qt::AscendingOrder);
}

void MainWindow::refreshSearches() {
    queries::selectSearchesQuery();
    refreshComboBox(_comboBoxSearchLoad);
}

void MainWindow::refreshAll() {
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

double MainWindow::changeBookSizeUnit(double size, const QString &unit) {
    size /= _sizeConvFactors[unit];
    return size;
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QPoint globalPos = _ebooksListWidget->mapToGlobal(pos);

    QMenu menu;
    menu.setStyleSheet(common::openSheet(":/styles/style.qss"));

    menu.addAction("Rename", this, [this] {
        editListItem(_ebooksListWidget->currentItem());
    });
    menu.addAction("Open Ebook", this, [this] {
        openEbook(_ebooksListWidget->currentItem());
    });
    menu.addAction("Open Summary", this, SLOT(openSummaryWindow()));
    menu.addAction("Open MetaData", this, [this] {
        showBookDetailsWindow(_ebooksListWidget->currentItem()->text());
    });
    menu.addAction("Show in Folder", this, SLOT(openFolder()));
    menu.addAction("Delete Ebook", this, SLOT(deleteListItem()));

    if (!_ebooksListWidget->selectedItems().isEmpty()) {
        menu.exec(globalPos);
    }
}

void MainWindow::showSearchBoxContextMenu(const QPoint &pos) {
    QPoint globalPos = _comboBoxSearchLoad->mapToGlobal(pos);

    QMenu menu;
    menu.setStyleSheet(common::openSheet(":/styles/style.qss"));

    menu.addAction("Delete Search", this, [this] {
        QString item = _comboBoxSearchLoad->currentText();
        queries::deleteSearch(item);
        refreshSearches();
    });

    menu.exec(globalPos);
}

void MainWindow::openSummaryWindow() {
    showSummary(_ebooksListWidget->currentItem()->text());
}

void MainWindow::deleteListItem() {
    QString itemName = _ebooksListWidget->currentItem()->text();
    QListWidgetItem *currentItem = _ebooksListWidget->currentItem();

    YesNoDialog dialog(this, "Delete File", "Delete File",
                       "Do you wish to delete the file on your hard drive as well?");
    common::openDialog(&dialog, ":/styles/style.qss");
    bool result = dialog.getResult();

    if (result) {
        queries::selectPathBasedonName(itemName);
        queries::query.next();
        QString path = queries::query.value(0).toString();
        QFile file(path);
        file.remove();
    }

    queries::deleteBook(itemName);
    delete (currentItem);

    _statusBar->showMessage(itemName + " deleted.");
}

void MainWindow::openFolder() {
    QString fileName = _ebooksListWidget->currentItem()->text();
    queries::selectPathBasedonName(fileName);
    queries::query.next();
    QString filePath = queries::query.value(0).toString();
    QFileInfo file(filePath);

    if (QOperatingSystemVersion::current().type() == 1) {
        QStringList args;
        args << "/select," << QDir::toNativeSeparators(queries::query.value(0).toString());
        QProcess *process = new QProcess(this);
        process->start("explorer.exe", args);
    } else {
        QDesktopServices::openUrl(QUrl::fromLocalFile((file.dir().path())));
    }
}

void MainWindow::showAddBooksDialog() {
    AddBooksDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");

    refreshAll();
}

void MainWindow::searchString() {
    _ebooksListWidget->clear();
    QString stringToSearch = _textSearchBar->text();
    queries::selectNameBasedOnString(stringToSearch);
    quint32 count = 0;
    while (queries::query.next()) {
        QListWidgetItem *item = new QListWidgetItem(_ebooksListWidget);
        item->setText(queries::query.value(0).toString());
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        _ebooksListWidget->addItem(item);
        count++;
    }

    _statusBar->showMessage("Number of ebooks: " + QString::number(count));
}

void MainWindow::clearCriteria() {
    _spinBoxFromSizeCriteria->setValue(0);
    _spinBoxFromPagesCriteria->setValue(0);
    _spinBoxToSizeCriteria->setValue(1024);
    _spinBoxToPagesCriteria->setValue(100000);
    _comboBoxGenreCriteria->setCurrentIndex(0);
    _comboBoxAuthorCriteria->setCurrentIndex(0);
    _comboBoxFolderCriteria->setCurrentIndex(0);
    _comboBoxExtensionsCriteria->setCurrentIndex(0);
    _comboBoxPublisherCriteria->setCurrentIndex(0);
    _comboBoxDatePublishedCriteria->setCurrentIndex(0);
    _comboBoxSeriesCriteria->setCurrentIndex(0);
    _comboBoxRatingCriteria->setCurrentIndex(0);
    _comboBoxStatusCriteria->setCurrentIndex(0);
    _comboBoxTagsCriteria->setCurrentIndex(0);
    _ebooksListWidget->clear();
    _statusBar->clearMessage();
}

void MainWindow::clearDetails() {
    _textDetailsAuthor->clear();
    _textDetailsName->clear();
    _textDetailsGenre->clear();
    _textDetailsTags->clear();
    _textDetailsExt->clear();
    _textDetailsSize->clear();
    _textDetailsFolder->clear();
    _textDetailsPages->clear();
    _textDetailsPublisher->clear();
    _textDetailsDatePublished->clear();
    _textDetailsSeries->clear();
    _comboBoxDetailsRating->setCurrentIndex(0);
    _comboBoxDetailsStatus->setCurrentIndex(0);
    _textEditDetailsComments->clear();
    _statusBar->clearMessage();
}

void MainWindow::sortSearch() {
    _SORT = (_SORT == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
    _ebooksListWidget->sortItems(_SORT);
}

void MainWindow::resetEbooks() {
    queries::resetEbooksTableQuery();
    refreshAll();
    _ebooksListWidget->clear();
    _statusBar->showMessage("Ebooks, summaries, and tags have been deleted.");
}

void MainWindow::hideSearchBar() {
    common::changeWidgetVisibility(_frameSearchToolBar, _actionHideSearchBar);
}

void MainWindow::hideDetailsSection() {
    _frameDetails->setHidden(!_frameDetails->isHidden());
    _actionHideDetailsSection->setText(_frameDetails->isHidden() ? "Show Details Section" : "Hide Details Section");
    _frameMainRight->setHidden(_ebooksListWidget->isHidden() && _frameDetails->isHidden());
}

void MainWindow::hideRightFrame() {
    common::changeWidgetVisibility(_frameMainRight, _actionHideRightFrame);
}

void MainWindow::hideUtilities() {
    common::changeWidgetVisibility(_frameBottomButtons, _actionHideUtilities);
}

void MainWindow::hideLeftFrame() {
    common::changeWidgetVisibility(_frameMainLeft, _actionHideLeftFrame);
}

void MainWindow::showAddBookDialog() {
    AddBookDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");
    refreshAll();
}

void MainWindow::clearSearch() {
    _textSearchBar->clear();
    _ebooksListWidget->clear();
    clearDetails();
    _statusBar->clearMessage();
}

void MainWindow::searchCriteria() {
    _ebooksListWidget->clear();
    QString folder = _comboBoxFolderCriteria->currentText();
    QString author = _comboBoxAuthorCriteria->currentText();
    QString genre = _comboBoxGenreCriteria->currentText();
    QString tags = _comboBoxTagsCriteria->currentText();
    quint64 sizeTo = _spinBoxToSizeCriteria->value();
    quint64 sizeFrom = _spinBoxFromSizeCriteria->value();
    QString convUnit = _buttonSizeCriteria->text();
    sizeTo = sizeTo * _sizeConvFactors[convUnit];
    sizeFrom = sizeFrom * _sizeConvFactors[convUnit];
    quint32 pagesTo = _spinBoxToPagesCriteria->value();
    quint32 pagesFrom = _spinBoxFromPagesCriteria->value();
    QString ext = _comboBoxExtensionsCriteria->currentText();
    QString publisher = _comboBoxPublisherCriteria->currentText();
    QString published = _comboBoxDatePublishedCriteria->currentText();
    QString series = _comboBoxSeriesCriteria->currentText();
    // Transform ratings from strings to indices
    QString ratings = _comboBoxRatingCriteria->currentText();
    ratings = convertValuesToIndices(ratings, &RatingComboBox::convertToIndex);
    // Transform status from strings to indices
    QString status = _comboBoxStatusCriteria->currentText();
    status = convertValuesToIndices(status, &StatusComboBox::convertToIndex);

    queries::selectNameBasedOnCriteria(folder, genre, author, tags, ext, pagesFrom, pagesTo, sizeFrom, sizeTo,
                                       publisher, published, series, ratings, status);
    quint32 count = 0;
    while (queries::query.next()) {
        _ebooksListWidget->addItem(queries::query.value(0).toString());
        count++;
    }

    _statusBar->showMessage("Number of ebooks: " + QString::number(count));
}

QString MainWindow::convertValuesToIndices(QString values, std::function<QString(QString)> func) {
    if (values.isEmpty()) {
        return values;
    }
    QStringList valueList = values.split(common::SEP);
    std::transform(valueList.begin(), valueList.end(), valueList.begin(), func);
    return valueList.join(common::SEP);
}

void MainWindow::showEbookDetails(QListWidgetItem *item) {
    QString fileName = item->text();
    queries::selectAllBasedOnName(fileName);
    queries::query.next();
    QString author = queries::query.value("author").toString();
    author = (author == "N/A" ? "" : author);
    QString genre = queries::query.value("genre").toString();
    genre = (genre == "N/A" ? "" : genre);
    QString ext = queries::query.value("ext").toString();
    QString pages = queries::query.value("pages").toString();
    double size = changeBookSizeUnit(queries::query.value("size").toDouble(), _buttonSizeUnit->text());
    QString folder = queries::query.value("folder").toString();
    QString publisher = queries::query.value("publisher").toString();
    publisher = (publisher == "N/A" ? "" : publisher);
    QString datePublished = queries::query.value("date_published").toString();
    datePublished = (datePublished == "N/A" ? "" : datePublished);
    QString series = queries::query.value("series").toString();
    series = (series == "N/A" ? "" : series);
    quint32 rating = queries::query.value("rating").toInt();
    quint32 status = queries::query.value("status").toInt();
    QString comments = queries::query.value("comments").toString();
    comments = (comments == "N/A" ? "" : comments);
    QString tags = queries::selectTagsBasedOnName(fileName);

    _textDetailsName->setText(fileName);
    _textDetailsName->setCursorPosition(0);
    _textDetailsAuthor->setText(author);
    _textDetailsGenre->setText(genre);
    _textDetailsPages->setText(pages);
    _textDetailsExt->setText(ext);
    _textDetailsSize->setText(QString::number(size));
    _textDetailsFolder->setText(folder);
    _textDetailsFolder->setCursorPosition(0);
    _textDetailsTags->setText(tags);
    _textDetailsPublisher->setText(publisher);
    _textDetailsDatePublished->setText(datePublished);
    _textDetailsSeries->setText(series);
    _comboBoxDetailsRating->setCurrentIndex(rating);
    _comboBoxDetailsStatus->setCurrentIndex(status);
    _textDetailsTags->setCursorPosition(0);
    _textEditDetailsComments->setPlainText(comments);
    _statusBar->showMessage("Current selected ebook: " + fileName);
}

void MainWindow::restoreDetails() {
    if (!_ebooksListWidget->selectedItems().isEmpty()) {
        showEbookDetails(_ebooksListWidget->currentItem());
        _statusBar->showMessage("Details restored successfully.");
    }
}

void MainWindow::updateDetails() {
    if (_ebooksListWidget->selectedItems().isEmpty()) {
        return;
    }
    QString oldName = _ebooksListWidget->currentItem()->text();
    QString newName = _textDetailsName->text();
    QString author = _textDetailsAuthor->text();
    QString folder = _textDetailsFolder->text();
    QString genre = _textDetailsGenre->text();
    QString publisher = _textDetailsPublisher->text();
    QString datePublished = _textDetailsDatePublished->text();
    QString series = _textDetailsSeries->text();
    quint32 rating = _comboBoxDetailsRating->currentIndex();
    quint32 status = _comboBoxDetailsStatus->currentIndex();
    QString comments = _textEditDetailsComments->toPlainText();

    QString tags = _textDetailsTags->text().trimmed();
    quint32 pages = _textDetailsPages->text().toUInt();

    queries::selectPathBasedonName(oldName);
    queries::query.next();
    QString path = queries::query.value(0).toString();

    if (newName != oldName) {
        common::renameFile(this, path, newName);
    }
    queries::updateBookQuery(oldName, newName, folder, genre, author, pages, tags, path, publisher, datePublished,
                             series, rating, status, comments);
    _ebooksListWidget->currentItem()->setText(newName);

    refreshAll();
    _statusBar->showMessage("Details updated successfully.");
}

void MainWindow::saveCriteria() {
    QString folder = _comboBoxFolderCriteria->currentText();
    QString author = _comboBoxAuthorCriteria->currentText();
    QString genre = _comboBoxGenreCriteria->currentText();
    QString tags = _comboBoxTagsCriteria->currentText();
    QString ext = _comboBoxExtensionsCriteria->currentText();
    quint32 sizeTo = _spinBoxToSizeCriteria->value();
    quint32 sizeFrom = _spinBoxFromSizeCriteria->value();
    QString sizeIn = _buttonSizeCriteria->text();
    quint32 pagesTo = _spinBoxToPagesCriteria->value();
    quint32 pagesFrom = _spinBoxFromPagesCriteria->value();
    QString publisher = _comboBoxPublisherCriteria->currentText();
    QString datePublished = _comboBoxDatePublishedCriteria->currentText();
    QString series = _comboBoxSeriesCriteria->currentText();
    QString rating = _comboBoxRatingCriteria->currentText();
    QString status = _comboBoxStatusCriteria->currentText();

    GetNameDialog dialog(this, "Search Name", "Please provide a name for the search you want to save:");
    common::openDialog(&dialog, ":/styles/style.qss");
    if (!dialog._name.isEmpty()) {
        queries::insertSearchQuery(dialog._name, folder, author, genre, tags, ext,
                                   sizeFrom, sizeTo, sizeIn, pagesFrom, pagesTo,
                                   publisher, datePublished, series, rating, status);
        _statusBar->showMessage("Search saved successfully.");
    }

    refreshSearches();
}

void MainWindow::loadSearch() {
    if (_comboBoxSearchLoad->currentText().isEmpty()) {
        clearCriteria();
        return;
    }

    QString searchName = _comboBoxSearchLoad->currentText();
    queries::selectSearchCriteriaQuery(searchName);
    queries::query.next();
    _comboBoxFolderCriteria->setCurrentText(queries::query.value("folder").toString());
    _comboBoxAuthorCriteria->setCurrentText(queries::query.value("author").toString());
    _comboBoxGenreCriteria->setCurrentText(queries::query.value("genre").toString());
    _comboBoxTagsCriteria->setCurrentText(queries::query.value("tags").toString());
    _comboBoxExtensionsCriteria->setCurrentText(queries::query.value("ext").toString());
    _spinBoxFromSizeCriteria->setValue(queries::query.value("size_from").toUInt());
    _spinBoxToSizeCriteria->setValue(queries::query.value("size_to").toUInt());
    _buttonSizeCriteria->setText(queries::query.value("size_in").toString());
    _spinBoxFromPagesCriteria->setValue(queries::query.value("pages_from").toUInt());
    _spinBoxToPagesCriteria->setValue(queries::query.value("pages_to").toUInt());
    _comboBoxPublisherCriteria->setCurrentText(queries::query.value("publisher").toString());
    _comboBoxDatePublishedCriteria->setCurrentText(queries::query.value("date_published").toString());
    _comboBoxSeriesCriteria->setCurrentText(queries::query.value("series").toString());
    _comboBoxRatingCriteria->setCurrentText(queries::query.value("rating").toString());
    _comboBoxStatusCriteria->setCurrentText(queries::query.value("status").toString());

    _statusBar->showMessage("Search loaded.");
}

void MainWindow::showCleanEbooksDialog() {
    CleanBooksDialog dialog(this);
    common::openDialog(&dialog, ":/styles/style.qss");
}

void MainWindow::chooseRandomBook() {
    queries::selectCountEbooks();
    queries::query.next();
    quint32 numberOfEbooks = queries::query.value(0).toUInt();
    if (numberOfEbooks != 0) {
        quint32 randomNumber = QRandomGenerator::global()->bounded(numberOfEbooks);
        queries::selectNameBasedOnRowid(randomNumber);
        queries::query.next();
        QString randomName = queries::query.value(0).toString();
        _textSearchBar->setText(randomName);
        _ebooksListWidget->setCurrentRow(0);
        _statusBar->showMessage("Random Book: " + randomName);
    }
}

void MainWindow::openEbook(QListWidgetItem *item) {
    QString ebookName = item->text();
    queries::selectPathBasedonName(ebookName);
    queries::query.next();
    QString path = queries::query.value(0).toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    _statusBar->showMessage(ebookName + " ebook opened.");
}

void MainWindow::toggleSizeCriteria() {
    QString currentText = _buttonSizeCriteria->text();
    _buttonSizeCriteria->setText(_sizeUnits[currentText]);
}

void MainWindow::extSelectionSetup(const QString &title, const QString &prompt, QWidget *widget) {
    QVector<QString> results;
    while (queries::query.next()) {
        QString value = queries::query.value(0).toString();
        if (!results.contains(value)) {
            results.push_back(value);
        }
    }

    ExtSelectionDialog *dialog = new ExtSelectionDialog(this, results, title, prompt);
    common::openDialog(dialog, ":/styles/style.qss");

    results = dialog->getExtVector();
    QString resultString = results.join(common::SEP);

    if (QComboBox *cb = qobject_cast<QComboBox *>(widget)) {
        cb->setCurrentText(resultString);
    } else if (QLineEdit *le = qobject_cast<QLineEdit *>(widget)) {
        le->setText(resultString);
    }
}

void MainWindow::selectExtensions() {
    queries::selectExt();
    extSelectionSetup("Extensions", "Select Available Extensions", _comboBoxExtensionsCriteria);
}

void MainWindow::selectFolders() {
    queries::selectFoldersQuery();
    extSelectionSetup("Folders", "Select Available Folders", _comboBoxFolderCriteria);
}

void MainWindow::selectAuthors() {
    queries::selectAuthorsQuery();
    extSelectionSetup("Authors", "Select Available Authors", _comboBoxAuthorCriteria);
}

void MainWindow::selectGenres() {
    queries::selectGenreQuery();
    extSelectionSetup("Genres", "Select Available Genres", _comboBoxGenreCriteria);
}

void MainWindow::selectPublishers() {
    queries::selectPublisherQuery();
    extSelectionSetup("Publishers", "Select Available Publishers", _comboBoxPublisherCriteria);
}

void MainWindow::selectDatePublished() {
    queries::selectDatePublishedQuery();
    extSelectionSetup("Dates Published", "Select Available Dates", _comboBoxDatePublishedCriteria);
}

void MainWindow::selectSeries() {
    queries::selectSeriesQuery();
    extSelectionSetup("Series", "Select Available Series", _comboBoxSeriesCriteria);
}

void MainWindow::selectStatus() {
    queries::selectStatusQuery();
    QVector<QString> results;
    while (queries::query.next()) {
        results.push_back(StatusComboBox::convertToStatus(queries::query.value(0).toInt()));
    }

    ExtSelectionDialog *dialog = new ExtSelectionDialog(this, results, "Status", "Select Available Status");
    common::openDialog(dialog, ":/styles/style.qss");

    results = dialog->getExtVector();
    QString resultString = results.join(common::SEP);

    _comboBoxStatusCriteria->setCurrentText(resultString);
}

void MainWindow::selectRatings() {
    queries::selectRatingsQuery();
    QVector<QString> results;
    while (queries::query.next()) {
        results.push_back(RatingComboBox::convertToStars(queries::query.value(0).toInt()));
    }

    ExtSelectionDialog *dialog = new ExtSelectionDialog(this, results, "Ratings", "Select Available Ratings");
    common::openDialog(dialog, ":/styles/style.qss");

    results = dialog->getExtVector();
    QString resultString = results.join(common::SEP);

    _comboBoxRatingCriteria->setCurrentText(resultString);
}

void MainWindow::selectTags() {
    QVector<QString> tags;
    queries::selectTags();
    while (queries::query.next()) {
        QString tagString = queries::query.value(0).toString();
        QStringList tagList = tagString.split(common::SEP);
        for (QString &tag: tagList) {
            if (!tags.contains(tag)) {
                tags.push_back(tag);
            }
        }
    }

    ExtSelectionDialog *dialog = new ExtSelectionDialog(this, tags, "Tags", "Select Available Tags");
    common::openDialog(dialog, ":/styles/style.qss");

    tags = dialog->getExtVector();
    QString tagString = tags.join(common::SEP);
    _comboBoxTagsCriteria->setCurrentText(tagString);
}

void MainWindow::toggleSizeUnit() {
    QString currentText = _buttonSizeUnit->text();
    _buttonSizeUnit->setText(_sizeUnits[currentText]);
    if (!_ebooksListWidget->selectedItems().isEmpty()) {
        showEbookDetails(_ebooksListWidget->currentItem());
    }
}

void MainWindow::resetSummaries() {
    queries::resetSummaries();
    _statusBar->showMessage("All summaries have been deleted.");
}

void MainWindow::resetTags() {
    queries::resetTags();
    _statusBar->showMessage("All tags have been deleted.");
}

void MainWindow::resetSearches() {
    queries::resetSearchesTable();
    refreshSearches();
    _statusBar->showMessage("All searches have been deleted.");
}

void MainWindow::resetDb() {
    resetEbooks();
    resetSearches();
    _statusBar->showMessage("All data has been deleted.");
}

void MainWindow::toggleWindowOnTop() {
    Qt::WindowFlags flags = windowFlags();
    flags ^= Qt::WindowStaysOnTopHint;
    setWindowFlags(flags);
    show();
}

void MainWindow::genEbooksReport() {
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
    while (queries::query.next()) {
        file.write(R"(<tr style="overflow: hidden; text-overflow: ellipsis; word-wrap: break-word;">)");
        for (int i = 0; i < fieldCount; i++) {
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

void MainWindow::hideStatusBar() {
    common::changeWidgetVisibility(_statusBar, _actionHideStatusBar);
}

void MainWindow::hideListWidget() {
    common::changeWidgetVisibility(_ebooksListWidget, _actionHideListWidget);
}

void MainWindow::editListItem(QListWidgetItem *item) {
    QString oldName = item->text();
    _ebooksListWidget->editItem(item);
    QEventLoop loop;
    connect(_ebooksListWidget->itemDelegate(), &QAbstractItemDelegate::closeEditor, &loop, &QEventLoop::quit);
    loop.exec();
    QString newName = item->text();
    if (newName == oldName) {
        return;
    }
    queries::selectPathBasedonName(oldName);
    queries::query.next();
    QString oldPath = queries::query.value(0).toString();
    YesNoDialog dialog(this, "Rename File", "Rename File",
                       "Do you wish to rename the file on your hard drive as well?");
    common::openDialog(&dialog, ":/styles/style.qss");
    bool result = dialog.getResult();
    if (result) {
        QFile file(oldPath);
        QFileInfo info(file);
        QString newPath = info.absolutePath() + "/" + newName + "." + info.suffix();
        file.rename(newPath);
        queries::updateBookPath(oldPath, newPath);
    }
    queries::updateBookName(oldName, newName);
    _textDetailsName->setText(newName);
}
