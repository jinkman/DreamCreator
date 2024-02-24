#ifndef DOUBLE_SLIDER_WIDGET_H
#define DOUBLE_SLIDER_WIDGET_H

#include <QWidget>
#include <QSlider>

namespace DM {
class DoubleSliderWidget : public QSlider {
    Q_OBJECT
public:
    explicit DoubleSliderWidget(QWidget *parent = nullptr);
    ~DoubleSliderWidget();
    void setDecimals(int val);
    void setRange(double min, double max);

    double value();

    void setValue(double v);

signals:
    void valueChanged(double val);
protected slots:
    void valueIntToDouble(int val);

    void paintEvent(QPaintEvent *event) override;

private:
    double mMinValue = 0.0;
    double mMaxValue = 0.0;
    int mDecimals = 0;
};
} // namespace DM
#endif // DOUBLE_SLIDER_WIDGET_H