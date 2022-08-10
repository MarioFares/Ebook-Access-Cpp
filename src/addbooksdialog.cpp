#include "include/common.h"
#include "include/queries.h"
#include "include/addbooksdialog.h"
#include "include/bulkdetailsdialog.h"
#include "include/extselectiondialog.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QPushButton>
#include <QDirIterator>
#include <QProgressBar>

AddBooksDialog::AddBooksDialog(QWidget* parent) : QDialog(parent)
{
	setupInterface();
	setupConnections();
	setupTabOrder();
}

void AddBooksDialog::setupInterface()
{
	// Window
	setWindowTitle("Add Books");
	setFixedSize(405, 300);

	// Widgets
	m_buttonAdd = new QPushButton("Add");
	m_buttonAdd->setCursor(Qt::PointingHandCursor);
	m_buttonAdd->setFlat(true);
	m_buttonAdd->setMinimumSize(80, 25);

	m_buttonClose = new QPushButton("Close");
	m_buttonClose->setCursor(Qt::PointingHandCursor);
	m_buttonClose->setFlat(true);
	m_buttonClose->setMinimumSize(80, 25);

	m_buttonBrowseFolders = new QPushButton("...");
	m_buttonBrowseFolders->setCursor(Qt::PointingHandCursor);
	m_buttonBrowseFolders->setFlat(true);
	m_buttonBrowseFolders->setMinimumSize(50, 20);

	m_checkBoxRecursive = new QCheckBox("Recursive");
	m_checkBoxRecursive->setCheckable(true);
	m_checkBoxRecursive->setChecked(false);

	m_textFolderPath = new QLineEdit();
	m_textFolderPath->setCompleter(common::dirCompleter(this));
	m_textFolderPath->setClearButtonEnabled(true);

	m_labelAddBooks = new QLabel("Add Books");
	m_labelFolder = new QLabel("Folder: ");

	m_progressBar = new QProgressBar();
	m_progressBar->setValue(0);
	m_progressBar->setMinimumSize(381, 23);

	// Layouts
	m_horLayTitle = new QHBoxLayout();
	m_horSpacerTitleLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horSpacerTitleRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayTitle->addSpacerItem(m_horSpacerTitleLeft);
	m_horLayTitle->addWidget(m_labelAddBooks);
	m_horLayTitle->addSpacerItem(m_horSpacerTitleRight);

	m_horLayFolder = new QHBoxLayout();
	m_horLayFolder->addWidget(m_labelFolder);
	m_horLayFolder->addWidget(m_textFolderPath);
	m_horLayFolder->addWidget(m_buttonBrowseFolders);

	m_horLayCheckBox = new QHBoxLayout();
	m_horSpacerCheckBoxRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayCheckBox->addWidget(m_checkBoxRecursive);
	m_horLayCheckBox->addSpacerItem(m_horSpacerCheckBoxRight);

	m_horLayProgressBar = new QHBoxLayout();
	m_horSpacerProgressBarLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horSpacerProgressBarRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayProgressBar->addSpacerItem(m_horSpacerProgressBarLeft);
	m_horLayProgressBar->addWidget(m_progressBar);
	m_horLayProgressBar->addSpacerItem(m_horSpacerProgressBarRight);

	m_horLayButtons = new QHBoxLayout();
	m_horSpacerButtonsLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horSpacerButtonsRight = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_horLayButtons->addSpacerItem(m_horSpacerButtonsLeft);
	m_horLayButtons->addWidget(m_buttonAdd);
	m_horLayButtons->addWidget(m_buttonClose);
	m_horLayButtons->addSpacerItem(m_horSpacerButtonsRight);

	m_vertLayMain = new QVBoxLayout();
	m_vertLayMain->addLayout(m_horLayTitle);
	m_vertLayMain->addLayout(m_horLayFolder);
	m_vertLayMain->addLayout(m_horLayCheckBox);
	m_vertLayMain->addLayout(m_horLayProgressBar);
	m_vertLayMain->addLayout(m_horLayButtons);

	setLayout(m_vertLayMain);
}

void AddBooksDialog::setupConnections()
{
	connect(m_buttonClose, &QPushButton::clicked, this, &AddBooksDialog::close);
	connect(m_buttonBrowseFolders, &QPushButton::clicked, this, &AddBooksDialog::browseDirs);
	connect(m_buttonAdd, &QPushButton::clicked, this, &AddBooksDialog::addDir);
}

