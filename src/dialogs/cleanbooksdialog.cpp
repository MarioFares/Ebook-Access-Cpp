#include "include/dialogs/cleanbooksdialog.h"
#include "include/queries.h"

#include <QFileInfo>
#include <QBoxLayout>
#include <QPushButton>

CleanBooksDialog::CleanBooksDialog(QWidget *parent) : QDialog(parent) {
    setupInterface();
    setupConnections();
    setupTabOrder();
}

void CleanBooksDialog::setupInterface() {
    // Window
    setWindowTitle("Clean Ebooks");
    setFixedSize(400, 300);

    // Widgets
    _labelTitle = new QLabel("Clean Ebooks");

    _buttonBegin = new QPushButton("Begin");
    _buttonBegin->setCursor(Qt::PointingHandCursor);
    _buttonBegin->setFlat(true);
    _buttonBegin->setMinimumSize(80, 25);

    _listWidget = new QListWidget();

    // Layouts
    _horLayTitle = new QHBoxLayout();
    _horSpacerTitleLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horSpacerTitleRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayTitle->addSpacerItem(_horSpacerTitleLeft);
    _horLayTitle->addWidget(_labelTitle);
    _horLayTitle->addSpacerItem(_horSpacerTitleRight);

    _horLayButton = new QHBoxLayout();
    _horSpacerButtonLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horSpacerButtonRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayButton->addSpacerItem(_horSpacerButtonLeft);
    _horLayButton->addWidget(_buttonBegin);
    _horLayButton->addSpacerItem(_horSpacerButtonRight);

    _vertLayMain = new QVBoxLayout();
    _vertLayMain->addLayout(_horLayTitle);
    _vertLayMain->addLayout(_horLayButton);
    _vertLayMain->addWidget(_listWidget);
    _vertLayMain->setSpacing(30);

    setLayout(_vertLayMain);
}

void CleanBooksDialog::setupConnections() {
    connect(_buttonBegin, &QPushButton::clicked, this, &CleanBooksDialog::beginClean);
}

void CleanBooksDialog::setupTabOrder() {
    QWidget::setTabOrder(_buttonBegin, _listWidget);
}

void CleanBooksDialog::beginClean() {
    queries::selectPath();
    while (queries::query.next()) {
        QString path = queries::query.value(0).toString();
        if (!QFileInfo::exists(path)) {
            _listWidget->addItem(path);
        }
    }
}
