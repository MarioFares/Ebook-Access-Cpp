#include "include/dialogs/inserttabledialog.h"

InsertTableDialog::InsertTableDialog(QWidget* parent) : QDialog(parent)
{
	m_columnCount = 1;
	m_rowCount = 1;

	setupInterface();
	setupConnections();
	setupTabOrder();
}

void InsertTableDialog::setupInterface()
{
	// Window
	setWindowTitle("Insert Table");
	setFixedSize(250, 115);

	// Widgets
	m_labelRows = new QLabel("Rows: ");
	m_labelColumns = new QLabel("Columns: ");

	m_spinBoxRows = new QSpinBox();
	m_spinBoxRows->setValue(1);
	m_spinBoxRows->setMinimum(1);

	m_spinBoxColumns = new QSpinBox();
	m_spinBoxColumns->setValue(1);
	m_spinBoxColumns->setMinimum(1);

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
	m_horLayButtons->addWidget(m_buttonOk);
	m_horLayButtons->addWidget(m_buttonCancel);

	m_gridLayMain = new QGridLayout;
	m_gridLayMain->addWidget(m_labelRows, 0, 0, 1, 1);
	m_gridLayMain->addWidget(m_labelColumns, 1, 0, 1, 1);
	m_gridLayMain->addWidget(m_spinBoxRows, 0, 1, 1, 1);
	m_gridLayMain->addWidget(m_spinBoxColumns, 1, 1, 1, 1);
	m_gridLayMain->addLayout(m_horLayButtons, 2, 1, 1, 1);

	setLayout(m_gridLayMain);
}

void InsertTableDialog::setupConnections()
{
	connect(m_buttonCancel, &QPushButton::clicked, this, &InsertTableDialog::close);
	connect(m_buttonOk, &QPushButton::clicked, this, &InsertTableDialog::getTableDims);
}

void InsertTableDialog::setupTabOrder()
{
	QWidget::setTabOrder(m_spinBoxRows, m_spinBoxColumns);
	QWidget::setTabOrder(m_spinBoxColumns, m_buttonOk);
	QWidget::setTabOrder(m_buttonOk, m_buttonCancel);
}

void InsertTableDialog::getTableDims()
{
	m_rowCount = m_spinBoxRows->value();
	m_columnCount = m_spinBoxColumns->value();
	close();
}
