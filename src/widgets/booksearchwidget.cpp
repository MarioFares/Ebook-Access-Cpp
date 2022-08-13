#include "include/queries.h"
#include "include/widgets/booksearchwidget.h"

#include <QUrl>
#include <QBoxLayout>
#include <QDesktopServices>

BookSearchWidget::BookSearchWidget(QWidget* parent) : QWidget(parent)
{
	setupInterface();
	setupConnections();
	searchString();
}

void BookSearchWidget::setupInterface()
{
	m_textSearchBar = new QLineEdit(this);
	m_textSearchBar->setClearButtonEnabled(true);
	m_textSearchBar->setMinimumHeight(25);

	m_listWidget = new QListWidget(this);
	m_listWidget->setSortingEnabled(true);

	m_vertLayMain = new QVBoxLayout(this);
	m_vertLayMain->addWidget(m_textSearchBar);
	m_vertLayMain->addWidget(m_listWidget);

	setLayout(m_vertLayMain);
}

void BookSearchWidget::setupConnections()
{
	// Outward
	connect(m_listWidget, &QListWidget::itemClicked, this, &BookSearchWidget::setupItemClicked);
	connect(m_listWidget, &QListWidget::currentTextChanged, this, &BookSearchWidget::selectionChanged);

	connect(m_listWidget, &QListWidget::itemActivated, this, &BookSearchWidget::openEbook);
	connect(m_listWidget, &QListWidget::itemDoubleClicked, this, &BookSearchWidget::openEbook);

	connect(m_textSearchBar, &QLineEdit::textChanged, this, &BookSearchWidget::searchString);
	connect(m_textSearchBar, &QLineEdit::returnPressed, this, &BookSearchWidget::searchString);
}

void BookSearchWidget::openEbook(QListWidgetItem* item)
{
	QString ebookName = item->text();
	queries::selectPathBasedonName(ebookName);
	queries::query.next();
	QString path = queries::query.value(0).toString();
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void BookSearchWidget::searchString()
{
	m_listWidget->clear();
	QString stringToSearch = m_textSearchBar->text();
	queries::selectNameBasedOnString(stringToSearch);
	quint32 count = 0;
	while (queries::query.next())
	{
		QListWidgetItem* item = new QListWidgetItem(m_listWidget);
		item->setText(queries::query.value(0).toString());
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		m_listWidget->addItem(item);
		count++;
	}
}

void BookSearchWidget::setMainLayoutMargin(int left, int top, int right, int bottom)
{
	m_vertLayMain->setContentsMargins(left, top, right, bottom);
}

void BookSearchWidget::setMainLayoutSpacing(int spacing)
{
	m_vertLayMain->setSpacing(spacing);
}

void BookSearchWidget::clearSearchText()
{
	m_textSearchBar->clear();
}

void BookSearchWidget::clearListWidget()
{
	m_listWidget->clear();
}

QString BookSearchWidget::findItem(const QString& name)
{
	QList listItems = m_listWidget->findItems(name, Qt::MatchExactly);
	QListWidgetItem* item = listItems[0];
	m_listWidget->setCurrentItem(item);
	return item->text();
}

void BookSearchWidget::setHideSearchBar(bool hide)
{
	m_textSearchBar->setHidden(hide);
}

void BookSearchWidget::setHideWidget(bool hide)
{
	setHidden(hide);
}

bool BookSearchWidget::searchBarHidden()
{
	bool hidden = m_textSearchBar->isHidden();
	return hidden;
}

bool BookSearchWidget::widgetHidden()
{
	return isHidden();
}

void BookSearchWidget::setupItemClicked(QListWidgetItem* item)
{
	QString text = item->text();
	emit itemClicked(text);
}

int BookSearchWidget::currentRow()
{
	return m_listWidget->currentRow();
}

void BookSearchWidget::setCurrentRow(int row)
{
	int lastIndex = m_listWidget->count() - 1;
	if (row < 0)
	{
		m_listWidget->setCurrentRow(0);
	}
	else if (row > lastIndex)
	{
		m_listWidget->setCurrentRow(lastIndex);
	}
	else
	{
		m_listWidget->setCurrentRow(row);
	}
}
