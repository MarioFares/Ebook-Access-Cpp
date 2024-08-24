#include "include/dialogs/extselectiondialog.h"

#include <QVector>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidgetItem>

ExtSelectionDialog::ExtSelectionDialog(QWidget *parent, QVector<QString> extVector, QString title, QString label)
    : QDialog(parent) {
    m_inputVector = extVector;
    m_title = title;
    m_label = label;

    setupInterface();
    setupConnections();
    setupTabOrder();
}

void ExtSelectionDialog::setupInterface() {
    // Window
    setWindowTitle(m_title);
    setFixedSize(400, 300);

    // Widgets
    m_labelTitle = new QLabel(m_label);

    m_textSearch = new QLineEdit();
    m_textSearch->setClearButtonEnabled(true);

    m_listWidget = new QListWidget();
    m_listWidget->setSortingEnabled(true);
    fillListWidget(m_inputVector);

    m_buttonSelectAll = new QPushButton("Select All");
    m_buttonSelectAll->setCursor(Qt::PointingHandCursor);
    m_buttonSelectAll->setFlat(true);
    m_buttonSelectAll->setMinimumSize(85, 25);

    m_buttonDeselectAll = new QPushButton("Deselect All");
    m_buttonDeselectAll->setCursor(Qt::PointingHandCursor);
    m_buttonDeselectAll->setFlat(true);
    m_buttonDeselectAll->setMinimumSize(85, 25);

    m_buttonContinue = new QPushButton("Continue");
    m_buttonContinue->setCursor(Qt::PointingHandCursor);
    m_buttonContinue->setFlat(true);
    m_buttonContinue->setMinimumSize(85, 25);

    // Layouts
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
    m_horLayButtons->addWidget(m_buttonDeselectAll);
    m_horLayButtons->addWidget(m_buttonSelectAll);
    m_horLayButtons->addWidget(m_buttonContinue);
    m_horLayButtons->addSpacerItem(m_horSpacerButtonsRight);

    m_vertLayMain = new QVBoxLayout();
    m_vertLayMain->addLayout(m_horLayTitle);
    m_vertLayMain->addWidget(m_textSearch);
    m_vertLayMain->addWidget(m_listWidget);
    m_vertLayMain->addLayout(m_horLayButtons);
    m_vertLayMain->setSpacing(10);

    setLayout(m_vertLayMain);
}

void ExtSelectionDialog::setupConnections() {
    connect(m_listWidget, &QListWidget::itemActivated, this, &ExtSelectionDialog::setItemState);
    connect(m_textSearch, &QLineEdit::textChanged, this, &ExtSelectionDialog::searchText);

    connect(m_buttonDeselectAll, &QPushButton::clicked, [this] {
        setItemsCheckState(Qt::Unchecked);
    });
    connect(m_buttonSelectAll, &QPushButton::clicked, [this] {
        setItemsCheckState(Qt::Checked);
    });
    connect(m_buttonContinue, &QPushButton::clicked, [this] {
        setExtVector();
        close();
    });
}

void ExtSelectionDialog::setupTabOrder() {
    QWidget::setTabOrder(m_textSearch, m_listWidget);
    QWidget::setTabOrder(m_listWidget, m_buttonDeselectAll);
    QWidget::setTabOrder(m_buttonDeselectAll, m_buttonSelectAll);
    QWidget::setTabOrder(m_buttonSelectAll, m_buttonContinue);
}

void ExtSelectionDialog::fillListWidget(QVector<QString> extVector) {
    m_listWidget->clear();
    for (QString &entry: extVector) {
        if (entry.isEmpty()) {
            continue;
        }
        QListWidgetItem *item = new QListWidgetItem(m_listWidget);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setCheckState(Qt::Checked);
        item->setText(entry);
        m_listWidget->addItem(item);
    }
}

QVector<QString> ExtSelectionDialog::getExtVector() {
    return m_outputVector;
}

void ExtSelectionDialog::setExtVector() {
    for (int i = 0; i < m_listWidget->count(); i++) {
        QListWidgetItem *item = m_listWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            m_outputVector.push_back(item->text());
        }
    }
}

void ExtSelectionDialog::setItemState(QListWidgetItem *item) {
    item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
}

void ExtSelectionDialog::searchText(const QString &arg1) {
    QList<QListWidgetItem *> items = m_listWidget->findItems(arg1, Qt::MatchContains);
    m_listWidget->setCurrentItem(items.count() > 0 ? items[0] : m_listWidget->currentItem());
}

void ExtSelectionDialog::setItemsCheckState(Qt::CheckState state) {
    for (int i = 0; i < m_listWidget->count(); i++) {
        m_listWidget->item(i)->setCheckState(state);
    }
}
