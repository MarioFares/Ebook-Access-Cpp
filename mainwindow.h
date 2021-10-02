#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QList>
#include <QPointer>
#include <QMainWindow>
#include <QApplication>
#include <summarywindow.h>
#include <QListWidgetItem>
#include <linkmanagerwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showSummary(QString name)
    {

        SummaryWindow *summaryWindow = new SummaryWindow();
        summaryPointerList.push_back(summaryWindow);
        QFile file(":/summarystyle.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        summaryWindow->setStyleSheet(styleSheet);
        summaryWindow->ensurePolished();
        summaryWindow->show();
        summaryWindow->callSelectEbookSummary(name);
    }

    void showLinkManager()
    {
        LinkManagerWindow *linkManagerWindow = new LinkManagerWindow();
        linkPointerList.push_back(linkManagerWindow);
        QFile file(":/summarystyle.qss");
        file.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(file.readAll());
        linkManagerWindow->setStyleSheet(styleSheet);
        linkManagerWindow->ensurePolished();
        linkManagerWindow->show();
    }

//    template <typename windowPointer>

//    static void deleteWindow(QString windowAlias, windowPointer window)
//    {
//        if(windowAlias == "summary")
//        {
//            QPointer<SummaryWindow> pointer(window);
//            summaryPointerList.removeAll(summaryPointerList.indexOf(pointer));
//        }
//    }

private slots:
    void refreshFolders();

    void refreshAuthors();

    void refreshGenres();

    void refreshSearches();

    void openSummaryWindow();

    double changeBookSizeUnit(double size, QString unit);

    void showContextMenu(const QPoint&);

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

    void on_buttonSettings_clicked();

    void on_buttonSearchCriteria_clicked();

    void on_ebooksListWidget_itemClicked(QListWidgetItem *item);

    void on_buttonDetailsRestore_clicked();

    void on_buttonDetailsUpdate_clicked();

    void on_buttonSaveCriteria_clicked();

    void on_buttonSearchLoad_clicked();

    void on_actionCleanEbooks_triggered();

    void on_comboBoxSizeUnit_currentTextChanged();

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

    void on_buttonExtensions_clicked();

    void on_buttonFolder_clicked();

    void on_buttonAuthor_clicked();

    void on_buttonGenre_clicked();

    void on_buttonTags_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    Qt::SortOrder SORT;
    QList<SummaryWindow*> summaryPointerList;
    QList<LinkManagerWindow*> linkPointerList;

};
#endif // MAINWINDOW_H
