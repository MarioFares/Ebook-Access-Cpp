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

LinkManagerWindow::LinkManagerWindow(QWidget *parent) : QMainWindow(parent) {
    queries::connectToDatabase();

    _LINKS_SORT = Qt::AscendingOrder;
    _COLL_SORT = Qt::AscendingOrder;

    setupInterface();
    setupConnections();
    setupTabOrder();
    setupShortcuts();
}

void LinkManagerWindow::setupInterface() {
    // Window
    setWindowTitle("Link Manager");
    resize(870, 380);

    // Major Frames Left
    _centralWidget = new QWidget(this);
    _splitter = new QSplitter(_centralWidget);
    _splitter->setObjectName(QString::fromUtf8("_splitter"));
    _splitter->setFrameShadow(QFrame::Plain);
    _splitter->setLineWidth(0);
    _splitter->setOrientation(Qt::Horizontal);
    _splitter->setHandleWidth(0);

    _frameLeft = new QFrame(_splitter);
    _frameLeft->setStyleSheet("QFrame { border: none; }");
    _vertLayLeft = new QVBoxLayout(_frameLeft);
    _vertLayLeft->setContentsMargins(-1, -1, 9, -1);

    // Search Bar Collections
    _frameCollectionsSearch = new QFrame(_frameLeft);
    _horLayCollectionsSearch = new QHBoxLayout(_frameCollectionsSearch);
    _horLayCollectionsSearch->setSpacing(0);

    _labelCollectionSearch = new QLabel("Collections: ", _frameCollectionsSearch);

    _textCollections = new QLineEdit(_frameCollectionsSearch);
    _textCollections->setClearButtonEnabled(true);

    _buttonSearchCollections = new QToolButton(_frameCollectionsSearch);
    _buttonSearchCollections->setMinimumSize(QSize(30, 20));
    _buttonSearchCollections->setMaximumSize(QSize(30, 20));
    _buttonSearchCollections->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconSearchCollection(":/icons/search_icon.png");
    _buttonSearchCollections->setIcon(iconSearchCollection);

    _buttonClearCollections = new QToolButton(_frameCollectionsSearch);
    _buttonClearCollections->setMinimumSize(QSize(30, 20));
    _buttonClearCollections->setMaximumSize(QSize(30, 20));
    _buttonClearCollections->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconClearCollections(":/icons/clear_search_white_icon.png");
    _buttonClearCollections->setIcon(iconClearCollections);

    _buttonSortCollections = new QToolButton(_frameCollectionsSearch);
    _buttonSortCollections->setMinimumSize(QSize(30, 20));
    _buttonSortCollections->setMaximumSize(QSize(30, 20));
    _buttonSortCollections->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconSortCollections(":/icons/sort_icon.png");
    _buttonSortCollections->setIcon(iconSortCollections);

    _buttonAddCollection = new QToolButton(_frameCollectionsSearch);
    _buttonAddCollection->setMinimumSize(QSize(30, 20));
    _buttonAddCollection->setMaximumSize(QSize(30, 20));
    _buttonAddCollection->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconAddCollection(":/icons/add_icon.png");
    _buttonAddCollection->setIcon(iconAddCollection);

    _horLayCollectionsSearch->addWidget(_labelCollectionSearch);
    _horLayCollectionsSearch->addWidget(_textCollections);
    _horLayCollectionsSearch->addWidget(_buttonSearchCollections);
    _horLayCollectionsSearch->addWidget(_buttonClearCollections);
    _horLayCollectionsSearch->addWidget(_buttonSortCollections);
    _horLayCollectionsSearch->addWidget(_buttonAddCollection);

    // Left Frame
    _vertLayLeft->addWidget(_frameCollectionsSearch);

    _listWidgetCollections = new QListWidget(_frameLeft);
    _listWidgetCollections->setStyleSheet(QString("border: 1px solid grey;"));
    _listWidgetCollections->setSortingEnabled(true);
    _listWidgetCollections->setContextMenuPolicy(Qt::CustomContextMenu);

    _vertLayLeft->addWidget(_listWidgetCollections);
    _vertLayLeft->setSpacing(9);
    _splitter->addWidget(_frameLeft);

    // Search Bar Links
    _frameRight = new QFrame(_splitter);
    _frameRight->setStyleSheet("QFrame { border: none; } ");

    _vertLayRight = new QVBoxLayout(_frameRight);
    _vertLayRight->setSpacing(9);
    _vertLayRight->setContentsMargins(9, -1, 1, -1);
    _frameLinksSearch = new QFrame(_frameRight);

    _horLayLinksSearch = new QHBoxLayout(_frameLinksSearch);
    _horLayLinksSearch->setSpacing(0);

    _labelLinkSearch = new QLabel("Links: ", _frameLinksSearch);

    _textLinks = new QLineEdit(_frameLinksSearch);
    _textLinks->setClearButtonEnabled(true);

    _buttonSearchLinks = new QToolButton(_frameLinksSearch);
    _buttonSearchLinks->setMinimumSize(QSize(30, 20));
    _buttonSearchLinks->setMaximumSize(QSize(30, 20));
    _buttonSearchLinks->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconSearchLinks(":/icons/search_icon.png");
    _buttonSearchLinks->setIcon(iconSearchLinks);

    _buttonClearLinks = new QToolButton(_frameLinksSearch);
    _buttonClearLinks->setMinimumSize(QSize(30, 20));
    _buttonClearLinks->setMaximumSize(QSize(30, 20));
    _buttonClearLinks->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconClearLinks(":/icons/clear_search_white_icon.png");
    _buttonClearLinks->setIcon(iconClearLinks);

    _buttonSortLinks = new QToolButton(_frameLinksSearch);
    _buttonSortLinks->setMinimumSize(QSize(30, 20));
    _buttonSortLinks->setMaximumSize(QSize(30, 20));
    _buttonSortLinks->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconSortLinks(":/icons/sort_icon.png");
    _buttonSortLinks->setIcon(iconSortLinks);

    _buttonAddLink = new QToolButton(_frameLinksSearch);
    _buttonAddLink->setMinimumSize(QSize(30, 20));
    _buttonAddLink->setMaximumSize(QSize(30, 20));
    _buttonAddLink->setCursor(QCursor(Qt::PointingHandCursor));
    QIcon iconAddLink(":/icons/add_icon.png");
    _buttonAddLink->setIcon(iconAddLink);

    _horLayLinksSearch->addWidget(_labelLinkSearch);
    _horLayLinksSearch->addWidget(_textLinks);
    _horLayLinksSearch->addWidget(_buttonSearchLinks);
    _horLayLinksSearch->addWidget(_buttonClearLinks);
    _horLayLinksSearch->addWidget(_buttonSortLinks);
    _horLayLinksSearch->addWidget(_buttonAddLink);

    _vertLayRight->addWidget(_frameLinksSearch);
    // List Widget
    _listWidgetLinks = new QListWidget(_frameRight);
    _listWidgetLinks->setStyleSheet(QString("border: 1px solid grey;"));
    _listWidgetLinks->setSortingEnabled(true);
    _listWidgetLinks->setContextMenuPolicy(Qt::CustomContextMenu);
    _vertLayRight->addWidget(_listWidgetLinks);

    // Details Frame
    _frameLinkDetails = new QFrame(_frameRight);
    _frameLinkDetails->setObjectName(QString("_frameLinkDetails"));
    _frameLinkDetails->setStyleSheet(QString("QFrame#_frameLinkDetails { border: 1px solid grey; }"));
    _frameLinkDetails->setFrameShape(QFrame::StyledPanel);
    _frameLinkDetails->setFrameShadow(QFrame::Raised);
    _frameLinkDetails->setMaximumHeight(200);

    _vertLayDetails = new QVBoxLayout(_frameLinkDetails);
    _vertLayDetails->setSpacing(15);

    _gridLayDetails = new QGridLayout();

    _textDetailsTitle = new QLineEdit(_frameLinkDetails);
    _textDetailsTitle->setClearButtonEnabled(true);

    _labelDetailsTitle = new QLabel("Title", _frameLinkDetails);

    _textDetailsUrl = new QLineEdit(_frameLinkDetails);
    _textDetailsUrl->setClearButtonEnabled(true);

    _labelDetailsUrl = new QLabel("URL", _frameLinkDetails);

    _labelDetailsComments = new QLabel("Comments", _frameLinkDetails);

    _plainTextDetailsComments = new QPlainTextEdit(_frameLinkDetails);
    _plainTextDetailsComments->setStyleSheet("QPlainTextEdit{border: 1px solid grey; color: white;}");
    _plainTextDetailsComments->setMaximumHeight(50);

    _gridLayDetails->addWidget(_labelDetailsTitle, 0, 0, 1, 1);
    _gridLayDetails->addWidget(_textDetailsTitle, 0, 1, 1, 1);
    _gridLayDetails->addWidget(_labelDetailsUrl, 1, 0, 1, 1);
    _gridLayDetails->addWidget(_textDetailsUrl, 1, 1, 1, 1);
    _gridLayDetails->addWidget(_labelDetailsComments, 2, 0, 1, 1);
    _gridLayDetails->addWidget(_plainTextDetailsComments, 2, 1, 1, 1);

    _vertLayDetails->addLayout(_gridLayDetails);

    _horLayBotButtons = new QHBoxLayout();
    _horSpacerBotButtonsLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horSpacerBotButtonsRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    _buttonDetailsRestore = new QPushButton("Restore", _frameLinkDetails);
    _buttonDetailsRestore->setFlat(true);
    _buttonDetailsRestore->setCursor(Qt::PointingHandCursor);
    _buttonDetailsRestore->setMinimumSize(QSize(80, 30));
    _buttonDetailsRestore->setMaximumSize(QSize(80, 30));

    _buttonDetailsUpdate = new QPushButton("Update", _frameLinkDetails);
    _buttonDetailsUpdate->setFlat(true);
    _buttonDetailsUpdate->setCursor(Qt::PointingHandCursor);
    _buttonDetailsUpdate->setMinimumSize(QSize(80, 30));
    _buttonDetailsUpdate->setMaximumSize(QSize(80, 30));

    _buttonDetailsClear = new QPushButton("Clear", _frameLinkDetails);
    _buttonDetailsClear->setFlat(true);
    _buttonDetailsClear->setCursor(Qt::PointingHandCursor);
    _buttonDetailsClear->setMinimumSize(QSize(80, 30));
    _buttonDetailsClear->setMaximumSize(QSize(80, 30));

    _horLayBotButtons->addItem(_horSpacerBotButtonsLeft);
    _horLayBotButtons->addWidget(_buttonDetailsRestore);
    _horLayBotButtons->addWidget(_buttonDetailsUpdate);
    _horLayBotButtons->addWidget(_buttonDetailsClear);
    _horLayBotButtons->addItem(_horSpacerBotButtonsRight);

    _vertLayDetails->addLayout(_horLayBotButtons);
    _vertLayDetails->setSpacing(9);
    _vertLayRight->addWidget(_frameLinkDetails);

    _splitter->addWidget(_frameRight);

    // Central
    _horLayMain = new QHBoxLayout(_centralWidget);
    _horLayMain->addWidget(_splitter);
    setCentralWidget(_centralWidget);

    // Actions
    setupActions();

    // MenuBar
    _menuBar = new QMenuBar(this);
    _menuBar->setGeometry(QRect(0, 0, 867, 21));
    setMenuBar(_menuBar);

    // StatusBar
    _statusBar = new QStatusBar(this);
    setStatusBar(_statusBar);

    // Menus
    setupMenus();

    // Refresh Data
    refreshCollections("");
}

