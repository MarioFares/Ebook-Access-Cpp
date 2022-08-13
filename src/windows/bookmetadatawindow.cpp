#include "include/queries.h"
#include "include/widgets/statuscombobox.h"
#include "include/widgets/ratingcombobox.h"
#include "include/widgets/booksearchwidget.h"
#include "include/windows/bookmetadatawindow.h"

#include <QDesktopServices>

BookMetadataWindow::BookMetadataWindow(QWidget* parent) : QMainWindow(parent)
{
	queries::connectToDatabase();

	setupInterface();
	setupConnections();
	setupTabOrder();
}

void BookMetadataWindow::setupInterface()
{
	// Window
	resize(1075, 706);
	setWindowTitle("Metadata");

	// Widgets
	m_centralwidget = new QWidget(this);

	m_rightSplitter = new QSplitter(m_centralwidget);
	m_rightSplitter->setOrientation(Qt::Horizontal);
	m_rightSplitter->setHandleWidth(0);

	m_splitter = new QSplitter(m_rightSplitter);
	m_splitter->setOrientation(Qt::Horizontal);
	m_splitter->setHandleWidth(0);

	m_bookSearchWidget = new BookSearchWidget(this);
	m_bookSearchWidget->setMainLayoutMargin(0, 0, 0, 0);

	m_verticalSplitter = new QSplitter(m_splitter);
	m_verticalSplitter->setObjectName(QString::fromUtf8("verticalSplitter"));
	m_verticalSplitter->setStyleSheet(QString::fromUtf8("#verticalSplitter { border: none; } "));
	m_verticalSplitter->setOrientation(Qt::Vertical);
	m_verticalSplitter->setHandleWidth(0);

	m_labelName = new QLabel("Name");
	m_labelName->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_textGenre = new QLineEdit();
	m_textGenre->setMinimumSize(QSize(0, 23));

	m_labelDatePublished = new QLabel("Published");
	m_labelDatePublished->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_textPublisher = new QLineEdit();
	m_textPublisher->setMinimumSize(QSize(0, 23));

	m_textDatePublished = new QLineEdit();
	m_textDatePublished->setMinimumSize(QSize(0, 23));

	m_labelPath = new QLabel("Path");
	m_labelPath->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_textExt = new QLineEdit();
	m_textExt->setMinimumSize(QSize(0, 23));

	m_labelDateAdded = new QLabel("Added");
	m_labelDateAdded->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_textAuthor = new QLineEdit();
	m_textAuthor->setMinimumSize(QSize(0, 23));

	m_comboBoxRating = new RatingComboBox();
	m_comboBoxRating->setMinimumSize(QSize(170, 23));
	m_comboBoxRating->setMaximumSize(QSize(170, 23));

	m_labelExt = new QLabel("Extension");
	m_labelExt->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_labelFolder = new QLabel("Folder");
	m_labelFolder->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_textFolder = new QLineEdit();
	m_textFolder->setMinimumSize(QSize(0, 23));

	m_labelPages = new QLabel("Pages");
	m_labelPages->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_textSeries = new QLineEdit();
	m_textSeries->setMinimumSize(QSize(0, 23));

	m_labelSeries = new QLabel("Series");
	m_labelSeries->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_textDateAdded = new QLineEdit();
	m_textDateAdded->setMinimumSize(QSize(0, 23));

	m_labelRating = new QLabel("Rating");
	m_labelRating->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_textPages = new QLineEdit();
	m_textPages->setMinimumSize(QSize(0, 23));

	m_labelSize = new QLabel("Size");
	m_labelSize->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_labelAuthor = new QLabel("Author");
	m_labelAuthor->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_labelGenre = new QLabel("Genre");
	m_labelGenre->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_labelPublisher = new QLabel("Publisher");
	m_labelPublisher->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_textName = new QLineEdit();
	m_textName->setMinimumSize(QSize(0, 23));

	m_labelStatus = new QLabel("Status");
	m_labelStatus->setMinimumSize(QSize(70, 0));
	m_labelStatus->setMaximumSize(QSize(70, 180));
	m_labelStatus->setAlignment(Qt::AlignCenter);

	m_comboBoxStatus = new StatusComboBox();
	m_comboBoxStatus->setMinimumSize(QSize(170, 23));
	m_comboBoxStatus->setMaximumSize(QSize(170, 180));

	m_textPath = new QLineEdit();
	m_textPath->setMinimumSize(QSize(0, 23));

	m_textSize = new QLineEdit();
	m_textSize->setMinimumSize(QSize(0, 23));

	m_labelTags = new QLabel("Tags");
	m_labelTags->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

	m_textTags = new QLineEdit();
	m_textTags->setMinimumSize(QSize(0, 23));

	m_labelModified = new QLabel("Modified");

	m_textDateModified = new QLineEdit();

	m_frameComments = new QFrame();
	m_frameComments->setObjectName(QString::fromUtf8("frameComments"));
	m_frameComments->setStyleSheet(QString::fromUtf8("#frameComments{ border:none;}"));

	m_labelComments = new QLabel("Notes/Comments");

	m_textEditComments = new QPlainTextEdit();
	QPalette p = m_textEditComments->palette();
	p.setColor(QPalette::Text, Qt::white);
	m_textEditComments->setPalette(p);

	m_graphicsView = new QGraphicsView();

	m_frameButtons = new QFrame();
	m_frameButtons->setObjectName(QString::fromUtf8("frameButtons"));
	m_frameButtons->setStyleSheet(QString::fromUtf8("#frameButtons { border: none;\n }"));

	m_buttonUpdate = new QPushButton("Update");
	m_buttonUpdate->setMinimumSize(QSize(70, 30));
	m_buttonUpdate->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonUpdate->setFlat(true);

	m_buttonPrev = new QPushButton("Previous");
	m_buttonPrev->setMinimumSize(QSize(70, 30));
	m_buttonPrev->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonPrev->setFlat(true);

	m_buttonNext = new QPushButton("Next");
	m_buttonNext->setMinimumSize(QSize(70, 30));
	m_buttonNext->setCursor(QCursor(Qt::PointingHandCursor));
	m_buttonNext->setFlat(true);

	m_vertLineButtons = new QFrame(m_frameButtons);
	m_vertLineButtons->setFrameShape(QFrame::VLine);
	m_vertLineButtons->setFrameShadow(QFrame::Sunken);
	m_vertLineButtons->setStyleSheet(QString("color: grey; background-color: grey;"));

	// Layouts
	m_horLayStatus = new QHBoxLayout();
	m_horLayStatus->addWidget(m_labelStatus);
	m_horLayStatus->addWidget(m_comboBoxStatus);

	m_gridLayData = new QGridLayout();
	m_gridLayData->setHorizontalSpacing(0);
	m_gridLayData->setVerticalSpacing(10);
	m_gridLayData->setContentsMargins(10, 0, 10, 20);

	m_gridLayData->addWidget(m_labelName, 0, 0, 1, 1);
	m_gridLayData->addWidget(m_textName, 0, 1, 1, 4);
	m_gridLayData->addWidget(m_labelAuthor, 1, 0, 1, 1);
	m_gridLayData->addWidget(m_textAuthor, 1, 1, 1, 4);
	m_gridLayData->addWidget(m_labelGenre, 2, 0, 1, 1);
	m_gridLayData->addWidget(m_textGenre, 2, 1, 1, 4);
	m_gridLayData->addWidget(m_labelFolder, 3, 0, 1, 1);
	m_gridLayData->addWidget(m_textFolder, 3, 1, 1, 4);
	m_gridLayData->addWidget(m_labelPages, 4, 0, 1, 1);
	m_gridLayData->addWidget(m_textPages, 4, 1, 1, 4);
	m_gridLayData->addWidget(m_labelSize, 5, 0, 1, 1);
	m_gridLayData->addWidget(m_textSize, 5, 1, 1, 4);
	m_gridLayData->addWidget(m_labelTags, 6, 0, 1, 1);
	m_gridLayData->addWidget(m_textTags, 6, 1, 1, 4);
	m_gridLayData->addWidget(m_labelPath, 7, 0, 1, 1);
	m_gridLayData->addWidget(m_textPath, 7, 1, 1, 4);
	m_gridLayData->addWidget(m_textExt, 8, 1, 1, 4);
	m_gridLayData->addWidget(m_labelExt, 8, 0, 1, 1);
	m_gridLayData->addWidget(m_labelDateAdded, 9, 0, 1, 1);
	m_gridLayData->addWidget(m_textDateAdded, 9, 1, 1, 4);
	m_gridLayData->addWidget(m_labelModified, 10, 0, 1, 1);
	m_gridLayData->addWidget(m_labelPublisher, 11, 0, 1, 1);
	m_gridLayData->addWidget(m_textPublisher, 11, 1, 1, 4);
	m_gridLayData->addWidget(m_textDateModified, 10, 1, 1, 4);
	m_gridLayData->addWidget(m_labelDatePublished, 12, 0, 1, 1);
	m_gridLayData->addWidget(m_textDatePublished, 12, 1, 1, 4);
	m_gridLayData->addWidget(m_labelSeries, 13, 0, 1, 1);
	m_gridLayData->addWidget(m_textSeries, 13, 1, 1, 4);
	m_gridLayData->addWidget(m_labelRating, 14, 0, 1, 1);
	m_gridLayData->addWidget(m_comboBoxRating, 14, 1, 1, 1);
	m_gridLayData->addLayout(m_horLayStatus, 14, 2, 1, 1);

	m_vertLayComments = new QVBoxLayout(m_frameComments);
	m_vertLayComments->setSpacing(2);
	m_vertLayComments->setContentsMargins(10, 0, 10, 0);

	m_vertLayComments->addWidget(m_labelComments);
	m_vertLayComments->addWidget(m_textEditComments);

	m_gridWidget = new QWidget();
	m_gridWidget->setLayout(m_gridLayData);

	m_verticalSplitter->addWidget(m_gridWidget);
	m_verticalSplitter->addWidget(m_frameComments);
	m_splitter->addWidget(m_bookSearchWidget);
	m_splitter->addWidget(m_verticalSplitter);
	m_rightSplitter->addWidget(m_splitter);
	m_rightSplitter->addWidget(m_graphicsView);

	m_horLayButtons = new QHBoxLayout(m_frameButtons);
	m_horLayButtons->setContentsMargins(0, 10, 0, 0);
	m_horSpacerButtonsLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_horSpacerButtonsRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	m_horLayButtons->addItem(m_horSpacerButtonsLeft);
	m_horLayButtons->addWidget(m_buttonUpdate);
	m_horLayButtons->addWidget(m_vertLineButtons);
	m_horLayButtons->addWidget(m_buttonPrev);
	m_horLayButtons->addWidget(m_buttonNext);
	m_horLayButtons->addItem(m_horSpacerButtonsRight);

	m_vertLayMain = new QVBoxLayout(m_centralwidget);

	m_vertLayMain->addWidget(m_rightSplitter);
	m_vertLayMain->addWidget(m_frameButtons);

	// Central Widget
	setCentralWidget(m_centralwidget);
}

