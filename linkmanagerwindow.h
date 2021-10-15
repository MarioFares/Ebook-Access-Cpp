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

    void refreshCollections(QString searchString);

    void refreshLinks(QString collectionName, QString searchString);

    void showLinksContextMenu(const QPoint &pos);

    void showCollectionsContextMenu(const QPoint &pos);

    void deleteLink(QString name);

    void deleteCollection(QString name);

    void editLinkDetails(QString linkName);

    void renameCollection(QString name);

    void copyLink(QString linkName);

    void openAllLinks();

    void on_buttonInsertCollection_clicked();

    void on_buttonInsertLink_clicked();

    void on_textCollections_textChanged(const QString &arg1);

    void on_textLinks_textChanged(const QString &arg1);

    void on_listWidgetCollections_itemClicked(QListWidgetItem *item);

    void on_listWidgetLinks_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidgetLinks_itemClicked(QListWidgetItem *item);

private:
    Ui::LinkManagerWindow *ui;
};

#endif // LINKMANAGERWINDOW_H
