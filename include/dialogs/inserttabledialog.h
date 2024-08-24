#ifndef INSERTTABLEDIALOG_H
#define INSERTTABLEDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QSpinBox>
#include <QBoxLayout>
#include <QPushButton>

class InsertTableDialog : public QDialog {
    Q_OBJECT

public:
    explicit InsertTableDialog(QWidget *parent = nullptr);

    int _columnCount;
    int _rowCount;

private slots:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    void getTableDims();

private:
    // Widgets
    QLabel *_labelColumns;
    QLabel *_labelRows;
    QSpinBox *_spinBoxRows;
    QSpinBox *_spinBoxColumns;
    QPushButton *_buttonOk;
    QPushButton *_buttonCancel;

    // Layouts
    QHBoxLayout *_horLayButtons;
    QGridLayout *_gridLayMain;
};

#endif // INSERTTABLEDIALOG_H
