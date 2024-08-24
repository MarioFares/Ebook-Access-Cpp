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

    QColor m_defaultColor;

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
    int m_rows;
    int m_columns;
    QColor m_currentColor;
    bool m_isColorSelected;

    // Widgets
    QPushButton *m_defaultColorButton;
    QPushButton *m_moreColorsButton;
    QFrame *m_mainFrame;

    // Layouts
    QVBoxLayout *m_vertLayMain;
    QGridLayout *m_colorGridLayout;
    QHBoxLayout *m_horLayDefaultButton;
    QHBoxLayout *m_horLayMoreColors;

    // Spacers
    QSpacerItem *m_horSpacerDefaultButtonRight;
    QSpacerItem *m_horSpacerMoreColorsRight;
};

#endif // COLORPICKERWIDGET_H