void LinkManagerWindow::setupActions() {
    // File Menu
    _actionClose = new QAction("Close", this);

    // View Menu
    _actionFullscreen = new QAction("Fullscreen", this);
    _actionMaximize = new QAction("Maximize", this);
    _actionMinimize = new QAction("Minimize", this);
    _actionHideLeftPanel = new QAction("Hide Left Panel", this);
    _actionHideRightPanel = new QAction("Hide Right Panel", this);
    _actionHideSearchBars = new QAction("Hide Search Bars", this);
    _actionHideStatusBar = new QAction("Hide Status Bar", this);

    // Reset Menu
    _actionResetLinks = new QAction("Reset Links", this);
    _actionResetCollections = new QAction("Reset Collections", this);
    _actionResetAll = new QAction("Reset All", this);

    // Links Menu
    _actionAddLink = new QAction("Add Link", this);
    _actionDeleteLink = new QAction("Delete Link", this);
    _actionEditLink = new QAction("Edit Link", this);

    // Collections Menu
    _actionAddCollection = new QAction("Add Collection", this);
    _actionDeleteCollection = new QAction("Delete Collection", this);
    _actionRenameCollection = new QAction("Rename Collection", this);
    _actionOpenAllLinks = new QAction("Open All Links", this);
}

void LinkManagerWindow::setupMenus() {
    _menuFile = _menuBar->addMenu(tr("&File"));
    _menuView = _menuBar->addMenu(tr("&View"));
    _menuReset = _menuBar->addMenu(tr("&Reset"));
    _menuLinks = _menuBar->addMenu(tr("&Links"));
    _menuCollections = _menuBar->addMenu(tr("&Collections"));

    // Add Actions to Menus
    _menuBar->addAction(_menuFile->menuAction());
    _menuBar->addAction(_menuView->menuAction());
    _menuBar->addAction(_menuReset->menuAction());
    _menuBar->addAction(_menuLinks->menuAction());
    _menuBar->addAction(_menuCollections->menuAction());
    _menuFile->addAction(_actionClose);
    _menuView->addAction(_actionFullscreen);
    _menuView->addAction(_actionMaximize);
    _menuView->addAction(_actionMinimize);
    _menuView->addSeparator();
    _menuView->addAction(_actionHideLeftPanel);
    _menuView->addAction(_actionHideRightPanel);
    _menuView->addSeparator();
    _menuView->addAction(_actionHideSearchBars);
    _menuView->addSeparator();
    _menuView->addAction(_actionHideStatusBar);
    _menuLinks->addAction(_actionAddLink);
    _menuLinks->addAction(_actionEditLink);
    _menuLinks->addAction(_actionDeleteLink);
    _menuReset->addAction(_actionResetLinks);
    _menuReset->addAction(_actionResetCollections);
    _menuCollections->addAction(_actionAddCollection);
    _menuCollections->addAction(_actionDeleteCollection);
    _menuCollections->addAction(_actionRenameCollection);
    _menuCollections->addAction(_actionOpenAllLinks);
}

