#include "include/yesnodialog.h"

YesNoDialog::YesNoDialog(QWidget* parent, QString windowTitle, QString title, QString prompt)
		:
		QDialog(parent)
{
	m_windowTitle = windowTitle;
	m_title = title;
	m_prompt = prompt;
	m_result = false;

	setupInterface();
	setupConnections();
}

void YesNoDialog::setupInterface()
{
	// Window
	setWindowTitle(m_windowTitle);
	setFixedSize(390, 125);

	// Widgets
	m_labelTitle = new QLabel(m_title);
	m_labelPrompt = new QLabel(m_prompt);

	m_buttonYes = new QPushButton("Yes");
	m_buttonYes->setFlat(true);
	m_buttonYes->setCursor(Qt::PointingHandCursor);
	m_buttonYes->setMinimumSize(80, 25);

	m_buttonNo = new QPushButton("No");
	m_buttonNo->setFlat(true);
	m_buttonNo->setCursor(Qt::PointingHandCursor);
	m_buttonNo->setMinimumSize(80, 25);

	// Layouts
	m_horLayTitle = new QHBoxLayout();
	m_horSpacerTitleLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horSpacerTitleRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayTitle->addSpacerItem(m_horSpacerTitleLeft);
	m_horLayTitle->addWidget(m_labelTitle);
	m_horLayTitle->addSpacerItem(m_horSpacerTitleRight);

	m_horLayButtons = new QHBoxLayout();
	m_horSpacerButtonsLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayButtons->addSpacerItem(m_horSpacerButtonsLeft);
	m_horLayButtons->addWidget(m_buttonYes);
	m_horLayButtons->addWidget(m_buttonNo);

	m_vertLayMain = new QVBoxLayout();
	m_vertLayMain->addLayout(m_horLayTitle);
	m_vertLayMain->addWidget(m_labelPrompt);
	m_vertLayMain->addLayout(m_horLayButtons);

	setLayout(m_vertLayMain);
}

void YesNoDialog::setupConnections()
{
	connect(m_buttonYes, &QPushButton::clicked, this, &YesNoDialog::setResult);
	connect(m_buttonNo, &QPushButton::clicked, this, &YesNoDialog::setResult);
}

void YesNoDialog::setResult()
{
	m_result = sender() == m_buttonYes;
	close();
}

bool YesNoDialog::getResult() const
{
	return m_result;
}