void BookMetadataWindow::setupConnections()
{
	connect(m_bookSearchWidget, &BookSearchWidget::itemClicked, this, &BookMetadataWindow::showBookDetails);
	connect(m_bookSearchWidget, &BookSearchWidget::selectionChanged, this, &BookMetadataWindow::showBookDetails);

	connect(m_buttonNext, &QPushButton::clicked, this, &BookMetadataWindow::nextBook);
	connect(m_buttonPrev, &QPushButton::clicked, this, &BookMetadataWindow::prevBook);
}

void BookMetadataWindow::setupTabOrder()
{
	QWidget::setTabOrder(m_bookSearchWidget, m_textName);
	QWidget::setTabOrder(m_textName, m_textAuthor);
	QWidget::setTabOrder(m_textAuthor, m_textGenre);
	QWidget::setTabOrder(m_textGenre, m_textFolder);
	QWidget::setTabOrder(m_textFolder, m_textPages);
	QWidget::setTabOrder(m_textPages, m_textSize);
	QWidget::setTabOrder(m_textSize, m_textTags);
	QWidget::setTabOrder(m_textTags, m_textPath);
	QWidget::setTabOrder(m_textPath, m_textExt);
	QWidget::setTabOrder(m_textExt, m_textDateAdded);
	QWidget::setTabOrder(m_textDateAdded, m_textDateModified);
	QWidget::setTabOrder(m_textDateModified, m_textPublisher);
	QWidget::setTabOrder(m_textPublisher, m_textDatePublished);
	QWidget::setTabOrder(m_textDatePublished, m_textSeries);
	QWidget::setTabOrder(m_textSeries, m_comboBoxRating);
	QWidget::setTabOrder(m_comboBoxRating, m_comboBoxStatus);
	QWidget::setTabOrder(m_comboBoxStatus, m_textEditComments);
	QWidget::setTabOrder(m_textEditComments, m_buttonUpdate);
	QWidget::setTabOrder(m_buttonUpdate, m_buttonPrev);
	QWidget::setTabOrder(m_buttonPrev, m_buttonNext);
}

