#include "include/addbookdialog.h"
#include "include/queries.h"
#include "include/common.h"

#include <QFileDialog>

AddBookDialog::AddBookDialog(QWidget* parent) : QDialog(parent)
{
	setupInterface();
	setupConnections();
	setupTabOrder();
}

void AddBookDialog::setupInterface()
{
	// Window
	resize(700, 520);
	setWindowTitle("Add Book");

	// Widgets
	m_labelTitle = new QLabel("Add Book");
	m_labelTitle->setAlignment(Qt::AlignCenter);

	m_frame = new QFrame();
	m_frame->setObjectName(QString::fromUtf8("frame"));
	m_frame->setStyleSheet(QString::fromUtf8("QFrame#frame { border:  0px; }"));
	m_frame->setFrameShape(QFrame::StyledPanel);
	m_frame->setFrameShadow(QFrame::Raised);

	m_textSize = new QLineEdit();
	m_textSize->setEnabled(false);
	m_textTags = new QLineEdit();
	m_textExtension = new QLineEdit();
	m_textExtension->setEnabled(false);
	m_textExtension->setCursor(QCursor(Qt::PointingHandCursor));
	m_textFolder = new QLineEdit();
	m_textGenre = new QLineEdit();
	m_textPath = new QLineEdit();
	m_textName = new QLineEdit();
	m_textAuthor = new QLineEdit();
	m_textPages = new QLineEdit();
	m_textPublisher = new QLineEdit();
	m_textDatePublished = new QLineEdit();
	m_textSeries = new QLineEdit();
	m_comboBoxRating = new RatingComboBox();
	m_comboBoxRating->setFixedWidth(170);
	m_comboBoxStatus = new StatusComboBox();
	m_comboBoxStatus->setFixedWidth(170);

	m_buttonBrowse = new QPushButton("...");
	m_buttonBrowse->setMinimumSize(40, 0);
	m_buttonBrowse->setMaximumSize(40, 20);
	m_buttonBrowse->setCursor(QCursor(Qt::PointingHandCursor));

	m_labelAuthor = new QLabel("Author");
	m_labelPages = new QLabel("Pages");
	m_labelPath = new QLabel("Path");
	m_labelSize = new QLabel("Size");
	m_labelTags = new QLabel("Tags");
	m_labelGenre = new QLabel("Genre");
	m_labelFolder = new QLabel("Folder");
	m_labelName = new QLabel("Name");
	m_labelExtension = new QLabel("Ext");
	m_labelPublisher = new QLabel("Publisher");
	m_labelDatePublished = new QLabel("Published");
	m_labelSeries = new QLabel("Series");
	m_labelRating = new QLabel("Rating");
	m_labelStatus = new QLabel("Status");

	m_buttonAdd = new QPushButton("Add");
	m_buttonAdd->setMinimumSize(80, 0);
	m_buttonAdd->setCursor(QCursor(Qt::PointingHandCursor));

	m_buttonClear = new QPushButton("Clear");
	m_buttonClear->setMinimumSize(80, 0);
	m_buttonClear->setCursor(QCursor(Qt::PointingHandCursor));

	m_buttonCancel = new QPushButton("Cancel");
	m_buttonCancel->setMinimumSize(80, 0);
	m_buttonCancel->setCursor(QCursor(Qt::PointingHandCursor));

	// Layouts
	m_horLayStatus = new QHBoxLayout();
	m_horLayStatus->setContentsMargins(5, 0, 0, 0);
	m_horLayStatus->setSpacing(10);
	m_horLayStatus->addWidget(m_labelStatus);
	m_horLayStatus->addWidget(m_comboBoxStatus);

	m_gridLay = new QGridLayout(m_frame);
	m_gridLay->setHorizontalSpacing(4);
	m_gridLay->setVerticalSpacing(10);
	m_gridLay->addWidget(m_labelName, 0, 0, 1, 1);
	m_gridLay->addWidget(m_textName, 0, 1, 1, 4);
	m_gridLay->addWidget(m_labelFolder, 1, 0, 1, 1);
	m_gridLay->addWidget(m_textFolder, 1, 1, 1, 4);
	m_gridLay->addWidget(m_textPath, 2, 1, 1, 4);
	m_gridLay->addWidget(m_buttonBrowse, 2, 5, 1, 1);
	m_gridLay->addWidget(m_labelPath, 2, 0, 1, 1);
	m_gridLay->addWidget(m_labelExtension, 3, 0, 1, 1);
	m_gridLay->addWidget(m_textExtension, 3, 1, 1, 4);
	m_gridLay->addWidget(m_labelAuthor, 4, 0, 1, 1);
	m_gridLay->addWidget(m_textAuthor, 4, 1, 1, 4);
	m_gridLay->addWidget(m_labelGenre, 5, 0, 1, 1);
	m_gridLay->addWidget(m_textGenre, 5, 1, 1, 4);
	m_gridLay->addWidget(m_labelSize, 6, 0, 1, 1);
	m_gridLay->addWidget(m_textSize, 6, 1, 1, 4);
	m_gridLay->addWidget(m_labelPages, 7, 0, 1, 1);
	m_gridLay->addWidget(m_textPages, 7, 1, 1, 4);
	m_gridLay->addWidget(m_labelTags, 8, 0, 1, 1);
	m_gridLay->addWidget(m_textTags, 8, 1, 1, 4);
	m_gridLay->addWidget(m_labelPublisher, 9, 0, 1, 1);
	m_gridLay->addWidget(m_textPublisher, 9, 1, 1, 4);
	m_gridLay->addWidget(m_labelDatePublished, 10, 0, 1, 1);
	m_gridLay->addWidget(m_textDatePublished, 10, 1, 1, 4);
	m_gridLay->addWidget(m_labelSeries, 11, 0, 1, 1);
	m_gridLay->addWidget(m_textSeries, 11, 1, 1, 4);
	m_gridLay->addWidget(m_labelRating, 12, 0, 1, 1);
	m_gridLay->addWidget(m_comboBoxRating, 12, 1, 1, 1);
	m_gridLay->addLayout(m_horLayStatus, 12, 2, 1, 1);

	m_horLayButtons = new QHBoxLayout();
	m_horLayButtons->setSpacing(6);
	m_horLayButtons->setContentsMargins(0, 10, 0, 5);
	m_horSpacerButtonsLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_horSpacerButtonsRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_horLayButtons->addItem(m_horSpacerButtonsLeft);
	m_horLayButtons->addWidget(m_buttonAdd);
	m_horLayButtons->addWidget(m_buttonClear);
	m_horLayButtons->addWidget(m_buttonCancel);
	m_horLayButtons->addItem(m_horSpacerButtonsRight);

	m_vertLayMain = new QVBoxLayout();
	m_vertLayMain->addWidget(m_labelTitle);
	m_vertLayMain->addWidget(m_frame);
	m_vertLayMain->addLayout(m_horLayButtons);

	setLayout(m_vertLayMain);
	m_textPath->setCompleter(common::dirCompleter(this));
}

