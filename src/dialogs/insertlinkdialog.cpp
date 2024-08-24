#include "include/dialogs/insertlinkdialog.h"

InsertLinkDialog::InsertLinkDialog(QWidget *parent) : QDialog(parent) {
    _title = "";
    _link = "";

    setupInterface();
    setupConnections();
    setupTabOrder();
}

void InsertLinkDialog::setupInterface() {
    // Window
    setWindowTitle("Link");
    setFixedSize(400, 104);

    // Widgets
    _labelLink = new QLabel("Link: ");
    _labelTitle = new QLabel("Title: ");

    _textLink = new QLineEdit();
    _textTitle = new QLineEdit();

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
    _horSpacerButtonsLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayButtons->addSpacerItem(_horSpacerButtonsLeft);
    _horLayButtons->addWidget(_buttonOk);
    _horLayButtons->addWidget(_buttonCancel);

    _gridLay = new QGridLayout();
    _gridLay->addWidget(_labelTitle, 0, 0, 1, 1);
    _gridLay->addWidget(_labelLink, 1, 0, 1, 1);
    _gridLay->addWidget(_textTitle, 0, 1, 1, 1);
    _gridLay->addWidget(_textLink, 1, 1, 1, 1);
    _gridLay->addLayout(_horLayButtons, 2, 1, 1, 1);

    setLayout(_gridLay);
}

void InsertLinkDialog::setupConnections() {
    connect(_buttonCancel, &QPushButton::clicked, this, &InsertLinkDialog::close);
    connect(_buttonOk, &QPushButton::clicked, this, &InsertLinkDialog::getInput);
}

void InsertLinkDialog::setupTabOrder() {
    QWidget::setTabOrder(_textTitle, _textLink);
    QWidget::setTabOrder(_textLink, _buttonOk);
    QWidget::setTabOrder(_buttonOk, _buttonCancel);
}

void InsertLinkDialog::getInput() {
    _title = _textTitle->text();
    _link = _textLink->text();
    close();
}

void InsertLinkDialog::setDataOnOpen() {
    if (!_title.isEmpty() && !_link.isEmpty()) {
        _textLink->setText(_link);
        _textTitle->setText(_title);
    }
}