void BookMetadataWindow::showBookDetails(const QString& name)
{
	queries::selectAllBasedOnName(name);
	while (queries::query.next())
	{
		m_textName->setText(queries::query.value("name").toString());
		m_textAuthor->setText(queries::query.value("author").toString());
		m_textGenre->setText(queries::query.value("genre").toString());
		m_textPages->setText(queries::query.value("pages").toString());
		m_textPath->setText(queries::query.value("path").toString());
		m_textSize->setText(queries::query.value("size").toString());
		m_textExt->setText(queries::query.value("ext").toString());
		m_textFolder->setText(queries::query.value("folder").toString());
		m_textDateAdded->setText(queries::query.value("time_added").toString());
		m_textDateModified->setText(queries::query.value("last_modified").toString());
		m_textPublisher->setText(queries::query.value("publisher").toString());
		m_textDatePublished->setText(queries::query.value("date_published").toString());
		m_textSeries->setText(queries::query.value("series").toString());
		m_comboBoxRating->setCurrentIndex(queries::query.value("rating").toInt());
		m_comboBoxStatus->setCurrentIndex(queries::query.value("status").toInt());

		m_textName->setCursorPosition(0);
		m_textPath->setCursorPosition(0);
		m_textFolder->setCursorPosition(0);

		setWindowTitle("MetaData - " + queries::query.value("name").toString());
	}
	QString tags = queries::selectTagsBasedOnName(name);
	m_textTags->setText(tags);
}

void BookMetadataWindow::nextBook()
{
	m_bookSearchWidget->setCurrentRow(m_bookSearchWidget->currentRow() + 1);
}

void BookMetadataWindow::prevBook()
{
	m_bookSearchWidget->setCurrentRow(m_bookSearchWidget->currentRow() - 1);
}
