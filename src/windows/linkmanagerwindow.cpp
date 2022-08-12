#include "include/common.h"
#include "include/queries.h"
#include "include/dialogs/getnamedialog.h"
#include "include/dialogs/insertlinkdialog.h"
#include "include/windows/linkmanagerwindow.h"

#include <QUrl>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
#include <QClipboard>
#include <QStatusBar>
#include <QPushButton>
#include <QApplication>
#include <QListWidgetItem>
#include <QDesktopServices>

LinkManagerWindow::LinkManagerWindow(QWidget* parent) : QMainWindow(parent)
{
	queries::connectToDatabase();

	m_LINKS_SORT = Qt::AscendingOrder;
	m_COLL_SORT = Qt::AscendingOrder;

	setupInterface();
	setupConnections();
	setupTabOrder();
	setupShortcuts();
}

void LinkManagerWindow::setupInterface()
{
	// Window
	setWindowTitle("Link Manager");
	resize(870, 380);

	// Major Frames Left
	m_centralWidget = new QWidget(this);
	m_splitter = new QSplitter(m_centralWidget);
	m_splitter->setObjectName(QString::fromUtf8("m_splitter"));
	m_splitter->setFrameShadow(QFrame::Plain);
	m_splitter->setLineWidth(0);
	m_splitter->setOrientation(Qt::Horizontal);
	m_splitter->setHandleWidth(0);

	m_frameLeft = new QFrame(m_splitter);
	m_frameLeft->setStyleSheet("QFrame { border: none; }");
	m_vertLayLeft = new QVBoxLayout(m_frameLeft);
	m_vertLayLeft->setContentsMargins(-1, -1, 9, -1);

	// Search Bar Collections
	m_frameCollectionsSearch = new QFrame(m_frameLeft);
	m_horLayCollectionsSearch = new QHBoxLayout(m_frameCollectionsSearch);
	m_horLayCollectionsSearch->setSpacing(0);

	m_labelCollectionSearch = new QLabel("Collections: ", m_frameCollectionsSearch);

	m_textCollections = new QLineEdit(m_frameCollectionsSearch);
	m_textCollections->setClearButtonEnabled(true);

	m_buttonSearchCollections = new QToolButton(m_frameCollectionsSearch);
	m_buttonSearchCollections->setMinimumSize(QSize(30, 20));
	m_buttonSearchCollections->setMaximumSize(QSize(30, 20));
	m_buttonSearchCollections->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconSearchCollection(":/icons/search_icon.png");
	m_buttonSearchCollections->setIcon(iconSearchCollection);

	m_buttonClearCollections = new QToolButton(m_frameCollectionsSearch);
	m_buttonClearCollections->setMinimumSize(QSize(30, 20));
	m_buttonClearCollections->setMaximumSize(QSize(30, 20));
	m_buttonClearCollections->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconClearCollections(":/icons/clear_search_white_icon.png");
	m_buttonClearCollections->setIcon(iconClearCollections);

	m_buttonSortCollections = new QToolButton(m_frameCollectionsSearch);
	m_buttonSortCollections->setMinimumSize(QSize(30, 20));
	m_buttonSortCollections->setMaximumSize(QSize(30, 20));
	m_buttonSortCollections->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconSortCollections(":/icons/sort_icon.png");
	m_buttonSortCollections->setIcon(iconSortCollections);

	m_buttonAddCollection = new QToolButton(m_frameCollectionsSearch);
	m_buttonAddCollection->setMinimumSize(QSize(30, 20));
	m_buttonAddCollection->setMaximumSize(QSize(30, 20));
	m_buttonAddCollection->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconAddCollection(":/icons/add_icon.png");
	m_buttonAddCollection->setIcon(iconAddCollection);

	m_horLayCollectionsSearch->addWidget(m_labelCollectionSearch);
	m_horLayCollectionsSearch->addWidget(m_textCollections);
	m_horLayCollectionsSearch->addWidget(m_buttonSearchCollections);
	m_horLayCollectionsSearch->addWidget(m_buttonClearCollections);
	m_horLayCollectionsSearch->addWidget(m_buttonSortCollections);
	m_horLayCollectionsSearch->addWidget(m_buttonAddCollection);

	// Left Frame
	m_vertLayLeft->addWidget(m_frameCollectionsSearch);

	m_listWidgetCollections = new QListWidget(m_frameLeft);
	m_listWidgetCollections->setStyleSheet(QString("border: 1px solid grey;"));
	m_listWidgetCollections->setSortingEnabled(true);
	m_listWidgetCollections->setContextMenuPolicy(Qt::CustomContextMenu);

	m_vertLayLeft->addWidget(m_listWidgetCollections);
	m_vertLayLeft->setSpacing(9);
	m_splitter->addWidget(m_frameLeft);

	// Search Bar Links
	m_frameRight = new QFrame(m_splitter);
	m_frameRight->setStyleSheet("QFrame { border: none; } ");

	m_vertLayRight = new QVBoxLayout(m_frameRight);
	m_vertLayRight->setSpacing(9);
	m_vertLayRight->setContentsMargins(9, -1, 1, -1);
	m_frameLinksSearch = new QFrame(m_frameRight);

	m_horLayLinksSearch = new QHBoxLayout(m_frameLinksSearch);
	m_horLayLinksSearch->setSpacing(0);

	m_labelLinkSearch = new QLabel("Links: ", m_frameLinksSearch);

	m_textLinks = new QLineEdit(m_frameLinksSearch);
	m_textLinks->setClearButtonEnabled(true);

	m_buttonSearchLinks = new QToolButton(m_frameLinksSearch);
	m_buttonSearchLinks->setMinimumSize(QSize(30, 20));
	m_buttonSearchLinks->setMaximumSize(QSize(30, 20));
	m_buttonSearchLinks->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconSearchLinks(":/icons/search_icon.png");
	m_buttonSearchLinks->setIcon(iconSearchLinks);

	m_buttonClearLinks = new QToolButton(m_frameLinksSearch);
	m_buttonClearLinks->setMinimumSize(QSize(30, 20));
	m_buttonClearLinks->setMaximumSize(QSize(30, 20));
	m_buttonClearLinks->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconClearLinks(":/icons/clear_search_white_icon.png");
	m_buttonClearLinks->setIcon(iconClearLinks);

	m_buttonSortLinks = new QToolButton(m_frameLinksSearch);
	m_buttonSortLinks->setMinimumSize(QSize(30, 20));
	m_buttonSortLinks->setMaximumSize(QSize(30, 20));
	m_buttonSortLinks->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconSortLinks(":/icons/sort_icon.png");
	m_buttonSortLinks->setIcon(iconSortLinks);

	m_buttonAddLink = new QToolButton(m_frameLinksSearch);
	m_buttonAddLink->setMinimumSize(QSize(30, 20));
	m_buttonAddLink->setMaximumSize(QSize(30, 20));
	m_buttonAddLink->setCursor(QCursor(Qt::PointingHandCursor));
	QIcon iconAddLink(":/icons/add_icon.png");
	m_buttonAddLink->setIcon(iconAddLink);

	m_horLayLinksSearch->addWidget(m_labelLinkSearch);
	m_horLayLinksSearch->addWidget(m_textLinks);
	m_horLayLinksSearch->addWidget(m_buttonSearchLinks);
	m_horLayLinksSearch->addWidget(m_buttonClearLinks);
	m_horLayLinksSearch->addWidget(m_buttonSortLinks);
	m_horLayLinksSearch->addWidget(m_buttonAddLink);

	m_vertLayRight->addWidget(m_frameLinksSearch);
	// List Widget
	m_listWidgetLinks = new QListWidget(m_frameRight);
	m_listWidgetLinks->setStyleSheet(QString("border: 1px solid grey;"));
	m_listWidgetLinks->setSortingEnabled(true);
	m_listWidgetLinks->setContextMenuPolicy(Qt::CustomContextMenu);
	m_vertLayRight->addWidget(m_listWidgetLinks);

	// Details Frame
	m_frameLinkDetails = new QFrame(m_frameRight);
	m_frameLinkDetails->setObjectName(QString("m_frameLinkDetails"));
	m_frameLinkDetails->setStyleSheet(QString("QFrame#m_frameLinkDetails { border: 1px solid grey; }"));
	m_frameLinkDetails->setFrameShape(QFrame::StyledPanel);
	m_frameLinkDetails->setFrameShadow(QFrame::Raised);
	m_frameLinkDetails->setMaximumHeight(200);

	m_vertLayDetails = new QVBoxLayout(m_frameLinkDetails);
	m_vertLayDetails->setSpacing(15);

	m_gridLayDetails = new QGridLayout();

	m_textDetailsTitle = new QLineEdit(m_frameLinkDetails);
	m_textDetailsTitle->setClearButtonEnabled(true);

	m_labelDetailsTitle = new QLabel("Title", m_frameLinkDetails);

	m_textDetailsUrl = new QLineEdit(m_frameLinkDetails);
	m_textDetailsUrl->setClearButtonEnabled(true);

	m_labelDetailsUrl = new QLabel("URL", m_frameLinkDetails);

	m_labelDetailsComments = new QLabel("Comments", m_frameLinkDetails);

	m_plainTextDetailsComments = new QPlainTextEdit(m_frameLinkDetails);
	m_plainTextDetailsComments->setStyleSheet("QPlainTextEdit{border: 1px solid grey; color: white;}");
	m_plainTextDetailsComments->setMaximumHeight(50);

	m_gridLayDetails->addWidget(m_labelDetailsTitle, 0, 0, 1, 1);
	m_gridLayDetails->addWidget(m_textDetailsTitle, 0, 1, 1, 1);
	m_gridLayDetails->addWidget(m_labelDetailsUrl, 1, 0, 1, 1);
	m_gridLayDetails->addWidget(m_textDetailsUrl, 1, 1, 1, 1);
	m_gridLayDetails->addWidget(m_labelDetailsComments, 2, 0, 1, 1);
	m_gridLayDetails->addWidget(m_plainTextDetailsComments, 2, 1, 1, 1);

	m_vertLayDetails->addLayout(m_gridLayDetails);

	m_horLayBotButtons = new QHBoxLayout();
	m_horSpacerBotButtonsLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_horSpacerBotButtonsRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_buttonDetailsRestore = new QPushButton("Restore", m_frameLinkDetails);
	m_buttonDetailsRestore->setFlat(true);
	m_buttonDetailsRestore->setCursor(Qt::PointingHandCursor);
	m_buttonDetailsRestore->setMinimumSize(QSize(80, 30));
	m_buttonDetailsRestore->setMaximumSize(QSize(80, 30));

	m_buttonDetailsUpdate = new QPushButton("Update", m_frameLinkDetails);
	m_buttonDetailsUpdate->setFlat(true);
	m_buttonDetailsUpdate->setCursor(Qt::PointingHandCursor);
	m_buttonDetailsUpdate->setMinimumSize(QSize(80, 30));
	m_buttonDetailsUpdate->setMaximumSize(QSize(80, 30));

	m_buttonDetailsClear = new QPushButton("Clear", m_frameLinkDetails);
	m_buttonDetailsClear->setFlat(true);
	m_buttonDetailsClear->setCursor(Qt::PointingHandCursor);
	m_buttonDetailsClear->setMinimumSize(QSize(80, 30));
	m_buttonDetailsClear->setMaximumSize(QSize(80, 30));

	m_horLayBotButtons->addItem(m_horSpacerBotButtonsLeft);
	m_horLayBotButtons->addWidget(m_buttonDetailsRestore);
	m_horLayBotButtons->addWidget(m_buttonDetailsUpdate);
	m_horLayBotButtons->addWidget(m_buttonDetailsClear);
	m_horLayBotButtons->addItem(m_horSpacerBotButtonsRight);

	m_vertLayDetails->addLayout(m_horLayBotButtons);
	m_vertLayDetails->setSpacing(9);
	m_vertLayRight->addWidget(m_frameLinkDetails);

	m_splitter->addWidget(m_frameRight);

	// Central
	m_horLayMain = new QHBoxLayout(m_centralWidget);
	m_horLayMain->addWidget(m_splitter);
	setCentralWidget(m_centralWidget);

	// Actions
	setupActions();

	// MenuBar
	m_menuBar = new QMenuBar(this);
	m_menuBar->setGeometry(QRect(0, 0, 867, 21));
	setMenuBar(m_menuBar);

	// StatusBar
	m_statusBar = new QStatusBar(this);
	setStatusBar(m_statusBar);

	// Menus
	setupMenus();

	// Refresh Data
	refreshCollections("");
}