void LinkManagerWindow::setupConnections() {
    connect(_textLinks, &QLineEdit::textChanged, this, &LinkManagerWindow::searchLinks);
    connect(_textLinks, &QLineEdit::returnPressed, [this] { searchLinks(_textLinks->text()); });
    connect(_textCollections, &QLineEdit::textChanged, this, &LinkManagerWindow::refreshCollections);
    connect(_textCollections, &QLineEdit::returnPressed, [this] { refreshCollections(_textCollections->text()); });

    connect(_listWidgetLinks, &QListWidget::itemActivated, this, &LinkManagerWindow::openLink);
    connect(_listWidgetLinks, &QListWidget::itemClicked, this, &LinkManagerWindow::linkClicked);
    connect(_listWidgetLinks, &QListWidget::itemSelectionChanged, this, &LinkManagerWindow::linkSelectionChanged);
    connect(_listWidgetLinks, &QListWidget::customContextMenuRequested, this,
            &LinkManagerWindow::showLinksContextMenu);
    connect(_listWidgetCollections, &QListWidget::itemClicked, this, &LinkManagerWindow::collectionClicked);
    connect(_listWidgetCollections, &QListWidget::customContextMenuRequested, this,
            &LinkManagerWindow::showCollectionsContextMenu);

    connect(_buttonDetailsClear, &QPushButton::clicked, this, &LinkManagerWindow::clearDetails);
    connect(_buttonDetailsRestore, &QPushButton::clicked, this, &LinkManagerWindow::restoreDetails);
    connect(_buttonDetailsUpdate, &QPushButton::clicked, this, &LinkManagerWindow::updateDetails);

    connect(_buttonAddCollection, &QToolButton::clicked, this, &LinkManagerWindow::addCollection);
    connect(_buttonAddLink, &QToolButton::clicked, this, &LinkManagerWindow::addLink);
    connect(_buttonClearCollections, &QToolButton::clicked, this, &LinkManagerWindow::clearCollections);
    connect(_buttonSortCollections, &QToolButton::clicked, this, &LinkManagerWindow::sortCollections);
    connect(_buttonClearLinks, &QToolButton::clicked, this, &LinkManagerWindow::clearLinks);
    connect(_buttonSortLinks, &QToolButton::clicked, this, &LinkManagerWindow::sortLinks);
    connect(_buttonSearchCollections, &QToolButton::clicked, [this] {
        refreshCollections(_textCollections->text());
    });
    connect(_buttonSearchLinks, &QToolButton::clicked, [this] {
        searchLinks(_textLinks->text());
    });

    connect(_actionFullscreen, &QAction::triggered, [this] { common::toggleFullscreen(this); });
    connect(_actionHideLeftPanel, &QAction::triggered, this, &LinkManagerWindow::hideLeftPanel);
    connect(_actionHideRightPanel, &QAction::triggered, this, &LinkManagerWindow::hideRightPanel);
    connect(_actionHideSearchBars, &QAction::triggered, this, &LinkManagerWindow::hideSearchBars);
    connect(_actionResetLinks, &QAction::triggered, this, &LinkManagerWindow::resetLinks);
    connect(_actionResetCollections, &QAction::triggered, this, &LinkManagerWindow::resetCollections);
    connect(_actionDeleteCollection, &QAction::triggered, this, &LinkManagerWindow::actionDeleteCollection);
    connect(_actionRenameCollection, &QAction::triggered, this, &LinkManagerWindow::actionRenameCollection);
    connect(_actionOpenAllLinks, &QAction::triggered, this, &LinkManagerWindow::actionOpenAllLinks);
    connect(_actionDeleteLink, &QAction::triggered, this, &LinkManagerWindow::actionDeleteLink);
    connect(_actionEditLink, &QAction::triggered, this, &LinkManagerWindow::actionEditLink);
    connect(_actionClose, &QAction::triggered, this, &LinkManagerWindow::close);
    connect(_actionMaximize, &QAction::triggered, [this] { common::toggleMaximized(this); });
    connect(_actionMinimize, &QAction::triggered, this, &LinkManagerWindow::showMinimized);
    connect(_actionAddCollection, &QAction::triggered, this, &LinkManagerWindow::addCollection);
    connect(_actionAddLink, &QAction::triggered, this, &LinkManagerWindow::addLink);
    connect(_actionHideStatusBar, &QAction::triggered, this, &LinkManagerWindow::hideStatusBar);
}

