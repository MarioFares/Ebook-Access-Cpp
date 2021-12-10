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
    void closeEvent(QCloseEvent *event);

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

    void on_buttonInsertCollection_clicked();

    void on_buttonInsertLink_clicked();

    void on_textCollections_textChanged(const QString &arg1);

    void on_textLinks_textChanged(const QString &arg1);

    void on_listWidgetCollections_itemClicked(QListWidgetItem *item);

    void on_listWidgetLinks_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidgetLinks_itemClicked(QListWidgetItem *item);

    void on_buttonDetailsClear_clicked();

    void on_buttonDetailsRestore_clicked();

    void on_buttonDetailsUpdate_clicked();

    void on_listWidgetLinks_itemSelectionChanged();

    void on_buttonSearchCollections_clicked();

    void on_buttonClearCollections_clicked();

    void on_buttonSortCollections_clicked();

    void on_buttonSearchLinks_clicked();

    void on_buttonClearLinks_clicked();

    void on_buttonSortLinks_clicked();

    void on_actionFullscreen_triggered();

    void on_actionHideLeftPanel_triggered();

    void on_actionHideRightPanel_triggered();

    void on_actionHideSearchBars_triggered();

    void on_actionResetLinks_triggered();

    void on_actionResetCollections_triggered();

    void on_actionResetAll_triggered();

    void on_actionAddLink_triggered();

    void on_actionEditLink_triggered();

    void on_actionAddCollection_triggered();

    void on_actionDeleteCollection_triggered();

    void on_actionRenameCollection_triggered();

    void on_actionOpenAllLinks_triggered();

    void on_actionDeleteLink_triggered();

private:
    Ui::LinkManagerWindow *ui;

    Qt::SortOrder LINKS_SORT;
    Qt::SortOrder COLL_SORT;
};

#endif // LINKMANAGERWINDOW_H
