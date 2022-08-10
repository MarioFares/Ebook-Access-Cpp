#ifndef STATUSCOMBOBOX_H
#define STATUSCOMBOBOX_H

#include <QWidget>
#include <QComboBox>

class StatusComboBox : public QComboBox
{
	Q_OBJECT

public:
	explicit StatusComboBox(QWidget* parent = nullptr);

};

#endif //STATUSCOMBOBOX_H