void LinkManagerWindow::setupActions()
{
	// File Menu
	m_actionClose = new QAction("Close", this);

	// View Menu
	m_actionFullscreen = new QAction("Fullscreen", this);
	m_actionMaximize = new QAction("Maximize", this);
	m_actionMinimize = new QAction("Minimize", this);
	m_actionHideLeftPanel = new QAction("Hide Left Panel", this);
	m_actionHideRightPanel = new QAction("Hide Right Panel", this);
	m_actionHideSearchBars = new QAction("Hide Search Bars", this);
	m_actionHideStatusBar = new QAction("Hide Status Bar", this);

	// Reset Menu
	m_actionResetLinks = new QAction("Reset Links", this);
	m_actionResetCollections = new QAction("Reset Collections", this);
	m_actionResetAll = new QAction("Reset All", this);

	// Links Menu
	m_actionAddLink = new QAction("Add Link", this);
	m_actionDeleteLink = new QAction("Delete Link", this);
	m_actionEditLink = new QAction("Edit Link", this);

	// Collections Menu
	m_actionAddCollection = new QAction("Add Collection", this);
	m_actionDeleteCollection = new QAction("Delete Collection", this);
	m_actionRenameCollection = new QAction("Rename Collection", this);
	m_actionOpenAllLinks = new QAction("Open All Links", this);
}

