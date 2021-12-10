#ifndef DATAVIEWERWINDOW_H
#define DATAVIEWERWINDOW_H

#include <QMainWindow>

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

    void closeEvent(QCloseEvent *event);

    void populateTable(const QString &tableName);

    void on_comboBoxTables_currentTextChanged(const QString &arg1);

private:
    Ui::DataViewerWindow *ui;
};

#endif // DATAVIEWERWINDOW_H