void LinkManagerWindow::setupTabOrder() {
    QWidget::setTabOrder(_textCollections, _buttonSearchCollections);
    QWidget::setTabOrder(_buttonSearchCollections, _buttonClearCollections);
    QWidget::setTabOrder(_buttonClearCollections, _buttonSortCollections);
    QWidget::setTabOrder(_buttonSortCollections, _buttonAddCollection);
    QWidget::setTabOrder(_buttonAddCollection, _listWidgetCollections);
    QWidget::setTabOrder(_listWidgetCollections, _textLinks);
    QWidget::setTabOrder(_textLinks, _buttonSearchLinks);
    QWidget::setTabOrder(_buttonSearchLinks, _buttonClearLinks);
    QWidget::setTabOrder(_buttonClearLinks, _buttonSortLinks);
    QWidget::setTabOrder(_buttonSortLinks, _buttonAddLink);
    QWidget::setTabOrder(_buttonAddLink, _listWidgetLinks);
    QWidget::setTabOrder(_listWidgetLinks, _textDetailsTitle);
    QWidget::setTabOrder(_textDetailsTitle, _textDetailsUrl);
    QWidget::setTabOrder(_textDetailsUrl, _plainTextDetailsComments);
    QWidget::setTabOrder(_plainTextDetailsComments, _buttonDetailsUpdate);
    QWidget::setTabOrder(_buttonDetailsUpdate, _buttonDetailsRestore);
    QWidget::setTabOrder(_buttonDetailsRestore, _buttonDetailsClear);
}

