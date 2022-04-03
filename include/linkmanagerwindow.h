#ifndef LINKMANAGERWINDOW_H
#define LINKMANAGERWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class LinkManagerWindow;
}

class LinkManagerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LinkManagerWindow(QWidget *parent = nullptr);
    ~LinkManagerWindow();

private slots:
    void setupConnections();

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

    void openLink(QListWidgetItem *item);

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

    void resetLinks();

    void resetCollections();

    void actionEditLink();

    void actionDeleteCollection();

    void actionRenameCollection();

    void actionOpenAllLinks();

    void actionDeleteLink();

    void hideStatusBar();

private:
    Ui::LinkManagerWindow *ui;

    Qt::SortOrder LINKS_SORT;
    Qt::SortOrder COLL_SORT;
};

#endif // LINKMANAGERWINDOW_H
