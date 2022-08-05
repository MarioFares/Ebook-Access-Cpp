#include "include/common.h"
#include "include/getnamedialog.h"

#include <QPushButton>

GetNameDialog::GetNameDialog(QWidget* parent, QString title, QString prompt)
		:
		QDialog(parent)
{
	m_title = title;
	m_prompt = prompt;

	setupInterface();
	setupConnections();
}

void GetNameDialog::setupInterface()
{
	// Window
	setWindowTitle(m_title);
	setFixedSize(400, 150);

	// Widgets
	m_labelTitle = new QLabel(m_title);
	m_labelPrompt = new QLabel(m_prompt);
	m_labelPrompt->setWordWrap(true);

	m_textName = new QLineEdit();
	m_textName->setClearButtonEnabled(true);

	m_buttonOk = new QPushButton("Ok");
	m_buttonOk->setMinimumSize(80, 25);
	m_buttonOk->setFlat(true);
	m_buttonOk->setCursor(Qt::PointingHandCursor);

	m_buttonCancel = new QPushButton("Cancel");
	m_buttonCancel->setMinimumSize(80, 25);
	m_buttonCancel->setFlat(true);
	m_buttonCancel->setCursor(Qt::PointingHandCursor);

	// Layout
	m_horLayTitle = new QHBoxLayout();
	m_horSpacerTitleLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horSpacerTitleRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayTitle->addSpacerItem(m_horSpacerTitleLeft);
	m_horLayTitle->addWidget(m_labelTitle);
	m_horLayTitle->addSpacerItem(m_horSpacerTitleRight);

	m_horLayButtons = new QHBoxLayout();
	m_horSpacerButtonsLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horSpacerButtonsRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayButtons->addSpacerItem(m_horSpacerButtonsLeft);
	m_horLayButtons->addWidget(m_buttonOk);
	m_horLayButtons->addWidget(m_buttonCancel);
	m_horLayButtons->addSpacerItem(m_horSpacerButtonsRight);

	m_vertLayMain = new QVBoxLayout();
	m_vertLayMain->addLayout(m_horLayTitle);
	m_vertLayMain->addWidget(m_labelPrompt);
	m_vertLayMain->addWidget(m_textName);
	m_vertLayMain->addLayout(m_horLayButtons);
	m_vertLayMain->setSpacing(10);

	setLayout(m_vertLayMain);
}

void GetNameDialog::setupConnections()
{
	connect(m_buttonCancel, &QPushButton::clicked, this, &GetNameDialog::close);
	connect(m_buttonOk, &QPushButton::clicked, this, &GetNameDialog::setName);
}

void GetNameDialog::setName()
{
	m_name = m_textName->text();
	if (m_name.isEmpty())
	{
		common::showMsgBox("Name Error!", "Name cannot be empty.", ":/styles/style.qss", QMessageBox::Warning,
				":/icons/books_icon.png");
	}
	else
	{
		this->close();
	}
}

void GetNameDialog::setDataOnOpen()
{
	m_textName->setText(m_name);
}