void LinkManagerWindow::setupMenus()
{
	m_menuFile = m_menuBar->addMenu(tr("&File"));
	m_menuView = m_menuBar->addMenu(tr("&View"));
	m_menuReset = m_menuBar->addMenu(tr("&Reset"));
	m_menuLinks = m_menuBar->addMenu(tr("&Links"));
	m_menuCollections = m_menuBar->addMenu(tr("&Collections"));

	// Add Actions to Menus
	m_menuBar->addAction(m_menuFile->menuAction());
	m_menuBar->addAction(m_menuView->menuAction());
	m_menuBar->addAction(m_menuReset->menuAction());
	m_menuBar->addAction(m_menuLinks->menuAction());
	m_menuBar->addAction(m_menuCollections->menuAction());
	m_menuFile->addAction(m_actionClose);
	m_menuView->addAction(m_actionFullscreen);
	m_menuView->addAction(m_actionMaximize);
	m_menuView->addAction(m_actionMinimize);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionHideLeftPanel);
	m_menuView->addAction(m_actionHideRightPanel);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionHideSearchBars);
	m_menuView->addSeparator();
	m_menuView->addAction(m_actionHideStatusBar);
	m_menuLinks->addAction(m_actionAddLink);
	m_menuLinks->addAction(m_actionEditLink);
	m_menuLinks->addAction(m_actionDeleteLink);
	m_menuReset->addAction(m_actionResetLinks);
	m_menuReset->addAction(m_actionResetCollections);
	m_menuCollections->addAction(m_actionAddCollection);
	m_menuCollections->addAction(m_actionDeleteCollection);
	m_menuCollections->addAction(m_actionRenameCollection);
	m_menuCollections->addAction(m_actionOpenAllLinks);
}

