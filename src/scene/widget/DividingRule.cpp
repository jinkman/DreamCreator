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
    mDuration = 10000;
    updateRule();
}

DividingRule::~DividingRule() {
}

void DividingRule::setDuration(const DMTime &v) {
    mDuration = v;
    updateRule();
}

void DividingRule::setCurentTime(const DMTime &v) {
    mCurrentTime = v;
}

void DividingRule::paintEvent(QPaintEvent *event) {
    // 初始化画笔
    QPainter painter(this);
    painter.setPen({104, 104, 104, 255});
    auto font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);
    painter.setBrush(QBrush({50, 50, 50, 255}));
    painter.drawRect(rect());

    // 绘制事件
    for (auto &ruleScaleInfo : mRuleScaleInfos) {
        auto rect = ruleScaleInfo.rect;
        painter.fillRect(rect, {104, 104, 104, 255});
        painter.drawText(QPointF(rect.x() + 4, rect.height() + rect.y()), ruleScaleInfo.text);
    }
}

void DividingRule::mousePressEvent(QMouseEvent *event) {
    m_clickPoint = event->pos();
    m_bClick = true;
}

void DividingRule::mouseMoveEvent(QMouseEvent *event) {
    if (m_bClick && event->buttons() == Qt::LeftButton && mRuleScaleInfos.size() != 0) {
        auto deltaX = event->pos().x() - m_clickPoint.x();
        auto firstRec = mRuleScaleInfos[0].rect;
        if (firstRec.x() + deltaX >= 0) {
            deltaX = -firstRec.x();
        }
        for (auto &ruleScaleInfo : mRuleScaleInfos) {
            ruleScaleInfo.rect.setX(deltaX + ruleScaleInfo.rect.x());
            ruleScaleInfo.rect.setWidth(1);
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
    float delta = std::max(std::min(static_cast<float>(event->angleDelta().y()), 10.0f), -10.0f);
    // 缩放
    // updateRule();
}

void DividingRule::resizeEvent(QResizeEvent *event) {
    // updateRule();
}

void DividingRule::updateRule() {
    DMTime maxTime = mDuration + 5000;
    int oneSecondNoScaleStep = 50; // 1s不缩放步长

    int ruleNum = maxTime / 1000 + 1;
    mRuleScaleInfos.clear();
    mRuleScaleInfos.reserve(ruleNum);
    for (int i = 0; i < ruleNum; i++) {
        int startX = i * oneSecondNoScaleStep;
        RuleScaleInfo info = {QString("%1s").arg(i), 0, QRect(QPoint(startX, 3), QSize(1, 10))};
        mRuleScaleInfos.emplace_back(info);
    }
    update();
}

} // namespace DM