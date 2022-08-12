#ifndef BULKDETAILSDIALOG_H
#define BULKDETAILSDIALOG_H

#include "include/widgets/ratingcombobox.h"
#include "include/widgets/statuscombobox.h"

#include <QDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>

class BulkDetailsDialog : public QDialog
{
Q_OBJECT

public:
	explicit BulkDetailsDialog(QWidget* parent = nullptr);

	QString m_author;
	QString m_genre;
	QString m_tags;
	QString m_publisher;
	QString m_datePublished;
	QString m_series;
	quint32 m_rating;
	quint32 m_status;

private slots:

	void setupInterface();

	void setupConnections();

	void setupTabOrder();

	void applyFields();

private:
	// Widgets
	QLabel* m_labelPrompt;
	QLabel* m_labelGenre;
	QCheckBox* m_checkBoxGenre;
	QLineEdit* m_textGenre;
	QLabel* m_labelTags;
	QCheckBox* m_checkBoxTags;
	QLineEdit* m_textTags;
	QLabel* m_labelAuthor;
	QCheckBox* m_checkBoxAuthor;
	QLineEdit* m_textAuthor;
	QLabel* m_labelPublisher;
	QCheckBox* m_checkBoxPublisher;
	QLineEdit* m_textPublisher;
	QLabel* m_labelDatePublished;
	QCheckBox* m_checkBoxDatePublished;
	QLineEdit* m_textDatePublished;
	QLabel* m_labelSeries;
	QCheckBox* m_checkBoxSeries;
	QLineEdit* m_textSeries;
	QLabel* m_labelStatus;
	QCheckBox* m_checkBoxStatus;
	StatusComboBox* m_comboBoxStatus;
	QLabel* m_labelRating;
	QCheckBox* m_checkBoxRating;
	RatingComboBox* m_comboBoxRating;
	QPushButton* m_buttonApply;
	QPushButton* m_buttonCancel;

	// Layouts
	QHBoxLayout* m_horLayButtons;
	QGridLayout* m_gridLay;
	QVBoxLayout* m_vertLayMain;

	// Spacers
	QSpacerItem* m_horSpacerButtonsLeft;
};

#endif // BULKDETAILSDIALOG_H
