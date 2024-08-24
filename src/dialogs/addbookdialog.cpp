#include "include/dialogs/addbookdialog.h"
#include "include/queries.h"
#include "include/common.h"

#include <QFileDialog>

AddBookDialog::AddBookDialog(QWidget *parent) : QDialog(parent) {
    setupInterface();
    setupConnections();
    setupTabOrder();
}

void AddBookDialog::setupInterface() {
    // Window
    resize(700, 520);
    setWindowTitle("Add Book");

    // Widgets
    _labelTitle = new QLabel("Add Book");
    _labelTitle->setAlignment(Qt::AlignCenter);

    _frame = new QFrame();
    _frame->setObjectName(QString::fromUtf8("frame"));
    _frame->setStyleSheet(QString::fromUtf8("QFrame#frame { border:  0px; }"));
    _frame->setFrameShape(QFrame::StyledPanel);
    _frame->setFrameShadow(QFrame::Raised);

    _textSize = new QLineEdit();
    _textSize->setEnabled(false);
    _textTags = new QLineEdit();
    _textExtension = new QLineEdit();
    _textExtension->setEnabled(false);
    _textExtension->setCursor(QCursor(Qt::PointingHandCursor));
    _textFolder = new QLineEdit();
    _textGenre = new QLineEdit();
    _textPath = new QLineEdit();
    _textName = new QLineEdit();
    _textAuthor = new QLineEdit();
    _textPages = new QLineEdit();
    _textPublisher = new QLineEdit();
    _textDatePublished = new QLineEdit();
    _textSeries = new QLineEdit();
    _comboBoxRating = new RatingComboBox();
    _comboBoxRating->setFixedWidth(170);
    _comboBoxStatus = new StatusComboBox();
    _comboBoxStatus->setFixedWidth(170);

    _buttonBrowse = new QPushButton("...");
    _buttonBrowse->setMinimumSize(40, 0);
    _buttonBrowse->setMaximumSize(40, 20);
    _buttonBrowse->setCursor(QCursor(Qt::PointingHandCursor));

    _labelAuthor = new QLabel("Author");
    _labelPages = new QLabel("Pages");
    _labelPath = new QLabel("Path");
    _labelSize = new QLabel("Size");
    _labelTags = new QLabel("Tags");
    _labelGenre = new QLabel("Genre");
    _labelFolder = new QLabel("Folder");
    _labelName = new QLabel("Name");
    _labelExtension = new QLabel("Ext");
    _labelPublisher = new QLabel("Publisher");
    _labelDatePublished = new QLabel("Published");
    _labelSeries = new QLabel("Series");
    _labelRating = new QLabel("Rating");
    _labelStatus = new QLabel("Status");

    _buttonAdd = new QPushButton("Add");
    _buttonAdd->setMinimumSize(80, 0);
    _buttonAdd->setCursor(QCursor(Qt::PointingHandCursor));

    _buttonClear = new QPushButton("Clear");
    _buttonClear->setMinimumSize(80, 0);
    _buttonClear->setCursor(QCursor(Qt::PointingHandCursor));

    _buttonCancel = new QPushButton("Cancel");
    _buttonCancel->setMinimumSize(80, 0);
    _buttonCancel->setCursor(QCursor(Qt::PointingHandCursor));

    // Layouts
    _horLayStatus = new QHBoxLayout();
    _horLayStatus->setContentsMargins(5, 0, 0, 0);
    _horLayStatus->setSpacing(10);
    _horLayStatus->addWidget(_labelStatus);
    _horLayStatus->addWidget(_comboBoxStatus);

    _gridLay = new QGridLayout(_frame);
    _gridLay->setHorizontalSpacing(4);
    _gridLay->setVerticalSpacing(10);
    _gridLay->addWidget(_labelName, 0, 0, 1, 1);
    _gridLay->addWidget(_textName, 0, 1, 1, 4);
    _gridLay->addWidget(_labelFolder, 1, 0, 1, 1);
    _gridLay->addWidget(_textFolder, 1, 1, 1, 4);
    _gridLay->addWidget(_textPath, 2, 1, 1, 4);
    _gridLay->addWidget(_buttonBrowse, 2, 5, 1, 1);
    _gridLay->addWidget(_labelPath, 2, 0, 1, 1);
    _gridLay->addWidget(_labelExtension, 3, 0, 1, 1);
    _gridLay->addWidget(_textExtension, 3, 1, 1, 4);
    _gridLay->addWidget(_labelAuthor, 4, 0, 1, 1);
    _gridLay->addWidget(_textAuthor, 4, 1, 1, 4);
    _gridLay->addWidget(_labelGenre, 5, 0, 1, 1);
    _gridLay->addWidget(_textGenre, 5, 1, 1, 4);
    _gridLay->addWidget(_labelSize, 6, 0, 1, 1);
    _gridLay->addWidget(_textSize, 6, 1, 1, 4);
    _gridLay->addWidget(_labelPages, 7, 0, 1, 1);
    _gridLay->addWidget(_textPages, 7, 1, 1, 4);
    _gridLay->addWidget(_labelTags, 8, 0, 1, 1);
    _gridLay->addWidget(_textTags, 8, 1, 1, 4);
    _gridLay->addWidget(_labelPublisher, 9, 0, 1, 1);
    _gridLay->addWidget(_textPublisher, 9, 1, 1, 4);
    _gridLay->addWidget(_labelDatePublished, 10, 0, 1, 1);
    _gridLay->addWidget(_textDatePublished, 10, 1, 1, 4);
    _gridLay->addWidget(_labelSeries, 11, 0, 1, 1);
    _gridLay->addWidget(_textSeries, 11, 1, 1, 4);
    _gridLay->addWidget(_labelRating, 12, 0, 1, 1);
    _gridLay->addWidget(_comboBoxRating, 12, 1, 1, 1);
    _gridLay->addLayout(_horLayStatus, 12, 2, 1, 1);

    _horLayButtons = new QHBoxLayout();
    _horLayButtons->setSpacing(6);
    _horLayButtons->setContentsMargins(0, 10, 0, 5);
    _horSpacerButtonsLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horSpacerButtonsRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    _horLayButtons->addItem(_horSpacerButtonsLeft);
    _horLayButtons->addWidget(_buttonAdd);
    _horLayButtons->addWidget(_buttonClear);
    _horLayButtons->addWidget(_buttonCancel);
    _horLayButtons->addItem(_horSpacerButtonsRight);

    _vertLayMain = new QVBoxLayout();
    _vertLayMain->addWidget(_labelTitle);
    _vertLayMain->addWidget(_frame);
    _vertLayMain->addLayout(_horLayButtons);

    setLayout(_vertLayMain);
    _textPath->setCompleter(common::dirCompleter(this));
}

