#include "include/dialogs/yesnodialog.h"

YesNoDialog::YesNoDialog(QWidget *parent, QString windowTitle, QString title, QString prompt) : QDialog(parent) {
    _windowTitle = windowTitle;
    _title = title;
    _prompt = prompt;
    _result = false;

    setupInterface();
    setupConnections();
    setupTabOrder();
}

void YesNoDialog::setupInterface() {
    // Window
    setWindowTitle(_windowTitle);
    setFixedSize(390, 125);

    // Widgets
    _labelTitle = new QLabel(_title);
    _labelPrompt = new QLabel(_prompt);

    _buttonYes = new QPushButton("Yes");
    _buttonYes->setFlat(true);
    _buttonYes->setCursor(Qt::PointingHandCursor);
    _buttonYes->setMinimumSize(80, 25);

    _buttonNo = new QPushButton("No");
    _buttonNo->setFlat(true);
    _buttonNo->setCursor(Qt::PointingHandCursor);
    _buttonNo->setMinimumSize(80, 25);

    // Layouts
    _horLayTitle = new QHBoxLayout();
    _horSpacerTitleLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horSpacerTitleRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayTitle->addSpacerItem(_horSpacerTitleLeft);
    _horLayTitle->addWidget(_labelTitle);
    _horLayTitle->addSpacerItem(_horSpacerTitleRight);

    _horLayButtons = new QHBoxLayout();
    _horSpacerButtonsLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayButtons->addSpacerItem(_horSpacerButtonsLeft);
    _horLayButtons->addWidget(_buttonYes);
    _horLayButtons->addWidget(_buttonNo);

    _vertLayMain = new QVBoxLayout();
    _vertLayMain->addLayout(_horLayTitle);
    _vertLayMain->addWidget(_labelPrompt);
    _vertLayMain->addLayout(_horLayButtons);

    setLayout(_vertLayMain);
}

void YesNoDialog::setupConnections() {
    connect(_buttonYes, &QPushButton::clicked, this, &YesNoDialog::setResult);
    connect(_buttonNo, &QPushButton::clicked, this, &YesNoDialog::setResult);
}

void YesNoDialog::setupTabOrder() {
    QWidget::setTabOrder(_buttonYes, _buttonNo);
}

void YesNoDialog::setResult() {
    _result = sender() == _buttonYes;
    close();
}

bool YesNoDialog::getResult() const {
    return _result;
}