void LinkManagerWindow::setupShortcuts() {
    // File Menu
    _actionClose->setShortcut(QKeySequence("Ctrl+Q"));

    // View Menu
    _actionFullscreen->setShortcut(QKeySequence("Ctrl+F, Ctrl+S"));
    _actionMaximize->setShortcut(QKeySequence("Ctrl+M, Ctrl+X"));
    _actionMinimize->setShortcut(QKeySequence("Ctrl+M, Ctrl+N"));
    _actionHideLeftPanel->setShortcut(QKeySequence("Ctrl+L, Ctrl+P"));
    _actionHideRightPanel->setShortcut(QKeySequence("Ctrl+R, Ctrl+P"));
    _actionHideSearchBars->setShortcut(QKeySequence("Ctrl+H, Ctrl+S"));
    _actionHideStatusBar->setShortcut(QKeySequence("Ctrl+S, Ctrl+S"));

    // Links Menu
    _actionAddLink->setShortcut(QKeySequence("Ctrl+A, Ctrl+L"));
    _actionEditLink->setShortcut(QKeySequence("Ctrl+E, Ctrl+L"));
    _actionDeleteLink->setShortcut(QKeySequence("Ctrl+D, Ctrl+L"));

    // Collections Menu
    _actionAddCollection->setShortcut(QKeySequence("Ctrl+A, Ctrl+C"));
    _actionDeleteCollection->setShortcut(QKeySequence("Ctrl+D, Ctrl+C"));
    _actionRenameCollection->setShortcut(QKeySequence("Ctrl+R, Ctrl+C"));
    _actionOpenAllLinks->setShortcut(QKeySequence("Ctrl+O, Ctrl+A"));
}

