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

private:
    Ui::DataViewerWindow *ui;
    QHeaderView::ResizeMode columnsResizeMode;
};

#endif // DATAVIEWERWINDOW_H
