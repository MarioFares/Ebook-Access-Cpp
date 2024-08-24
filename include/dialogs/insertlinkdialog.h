#ifndef INSERTLINKDIALOG_H
#define INSERTLINKDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QBoxLayout>
#include <QPushButton>

class InsertLinkDialog : public QDialog {
    Q_OBJECT

public:
    explicit InsertLinkDialog(QWidget *parent = nullptr);

    QString _title;
    QString _link;

    void setDataOnOpen();

private slots:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    void getInput();

private:
    // Widgets
    QLabel *_labelLink;
    QLabel *_labelTitle;
    QLineEdit *_textLink;
    QLineEdit *_textTitle;
    QPushButton *_buttonOk;
    QPushButton *_buttonCancel;

    // Layouts
    QHBoxLayout *_horLayButtons;
    QGridLayout *_gridLay;

    // Spacers
    QSpacerItem *_horSpacerButtonsLeft;
};

#endif // INSERTLINKDIALOG_H