void LinkManagerWindow::showLinksContextMenu(const QPoint &pos) {
    QPoint globalPos = _listWidgetLinks->mapToGlobal(pos);

    QMenu menu;
    menu.setStyleSheet(common::openSheet(":/styles/style.qss"));
    menu.addAction("Open", this, [this] {
        openLink(_listWidgetLinks->currentItem());
    });
    menu.addAction("Edit", this, [this] {
        editLinkDetails(_listWidgetLinks->currentItem()->text());
    });
    menu.addAction("Delete", this, [this] {
        deleteLink(_listWidgetLinks->currentItem()->text());
    });
    menu.addAction("Copy Link", this, [this] {
        copyLink(_listWidgetLinks->currentItem()->text());
    });
    if (!_listWidgetLinks->selectedItems().isEmpty()) {
        menu.exec(globalPos);
    }
}

void LinkManagerWindow::showCollectionsContextMenu(const QPoint &pos) {
    QPoint globalPos = _listWidgetCollections->mapToGlobal(pos);
    QMenu menu;
    menu.setStyleSheet(common::openSheet(":/styles/style.qss"));
    menu.addAction("Add Link", this, [this] {
        addLink();
    });
    menu.addAction("Rename", this, [this] {
        renameCollection(_listWidgetCollections->currentItem()->text());
    });
    menu.addAction("Delete", this, [this] {
        deleteCollection(_listWidgetCollections->currentItem()->text());
    });
    menu.addAction("Open Links", this, [this] {
        openAllLinks();
    });
    if (!_listWidgetCollections->selectedItems().isEmpty()) {
        menu.exec(globalPos);
    }
}

void LinkManagerWindow::openAllLinks() {
    QString currentColl = _listWidgetCollections->currentItem()->text();
    queries::selectLinksBasedOnCollection(currentColl, "");
    while (queries::query.next()) {
        QUrl url = QUrl(queries::query.value(1).toString());
        if (url.isValid()) {
            QDesktopServices::openUrl(url);
        }
    }
}

