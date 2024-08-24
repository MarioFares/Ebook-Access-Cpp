#include "include/queries.h"
#include "include/widgets/booksearchwidget.h"

#include <QUrl>
#include <QBoxLayout>
#include <QDesktopServices>

BookSearchWidget::BookSearchWidget(QWidget *parent) : QWidget(parent) {
    setupInterface();
    setupConnections();
    searchString();
}

void BookSearchWidget::setupInterface() {
    _textSearchBar = new QLineEdit(this);
    _textSearchBar->setClearButtonEnabled(true);
    _textSearchBar->setMinimumHeight(25);

    _listWidget = new QListWidget(this);
    _listWidget->setSortingEnabled(true);

    _vertLayMain = new QVBoxLayout(this);
    _vertLayMain->addWidget(_textSearchBar);
    _vertLayMain->addWidget(_listWidget);

    setLayout(_vertLayMain);
}

void BookSearchWidget::setupConnections() {
    // Outward
    connect(_listWidget, &QListWidget::itemClicked, this, &BookSearchWidget::setupItemClicked);
    connect(_listWidget, &QListWidget::currentTextChanged, this, &BookSearchWidget::selectionChanged);

    connect(_listWidget, &QListWidget::itemActivated, this, &BookSearchWidget::openEbook);
    connect(_listWidget, &QListWidget::itemDoubleClicked, this, &BookSearchWidget::openEbook);

    connect(_textSearchBar, &QLineEdit::textChanged, this, &BookSearchWidget::searchString);
    connect(_textSearchBar, &QLineEdit::returnPressed, this, &BookSearchWidget::searchString);
}

void BookSearchWidget::openEbook(QListWidgetItem *item) {
    QString ebookName = item->text();
    queries::selectPathBasedonName(ebookName);
    queries::query.next();
    QString path = queries::query.value(0).toString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void BookSearchWidget::searchString() {
    _listWidget->clear();
    QString stringToSearch = _textSearchBar->text();
    queries::selectNameBasedOnString(stringToSearch);
    quint32 count = 0;
    while (queries::query.next()) {
        QListWidgetItem *item = new QListWidgetItem(_listWidget);
        item->setText(queries::query.value(0).toString());
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        _listWidget->addItem(item);
        count++;
    }
}

void BookSearchWidget::setMainLayoutMargin(int left, int top, int right, int bottom) {
    _vertLayMain->setContentsMargins(left, top, right, bottom);
}

void BookSearchWidget::setMainLayoutSpacing(int spacing) {
    _vertLayMain->setSpacing(spacing);
}

void BookSearchWidget::clearSearchText() {
    _textSearchBar->clear();
}

void BookSearchWidget::clearListWidget() {
    _listWidget->clear();
}

QString BookSearchWidget::findItem(const QString &name) {
    QList listItems = _listWidget->findItems(name, Qt::MatchExactly);
    QListWidgetItem *item = listItems[0];
    _listWidget->setCurrentItem(item);
    return item->text();
}

void BookSearchWidget::setHideSearchBar(bool hide) {
    _textSearchBar->setHidden(hide);
}

void BookSearchWidget::setHideWidget(bool hide) {
    setHidden(hide);
}

bool BookSearchWidget::searchBarHidden() {
    bool hidden = _textSearchBar->isHidden();
    return hidden;
}

bool BookSearchWidget::widgetHidden() {
    return isHidden();
}

void BookSearchWidget::setupItemClicked(QListWidgetItem *item) {
    QString text = item->text();
    emit itemClicked(text);
}

int BookSearchWidget::currentRow() {
    return _listWidget->currentRow();
}

void BookSearchWidget::setCurrentRow(int row) {
    int lastIndex = _listWidget->count() - 1;
    if (row < 0) {
        _listWidget->setCurrentRow(0);
    } else if (row > lastIndex) {
        _listWidget->setCurrentRow(lastIndex);
    } else {
        _listWidget->setCurrentRow(row);
    }
}

QString BookSearchWidget::currentItemText() {
    QListWidgetItem *item = _listWidget->currentItem();
    return (item ? item->text() : "");
}

void BookSearchWidget::setCurrentItemText(QString text) {
    _listWidget->currentItem()->setText(text);
}
