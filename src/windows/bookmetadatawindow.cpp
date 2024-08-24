#include "include/common.h"
#include "include/queries.h"
#include "include/widgets/statuscombobox.h"
#include "include/widgets/ratingcombobox.h"
#include "include/widgets/booksearchwidget.h"
#include "include/windows/bookmetadatawindow.h"

#include <QHeaderView>
#include <QDesktopServices>

BookMetadataWindow::BookMetadataWindow(QWidget *parent) : QMainWindow(parent) {
    queries::connectToDatabase();

    setupInterface();
    setupConnections();
    setupTabOrder();
}

void BookMetadataWindow::setupInterface() {
    // Window
    resize(1075, 706);
    setWindowTitle("Metadata");

    // Widgets
    _centralwidget = new QWidget(this);

    _rightSplitter = new QSplitter(_centralwidget);
    _rightSplitter->setOrientation(Qt::Horizontal);
    _rightSplitter->setHandleWidth(0);

    _splitter = new QSplitter(_rightSplitter);
    _splitter->setOrientation(Qt::Horizontal);
    _splitter->setHandleWidth(0);

    _bookSearchWidget = new BookSearchWidget(this);
    _bookSearchWidget->setMainLayoutMargin(0, 0, 0, 0);

    _verticalSplitter = new QSplitter(_splitter);
    _verticalSplitter->setObjectName(QString::fromUtf8("verticalSplitter"));
    _verticalSplitter->setStyleSheet(QString::fromUtf8("#verticalSplitter { border: none; } "));
    _verticalSplitter->setOrientation(Qt::Vertical);
    _verticalSplitter->setHandleWidth(0);

    _labelName = new QLabel("Name");
    _labelName->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _textGenre = new QLineEdit();
    _textGenre->setMinimumSize(QSize(0, 23));

    _labelDatePublished = new QLabel("Published");
    _labelDatePublished->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _textPublisher = new QLineEdit();
    _textPublisher->setMinimumSize(QSize(0, 23));

    _textDatePublished = new QLineEdit();
    _textDatePublished->setMinimumSize(QSize(0, 23));

    _labelPath = new QLabel("Path");
    _labelPath->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _textExt = new QLineEdit();
    _textExt->setMinimumSize(QSize(0, 23));
    _textExt->setEnabled(false);

    _labelDateAdded = new QLabel("Added");
    _labelDateAdded->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _textAuthor = new QLineEdit();
    _textAuthor->setMinimumSize(QSize(0, 23));

    _comboBoxRating = new RatingComboBox();
    _comboBoxRating->setMinimumSize(QSize(170, 23));
    _comboBoxRating->setMaximumSize(QSize(170, 23));

    _labelExt = new QLabel("Extension");
    _labelExt->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _labelFolder = new QLabel("Folder");
    _labelFolder->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _textFolder = new QLineEdit();
    _textFolder->setMinimumSize(QSize(0, 23));

    _labelPages = new QLabel("Pages");
    _labelPages->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _textSeries = new QLineEdit();
    _textSeries->setMinimumSize(QSize(0, 23));

    _labelSeries = new QLabel("Series");
    _labelSeries->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _textDateAdded = new QLineEdit();
    _textDateAdded->setMinimumSize(QSize(0, 23));
    _textDateAdded->setEnabled(false);

    _labelRating = new QLabel("Rating");
    _labelRating->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _textPages = new QLineEdit();
    _textPages->setMinimumSize(QSize(0, 23));

    _labelSize = new QLabel("Size");
    _labelSize->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _labelAuthor = new QLabel("Author");
    _labelAuthor->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _labelGenre = new QLabel("Genre");
    _labelGenre->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _labelPublisher = new QLabel("Publisher");
    _labelPublisher->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _textName = new QLineEdit();
    _textName->setMinimumSize(QSize(0, 23));

    _labelStatus = new QLabel("Status");
    _labelStatus->setMinimumSize(QSize(70, 0));
    _labelStatus->setMaximumSize(QSize(70, 180));
    _labelStatus->setAlignment(Qt::AlignCenter);

    _comboBoxStatus = new StatusComboBox();
    _comboBoxStatus->setMinimumSize(QSize(170, 23));
    _comboBoxStatus->setMaximumSize(QSize(170, 180));

    _textPath = new QLineEdit();
    _textPath->setMinimumSize(QSize(0, 23));

    _textSize = new QLineEdit();
    _textSize->setMinimumSize(QSize(0, 23));
    _textSize->setEnabled(false);

    _labelTags = new QLabel("Tags");
    _labelTags->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);

    _textTags = new QLineEdit();
    _textTags->setMinimumSize(QSize(0, 23));

    _labelModified = new QLabel("Modified");

    _textDateModified = new QLineEdit();
    _textDateModified->setEnabled(false);

    _frameComments = new QFrame();
    _frameComments->setObjectName(QString::fromUtf8("frameComments"));
    _frameComments->setStyleSheet(QString::fromUtf8("#frameComments{ border:none;}"));

    _labelComments = new QLabel("Notes/Comments");

    _textEditComments = new QPlainTextEdit();
    QPalette p = _textEditComments->palette();
    p.setColor(QPalette::Text, Qt::white);
    _textEditComments->setPalette(p);

    _frameButtons = new QFrame();
    _frameButtons->setObjectName(QString::fromUtf8("frameButtons"));
    _frameButtons->setStyleSheet(QString::fromUtf8("#frameButtons { border: none; }"));

    _buttonUpdate = new QPushButton("Update");
    _buttonUpdate->setMinimumSize(QSize(70, 30));
    _buttonUpdate->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonUpdate->setFlat(true);

    _buttonPrev = new QPushButton("Previous");
    _buttonPrev->setMinimumSize(QSize(70, 30));
    _buttonPrev->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonPrev->setFlat(true);

    _buttonNext = new QPushButton("Next");
    _buttonNext->setMinimumSize(QSize(70, 30));
    _buttonNext->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonNext->setFlat(true);

    _buttonClear = new QPushButton("Clear");
    _buttonClear->setMinimumSize(QSize(70, 30));
    _buttonClear->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonClear->setFlat(true);

    _buttonRestore = new QPushButton("Restore");
    _buttonRestore->setMinimumSize(QSize(70, 30));
    _buttonRestore->setCursor(QCursor(Qt::PointingHandCursor));
    _buttonRestore->setFlat(true);

    _vertLineButtons = new QFrame(_frameButtons);
    _vertLineButtons->setFrameShape(QFrame::VLine);
    _vertLineButtons->setFrameShadow(QFrame::Sunken);
    _vertLineButtons->setStyleSheet(QString("color: grey; background-color: grey;"));

    _frameBookmarks = new QFrame(_rightSplitter);
    _frameBookmarks->setContentsMargins(0, 0, 0, 0);
    _frameBookmarks->setObjectName(QString::fromUtf8("frameBookmarks"));
    _frameBookmarks->setStyleSheet(QString::fromUtf8("#frameBookmarks { border: none; }"));

    _tableWidgetBookmarks = new QTableWidget(_frameBookmarks);
    _tableWidgetBookmarks->setColumnCount(2);
    QStringList headers = {"Bookmark", "Notes/Comments"};
    _tableWidgetBookmarks->setHorizontalHeaderLabels(headers);
    _tableWidgetBookmarks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Layouts
    _horLayStatus = new QHBoxLayout();
    _horLayStatus->addWidget(_labelStatus);
    _horLayStatus->addWidget(_comboBoxStatus);

    _gridLayData = new QGridLayout();
    _gridLayData->setHorizontalSpacing(0);
    _gridLayData->setVerticalSpacing(10);
    _gridLayData->setContentsMargins(10, 0, 10, 20);

    _gridLayData->addWidget(_labelName, 0, 0, 1, 1);
    _gridLayData->addWidget(_textName, 0, 1, 1, 4);
    _gridLayData->addWidget(_labelAuthor, 1, 0, 1, 1);
    _gridLayData->addWidget(_textAuthor, 1, 1, 1, 4);
    _gridLayData->addWidget(_labelGenre, 2, 0, 1, 1);
    _gridLayData->addWidget(_textGenre, 2, 1, 1, 4);
    _gridLayData->addWidget(_labelFolder, 3, 0, 1, 1);
    _gridLayData->addWidget(_textFolder, 3, 1, 1, 4);
    _gridLayData->addWidget(_labelPages, 4, 0, 1, 1);
    _gridLayData->addWidget(_textPages, 4, 1, 1, 4);
    _gridLayData->addWidget(_labelSize, 5, 0, 1, 1);
    _gridLayData->addWidget(_textSize, 5, 1, 1, 4);
    _gridLayData->addWidget(_labelTags, 6, 0, 1, 1);
    _gridLayData->addWidget(_textTags, 6, 1, 1, 4);
    _gridLayData->addWidget(_labelPath, 7, 0, 1, 1);
    _gridLayData->addWidget(_textPath, 7, 1, 1, 4);
    _gridLayData->addWidget(_textExt, 8, 1, 1, 4);
    _gridLayData->addWidget(_labelExt, 8, 0, 1, 1);
    _gridLayData->addWidget(_labelDateAdded, 9, 0, 1, 1);
    _gridLayData->addWidget(_textDateAdded, 9, 1, 1, 4);
    _gridLayData->addWidget(_labelModified, 10, 0, 1, 1);
    _gridLayData->addWidget(_labelPublisher, 11, 0, 1, 1);
    _gridLayData->addWidget(_textPublisher, 11, 1, 1, 4);
    _gridLayData->addWidget(_textDateModified, 10, 1, 1, 4);
    _gridLayData->addWidget(_labelDatePublished, 12, 0, 1, 1);
    _gridLayData->addWidget(_textDatePublished, 12, 1, 1, 4);
    _gridLayData->addWidget(_labelSeries, 13, 0, 1, 1);
    _gridLayData->addWidget(_textSeries, 13, 1, 1, 4);
    _gridLayData->addWidget(_labelRating, 14, 0, 1, 1);
    _gridLayData->addWidget(_comboBoxRating, 14, 1, 1, 1);
    _gridLayData->addLayout(_horLayStatus, 14, 2, 1, 1);

    _vertLayComments = new QVBoxLayout(_frameComments);
    _vertLayComments->setSpacing(2);
    _vertLayComments->setContentsMargins(10, 0, 10, 0);

    _vertLayComments->addWidget(_labelComments);
    _vertLayComments->addWidget(_textEditComments);

    _gridWidget = new QWidget();
    _gridWidget->setLayout(_gridLayData);

    _vertLayBookmarks = new QVBoxLayout(_frameBookmarks);
    _vertLayBookmarks->setContentsMargins(0, 0, 0, 0);
    _vertLayBookmarks->addWidget(_tableWidgetBookmarks);

    _verticalSplitter->addWidget(_gridWidget);
    _verticalSplitter->addWidget(_frameComments);
    _splitter->addWidget(_bookSearchWidget);
    _splitter->addWidget(_verticalSplitter);
    _rightSplitter->addWidget(_splitter);
    _rightSplitter->addWidget(_frameBookmarks);

    _horLayButtons = new QHBoxLayout(_frameButtons);
    _horLayButtons->setContentsMargins(0, 10, 0, 0);
    _horSpacerButtonsLeft = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horSpacerButtonsRight = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    _horLayButtons->addItem(_horSpacerButtonsLeft);
    _horLayButtons->addWidget(_buttonRestore);
    _horLayButtons->addWidget(_buttonUpdate);
    _horLayButtons->addWidget(_buttonClear);
    _horLayButtons->addWidget(_vertLineButtons);
    _horLayButtons->addWidget(_buttonPrev);
    _horLayButtons->addWidget(_buttonNext);
    _horLayButtons->addItem(_horSpacerButtonsRight);

    _vertLayMain = new QVBoxLayout(_centralwidget);

    _vertLayMain->addWidget(_rightSplitter);
    _vertLayMain->addWidget(_frameButtons);

    // Central Widget
    setCentralWidget(_centralwidget);
}