void LinkManagerWindow::deleteLink(const QString &name) {
    queries::deleteLink(name);
    delete _listWidgetLinks->currentItem();
    _statusBar->showMessage("Link \"" + name + "\" deleted.");
}

void LinkManagerWindow::editLinkDetails(const QString &name) {
    InsertLinkDialog dialog(this);
    dialog.setWindowTitle("Edit Link");
    dialog._title = name;
    queries::selectLinkRecord(name);
    queries::query.next();
    dialog._link = queries::query.value(1).toString();
    dialog.setDataOnOpen();
    common::openDialog(&dialog, ":/styles/style.qss");

    queries::updateLinkDetails(name, dialog._title, dialog._link, "");
    _listWidgetLinks->currentItem()->setText(dialog._title);
    _statusBar->showMessage("Link \"" + name + "\" edited.");
}

void LinkManagerWindow::copyLink(const QString &name) {
    queries::selectLinkRecord(name);
    queries::query.next();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(queries::query.value(1).toString());
    _statusBar->showMessage("Link \"" + name + "\" copied.");
}

void LinkManagerWindow::renameCollection(const QString &name) {
    GetNameDialog dialog(this, "Collection Name", "Please provide a name for the collection you want to create:");

    dialog._name = name;
    dialog.setDataOnOpen();

    common::openDialog(&dialog, ":/styles/style.qss");
    queries::updateLinkCollectionName(name, dialog._name);
    _listWidgetCollections->currentItem()->setText(dialog._name);
    _statusBar->showMessage("Collection \"" + name + "\" renamed.");
}

void LinkManagerWindow::deleteCollection(const QString &name) {
    queries::deleteCollection(name);
    _listWidgetLinks->clear();
    delete _listWidgetCollections->currentItem();
    _statusBar->showMessage("Collection \"" + name + "\" deleted.");
}

void LinkManagerWindow::refreshCollections(const QString &searchString) {
    _listWidgetCollections->clear();
    queries::selectCollections(searchString);
    while (queries::query.next()) {
        _listWidgetCollections->addItem(queries::query.value(0).toString());
    }
}

void LinkManagerWindow::refreshLinks(const QString &collectionName, const QString &searchString) {
    _listWidgetLinks->clear();
    queries::selectLinksBasedOnCollection(collectionName, searchString);
    while (queries::query.next()) {
        _listWidgetLinks->addItem(queries::query.value(0).toString());
    }
}

void LinkManagerWindow::addCollection() {
    GetNameDialog dialog(this, "Collection Name", "Please provide a name for the collection you want to create:");
    common::openDialog(&dialog, ":/styles/style.qss");

    if (!dialog._name.isEmpty()) {
        queries::insertLinkCollection(dialog._name);
        _statusBar->showMessage("New collection \"" + dialog._name + "\" added.");
    }

    refreshCollections("");
}

void LinkManagerWindow::addLink() {
    InsertLinkDialog dialog(this);
    if (!_listWidgetCollections->selectedItems().isEmpty()) {
        common::openDialog(&dialog, ":/styles/style.qss");
    }

    if (!dialog._link.isEmpty() && !dialog._title.isEmpty()) {
        QString collectionName = _listWidgetCollections->currentItem()->text();
        int collectionId = queries::selectCollectionId(collectionName);
        queries::insertLink(collectionId, dialog._title, dialog._link, "");
        refreshLinks(_listWidgetCollections->currentItem()->text(), "");
        _statusBar->showMessage("New link \"" + dialog._title + "\" added.");
    }
}

void LinkManagerWindow::searchLinks(const QString &arg1) {
    if (!_listWidgetCollections->selectedItems().isEmpty()) {
        refreshLinks(_listWidgetCollections->currentItem()->text(), arg1);
    }
}

void LinkManagerWindow::collectionClicked(QListWidgetItem *item) {
    refreshLinks(item->text(), "");
    _statusBar->showMessage("Collection \"" + item->text() + "\" selected.");
}

void LinkManagerWindow::openLink(QListWidgetItem *item) {
    queries::selectLinkRecord(item->text());
    queries::query.next();

    QDesktopServices::openUrl(QUrl(queries::query.value(1).toString()));
}

