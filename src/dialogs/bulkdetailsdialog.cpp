#include "include/dialogs/bulkdetailsdialog.h"

#include <QPushButton>

BulkDetailsDialog::BulkDetailsDialog(QWidget *parent) : QDialog(parent) {
    _author = "";
    _genre = "";
    _tags = "";
    _publisher = "";
    _datePublished = "";
    _series = "";
    _rating = 0;
    _status = 0;
    setupInterface();
    setupConnections();
    setupTabOrder();
}

void BulkDetailsDialog::setupInterface() {
    // Window
    setWindowTitle("Bulk Details");
    setFixedSize(494, 320);

    // Widgets
    _labelPrompt = new QLabel("If you wish all ebooks in the selected folder to"
        " share the below data, check the boxes and input"
        " your specification:");
    _labelPrompt->setWordWrap(true);

    _checkBoxGenre = new QCheckBox("");
    _checkBoxGenre->setChecked(false);

    _checkBoxAuthor = new QCheckBox("");
    _checkBoxAuthor->setChecked(false);

    _checkBoxTags = new QCheckBox("");
    _checkBoxTags->setChecked(false);

    _checkBoxPublisher = new QCheckBox("");
    _checkBoxPublisher->setChecked(false);

    _checkBoxDatePublished = new QCheckBox("");
    _checkBoxDatePublished->setChecked(false);

    _checkBoxSeries = new QCheckBox("");
    _checkBoxSeries->setChecked(false);

    _checkBoxRating = new QCheckBox("");
    _checkBoxRating->setChecked(false);

    _checkBoxStatus = new QCheckBox("");
    _checkBoxStatus->setChecked(false);

    _labelGenre = new QLabel("Genre: ");
    _labelAuthor = new QLabel("Author: ");
    _labelTags = new QLabel("Tags: ");
    _labelPublisher = new QLabel("Publisher: ");
    _labelDatePublished = new QLabel("Published: ");
    _labelSeries = new QLabel("Series: ");
    _labelStatus = new QLabel("Status: ");
    _labelRating = new QLabel("Rating: ");

    _textGenre = new QLineEdit();
    _textGenre->setEnabled(false);
    _textGenre->setClearButtonEnabled(true);

    _textAuthor = new QLineEdit();
    _textAuthor->setEnabled(false);
    _textAuthor->setClearButtonEnabled(true);

    _textTags = new QLineEdit();
    _textTags->setEnabled(false);
    _textTags->setClearButtonEnabled(true);

    _textPublisher = new QLineEdit();
    _textPublisher->setEnabled(false);
    _textPublisher->setClearButtonEnabled(true);

    _textDatePublished = new QLineEdit();
    _textDatePublished->setEnabled(false);
    _textDatePublished->setClearButtonEnabled(true);

    _textSeries = new QLineEdit();
    _textSeries->setEnabled(false);
    _textSeries->setClearButtonEnabled(true);

    _comboBoxStatus = new StatusComboBox();
    _comboBoxStatus->setEnabled(false);

    _comboBoxRating = new RatingComboBox();
    _comboBoxRating->setEnabled(false);

    _buttonApply = new QPushButton("Apply");
    _buttonApply->setCursor(Qt::PointingHandCursor);
    _buttonApply->setFlat(true);
    _buttonApply->setMinimumSize(80, 25);

    _buttonCancel = new QPushButton("Cancel");
    _buttonCancel->setCursor(Qt::PointingHandCursor);
    _buttonCancel->setFlat(true);
    _buttonCancel->setMinimumSize(80, 25);

    // Layouts
    _horLayButtons = new QHBoxLayout();
    _horSpacerButtonsLeft = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    _horLayButtons->addSpacerItem(_horSpacerButtonsLeft);
    _horLayButtons->addWidget(_buttonApply);
    _horLayButtons->addWidget(_buttonCancel);

    _gridLay = new QGridLayout();
    _gridLay->addWidget(_checkBoxAuthor, 0, 0, 1, 1);
    _gridLay->addWidget(_labelAuthor, 0, 1, 1, 1);
    _gridLay->addWidget(_textAuthor, 0, 2, 1, 1);
    _gridLay->addWidget(_checkBoxGenre, 1, 0, 1, 1);
    _gridLay->addWidget(_labelGenre, 1, 1, 1, 1);
    _gridLay->addWidget(_textGenre, 1, 2, 1, 1);
    _gridLay->addWidget(_checkBoxTags, 2, 0, 1, 1);
    _gridLay->addWidget(_labelTags, 2, 1, 1, 1);
    _gridLay->addWidget(_textTags, 2, 2, 1, 1);
    _gridLay->addWidget(_checkBoxPublisher, 3, 0, 1, 1);
    _gridLay->addWidget(_labelPublisher, 3, 1, 1, 1);
    _gridLay->addWidget(_textPublisher, 3, 2, 1, 1);
    _gridLay->addWidget(_checkBoxDatePublished, 4, 0, 1, 1);
    _gridLay->addWidget(_labelDatePublished, 4, 1, 1, 1);
    _gridLay->addWidget(_textDatePublished, 4, 2, 1, 1);
    _gridLay->addWidget(_checkBoxSeries, 5, 0, 1, 1);
    _gridLay->addWidget(_labelSeries, 5, 1, 1, 1);
    _gridLay->addWidget(_textSeries, 5, 2, 1, 1);
    _gridLay->addWidget(_checkBoxRating, 6, 0, 1, 1);
    _gridLay->addWidget(_labelRating, 6, 1, 1, 1);
    _gridLay->addWidget(_comboBoxRating, 6, 2, 1, 1);
    _gridLay->addWidget(_checkBoxStatus, 7, 0, 1, 1);
    _gridLay->addWidget(_labelStatus, 7, 1, 1, 1);
    _gridLay->addWidget(_comboBoxStatus, 7, 2, 1, 1);

    _vertLayMain = new QVBoxLayout();
    _vertLayMain->addWidget(_labelPrompt);
    _vertLayMain->addLayout(_gridLay);
    _vertLayMain->addLayout(_horLayButtons);

    setLayout(_vertLayMain);
}

