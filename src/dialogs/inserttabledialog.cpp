#include "include/dialogs/inserttabledialog.h"

InsertTableDialog::InsertTableDialog(QWidget *parent) : QDialog(parent) {
    _columnCount = 1;
    _rowCount = 1;

    setupInterface();
    setupConnections();
    setupTabOrder();
}

void InsertTableDialog::setupInterface() {
    // Window
    setWindowTitle("Insert Table");
    setFixedSize(250, 115);

    // Widgets
    _labelRows = new QLabel("Rows: ");
    _labelColumns = new QLabel("Columns: ");

    _spinBoxRows = new QSpinBox();
    _spinBoxRows->setValue(1);
    _spinBoxRows->setMinimum(1);

    _spinBoxColumns = new QSpinBox();
    _spinBoxColumns->setValue(1);
    _spinBoxColumns->setMinimum(1);

    _buttonOk = new QPushButton("Ok");
    _buttonOk->setFlat(true);
    _buttonOk->setMinimumSize(80, 25);
    _buttonOk->setCursor(Qt::PointingHandCursor);

    _buttonCancel = new QPushButton("Cancel");
    _buttonCancel->setFlat(true);
    _buttonCancel->setMinimumSize(80, 25);
    _buttonCancel->setCursor(Qt::PointingHandCursor);

    // Layouts
    _horLayButtons = new QHBoxLayout();
    _horLayButtons->addWidget(_buttonOk);
    _horLayButtons->addWidget(_buttonCancel);

    _gridLayMain = new QGridLayout;
    _gridLayMain->addWidget(_labelRows, 0, 0, 1, 1);
    _gridLayMain->addWidget(_labelColumns, 1, 0, 1, 1);
    _gridLayMain->addWidget(_spinBoxRows, 0, 1, 1, 1);
    _gridLayMain->addWidget(_spinBoxColumns, 1, 1, 1, 1);
    _gridLayMain->addLayout(_horLayButtons, 2, 1, 1, 1);

    setLayout(_gridLayMain);
}

void InsertTableDialog::setupConnections() {
    connect(_buttonCancel, &QPushButton::clicked, this, &InsertTableDialog::close);
    connect(_buttonOk, &QPushButton::clicked, this, &InsertTableDialog::getTableDims);
}

void InsertTableDialog::setupTabOrder() {
    QWidget::setTabOrder(_spinBoxRows, _spinBoxColumns);
    QWidget::setTabOrder(_spinBoxColumns, _buttonOk);
    QWidget::setTabOrder(_buttonOk, _buttonCancel);
}

void InsertTableDialog::getTableDims() {
    _rowCount = _spinBoxRows->value();
    _columnCount = _spinBoxColumns->value();
    close();
}
