#ifndef EXTSELECTIONDIALOG_H
#define EXTSELECTIONDIALOG_H

#include <QDialog>
#include <QVector>
#include <QListWidgetItem>
#include <QBoxLayout>
#include <QLabel>

class ExtSelectionDialog : public QDialog {
    Q_OBJECT

public:
    explicit ExtSelectionDialog(QWidget *parent = nullptr, QVector<QString> extVector = {},
                                QString title = "Preferences", QString label = "Select Preferences");

    QVector<QString> getExtVector();

private slots:
    void setupInterface();

    void setupConnections();

    void setupTabOrder();

    void fillListWidget(QVector<QString> extVector);

    static void setItemState(QListWidgetItem *item);

    void searchText(const QString &arg1);

    void setItemsCheckState(Qt::CheckState state);

private:
    QVector<QString> _outputVector;
    QVector<QString> _inputVector;

    void setExtVector();

    QString _title;
    QString _label;

    // Widgets
    QLabel *_labelTitle;
    QLineEdit *_textSearch;
    QListWidget *_listWidget;
    QPushButton *_buttonSelectAll;
    QPushButton *_buttonDeselectAll;
    QPushButton *_buttonContinue;

    // Layouts
    QHBoxLayout *_horLayTitle;
    QHBoxLayout *_horLayButtons;
    QVBoxLayout *_vertLayMain;

    // Spacers
    QSpacerItem *_horSpacerTitleLeft;
    QSpacerItem *_horSpacerTitleRight;
    QSpacerItem *_horSpacerButtonsLeft;
    QSpacerItem *_horSpacerButtonsRight;
};

#endif // EXTSELECTIONDIALOG_H