void BookMetadataWindow::setupConnections() {
    connect(_bookSearchWidget, &BookSearchWidget::itemClicked, this, &BookMetadataWindow::showBookDetails);
    connect(_bookSearchWidget, &BookSearchWidget::selectionChanged, this, &BookMetadataWindow::showBookDetails);

    connect(_buttonUpdate, &QPushButton::clicked, this, &BookMetadataWindow::updateDetails);
    connect(_buttonRestore, &QPushButton::clicked, this, &BookMetadataWindow::restoreDetails);
    connect(_buttonClear, &QPushButton::clicked, this, &BookMetadataWindow::clearDetails);
    connect(_buttonNext, &QPushButton::clicked, this, &BookMetadataWindow::nextBook);
    connect(_buttonPrev, &QPushButton::clicked, this, &BookMetadataWindow::prevBook);
}

void BookMetadataWindow::setupTabOrder() {
    QWidget::setTabOrder(_bookSearchWidget, _textName);
    QWidget::setTabOrder(_textName, _textAuthor);
    QWidget::setTabOrder(_textAuthor, _textGenre);
    QWidget::setTabOrder(_textGenre, _textFolder);
    QWidget::setTabOrder(_textFolder, _textPages);
    QWidget::setTabOrder(_textPages, _textSize);
    QWidget::setTabOrder(_textSize, _textTags);
    QWidget::setTabOrder(_textTags, _textPath);
    QWidget::setTabOrder(_textPath, _textExt);
    QWidget::setTabOrder(_textExt, _textDateAdded);
    QWidget::setTabOrder(_textDateAdded, _textDateModified);
    QWidget::setTabOrder(_textDateModified, _textPublisher);
    QWidget::setTabOrder(_textPublisher, _textDatePublished);
    QWidget::setTabOrder(_textDatePublished, _textSeries);
    QWidget::setTabOrder(_textSeries, _comboBoxRating);
    QWidget::setTabOrder(_comboBoxRating, _comboBoxStatus);
    QWidget::setTabOrder(_comboBoxStatus, _textEditComments);
    QWidget::setTabOrder(_textEditComments, _buttonUpdate);
    QWidget::setTabOrder(_buttonUpdate, _buttonPrev);
    QWidget::setTabOrder(_buttonPrev, _buttonNext);
}