void LinkManagerWindow::linkClicked(QListWidgetItem *item) {
    queries::selectLinkRecord(item->text());
    queries::query.next();
    QString title = queries::query.value("name").toString();
    QString url = queries::query.value("path").toString();
    QString comments = queries::query.value("comments").toString();

    _textDetailsTitle->setText(title);
    _textDetailsUrl->setText(url);
    _plainTextDetailsComments->setPlainText(comments);

    _statusBar->showMessage(title + ": " + url);
}

void LinkManagerWindow::clearDetails() {
    _textDetailsTitle->clear();
    _textDetailsUrl->clear();
    _plainTextDetailsComments->clear();
}

void LinkManagerWindow::restoreDetails() {
    if (!_listWidgetLinks->selectedItems().isEmpty()) {
        linkClicked(_listWidgetLinks->currentItem());
        _statusBar->showMessage("Details restored successfully.");
    }
}

void LinkManagerWindow::updateDetails() {
    QString oldTitle = _listWidgetLinks->currentItem()->text();
    QString newTitle = _textDetailsTitle->text();
    QString newUrl = _textDetailsUrl->text();
    QString newComments = _plainTextDetailsComments->toPlainText();

    queries::updateLinkDetails(oldTitle, newTitle, newUrl, newComments);
    _listWidgetLinks->currentItem()->setText(newTitle);
    _statusBar->showMessage("Link \"" + oldTitle + "\" edited.");
}

void LinkManagerWindow::linkSelectionChanged() {
    if (!_listWidgetLinks->selectedItems().isEmpty()) {
        linkClicked(_listWidgetLinks->currentItem());
    }
}

void LinkManagerWindow::clearCollections() {
    _textCollections->clear();
    _listWidgetCollections->clear();
}

void LinkManagerWindow::sortCollections() {
    _COLL_SORT = (_COLL_SORT == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
    _listWidgetCollections->sortItems(_COLL_SORT);
}

void LinkManagerWindow::clearLinks() {
    _textLinks->clear();
    _listWidgetLinks->clear();
}

void LinkManagerWindow::sortLinks() {
    _LINKS_SORT = (_LINKS_SORT == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
    _listWidgetLinks->sortItems(_LINKS_SORT);
}

void LinkManagerWindow::hideLeftPanel() {
    common::changeWidgetVisibility(_frameLeft, _actionHideLeftPanel);
}

void LinkManagerWindow::hideRightPanel() {
    common::changeWidgetVisibility(_frameRight, _actionHideRightPanel);
}

void LinkManagerWindow::hideSearchBars() {
    _frameLinksSearch->setHidden(!_frameLinksSearch->isHidden());
    _frameCollectionsSearch->setHidden(!_frameCollectionsSearch->isHidden());
    _actionHideSearchBars->setText(_frameCollectionsSearch->isHidden() ? "Show Search Bars" : "Hide Search Bars");
}

void LinkManagerWindow::resetLinks() {
    queries::resetLinksTable();
}

void LinkManagerWindow::resetCollections() {
    queries::resetCollectionsTable();
}

void LinkManagerWindow::actionEditLink() {
    if (_listWidgetLinks->currentItem()) {
        editLinkDetails(_listWidgetLinks->currentItem()->text());
    }
}

void LinkManagerWindow::actionDeleteCollection() {
    if (_listWidgetCollections->currentItem()) {
        deleteCollection(_listWidgetCollections->currentItem()->text());
    }
}

void LinkManagerWindow::actionRenameCollection() {
    if (_listWidgetCollections->currentItem()) {
        renameCollection(_listWidgetCollections->currentItem()->text());
    }
}

void LinkManagerWindow::actionOpenAllLinks() {
    openAllLinks();
}

void LinkManagerWindow::actionDeleteLink() {
    if (_listWidgetLinks->currentItem()) {
        deleteLink(_listWidgetLinks->currentItem()->text());
    }
}

void LinkManagerWindow::hideStatusBar() {
    common::changeWidgetVisibility(_statusBar, _actionHideStatusBar);
}