void AddBooksDialog::setupTabOrder()
{
	QWidget::setTabOrder(m_textFolderPath, m_buttonBrowseFolders);
	QWidget::setTabOrder(m_buttonBrowseFolders, m_checkBoxRecursive);
	QWidget::setTabOrder(m_checkBoxRecursive, m_buttonAdd);
	QWidget::setTabOrder(m_buttonAdd, m_buttonClose);
}

void AddBooksDialog::browseDirs()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Folder"),
			"/",
			QFileDialog::ShowDirsOnly |
					QFileDialog::DontResolveSymlinks);
	m_textFolderPath->setText(dir);
}

void AddBooksDialog::addDir()
{
	QString dirPath = m_textFolderPath->text();
	QFileInfo dir(dirPath);
	if (dir.exists() && dir.isDir())
	{
		setupEntries(dirPath, m_checkBoxRecursive->isChecked());
	}
	else
	{
		common::showMsgBox("Path Error!", "Directory path not valid!", ":/styles/style.qss", QMessageBox::Warning,
				":/icons/books_icon.png");
	}
}

void AddBooksDialog::setupEntries(const QString& dir, bool recursive)
{
	// Iterate over directory and get fileInfo and extensions
	QVector<QString> extVector;
	QVector<QFileInfo> entriesVector;
	QDirIterator iterator(dir, QDir::Files, recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags);
	while (iterator.hasNext())
	{
		QFileInfo file(iterator.next());
		entriesVector.push_back(file);

		QString ext = "." + file.suffix().toLower();
		if (!extVector.contains(ext))
		{
			extVector.push_back(ext);
		}
	}

	// Input for common tags, genres, or authors
	BulkDetailsDialog dialog;
	common::openDialog(&dialog, ":/styles/style.qss");

	QString tags = dialog.m_tags;
	QString genres = dialog.m_genre.isEmpty() ? "N/A" : dialog.m_genre;
	QString authors = dialog.m_author.isEmpty() ? "N/A" : dialog.m_author;
	QString publisher = dialog.m_publisher.isEmpty() ? "N/A" : dialog.m_publisher;
	QString datePublished = dialog.m_datePublished.isEmpty() ? "N/A" : dialog.m_datePublished;
	QString series = dialog.m_series.isEmpty() ? "N/A" : dialog.m_series;
	quint32 rating = dialog.m_rating;
	quint32 status = dialog.m_status;

	// Let the user select desired extensions
	ExtSelectionDialog* extDialog = new ExtSelectionDialog(this, extVector, "Extensions", "Select Extensions");
	common::openDialog(extDialog, ":/styles/style.qss");

	QVector<QString> selectedExts = extDialog->getExtVector();
	iterateInsertEntries(entriesVector, selectedExts, tags, genres, authors, publisher, datePublished, series, rating,
			status);
}

void AddBooksDialog::iterateInsertEntries(const QVector<QFileInfo>& entriesVector, const QVector<QString>& selectedExts,
										  const QString& tags, const QString& genres, const QString& authors,
										  const QString& publisher, const QString& datePublished, const QString& series,
										  quint32 rating, quint32 status)
{
	queries::db.transaction();
	size_t count = entriesVector.size();
	double counter = 0;
	for (const QFileInfo& entry: entriesVector)
	{
		QString ext = "." + entry.suffix().toLower();
		quint32 progress = (counter / count) * 100;
		m_progressBar->setValue(progress);
		counter++;
		if (selectedExts.contains(ext))
		{
			insertBook(entry, tags, genres, authors, publisher, datePublished, series, rating, status);
		}
	}

	queries::db.commit();
	m_progressBar->setValue(100);
}

void AddBooksDialog::insertBook(const QFileInfo& entry, const QString& tags, const QString& genre,
								const QString& author, const QString& publisher, const QString& datePublished,
								const QString& series, quint32 rating, quint32 status)
{
	QString name = entry.completeBaseName();
	QString path = entry.absoluteFilePath();
	QString ext = "." + entry.suffix();
	QString folder = entry.dir().dirName();
	quint64 size = entry.size();
	quint32 pages = common::getPageCount(path);
	queries::insertBooksQuery(name, path, folder, ext, size, pages, tags, genre, author, publisher, datePublished,
			series, rating, status);
}
