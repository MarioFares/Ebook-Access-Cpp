#include "include/common.h"
#include "include/dialogs/getnamedialog.h"

#include <QPushButton>

GetNameDialog::GetNameDialog(QWidget *parent, QString title, QString prompt) : QDialog(parent) {
    _title = title;
    _prompt = prompt;

    setupInterface();
    setupConnections();
    setupTabOrder();
}

void GetNameDialog::setupInterface() {
    // Window
    setWindowTitle(_title);
    setFixedSize(400, 150);

    // Widgets
    _labelTitle = new QLabel(_title);
    _labelPrompt = new QLabel(_prompt);
    _labelPrompt->setWordWrap(true);

    _textName = new QLineEdit();
    _textName->setClearButtonEnabled(true);

    _buttonOk = new QPushButton("Ok");
    _buttonOk->setMinimumSize(80, 25);
    _buttonOk->setFlat(true);
    _buttonOk->setCursor(Qt::PointingHandCursor);

    _buttonCancel = new QPushButton("Cancel");
    _buttonCancel->setMinimumSize(80, 25);
    _buttonCancel->setFlat(true);
    _buttonCancel->setCursor(Qt::PointingHandCursor);

    // Layout
    _horLayTitle = new QHBoxLayout();
    _horSpacerTitleLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horSpacerTitleRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayTitle->addSpacerItem(_horSpacerTitleLeft);
    _horLayTitle->addWidget(_labelTitle);
    _horLayTitle->addSpacerItem(_horSpacerTitleRight);

    _horLayButtons = new QHBoxLayout();
    _horSpacerButtonsLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horSpacerButtonsRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayButtons->addSpacerItem(_horSpacerButtonsLeft);
    _horLayButtons->addWidget(_buttonOk);
    _horLayButtons->addWidget(_buttonCancel);
    _horLayButtons->addSpacerItem(_horSpacerButtonsRight);

    _vertLayMain = new QVBoxLayout();
    _vertLayMain->addLayout(_horLayTitle);
    _vertLayMain->addWidget(_labelPrompt);
    _vertLayMain->addWidget(_textName);
    _vertLayMain->addLayout(_horLayButtons);
    _vertLayMain->setSpacing(10);

    setLayout(_vertLayMain);
}

void GetNameDialog::setupConnections() {
    connect(_buttonCancel, &QPushButton::clicked, this, &GetNameDialog::close);
    connect(_buttonOk, &QPushButton::clicked, this, &GetNameDialog::setName);
}

void GetNameDialog::setupTabOrder() {
    QWidget::setTabOrder(_textName, _buttonOk);
    QWidget::setTabOrder(_buttonOk, _buttonCancel);
}

void GetNameDialog::setName() {
    _name = _textName->text();
    if (_name.isEmpty()) {
        common::showMsgBox("Name Error!", "Name cannot be empty.", ":/styles/style.qss", QMessageBox::Warning,
                           ":/icons/books_icon.png");
    } else {
        this->close();
    }
}

void GetNameDialog::setDataOnOpen() {
    _textName->setText(_name);
}
