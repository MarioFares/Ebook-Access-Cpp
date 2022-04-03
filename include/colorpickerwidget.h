#ifndef COLORPICKERWIDGET_H
#define COLORPICKERWIDGET_H

#include <QDialog>

class QGridLayout;
typedef QList<QPair<QColor,QColor>> GradientList;

class ColorPickerWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ColorPickerWidget(QWidget *parent = nullptr, QColor defaultColor = Qt::black);

    QColor getCurrentColor();

    bool colorSelected();

    QColor defaultColor;



private slots:
    QColor interpolateColor(QColor colorStart, QColor colorEnd, float percent);

    GradientList initialialzeColorGradients();

    void createColorsButtons();

    void setButtonColors(GradientList gradientList);

    void setCurrentColor(QColor color);

    void openColorDialog();


private:
    void setupConnections();

    void setupUi();

    int rows;

    int columns;

    QGridLayout* colorGridLayout;

    QColor currentColor;

    QPushButton *defaultColorButton;

    QPushButton *moreColorsButton;

    bool isColorSelected;
};

#endif // COLORPICKERWIDGET_H