void LinkManagerWindow::setupConnections()
{
	connect(m_textLinks, &QLineEdit::textChanged, this, &LinkManagerWindow::searchLinks);
	connect(m_textLinks, &QLineEdit::returnPressed, [this]
	{ searchLinks(m_textLinks->text()); });
	connect(m_textCollections, &QLineEdit::textChanged, this, &LinkManagerWindow::refreshCollections);
	connect(m_textCollections, &QLineEdit::returnPressed, [this]
	{ refreshCollections(m_textCollections->text()); });

	connect(m_listWidgetLinks, &QListWidget::itemActivated, this, &LinkManagerWindow::openLink);
	connect(m_listWidgetLinks, &QListWidget::itemClicked, this, &LinkManagerWindow::linkClicked);
	connect(m_listWidgetLinks, &QListWidget::itemSelectionChanged, this, &LinkManagerWindow::linkSelectionChanged);
	connect(m_listWidgetLinks, &QListWidget::customContextMenuRequested, this,
			&LinkManagerWindow::showLinksContextMenu);
	connect(m_listWidgetCollections, &QListWidget::itemClicked, this, &LinkManagerWindow::collectionClicked);
	connect(m_listWidgetCollections, &QListWidget::customContextMenuRequested, this,
			&LinkManagerWindow::showCollectionsContextMenu);

	connect(m_buttonDetailsClear, &QPushButton::clicked, this, &LinkManagerWindow::clearDetails);
	connect(m_buttonDetailsRestore, &QPushButton::clicked, this, &LinkManagerWindow::restoreDetails);
	connect(m_buttonDetailsUpdate, &QPushButton::clicked, this, &LinkManagerWindow::updateDetails);

	connect(m_buttonAddCollection, &QToolButton::clicked, this, &LinkManagerWindow::addCollection);
	connect(m_buttonAddLink, &QToolButton::clicked, this, &LinkManagerWindow::addLink);
	connect(m_buttonClearCollections, &QToolButton::clicked, this, &LinkManagerWindow::clearCollections);
	connect(m_buttonSortCollections, &QToolButton::clicked, this, &LinkManagerWindow::sortCollections);
	connect(m_buttonClearLinks, &QToolButton::clicked, this, &LinkManagerWindow::clearLinks);
	connect(m_buttonSortLinks, &QToolButton::clicked, this, &LinkManagerWindow::sortLinks);
	connect(m_buttonSearchCollections, &QToolButton::clicked, [this]
	{
		refreshCollections(m_textCollections->text());
	});
	connect(m_buttonSearchLinks, &QToolButton::clicked, [this]
	{
		searchLinks(m_textLinks->text());
	});

	connect(m_actionFullscreen, &QAction::triggered, [this]
	{ common::toggleFullscreen(this); });
	connect(m_actionHideLeftPanel, &QAction::triggered, this, &LinkManagerWindow::hideLeftPanel);
	connect(m_actionHideRightPanel, &QAction::triggered, this, &LinkManagerWindow::hideRightPanel);
	connect(m_actionHideSearchBars, &QAction::triggered, this, &LinkManagerWindow::hideSearchBars);
	connect(m_actionResetLinks, &QAction::triggered, this, &LinkManagerWindow::resetLinks);
	connect(m_actionResetCollections, &QAction::triggered, this, &LinkManagerWindow::resetCollections);
	connect(m_actionDeleteCollection, &QAction::triggered, this, &LinkManagerWindow::actionDeleteCollection);
	connect(m_actionRenameCollection, &QAction::triggered, this, &LinkManagerWindow::actionRenameCollection);
	connect(m_actionOpenAllLinks, &QAction::triggered, this, &LinkManagerWindow::actionOpenAllLinks);
	connect(m_actionDeleteLink, &QAction::triggered, this, &LinkManagerWindow::actionDeleteLink);
	connect(m_actionEditLink, &QAction::triggered, this, &LinkManagerWindow::actionEditLink);
	connect(m_actionClose, &QAction::triggered, this, &LinkManagerWindow::close);
	connect(m_actionMaximize, &QAction::triggered, [this]
	{ common::toggleMaximized(this); });
	connect(m_actionMinimize, &QAction::triggered, this, &LinkManagerWindow::showMinimized);
	connect(m_actionAddCollection, &QAction::triggered, this, &LinkManagerWindow::addCollection);
	connect(m_actionAddLink, &QAction::triggered, this, &LinkManagerWindow::addLink);
	connect(m_actionHideStatusBar, &QAction::triggered, this, &LinkManagerWindow::hideStatusBar);
}

