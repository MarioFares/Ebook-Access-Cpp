#include "include/bulkdetailsdialog.h"

#include <QPushButton>

BulkDetailsDialog::BulkDetailsDialog(QWidget* parent) : QDialog(parent)
{
	m_author = "";
	m_genre = "";
	m_tags = "";
	m_publisher = "";
	m_datePublished = "";
	m_series = "";
	m_rating = 0;
	m_status = 0;
	setupInterface();
	setupConnections();
	setupTabOrder();
}

void BulkDetailsDialog::setupInterface()
{
	// Window
	setWindowTitle("Bulk Details");
	setFixedSize(494, 320);

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

	m_checkBoxPublisher = new QCheckBox("");
	m_checkBoxPublisher->setChecked(false);

	m_checkBoxDatePublished = new QCheckBox("");
	m_checkBoxDatePublished->setChecked(false);

	m_checkBoxSeries = new QCheckBox("");
	m_checkBoxSeries->setChecked(false);

	m_checkBoxRating = new QCheckBox("");
	m_checkBoxRating->setChecked(false);

	m_checkBoxStatus = new QCheckBox("");
	m_checkBoxStatus->setChecked(false);

	m_labelGenre = new QLabel("Genre: ");
	m_labelAuthor = new QLabel("Author: ");
	m_labelTags = new QLabel("Tags: ");
	m_labelPublisher = new QLabel("Publisher: ");
	m_labelDatePublished = new QLabel("Published: ");
	m_labelSeries = new QLabel("Series: ");
	m_labelStatus = new QLabel("Status: ");
	m_labelRating = new QLabel("Rating: ");

	m_textGenre = new QLineEdit();
	m_textGenre->setEnabled(false);
	m_textGenre->setClearButtonEnabled(true);

	m_textAuthor = new QLineEdit();
	m_textAuthor->setEnabled(false);
	m_textAuthor->setClearButtonEnabled(true);

	m_textTags = new QLineEdit();
	m_textTags->setEnabled(false);
	m_textTags->setClearButtonEnabled(true);

	m_textPublisher = new QLineEdit();
	m_textPublisher->setEnabled(false);
	m_textPublisher->setClearButtonEnabled(true);

	m_textDatePublished = new QLineEdit();
	m_textDatePublished->setEnabled(false);
	m_textDatePublished->setClearButtonEnabled(true);

	m_textSeries = new QLineEdit();
	m_textSeries->setEnabled(false);
	m_textSeries->setClearButtonEnabled(true);

	m_comboBoxStatus = new StatusComboBox();
	m_comboBoxStatus->setEnabled(false);

	m_comboBoxRating = new RatingComboBox();
	m_comboBoxRating->setEnabled(false);

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
	m_gridLay->addWidget(m_labelAuthor, 0, 1, 1, 1);
	m_gridLay->addWidget(m_textAuthor, 0, 2, 1, 1);
	m_gridLay->addWidget(m_checkBoxGenre, 1, 0, 1, 1);
	m_gridLay->addWidget(m_labelGenre, 1, 1, 1, 1);
	m_gridLay->addWidget(m_textGenre, 1, 2, 1, 1);
	m_gridLay->addWidget(m_checkBoxTags, 2, 0, 1, 1);
	m_gridLay->addWidget(m_labelTags, 2, 1, 1, 1);
	m_gridLay->addWidget(m_textTags, 2, 2, 1, 1);
	m_gridLay->addWidget(m_checkBoxPublisher, 3, 0, 1, 1);
	m_gridLay->addWidget(m_labelPublisher, 3, 1, 1, 1);
	m_gridLay->addWidget(m_textPublisher, 3, 2, 1, 1);
	m_gridLay->addWidget(m_checkBoxDatePublished, 4, 0, 1, 1);
	m_gridLay->addWidget(m_labelDatePublished, 4, 1, 1, 1);
	m_gridLay->addWidget(m_textDatePublished, 4, 2, 1, 1);
	m_gridLay->addWidget(m_checkBoxSeries, 5, 0, 1, 1);
	m_gridLay->addWidget(m_labelSeries, 5, 1, 1, 1);
	m_gridLay->addWidget(m_textSeries, 5, 2, 1, 1);
	m_gridLay->addWidget(m_checkBoxRating, 6, 0, 1, 1);
	m_gridLay->addWidget(m_labelRating, 6, 1, 1, 1);
	m_gridLay->addWidget(m_comboBoxRating, 6, 2, 1, 1);
	m_gridLay->addWidget(m_checkBoxStatus, 7, 0, 1, 1);
	m_gridLay->addWidget(m_labelStatus, 7, 1, 1, 1);
	m_gridLay->addWidget(m_comboBoxStatus, 7, 2, 1, 1);

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
	connect(m_checkBoxPublisher, &QCheckBox::stateChanged, m_textPublisher, &QLineEdit::setEnabled);
	connect(m_checkBoxDatePublished, &QCheckBox::stateChanged, m_textDatePublished, &QLineEdit::setEnabled);
	connect(m_checkBoxSeries, &QCheckBox::stateChanged, m_textSeries, &QLineEdit::setEnabled);
	connect(m_checkBoxRating, &QCheckBox::stateChanged, m_comboBoxRating, &QLineEdit::setEnabled);
	connect(m_checkBoxStatus, &QCheckBox::stateChanged, m_comboBoxStatus, &QLineEdit::setEnabled);
}

void BulkDetailsDialog::setupTabOrder()
{
	QWidget::setTabOrder(m_checkBoxAuthor, m_textAuthor);
	QWidget::setTabOrder(m_textAuthor, m_checkBoxGenre);
	QWidget::setTabOrder(m_checkBoxGenre, m_textGenre);
	QWidget::setTabOrder(m_textGenre, m_checkBoxTags);
	QWidget::setTabOrder(m_checkBoxTags, m_textTags);
	QWidget::setTabOrder(m_textTags, m_checkBoxPublisher);
	QWidget::setTabOrder(m_checkBoxPublisher, m_textPublisher);
	QWidget::setTabOrder(m_textPublisher, m_checkBoxDatePublished);
	QWidget::setTabOrder(m_checkBoxDatePublished, m_textDatePublished);
	QWidget::setTabOrder(m_textDatePublished, m_checkBoxSeries);
	QWidget::setTabOrder(m_checkBoxSeries, m_textSeries);
	QWidget::setTabOrder(m_textSeries, m_checkBoxRating);
	QWidget::setTabOrder(m_checkBoxRating, m_comboBoxRating);
	QWidget::setTabOrder(m_comboBoxRating, m_checkBoxStatus);
	QWidget::setTabOrder(m_checkBoxStatus, m_comboBoxStatus);
	QWidget::setTabOrder(m_comboBoxStatus, m_buttonApply);
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

	if (m_textPublisher->isEnabled())
	{
		m_publisher = m_textPublisher->text();
	}

	if (m_textDatePublished->isEnabled())
	{
		m_datePublished = m_textDatePublished->text();
	}

	if (m_textSeries->isEnabled())
	{
		m_series = m_textSeries->text();
	}

	if (m_comboBoxRating->isEnabled())
	{
		m_rating = m_comboBoxRating->currentIndex();
	}

	if (m_comboBoxStatus->isEnabled())
	{
		m_status = m_comboBoxStatus->currentIndex();
	}

	close();
}
