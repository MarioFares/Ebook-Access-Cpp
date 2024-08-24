#ifndef LINKMANAGERWINDOW_H
#define LINKMANAGERWINDOW_H

#include <QLabel>
#include <QAction>
#include <QSplitter>
#include <QToolButton>
#include <QMainWindow>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QListWidgetItem>

class LinkManagerWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit LinkManagerWindow(QWidget *parent = nullptr);

private slots:
    void setupInterface();

    void setupActions();

    void setupMenus();

    void setupConnections();

    void setupTabOrder();

    void setupShortcuts();

    void refreshCollections(const QString &searchString);

    void refreshLinks(const QString &collectionName, const QString &searchString);

    void showLinksContextMenu(const QPoint &pos);

    void showCollectionsContextMenu(const QPoint &pos);

    void deleteLink(const QString &name);

    void deleteCollection(const QString &name);

    void editLinkDetails(const QString &name);

    void renameCollection(const QString &name);

    void copyLink(const QString &name);

    void openAllLinks();

    void addCollection();

    void addLink();

    void searchLinks(const QString &arg1);

    void collectionClicked(QListWidgetItem *item);

    static void openLink(QListWidgetItem *item);

    void linkClicked(QListWidgetItem *item);

    void clearDetails();

    void restoreDetails();

    void updateDetails();

    void linkSelectionChanged();

    void clearCollections();

    void sortCollections();

    void clearLinks();

    void sortLinks();

    void hideLeftPanel();

    void hideRightPanel();

    void hideSearchBars();

    static void resetLinks();

    static void resetCollections();

    void actionEditLink();

    void actionDeleteCollection();

    void actionRenameCollection();

    void actionOpenAllLinks();

    void actionDeleteLink();

    void hideStatusBar();

private:
    Qt::SortOrder _LINKS_SORT;
    Qt::SortOrder _COLL_SORT;

    // Widgets
    QAction *_actionMaximize;
    QAction *_actionMinimize;
    QAction *_actionFullscreen;
    QAction *_actionHideLeftPanel;
    QAction *_actionHideRightPanel;
    QAction *_actionHideSearchBars;
    QAction *_actionClose;
    QAction *_actionDeleteLink;
    QAction *_actionAddLink;
    QAction *_actionResetCollections;
    QAction *_actionResetLinks;
    QAction *_actionResetAll;
    QAction *_actionAddCollection;
    QAction *_actionDeleteCollection;
    QAction *_actionOpenAllLinks;
    QAction *_actionRenameCollection;
    QAction *_actionEditLink;
    QAction *_actionHideStatusBar;
    QWidget *_centralWidget;
    QFrame *_frameLeft;
    QFrame *_frameCollectionsSearch;
    QLabel *_labelCollectionSearch;
    QLineEdit *_textCollections;
    QSplitter *_splitter;
    QToolButton *_buttonSearchCollections;
    QToolButton *_buttonClearCollections;
    QToolButton *_buttonSortCollections;
    QToolButton *_buttonAddCollection;
    QListWidget *_listWidgetCollections;
    QFrame *_frameRight;
    QFrame *_frameLinksSearch;
    QLabel *_labelLinkSearch;
    QLineEdit *_textLinks;
    QToolButton *_buttonSearchLinks;
    QToolButton *_buttonClearLinks;
    QToolButton *_buttonSortLinks;
    QToolButton *_buttonAddLink;
    QListWidget *_listWidgetLinks;
    QFrame *_frameLinkDetails;
    QLineEdit *_textDetailsTitle;
    QLabel *_labelDetailsTitle;
    QLineEdit *_textDetailsUrl;
    QLabel *_labelDetailsUrl;
    QLabel *_labelDetailsComments;
    QPlainTextEdit *_plainTextDetailsComments;
    QPushButton *_buttonDetailsRestore;
    QPushButton *_buttonDetailsUpdate;
    QPushButton *_buttonDetailsClear;
    QStatusBar *_statusBar;
    QMenuBar *_menuBar;
    QMenu *_menuFile;
    QMenu *_menuView;
    QMenu *_menuLinks;
    QMenu *_menuReset;
    QMenu *_menuCollections;

    // Layouts
    QVBoxLayout *_vertLayLeft;
    QHBoxLayout *_horLayCollectionsSearch;
    QVBoxLayout *_vertLayRight;
    QHBoxLayout *_horLayLinksSearch;
    QVBoxLayout *_vertLayDetails;
    QGridLayout *_gridLayDetails;
    QHBoxLayout *_horLayBotButtons;
    QHBoxLayout *_horLayMain;

    // Spacers
    QSpacerItem *_horSpacerBotButtonsLeft;
    QSpacerItem *_horSpacerBotButtonsRight;
};

#endif // LINKMANAGERWINDOW_H
