#include "DividingRule.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>
#include <utils/LogUtils.h>
#include <QPen>

namespace DM {

DividingRule::DividingRule(QWidget *parent) :
    QWidget(parent) {
    this->setMinimumHeight(30);
    this->setMaximumHeight(30);
}

DividingRule::~DividingRule() {
}

void DividingRule::setDuration(const DMTime &v) {
    mDuration = v;
}

void DividingRule::paintEvent(QPaintEvent *event) {
    // 初始化画笔
    QPainter painter(this);
    painter.setPen({104, 104, 104, 255});
    auto font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);
    // painter.setBrush(QBrush({50, 50, 50, 255}));
    // painter.drawRect(rect());

    // 绘制
    for (int i = 0; i < rectList.size(); i++) {
        float value = round(showSize + (i - showSize) * mScale);
        // float value = i;
        painter.fillRect(rectList[i], {104, 104, 104, 255});
        QString strValue = tr("%1").arg(value);
        painter.drawText(QPointF(rectList[i].x() + 4, rectList[i].height() + rectList[i].y()), strValue);
    }
}

void DividingRule::mousePressEvent(QMouseEvent *event) {
    m_clickPoint = event->pos();
    m_bClick = true;
}

void DividingRule::mouseMoveEvent(QMouseEvent *event) {
    if (m_bClick && event->buttons() == Qt::LeftButton) {
        auto deltaX = event->pos().x() - m_clickPoint.x();
        if (rectList.size() > 0 && (rectList[0].x() + deltaX) >= 0) {
            deltaX = -rectList[0].x();
        }
        for (int i = 0; i < rectList.size(); i++) {
            rectList[i].setX(deltaX + rectList[i].x());
            rectList[i].setWidth(1);
        }
        m_clickPoint = event->pos();
        update();
    }
}

void DividingRule::mouseReleaseEvent(QMouseEvent *event) {
    m_bClick = false;
}

void DividingRule::wheelEvent(QWheelEvent *event) {
    // [-10,10]
    float delta = std::max(std::min(static_cast<float>(event->angleDelta().y()), 10.0f), -10.0f) * 0.01f;
    mScale *= 1.0 + delta;
    // 限制范围
    mScale = std::max(std::min(mScale, 10.0f), 0.1f);
    mDelta += delta;
    update();
}

void DividingRule::resizeEvent(QResizeEvent *event) {
    int k = 0;
    int i_size = (this->width() / showSize);
    rectList.clear();
    for (int i = 0; i < 100; i++) { // 创建100个数字
        QRect rect(k, 3, 1, 10);
        rectList.append(rect);
        k = k + i_size;
    }
}

} // namespace DM