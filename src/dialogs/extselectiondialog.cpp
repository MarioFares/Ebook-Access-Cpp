#include "include/dialogs/extselectiondialog.h"

#include <QVector>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidgetItem>

ExtSelectionDialog::ExtSelectionDialog(QWidget *parent, QVector<QString> extVector, QString title, QString label)
    : QDialog(parent) {
    _inputVector = extVector;
    _title = title;
    _label = label;

    setupInterface();
    setupConnections();
    setupTabOrder();
}

void ExtSelectionDialog::setupInterface() {
    // Window
    setWindowTitle(_title);
    setFixedSize(400, 300);

    // Widgets
    _labelTitle = new QLabel(_label);

    _textSearch = new QLineEdit();
    _textSearch->setClearButtonEnabled(true);

    _listWidget = new QListWidget();
    _listWidget->setSortingEnabled(true);
    fillListWidget(_inputVector);

    _buttonSelectAll = new QPushButton("Select All");
    _buttonSelectAll->setCursor(Qt::PointingHandCursor);
    _buttonSelectAll->setFlat(true);
    _buttonSelectAll->setMinimumSize(85, 25);

    _buttonDeselectAll = new QPushButton("Deselect All");
    _buttonDeselectAll->setCursor(Qt::PointingHandCursor);
    _buttonDeselectAll->setFlat(true);
    _buttonDeselectAll->setMinimumSize(85, 25);

    _buttonContinue = new QPushButton("Continue");
    _buttonContinue->setCursor(Qt::PointingHandCursor);
    _buttonContinue->setFlat(true);
    _buttonContinue->setMinimumSize(85, 25);

    // Layouts
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
    _horLayButtons->addWidget(_buttonDeselectAll);
    _horLayButtons->addWidget(_buttonSelectAll);
    _horLayButtons->addWidget(_buttonContinue);
    _horLayButtons->addSpacerItem(_horSpacerButtonsRight);

    _vertLayMain = new QVBoxLayout();
    _vertLayMain->addLayout(_horLayTitle);
    _vertLayMain->addWidget(_textSearch);
    _vertLayMain->addWidget(_listWidget);
    _vertLayMain->addLayout(_horLayButtons);
    _vertLayMain->setSpacing(10);

    setLayout(_vertLayMain);
}

void ExtSelectionDialog::setupConnections() {
    connect(_listWidget, &QListWidget::itemActivated, this, &ExtSelectionDialog::setItemState);
    connect(_textSearch, &QLineEdit::textChanged, this, &ExtSelectionDialog::searchText);

    connect(_buttonDeselectAll, &QPushButton::clicked, [this] {
        setItemsCheckState(Qt::Unchecked);
    });
    connect(_buttonSelectAll, &QPushButton::clicked, [this] {
        setItemsCheckState(Qt::Checked);
    });
    connect(_buttonContinue, &QPushButton::clicked, [this] {
        setExtVector();
        close();
    });
}

void ExtSelectionDialog::setupTabOrder() {
    QWidget::setTabOrder(_textSearch, _listWidget);
    QWidget::setTabOrder(_listWidget, _buttonDeselectAll);
    QWidget::setTabOrder(_buttonDeselectAll, _buttonSelectAll);
    QWidget::setTabOrder(_buttonSelectAll, _buttonContinue);
}

void ExtSelectionDialog::fillListWidget(QVector<QString> extVector) {
    _listWidget->clear();
    for (QString &entry: extVector) {
        if (entry.isEmpty()) {
            continue;
        }
        QListWidgetItem *item = new QListWidgetItem(_listWidget);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setCheckState(Qt::Checked);
        item->setText(entry);
        _listWidget->addItem(item);
    }
}

QVector<QString> ExtSelectionDialog::getExtVector() {
    return _outputVector;
}

void ExtSelectionDialog::setExtVector() {
    for (int i = 0; i < _listWidget->count(); i++) {
        QListWidgetItem *item = _listWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            _outputVector.push_back(item->text());
        }
    }
}

void ExtSelectionDialog::setItemState(QListWidgetItem *item) {
    item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
}

void ExtSelectionDialog::searchText(const QString &arg1) {
    QList<QListWidgetItem *> items = _listWidget->findItems(arg1, Qt::MatchContains);
    _listWidget->setCurrentItem(items.count() > 0 ? items[0] : _listWidget->currentItem());
}

void ExtSelectionDialog::setItemsCheckState(Qt::CheckState state) {
    for (int i = 0; i < _listWidget->count(); i++) {
        _listWidget->item(i)->setCheckState(state);
    }
}