void LinkManagerWindow::setupTabOrder()
{
	QWidget::setTabOrder(m_textCollections, m_buttonSearchCollections);
	QWidget::setTabOrder(m_buttonSearchCollections, m_buttonClearCollections);
	QWidget::setTabOrder(m_buttonClearCollections, m_buttonSortCollections);
	QWidget::setTabOrder(m_buttonSortCollections, m_buttonAddCollection);
	QWidget::setTabOrder(m_buttonAddCollection, m_listWidgetCollections);
	QWidget::setTabOrder(m_listWidgetCollections, m_textLinks);
	QWidget::setTabOrder(m_textLinks, m_buttonSearchLinks);
	QWidget::setTabOrder(m_buttonSearchLinks, m_buttonClearLinks);
	QWidget::setTabOrder(m_buttonClearLinks, m_buttonSortLinks);
	QWidget::setTabOrder(m_buttonSortLinks, m_buttonAddLink);
	QWidget::setTabOrder(m_buttonAddLink, m_listWidgetLinks);
	QWidget::setTabOrder(m_listWidgetLinks, m_textDetailsTitle);
	QWidget::setTabOrder(m_textDetailsTitle, m_textDetailsUrl);
	QWidget::setTabOrder(m_textDetailsUrl, m_plainTextDetailsComments);
	QWidget::setTabOrder(m_plainTextDetailsComments, m_buttonDetailsUpdate);
	QWidget::setTabOrder(m_buttonDetailsUpdate, m_buttonDetailsRestore);
	QWidget::setTabOrder(m_buttonDetailsRestore, m_buttonDetailsClear);
}