void AddBookDialog::setupConnections() {
    connect(_buttonClear, &QPushButton::clicked, this, &AddBookDialog::clearInputs);
    connect(_buttonBrowse, &QPushButton::clicked, this, &AddBookDialog::browseFile);
    connect(_buttonCancel, &QPushButton::clicked, this, &AddBookDialog::close);
    connect(_buttonAdd, &QPushButton::clicked, this, &AddBookDialog::addFile);
}

void AddBookDialog::setupTabOrder() {
    QWidget::setTabOrder(_textName, _textFolder);
    QWidget::setTabOrder(_textFolder, _textPath);
    QWidget::setTabOrder(_textPath, _buttonBrowse);
    QWidget::setTabOrder(_buttonBrowse, _textExtension);
    QWidget::setTabOrder(_textExtension, _textAuthor);
    QWidget::setTabOrder(_textAuthor, _textGenre);
    QWidget::setTabOrder(_textGenre, _textSize);
    QWidget::setTabOrder(_textSize, _textPages);
    QWidget::setTabOrder(_textPages, _textTags);
    QWidget::setTabOrder(_textTags, _textPublisher);
    QWidget::setTabOrder(_textPublisher, _textDatePublished);
    QWidget::setTabOrder(_textDatePublished, _textSeries);
    QWidget::setTabOrder(_textSeries, _comboBoxRating);
    QWidget::setTabOrder(_comboBoxRating, _comboBoxStatus);
    QWidget::setTabOrder(_comboBoxStatus, _buttonAdd);
    QWidget::setTabOrder(_buttonAdd, _buttonClear);
    QWidget::setTabOrder(_buttonClear, _buttonCancel);
}

void AddBookDialog::clearInputs() {
    _textName->clear();
    _textFolder->clear();
    _textAuthor->clear();
    _textGenre->clear();
    _textTags->clear();
    _textSize->clear();
    _textExtension->clear();
    _textPath->clear();
    _textPages->clear();
    _textPublisher->clear();
    _textDatePublished->clear();
    _textSeries->clear();
    _comboBoxStatus->setCurrentIndex(0);
    _comboBoxRating->setCurrentIndex(0);
}

void AddBookDialog::browseFile() {
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), "/", tr("All Files (*.*)"));

    QFileInfo file(filePath);
    _textName->setText(file.completeBaseName());
    _textExtension->setText("." + file.suffix());
    _textFolder->setText(file.dir().dirName());
    _textPath->setText(file.absoluteFilePath());
    _textSize->setText(QString::number(file.size()));

    quint32 pages = common::getPageCount(filePath);
    _textPages->setText(QString::number(pages));
}

void AddBookDialog::addFile() {
    QString name = _textName->text();
    QString folder = _textFolder->text();
    QString genre = _textGenre->text();
    QString path = _textPath->text();
    QString author = _textAuthor->text();
    quint32 pages = _textPages->text().toUInt();
    quint64 size = _textSize->text().toULongLong();
    QString ext = _textExtension->text();
    QString tags = _textTags->text();
    QString publisher = _textPublisher->text();
    QString datePublished = _textDatePublished->text();
    QString series = _textSeries->text();
    quint32 rating = _comboBoxRating->currentIndex();
    quint32 status = _comboBoxStatus->currentIndex();

    QFileInfo file(path);
    if (file.exists() && file.isFile()) {
        queries::insertBooksQuery(name, path, folder, ext, size,
                                  pages, tags, genre, author, publisher, datePublished, series, rating, status);
        common::showMsgBox("Success!", "Ebook successfully added.",
                           ":/styles/style.qss", QMessageBox::Information,
                           ":/icons/books_icon.png");
    } else {
        common::showMsgBox("Path Error!", "File path not valid!", ":/styles/style.qss", QMessageBox::Warning,
                           ":/icons/books_icon.png");
    }
}
