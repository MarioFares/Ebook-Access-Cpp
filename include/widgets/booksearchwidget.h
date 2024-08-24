#ifndef BOOKSEARCHWIDGET_H
#define BOOKSEARCHWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QBoxLayout>
#include <QListWidget>

class BookSearchWidget : public QWidget {
    Q_OBJECT

public:
    explicit BookSearchWidget(QWidget *parent = nullptr);

public slots:
    void setMainLayoutMargin(int left, int top, int right, int bottom);

    void setMainLayoutSpacing(int spacing);

    void clearSearchText();

    void clearListWidget();

    void setHideSearchBar(bool hide);

    bool searchBarHidden();

    bool widgetHidden();

    void setHideWidget(bool hide);

    QString findItem(const QString &name);

    void searchString();

    int currentRow();

    void setCurrentRow(int row);

    QString currentItemText();

    void setCurrentItemText(QString text);

signals:
    void itemClicked(const QString &currentText);

    void selectionChanged(const QString &currentText);

private slots:
    void setupInterface();

    void setupConnections();

    static void openEbook(QListWidgetItem *item);

    void setupItemClicked(QListWidgetItem *item);

private:
    QLineEdit *_textSearchBar;
    QListWidget *_listWidget;
    QVBoxLayout *_vertLayMain;
};

#endif // BOOKSEARCHWIDGET_H
