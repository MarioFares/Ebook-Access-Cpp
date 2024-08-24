#ifndef COLORPICKERWIDGET_H
#define COLORPICKERWIDGET_H

#include <QFrame>
#include <QDialog>
#include <QBoxLayout>

class QGridLayout;

typedef QList<QPair<QColor, QColor> > GradientList;

class ColorPickerWidget : public QDialog {
    Q_OBJECT

public:
    explicit ColorPickerWidget(QWidget *parent = nullptr, QColor defaultColor = Qt::black);

    QColor getCurrentColor();

    bool colorSelected() const;

    QColor _defaultColor;

private slots:
    static QColor interpolateColor(QColor colorStart, QColor colorEnd, float percent);

    static GradientList initialialzeColorGradients();

    void createColorsButtons();

    void setButtonColors(GradientList gradientList);

    void setCurrentColor(QColor color);

    void openColorDialog();

    void setupConnections();

    void setupInterface();

private:
    int _rows;
    int _columns;
    QColor _currentColor;
    bool _isColorSelected;

    // Widgets
    QPushButton *_defaultColorButton;
    QPushButton *_moreColorsButton;
    QFrame *_mainFrame;

    // Layouts
    QVBoxLayout *_vertLayMain;
    QGridLayout *_colorGridLayout;
    QHBoxLayout *_horLayDefaultButton;
    QHBoxLayout *_horLayMoreColors;

    // Spacers
    QSpacerItem *_horSpacerDefaultButtonRight;
    QSpacerItem *_horSpacerMoreColorsRight;
};

#endif // COLORPICKERWIDGET_H
