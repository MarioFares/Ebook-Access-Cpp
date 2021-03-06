#ifndef DATAVIEWERWINDOW_H
#define DATAVIEWERWINDOW_H

#include <QMainWindow>
#include <QHeaderView>

namespace Ui {
class DataViewerWindow;
}

class DataViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DataViewerWindow(QWidget *parent = nullptr);
    ~DataViewerWindow();


private slots:
    void setupConnections();

    void closeEvent(QCloseEvent *event);

    void populateTable();

    void tableSelected(int index);

    void toggleColors();

    void toggleGrid();

    void toggleFitColumns();

    void showCellText();

    void showTableContextMenu(const QPoint &pos);

    void hideColumn(int index);

    void showColumn(int index);

    void showAllColumns();

    void setupGridMenu();

    void setFontColor();

    void setBackColor();


private:
    Ui::DataViewerWindow *ui;
    QHeaderView::ResizeMode columnsResizeMode;
    QHash<int, QString> hiddenColumns;
};

#endif // DATAVIEWERWINDOW_H
