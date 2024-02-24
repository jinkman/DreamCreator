#include "DoubleSliderWidget.h"
#include <QStyleOptionSlider>
#include <QStylePainter>
#include <cmath>

namespace DM {
DoubleSliderWidget::DoubleSliderWidget(QWidget *parent) :
    QSlider(parent) {
    connect(this, &QSlider::valueChanged, this, &DoubleSliderWidget::valueIntToDouble);
}

DoubleSliderWidget::~DoubleSliderWidget() {
}

void DoubleSliderWidget::setDecimals(int val) {
    mDecimals = val;
}

void DoubleSliderWidget::setRange(double min, double max) {
    mMinValue = min;
    mMaxValue = max;
    int _pnum = int(pow(10, mDecimals));
    int _minval = min * _pnum;
    int _maxval = max * _pnum;
    QSlider::setRange(_minval, _maxval);
}

void DoubleSliderWidget::valueIntToDouble(int val) {
    double _val = double(val);
    double _pNum = pow(10, mDecimals);
    _val = _val / _pNum;
    emit valueChanged(_val);
}

double DoubleSliderWidget::value() {
    int val = QSlider::value();
    double _val = double(val);
    double _pNum = pow(10, mDecimals);
    _val = _val / _pNum;
    return _val;
}

void DoubleSliderWidget::setValue(double v) {
    int _pnum = int(pow(10, mDecimals));
    int value = round(_pnum * v);
    QSlider::setValue(value);
}

void DoubleSliderWidget::paintEvent(QPaintEvent *event) {
    QStylePainter p(this);
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    opt.subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderHandle;
    p.drawComplexControl(QStyle::CC_Slider, opt);
}

} // namespace DM