void BulkDetailsDialog::setupConnections() {
    connect(_buttonCancel, &QPushButton::clicked, this, &BulkDetailsDialog::close);
    connect(_buttonApply, &QPushButton::clicked, this, &BulkDetailsDialog::applyFields);
    connect(_checkBoxAuthor, &QCheckBox::stateChanged, _textAuthor, &QLineEdit::setEnabled);
    connect(_checkBoxGenre, &QCheckBox::stateChanged, _textGenre, &QLineEdit::setEnabled);
    connect(_checkBoxTags, &QCheckBox::stateChanged, _textTags, &QLineEdit::setEnabled);
    connect(_checkBoxPublisher, &QCheckBox::stateChanged, _textPublisher, &QLineEdit::setEnabled);
    connect(_checkBoxDatePublished, &QCheckBox::stateChanged, _textDatePublished, &QLineEdit::setEnabled);
    connect(_checkBoxSeries, &QCheckBox::stateChanged, _textSeries, &QLineEdit::setEnabled);
    connect(_checkBoxRating, &QCheckBox::stateChanged, _comboBoxRating, &QLineEdit::setEnabled);
    connect(_checkBoxStatus, &QCheckBox::stateChanged, _comboBoxStatus, &QLineEdit::setEnabled);
}

void BulkDetailsDialog::setupTabOrder() {
    QWidget::setTabOrder(_checkBoxAuthor, _textAuthor);
    QWidget::setTabOrder(_textAuthor, _checkBoxGenre);
    QWidget::setTabOrder(_checkBoxGenre, _textGenre);
    QWidget::setTabOrder(_textGenre, _checkBoxTags);
    QWidget::setTabOrder(_checkBoxTags, _textTags);
    QWidget::setTabOrder(_textTags, _checkBoxPublisher);
    QWidget::setTabOrder(_checkBoxPublisher, _textPublisher);
    QWidget::setTabOrder(_textPublisher, _checkBoxDatePublished);
    QWidget::setTabOrder(_checkBoxDatePublished, _textDatePublished);
    QWidget::setTabOrder(_textDatePublished, _checkBoxSeries);
    QWidget::setTabOrder(_checkBoxSeries, _textSeries);
    QWidget::setTabOrder(_textSeries, _checkBoxRating);
    QWidget::setTabOrder(_checkBoxRating, _comboBoxRating);
    QWidget::setTabOrder(_comboBoxRating, _checkBoxStatus);
    QWidget::setTabOrder(_checkBoxStatus, _comboBoxStatus);
    QWidget::setTabOrder(_comboBoxStatus, _buttonApply);
    QWidget::setTabOrder(_buttonApply, _buttonCancel);
}

void BulkDetailsDialog::applyFields() {
    if (_textAuthor->isEnabled()) {
        _author = _textAuthor->text();
    }

    if (_textGenre->isEnabled()) {
        _genre = _textGenre->text();
    }

    if (_textTags->isEnabled()) {
        _tags = _textTags->text();
    }

    if (_textPublisher->isEnabled()) {
        _publisher = _textPublisher->text();
    }

    if (_textDatePublished->isEnabled()) {
        _datePublished = _textDatePublished->text();
    }

    if (_textSeries->isEnabled()) {
        _series = _textSeries->text();
    }

    if (_comboBoxRating->isEnabled()) {
        _rating = _comboBoxRating->currentIndex();
    }

    if (_comboBoxStatus->isEnabled()) {
        _status = _comboBoxStatus->currentIndex();
    }

    close();
}
