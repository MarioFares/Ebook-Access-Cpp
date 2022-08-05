#include "include/insertlinkdialog.h"

InsertLinkDialog::InsertLinkDialog(QWidget* parent)
		:
		QDialog(parent)
{
	m_title = "";
	m_link = "";

	setupInterface();
	setupConnections();
}

void InsertLinkDialog::setupInterface()
{
	// Window
	setWindowTitle("Link");
	setFixedSize(400, 104);

	// Widgets
	m_labelLink = new QLabel("Link: ");
	m_labelTitle = new QLabel("Title: ");

	m_textLink = new QLineEdit();
	m_textTitle = new QLineEdit();

	m_buttonOk = new QPushButton("Ok");
	m_buttonOk->setFlat(true);
	m_buttonOk->setMinimumSize(80, 25);
	m_buttonOk->setCursor(Qt::PointingHandCursor);

	m_buttonCancel = new QPushButton("Cancel");
	m_buttonCancel->setFlat(true);
	m_buttonCancel->setMinimumSize(80, 25);
	m_buttonCancel->setCursor(Qt::PointingHandCursor);

	// Layouts
	m_horLayButtons = new QHBoxLayout();
	m_horSpacerButtonsLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayButtons->addSpacerItem(m_horSpacerButtonsLeft);
	m_horLayButtons->addWidget(m_buttonOk);
	m_horLayButtons->addWidget(m_buttonCancel);

	m_gridLay = new QGridLayout();
	m_gridLay->addWidget(m_labelTitle, 0, 0, 1, 1);
	m_gridLay->addWidget(m_labelLink, 1, 0, 1, 1);
	m_gridLay->addWidget(m_textTitle, 0, 1, 1, 1);
	m_gridLay->addWidget(m_textLink, 1, 1, 1, 1);
	m_gridLay->addLayout(m_horLayButtons, 2, 1, 1, 1);

	setLayout(m_gridLay);
}

void InsertLinkDialog::setupConnections()
{
	connect(m_buttonCancel, &QPushButton::clicked, this, &InsertLinkDialog::close);
	connect(m_buttonOk, &QPushButton::clicked, this, &InsertLinkDialog::getInput);
}

void InsertLinkDialog::getInput()
{
	m_title = m_textTitle->text();
	m_link = m_textLink->text();
	close();
}

void InsertLinkDialog::setDataOnOpen()
{
	if (!m_title.isEmpty() && !m_link.isEmpty())
	{
		m_textLink->setText(m_link);
		m_textTitle->setText(m_title);
	}
}