void LinkManagerWindow::setupShortcuts()
{
	// File Menu
	m_actionClose->setShortcut(QKeySequence("Ctrl+Q"));

	// View Menu
	m_actionFullscreen->setShortcut(QKeySequence("Ctrl+F, Ctrl+S"));
	m_actionMaximize->setShortcut(QKeySequence("Ctrl+M, Ctrl+X"));
	m_actionMinimize->setShortcut(QKeySequence("Ctrl+M, Ctrl+N"));
	m_actionHideLeftPanel->setShortcut(QKeySequence("Ctrl+L, Ctrl+P"));
	m_actionHideRightPanel->setShortcut(QKeySequence("Ctrl+R, Ctrl+P"));
	m_actionHideSearchBars->setShortcut(QKeySequence("Ctrl+H, Ctrl+S"));
	m_actionHideStatusBar->setShortcut(QKeySequence("Ctrl+S, Ctrl+S"));

	// Links Menu
	m_actionAddLink->setShortcut(QKeySequence("Ctrl+A, Ctrl+L"));
	m_actionEditLink->setShortcut(QKeySequence("Ctrl+E, Ctrl+L"));
	m_actionDeleteLink->setShortcut(QKeySequence("Ctrl+D, Ctrl+L"));

	// Collections Menu
	m_actionAddCollection->setShortcut(QKeySequence("Ctrl+A, Ctrl+C"));
	m_actionDeleteCollection->setShortcut(QKeySequence("Ctrl+D, Ctrl+C"));
	m_actionRenameCollection->setShortcut(QKeySequence("Ctrl+R, Ctrl+C"));
	m_actionOpenAllLinks->setShortcut(QKeySequence("Ctrl+O, Ctrl+A"));
}

void LinkManagerWindow::showLinksContextMenu(const QPoint& pos)
{
	QPoint globalPos = m_listWidgetLinks->mapToGlobal(pos);

	QMenu menu;
	menu.setStyleSheet(common::openSheet(":/styles/style.qss"));
	menu.addAction("Open", this, [this]
	{
		openLink(m_listWidgetLinks->currentItem());
	});
	menu.addAction("Edit", this, [this]
	{
		editLinkDetails(m_listWidgetLinks->currentItem()->text());
	});
	menu.addAction("Delete", this, [this]
	{
		deleteLink(m_listWidgetLinks->currentItem()->text());
	});
	menu.addAction("Copy Link", this, [this]
	{
		copyLink(m_listWidgetLinks->currentItem()->text());
	});
	if (!m_listWidgetLinks->selectedItems().isEmpty())
	{
		menu.exec(globalPos);
	}
}

void LinkManagerWindow::showCollectionsContextMenu(const QPoint& pos)
{
	QPoint globalPos = m_listWidgetCollections->mapToGlobal(pos);
	QMenu menu;
	menu.setStyleSheet(common::openSheet(":/styles/style.qss"));
	menu.addAction("Add Link", this, [this]
	{
		addLink();
	});
	menu.addAction("Rename", this, [this]
	{
		renameCollection(m_listWidgetCollections->currentItem()->text());
	});
	menu.addAction("Delete", this, [this]
	{
		deleteCollection(m_listWidgetCollections->currentItem()->text());
	});
	menu.addAction("Open Links", this, [this]
	{
		openAllLinks();
	});
	if (!m_listWidgetCollections->selectedItems().isEmpty())
	{
		menu.exec(globalPos);
	}
}

void LinkManagerWindow::openAllLinks()
{
	QString currentColl = m_listWidgetCollections->currentItem()->text();
	queries::selectLinksBasedOnCollection(currentColl, "");
	while (queries::query.next())
	{
		QUrl url = QUrl(queries::query.value(1).toString());
		if (url.isValid())
		{
			QDesktopServices::openUrl(url);
		}
	}
}

