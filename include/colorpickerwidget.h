#ifndef COLORPICKERWIDGET_H
#define COLORPICKERWIDGET_H

#include <QDialog>

class QGridLayout;
typedef QList<QPair<QColor,QColor>> GradientList;

class colorPickerWidget : public QDialog
{
    Q_OBJECT

public:
    explicit colorPickerWidget(QWidget *parent = nullptr);
    QColor       getCurrentColor();

private slots:
    QColor       interpolateColor(QColor colorStart, QColor colorEnd, float percent);
    GradientList initialialzeColorGradients();
    void         createColorsButtons();
    void         setButtonColors(GradientList gradientList);
    void         setCurrentColor(QColor color);
    void         openColorDialog();

private:
    int          rows;
    int          columns;
    QGridLayout* colorGridLayout;
    QColor       currentColor;
};

#endif // COLORPICKERWIDGET_H
