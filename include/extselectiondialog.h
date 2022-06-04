#ifndef EXTSELECTIONDIALOG_H
#define EXTSELECTIONDIALOG_H

#include <QDialog>
#include <QVector>
#include <QListWidgetItem>

namespace Ui {
class extSelectionDialog;
}

class ExtSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExtSelectionDialog(QWidget *parent = nullptr, QVector<QString> extVector = {},
                                QString title = "Preferences", QString label = "Select Preferences");
    ~ExtSelectionDialog();

    QVector<QString> getExtVector();

private slots:
    void setupConnections();

    void fillListWidget(QVector<QString> extVector);

    void setItemState(QListWidgetItem *item);

    void searchText(const QString &arg1);

    void setItemsCheckState(Qt::CheckState state);

private:
    Ui::extSelectionDialog *ui;

    QVector<QString> outputVector;

    QVector<QString> inputVector;

    void setExtVector();
};

#endif // EXTSELECTIONDIALOG_H