void LinkManagerWindow::deleteLink(const QString& name)
{
	queries::deleteLink(name);
	delete m_listWidgetLinks->currentItem();
	m_statusBar->showMessage("Link \"" + name + "\" deleted.");
}

void LinkManagerWindow::editLinkDetails(const QString& name)
{
	InsertLinkDialog dialog(this);
	dialog.setWindowTitle("Edit Link");
	dialog.m_title = name;
	queries::selectLinkRecord(name);
	queries::query.next();
	dialog.m_link = queries::query.value(1).toString();
	dialog.setDataOnOpen();
	common::openDialog(&dialog, ":/styles/style.qss");

	queries::updateLinkDetails(name, dialog.m_title, dialog.m_link);
	m_listWidgetLinks->currentItem()->setText(dialog.m_title);
	m_statusBar->showMessage("Link \"" + name + "\" edited.");
}

void LinkManagerWindow::copyLink(const QString& name)
{
	queries::selectLinkRecord(name);
	queries::query.next();

	QClipboard* clipboard = QApplication::clipboard();
	clipboard->setText(queries::query.value(1).toString());
	m_statusBar->showMessage("Link \"" + name + "\" copied.");
}

void LinkManagerWindow::renameCollection(const QString& name)
{
	GetNameDialog dialog(this, "Collection Name", "Please provide a name for the collection you want to create:");

	dialog.m_name = name;
	dialog.setDataOnOpen();

	common::openDialog(&dialog, ":/styles/style.qss");
	queries::updateLinkCollectionName(name, dialog.m_name);
	m_listWidgetCollections->currentItem()->setText(dialog.m_name);
	m_statusBar->showMessage("Collection \"" + name + "\" renamed.");
}

void LinkManagerWindow::deleteCollection(const QString& name)
{
	queries::deleteCollection(name);
	m_listWidgetLinks->clear();
	delete m_listWidgetCollections->currentItem();
	m_statusBar->showMessage("Collection \"" + name + "\" deleted.");
}

void LinkManagerWindow::refreshCollections(const QString& searchString)
{
	m_listWidgetCollections->clear();
	queries::selectCollections(searchString);
	while (queries::query.next())
	{
		m_listWidgetCollections->addItem(queries::query.value(0).toString());
	}
}

void LinkManagerWindow::refreshLinks(const QString& collectionName, const QString& searchString)
{
	m_listWidgetLinks->clear();
	queries::selectLinksBasedOnCollection(collectionName, searchString);
	while (queries::query.next())
	{
		m_listWidgetLinks->addItem(queries::query.value(0).toString());
	}
}

void LinkManagerWindow::addCollection()
{
	GetNameDialog dialog(this, "Collection Name", "Please provide a name for the collection you want to create:");
	common::openDialog(&dialog, ":/styles/style.qss");

	if (!dialog.m_name.isEmpty())
	{
		queries::insertLinkCollection(dialog.m_name);
		m_statusBar->showMessage("New collection \"" + dialog.m_name + "\" added.");
	}

	refreshCollections("");
}

void LinkManagerWindow::addLink()
{
	InsertLinkDialog dialog(this);
	if (!m_listWidgetCollections->selectedItems().isEmpty())
	{
		common::openDialog(&dialog, ":/styles/style.qss");
	}

	if (!dialog.m_link.isEmpty() && !dialog.m_title.isEmpty())
	{
		QString collectionName = m_listWidgetCollections->currentItem()->text();
		int collectionId = queries::selectCollectionId(collectionName);
		queries::insertLink(collectionId, dialog.m_title, dialog.m_link);
		refreshLinks(m_listWidgetCollections->currentItem()->text(), "");
		m_statusBar->showMessage("New link \"" + dialog.m_title + "\" added.");

	}
}

void LinkManagerWindow::searchLinks(const QString& arg1)
{
	if (!m_listWidgetCollections->selectedItems().isEmpty())
	{
		refreshLinks(m_listWidgetCollections->currentItem()->text(), arg1);
	}
}

void LinkManagerWindow::collectionClicked(QListWidgetItem* item)
{
	refreshLinks(item->text(), "");
	m_statusBar->showMessage("Collection \"" + item->text() + "\" selected.");
}

