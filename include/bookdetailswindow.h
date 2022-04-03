#ifndef BOOKDETAILSWINDOW_H
#define BOOKDETAILSWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class BookDetailsWindow;
}

class BookDetailsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BookDetailsWindow(QWidget *parent = nullptr);
    ~BookDetailsWindow();

    void showBookDetails(const QString &name);


private:
    Ui::BookDetailsWindow *ui;

    void setupConnections();

    void searchEbooks(const QString &name);

    void openEbook(QListWidgetItem *item);
};

#endif // BOOKDETAILSWINDOW_H
