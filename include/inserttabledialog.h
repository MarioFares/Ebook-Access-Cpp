#ifndef INSERTTABLEDIALOG_H
#define INSERTTABLEDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QSpinBox>
#include <QBoxLayout>
#include <QPushButton>

class InsertTableDialog : public QDialog
{
Q_OBJECT

public:
	explicit InsertTableDialog(QWidget* parent = nullptr);

	int m_columnCount;
	int m_rowCount;

private slots:

	void setupInterface();

	void setupConnections();

	void setupTabOrder();

	void getTableDims();

private:
	// Widgets
	QLabel* m_labelColumns;
	QLabel* m_labelRows;
	QSpinBox* m_spinBoxRows;
	QSpinBox* m_spinBoxColumns;
	QPushButton* m_buttonOk;
	QPushButton* m_buttonCancel;

	// Layouts
	QHBoxLayout* m_horLayButtons;
	QGridLayout* m_gridLayMain;
};

#endif // INSERTTABLEDIALOG_H