void LinkManagerWindow::openLink(QListWidgetItem* item)
{
	queries::selectLinkRecord(item->text());
	queries::query.next();

	QDesktopServices::openUrl(QUrl(queries::query.value(1).toString()));
}

void LinkManagerWindow::linkClicked(QListWidgetItem* item)
{
	queries::selectLinkRecord(item->text());
	queries::query.next();
	QString title = queries::query.value(0).toString();
	QString url = queries::query.value(1).toString();

	m_textDetailsTitle->setText(title);
	m_textDetailsUrl->setText(url);

	m_statusBar->showMessage(title + ": " + url);
}

void LinkManagerWindow::clearDetails()
{
	m_textDetailsTitle->clear();
	m_textDetailsUrl->clear();
}

void LinkManagerWindow::restoreDetails()
{
	if (!m_listWidgetLinks->selectedItems().isEmpty())
	{
		linkClicked(m_listWidgetLinks->currentItem());
		m_statusBar->showMessage("Details restored successfully.");
	}
}

void LinkManagerWindow::updateDetails()
{
	QString oldTitle = m_listWidgetLinks->currentItem()->text();
	QString newTitle = m_textDetailsTitle->text();
	QString newUrl = m_textDetailsUrl->text();

	queries::updateLinkDetails(oldTitle, newTitle, newUrl);
	m_listWidgetLinks->currentItem()->setText(newTitle);
	m_statusBar->showMessage("Link \"" + oldTitle + "\" edited.");
}

void LinkManagerWindow::linkSelectionChanged()
{
	if (!m_listWidgetLinks->selectedItems().isEmpty())
	{
		linkClicked(m_listWidgetLinks->currentItem());
	}
}

void LinkManagerWindow::clearCollections()
{
	m_textCollections->clear();
	m_listWidgetCollections->clear();
}

void LinkManagerWindow::sortCollections()
{
	m_COLL_SORT = (m_COLL_SORT == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
	m_listWidgetCollections->sortItems(m_COLL_SORT);
}

void LinkManagerWindow::clearLinks()
{
	m_textLinks->clear();
	m_listWidgetLinks->clear();
}

void LinkManagerWindow::sortLinks()
{
	m_LINKS_SORT = (m_LINKS_SORT == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
	m_listWidgetLinks->sortItems(m_LINKS_SORT);
}

void LinkManagerWindow::hideLeftPanel()
{
	common::changeWidgetVisibility(m_frameLeft, m_actionHideLeftPanel);
}

void LinkManagerWindow::hideRightPanel()
{
	common::changeWidgetVisibility(m_frameRight, m_actionHideRightPanel);
}

void LinkManagerWindow::hideSearchBars()
{
	m_frameLinksSearch->setHidden(!m_frameLinksSearch->isHidden());
	m_frameCollectionsSearch->setHidden(!m_frameCollectionsSearch->isHidden());
	m_actionHideSearchBars->setText(m_frameCollectionsSearch->isHidden() ? "Show Search Bars" : "Hide Search Bars");
}

void LinkManagerWindow::resetLinks()
{
	queries::resetLinksTable();
}

void LinkManagerWindow::resetCollections()
{
	queries::resetCollectionsTable();
}

void LinkManagerWindow::actionEditLink()
{
	if (m_listWidgetLinks->currentItem())
	{
		editLinkDetails(m_listWidgetLinks->currentItem()->text());
	}
}

void LinkManagerWindow::actionDeleteCollection()
{
	if (m_listWidgetCollections->currentItem())
	{
		deleteCollection(m_listWidgetCollections->currentItem()->text());
	}
}

void LinkManagerWindow::actionRenameCollection()
{
	if (m_listWidgetCollections->currentItem())
	{
		renameCollection(m_listWidgetCollections->currentItem()->text());
	}
}

void LinkManagerWindow::actionOpenAllLinks()
{
	openAllLinks();
}

void LinkManagerWindow::actionDeleteLink()
{
	if (m_listWidgetLinks->currentItem())
	{
		deleteLink(m_listWidgetLinks->currentItem()->text());
	}
}

void LinkManagerWindow::hideStatusBar()
{
	common::changeWidgetVisibility(m_statusBar, m_actionHideStatusBar);
}
