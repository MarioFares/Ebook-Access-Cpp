#ifndef STATUSCOMBOBOX_H
#define STATUSCOMBOBOX_H

#include <QWidget>
#include <QComboBox>

class StatusComboBox : public QComboBox {
    Q_OBJECT

private:
    static const inline QStringList m_status = {
        "No Status",
        "Prospective",
        "Pending",
        "Complete",
    };

public:
    explicit StatusComboBox(QWidget *parent = nullptr);

    static QString convertToStatus(quint32 index);

    static QString convertToIndex(QString value);
};

#endif //STATUSCOMBOBOX_H