void BookMetadataWindow::showBookDetails(const QString &name) {
    if (name.isEmpty()) {
        return;
    }
    queries::selectAllBasedOnName(name);
    while (queries::query.next()) {
        QString path = queries::query.value("path").toString();
        QString author = queries::query.value("author").toString();
        author = (author == "N/A" ? "" : author);
        QString genre = queries::query.value("genre").toString();
        genre = (genre == "N/A" ? "" : genre);
        QString ext = queries::query.value("ext").toString();
        QString pages = queries::query.value("pages").toString();
        double size = (queries::query.value("size").toDouble());
        QString folder = queries::query.value("folder").toString();
        QString publisher = queries::query.value("publisher").toString();
        publisher = (publisher == "N/A" ? "" : publisher);
        QString datePublished = queries::query.value("date_published").toString();
        datePublished = (datePublished == "N/A" ? "" : datePublished);
        QString dateAdded = queries::query.value("time_added").toString();
        QString dateModified = queries::query.value("last_modified").toString();
        QString series = queries::query.value("series").toString();
        series = (series == "N/A" ? "" : series);
        QString comments = queries::query.value("comments").toString();
        comments = (comments == "N/A" ? "" : comments);
        quint32 rating = queries::query.value("rating").toInt();
        quint32 status = queries::query.value("status").toInt();

        _textName->setText(name);
        _textAuthor->setText(author);
        _textGenre->setText(genre);
        _textPages->setText(pages);
        _textPath->setText(path);
        _textSize->setText(QString::number(size));
        _textExt->setText(ext);
        _textFolder->setText(folder);
        _textDateAdded->setText(dateAdded);
        _textDateModified->setText(dateModified);
        _textPublisher->setText(publisher);
        _textDatePublished->setText(datePublished);
        _textSeries->setText(series);
        _textEditComments->setPlainText(comments);
        _comboBoxRating->setCurrentIndex(rating);
        _comboBoxStatus->setCurrentIndex(status);

        _textName->setCursorPosition(0);
        _textPath->setCursorPosition(0);
        _textFolder->setCursorPosition(0);

        setWindowTitle("MetaData - " + queries::query.value("name").toString());
    }
    QString tags = queries::selectTagsBasedOnName(name);
    _textTags->setText(tags);
}