void AddBookDialog::setupConnections()
{
	connect(m_buttonClear, &QPushButton::clicked, this, &AddBookDialog::clearInputs);
	connect(m_buttonBrowse, &QPushButton::clicked, this, &AddBookDialog::browseFile);
	connect(m_buttonCancel, &QPushButton::clicked, this, &AddBookDialog::close);
	connect(m_buttonAdd, &QPushButton::clicked, this, &AddBookDialog::addFile);
}

void AddBookDialog::setupTabOrder()
{
	QWidget::setTabOrder(m_textName, m_textFolder);
	QWidget::setTabOrder(m_textFolder, m_textPath);
	QWidget::setTabOrder(m_textPath, m_buttonBrowse);
	QWidget::setTabOrder(m_buttonBrowse, m_textExtension);
	QWidget::setTabOrder(m_textExtension, m_textAuthor);
	QWidget::setTabOrder(m_textAuthor, m_textGenre);
	QWidget::setTabOrder(m_textGenre, m_textSize);
	QWidget::setTabOrder(m_textSize, m_textPages);
	QWidget::setTabOrder(m_textPages, m_textTags);
	QWidget::setTabOrder(m_textTags, m_textPublisher);
	QWidget::setTabOrder(m_textPublisher, m_textDatePublished);
	QWidget::setTabOrder(m_textDatePublished, m_textSeries);
	QWidget::setTabOrder(m_textSeries, m_comboBoxRating);
	QWidget::setTabOrder(m_comboBoxRating, m_comboBoxStatus);
	QWidget::setTabOrder(m_comboBoxStatus, m_buttonAdd);
	QWidget::setTabOrder(m_buttonAdd, m_buttonClear);
	QWidget::setTabOrder(m_buttonClear, m_buttonCancel);
}

void AddBookDialog::clearInputs()
{
	m_textName->clear();
	m_textFolder->clear();
	m_textAuthor->clear();
	m_textGenre->clear();
	m_textTags->clear();
	m_textSize->clear();
	m_textExtension->clear();
	m_textPath->clear();
	m_textPages->clear();
	m_textPublisher->clear();
	m_textDatePublished->clear();
	m_textSeries->clear();
	m_comboBoxStatus->setCurrentIndex(0);
	m_comboBoxRating->setCurrentIndex(0);
}

void AddBookDialog::browseFile()
{
	QString filePath = QFileDialog::getOpenFileName(this,
			tr("Open File"), "/", tr("All Files (*.*)"));

	QFileInfo file(filePath);
	m_textName->setText(file.completeBaseName());
	m_textExtension->setText("." + file.suffix());
	m_textFolder->setText(file.dir().dirName());
	m_textPath->setText(file.absoluteFilePath());
	m_textSize->setText(QString::number(file.size()));

	quint32 pages = common::getPageCount(filePath);
	m_textPages->setText(QString::number(pages));
}

void AddBookDialog::addFile()
{
	QString name = m_textName->text();
	QString folder = m_textFolder->text();
	QString genre = m_textGenre->text();
	QString path = m_textPath->text();
	QString author = m_textAuthor->text();
	quint32 pages = m_textPages->text().toUInt();
	quint64 size = m_textSize->text().toULongLong();
	QString ext = m_textExtension->text();
	QString tags = m_textTags->text();
	QString publisher = m_textPublisher->text();
	QString datePublished = m_textDatePublished->text();
	QString series = m_textSeries->text();
	quint32 rating = m_comboBoxRating->currentIndex();
	quint32 status = m_comboBoxStatus->currentIndex();

	QFileInfo file(path);
	if (file.exists() && file.isFile())
	{
		queries::insertBooksQuery(name, path, folder, ext, size,
				pages, tags, genre, author, publisher, datePublished, series, rating, status);
		common::showMsgBox("Success!", "Ebook successfully added.",
				":/styles/style.qss", QMessageBox::Information,
				":/icons/books_icon.png");
	}
	else
	{
		common::showMsgBox("Path Error!", "File path not valid!", ":/styles/style.qss", QMessageBox::Warning,
				":/icons/books_icon.png");
	}
}
