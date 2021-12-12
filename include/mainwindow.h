#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QList>
#include <QPointer>
#include <QComboBox>
#include <QMainWindow>
#include <QApplication>
#include <QListWidgetItem>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void closeEvent(QCloseEvent *event);

    void showSummary(const QString &name);

    void showLinkManager();

    void refreshComboBox(QComboBox *comboBox);

    void refreshFolders();

    void refreshAuthors();

    void refreshGenres();

    void refreshSearches();

    void openSummaryWindow();

    double changeBookSizeUnit(double size, const QString &unit);

    void showContextMenu(const QPoint&);

    void trayClicked(QSystemTrayIcon::ActivationReason r);

    void deleteListItem();

    void openFolder();

    void on_buttonAddBooks_clicked();

    void on_buttonSearchString_clicked();

    void on_buttonClearCriteria_clicked();

    void on_buttonDetailsClear_clicked();

    void on_buttonSortSearch_clicked();

    void on_actionResetEbooks_triggered();

    void on_actionHideSearchBar_triggered();

    void on_actionHideDetailsSection_triggered();

    void on_buttonAddBook_clicked();

    void on_buttonClearSearch_clicked();

    void on_buttonSearchCriteria_clicked();

    void on_ebooksListWidget_itemClicked(QListWidgetItem *item);

    void on_buttonDetailsRestore_clicked();

    void on_buttonDetailsUpdate_clicked();

    void on_buttonSaveCriteria_clicked();

    void on_buttonSearchLoad_clicked();

    void on_actionCleanEbooks_triggered();

    void on_buttonSummaries_clicked();

    void on_buttonLinkManager_clicked();

    void on_actionChooseRandomBook_triggered();

    void on_ebooksListWidget_itemSelectionChanged();

    void on_actionFullscreen_triggered();

    void on_ebooksListWidget_itemActivated(QListWidgetItem *item);

    void on_buttonSizeCriteria_clicked();

    void on_actionHideRightFrame_triggered();

    void on_actionHideUtilities_triggered();

    void on_actionHideLeftFrame_triggered();

    void extSelectionSetup(const QString &title, const QString &prompt, QWidget *widget);

    void on_buttonExtensions_clicked();

    void on_buttonFolder_clicked();

    void on_buttonAuthor_clicked();

    void on_buttonGenre_clicked();

    void on_buttonTags_clicked();

    void on_buttonSizeUnit_clicked();

    void on_actionMinimizeTray_triggered();

    void on_actionResetSummaries_triggered();

    void on_actionResetTags_triggered();

    void on_actionResetSearches_triggered();

    void on_actionResetDatabase_triggered();

    void on_actionWindowTop_triggered();

    void on_buttonDbViewer_clicked();

    void on_actionSummaries_triggered();

    void on_actionLinkManager_triggered();

    void on_actionDataViewer_triggered();

private:
    Ui::MainWindow *ui;
    Qt::SortOrder SORT;
    QHash<QString, quint32> sizeConvFactors;
    QHash<QString, QString> sizeUnits;

};
#endif // MAINWINDOW_H
