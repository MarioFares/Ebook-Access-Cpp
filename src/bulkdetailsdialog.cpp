#include "include/bulkdetailsdialog.h"

#include <QPushButton>

BulkDetailsDialog::BulkDetailsDialog(QWidget* parent) : QDialog(parent)
{
	m_author = "";
	m_genre = "";
	m_tags = "";
	setupInterface();
	setupConnections();
	setupTabOrder();
}

void BulkDetailsDialog::setupInterface()
{
	// Window
	setWindowTitle("Bulk Details");
	setFixedSize(494, 220);

	// Widgets
	m_labelPrompt = new QLabel("If you wish all ebooks in the selected folder to"
							   " share the below data, check the boxes and input"
							   " your specification:");
	m_labelPrompt->setWordWrap(true);

	m_checkBoxGenre = new QCheckBox("");
	m_checkBoxGenre->setChecked(false);

	m_checkBoxAuthor = new QCheckBox("");
	m_checkBoxAuthor->setChecked(false);

	m_checkBoxTags = new QCheckBox("");
	m_checkBoxTags->setChecked(false);

	m_labelGenre = new QLabel("Genre: ");
	m_labelAuthor = new QLabel("Author: ");
	m_labelTags = new QLabel("Tags: ");

	m_textGenre = new QLineEdit();
	m_textGenre->setEnabled(false);
	m_textGenre->setClearButtonEnabled(true);

	m_textAuthor = new QLineEdit();
	m_textAuthor->setEnabled(false);
	m_textAuthor->setClearButtonEnabled(true);

	m_textTags = new QLineEdit();
	m_textTags->setEnabled(false);
	m_textTags->setClearButtonEnabled(true);

	m_buttonApply = new QPushButton("Apply");
	m_buttonApply->setCursor(Qt::PointingHandCursor);
	m_buttonApply->setFlat(true);
	m_buttonApply->setMinimumSize(80, 25);

	m_buttonCancel = new QPushButton("Cancel");
	m_buttonCancel->setCursor(Qt::PointingHandCursor);
	m_buttonCancel->setFlat(true);
	m_buttonCancel->setMinimumSize(80, 25);

	// Layouts
	m_horLayButtons = new QHBoxLayout();
	m_horSpacerButtonsLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayButtons->addSpacerItem(m_horSpacerButtonsLeft);
	m_horLayButtons->addWidget(m_buttonApply);
	m_horLayButtons->addWidget(m_buttonCancel);

	m_gridLay = new QGridLayout();
	m_gridLay->addWidget(m_checkBoxAuthor, 0, 0, 1, 1);
	m_gridLay->addWidget(m_checkBoxGenre, 1, 0, 1, 1);
	m_gridLay->addWidget(m_checkBoxTags, 2, 0, 1, 1);
	m_gridLay->addWidget(m_labelAuthor, 0, 1, 1, 1);
	m_gridLay->addWidget(m_labelGenre, 1, 1, 1, 1);
	m_gridLay->addWidget(m_labelTags, 2, 1, 1, 1);
	m_gridLay->addWidget(m_textAuthor, 0, 2, 1, 1);
	m_gridLay->addWidget(m_textGenre, 1, 2, 1, 1);
	m_gridLay->addWidget(m_textTags, 2, 2, 1, 1);

	m_vertLayMain = new QVBoxLayout();
	m_vertLayMain->addWidget(m_labelPrompt);
	m_vertLayMain->addLayout(m_gridLay);
	m_vertLayMain->addLayout(m_horLayButtons);

	setLayout(m_vertLayMain);
}

void BulkDetailsDialog::setupConnections()
{
	connect(m_buttonCancel, &QPushButton::clicked, this, &BulkDetailsDialog::close);
	connect(m_buttonApply, &QPushButton::clicked, this, &BulkDetailsDialog::applyFields);
	connect(m_checkBoxAuthor, &QCheckBox::stateChanged, m_textAuthor, &QLineEdit::setEnabled);
	connect(m_checkBoxGenre, &QCheckBox::stateChanged, m_textGenre, &QLineEdit::setEnabled);
	connect(m_checkBoxTags, &QCheckBox::stateChanged, m_textTags, &QLineEdit::setEnabled);
}

void BulkDetailsDialog::setupTabOrder()
{
	QWidget::setTabOrder(m_checkBoxAuthor, m_textAuthor);
	QWidget::setTabOrder(m_textAuthor, m_checkBoxGenre);
	QWidget::setTabOrder(m_checkBoxGenre, m_textGenre);
	QWidget::setTabOrder(m_textGenre, m_checkBoxTags);
	QWidget::setTabOrder(m_checkBoxTags, m_textTags);
	QWidget::setTabOrder(m_textTags, m_buttonApply);
	QWidget::setTabOrder(m_buttonApply, m_buttonCancel);
}

void BulkDetailsDialog::applyFields()
{
	if (m_textAuthor->isEnabled())
	{
		m_author = m_textAuthor->text();
	}

	if (m_textGenre->isEnabled())
	{
		m_genre = m_textGenre->text();
	}

	if (m_textTags->isEnabled())
	{
		m_tags = m_textTags->text();
	}

	close();
}
