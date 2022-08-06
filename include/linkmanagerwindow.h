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

class LinkManagerWindow : public QMainWindow
{
Q_OBJECT

public:
	explicit LinkManagerWindow(QWidget* parent = nullptr);

private slots:

	void setupInterface();

	void setupActions();

	void setupMenus();

	void setupConnections();

	void setupTabOrder();

	void setupShortcuts();

	void refreshCollections(const QString& searchString);

	void refreshLinks(const QString& collectionName, const QString& searchString);

	void showLinksContextMenu(const QPoint& pos);

	void showCollectionsContextMenu(const QPoint& pos);

	void deleteLink(const QString& name);

	void deleteCollection(const QString& name);

	void editLinkDetails(const QString& name);

	void renameCollection(const QString& name);

	void copyLink(const QString& name);

	void openAllLinks();

	void addCollection();

	void addLink();

	void searchLinks(const QString& arg1);

	void collectionClicked(QListWidgetItem* item);

	static void openLink(QListWidgetItem* item);

	void linkClicked(QListWidgetItem* item);

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
	Qt::SortOrder m_LINKS_SORT;
	Qt::SortOrder m_COLL_SORT;

	// Widgets
	QAction* m_actionMaximize;
	QAction* m_actionMinimize;
	QAction* m_actionFullscreen;
	QAction* m_actionHideLeftPanel;
	QAction* m_actionHideRightPanel;
	QAction* m_actionHideSearchBars;
	QAction* m_actionClose;
	QAction* m_actionDeleteLink;
	QAction* m_actionAddLink;
	QAction* m_actionResetCollections;
	QAction* m_actionResetLinks;
	QAction* m_actionResetAll;
	QAction* m_actionAddCollection;
	QAction* m_actionDeleteCollection;
	QAction* m_actionOpenAllLinks;
	QAction* m_actionRenameCollection;
	QAction* m_actionEditLink;
	QAction* m_actionHideStatusBar;
	QWidget* m_centralWidget;
	QFrame* m_frameLeft;
	QFrame* m_frameCollectionsSearch;
	QLabel* m_labelCollectionSearch;
	QLineEdit* m_textCollections;
	QSplitter* m_splitter;
	QToolButton* m_buttonSearchCollections;
	QToolButton* m_buttonClearCollections;
	QToolButton* m_buttonSortCollections;
	QToolButton* m_buttonAddCollection;
	QListWidget* m_listWidgetCollections;
	QFrame* m_frameRight;
	QFrame* m_frameLinksSearch;
	QLabel* m_labelLinkSearch;
	QLineEdit* m_textLinks;
	QToolButton* m_buttonSearchLinks;
	QToolButton* m_buttonClearLinks;
	QToolButton* m_buttonSortLinks;
	QToolButton* m_buttonAddLink;
	QListWidget* m_listWidgetLinks;
	QFrame* m_frameLinkDetails;
	QLineEdit* m_textDetailsTitle;
	QLabel* m_labelDetailsTitle;
	QLineEdit* m_textDetailsUrl;
	QLabel* m_labelDetailsUrl;
	QLabel* m_labelDetailsComments;
	QPlainTextEdit* m_plainTextDetailsComments;
	QPushButton* m_buttonDetailsRestore;
	QPushButton* m_buttonDetailsUpdate;
	QPushButton* m_buttonDetailsClear;
	QStatusBar* m_statusBar;
	QMenuBar* m_menuBar;
	QMenu* m_menuFile;
	QMenu* m_menuView;
	QMenu* m_menuLinks;
	QMenu* m_menuReset;
	QMenu* m_menuCollections;

	// Layouts
	QVBoxLayout* m_vertLayLeft;
	QHBoxLayout* m_horLayCollectionsSearch;
	QVBoxLayout* m_vertLayRight;
	QHBoxLayout* m_horLayLinksSearch;
	QVBoxLayout* m_vertLayDetails;
	QGridLayout* m_gridLayDetails;
	QHBoxLayout* m_horLayBotButtons;
	QHBoxLayout* m_horLayMain;

	// Spacers
	QSpacerItem* m_horSpacerBotButtonsLeft;
	QSpacerItem* m_horSpacerBotButtonsRight;
};

#endif // LINKMANAGERWINDOW_H
