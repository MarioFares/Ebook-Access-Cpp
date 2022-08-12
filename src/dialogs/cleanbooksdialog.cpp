#include "include/dialogs/cleanbooksdialog.h"
#include "include/queries.h"

#include <QFileInfo>
#include <QBoxLayout>
#include <QPushButton>

CleanBooksDialog::CleanBooksDialog(QWidget* parent) : QDialog(parent)
{
	setupInterface();
	setupConnections();
	setupTabOrder();
}

void CleanBooksDialog::setupInterface()
{
	// Window
	setWindowTitle("Clean Ebooks");
	setFixedSize(400, 300);

	// Widgets
	m_labelTitle = new QLabel("Clean Ebooks");

	m_buttonBegin = new QPushButton("Begin");
	m_buttonBegin->setCursor(Qt::PointingHandCursor);
	m_buttonBegin->setFlat(true);
	m_buttonBegin->setMinimumSize(80, 25);

	m_listWidget = new QListWidget();

	// Layouts
	m_horLayTitle = new QHBoxLayout();
	m_horSpacerTitleLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horSpacerTitleRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayTitle->addSpacerItem(m_horSpacerTitleLeft);
	m_horLayTitle->addWidget(m_labelTitle);
	m_horLayTitle->addSpacerItem(m_horSpacerTitleRight);

	m_horLayButton = new QHBoxLayout();
	m_horSpacerButtonLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horSpacerButtonRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayButton->addSpacerItem(m_horSpacerButtonLeft);
	m_horLayButton->addWidget(m_buttonBegin);
	m_horLayButton->addSpacerItem(m_horSpacerButtonRight);

	m_vertLayMain = new QVBoxLayout();
	m_vertLayMain->addLayout(m_horLayTitle);
	m_vertLayMain->addLayout(m_horLayButton);
	m_vertLayMain->addWidget(m_listWidget);
	m_vertLayMain->setSpacing(30);

	setLayout(m_vertLayMain);
}

void CleanBooksDialog::setupConnections()
{
	connect(m_buttonBegin, &QPushButton::clicked, this, &CleanBooksDialog::beginClean);
}

void CleanBooksDialog::setupTabOrder()
{
	QWidget::setTabOrder(m_buttonBegin, m_listWidget);
}

void CleanBooksDialog::beginClean()
{
	queries::selectPath();
	while (queries::query.next())
	{
		QString path = queries::query.value(0).toString();
		if (!QFileInfo::exists(path))
		{
			m_listWidget->addItem(path);
		}
	}
}