void BookMetadataWindow::nextBook() {
    _bookSearchWidget->setCurrentRow(_bookSearchWidget->currentRow() + 1);
}

void BookMetadataWindow::prevBook() {
    _bookSearchWidget->setCurrentRow(_bookSearchWidget->currentRow() - 1);
}

void BookMetadataWindow::clearDetails() {
    _textName->clear();
    _textAuthor->clear();
    _textGenre->clear();
    _textFolder->clear();
    _textPages->clear();
    _textSize->clear();
    _textTags->clear();
    _textPath->clear();
    _textExt->clear();
    _textDateAdded->clear();
    _textDateModified->clear();
    _textPublisher->clear();
    _textDatePublished->clear();
    _textSeries->clear();
    _comboBoxRating->setCurrentIndex(0);
    _comboBoxStatus->setCurrentIndex(0);
    _textEditComments->clear();
}

void BookMetadataWindow::restoreDetails() {
    showBookDetails(_bookSearchWidget->currentItemText());
}

void BookMetadataWindow::updateDetails() {
    QString currentText = _bookSearchWidget->currentItemText();
    if (currentText.isEmpty()) {
        return;
    }
    QString newName = _textName->text();
    QString author = _textAuthor->text();
    QString folder = _textFolder->text();
    QString genre = _textGenre->text();
    quint32 pages = _textPages->text().toUInt();
    QString publisher = _textPublisher->text();
    QString datePublished = _textDatePublished->text();
    QString series = _textSeries->text();
    quint32 rating = _comboBoxRating->currentIndex();
    quint32 status = _comboBoxStatus->currentIndex();
    QString tags = _textTags->text().trimmed();
    QString comments = _textEditComments->toPlainText();

    queries::selectPathBasedonName(currentText);
    queries::query.next();
    QString path = queries::query.value(0).toString();
    if (newName != currentText) {
        common::renameFile(this, path, newName);
    }
    queries::updateBookQuery(currentText, newName, folder, genre, author, pages, tags, path, publisher,
                             datePublished, series, rating, status, comments);

    _bookSearchWidget->setCurrentItemText(newName);
    showBookDetails(newName);
}
