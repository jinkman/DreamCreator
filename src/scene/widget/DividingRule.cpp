#include "DividingRule.h"
#include <QPainter>
#include <QPen>
#include <QScrollBar>
#include <QResizeEvent>
#include "utils/GlobalMsgMgr.h"
#include "rendering/Player.h"

namespace DM {

RuleWidget::RuleWidget(QWidget *parent) :
    QWidget(parent) {
    connect(&GlobalMsgMgr::getInstance(), &GlobalMsgMgr::updateProgress, [this](Player *player) {
        setCurentTime(player->currentTime());
    });

    connect(&GlobalMsgMgr::getInstance(), &GlobalMsgMgr::initSceneFinished, [this](Player *player) {
        setDuration(player->duration());
    });
}

RuleWidget::~RuleWidget() {
}

void RuleWidget::setDuration(const DMTime &v) {
    mDuration = v;
    updateRule();
}

void RuleWidget::updateRule() {
    DMTime maxTime = mDuration + 5000;
    int ruleNum = maxTime / 1000 + 1;
    mRuleSliceInfos.clear();
    mRuleSliceInfos.reserve(ruleNum);
    for (int i = 0; i < ruleNum; i++) {
        auto startX = i * oneSecondNoScaleStep;
        int height = i % 5 == 0 ? 13 : 10;
        RuleSliceInfo info = {QString("%1s").arg(i), 0, QRect(QPoint(startX, 3), QSize(1, height))};
        mRuleSliceInfos.emplace_back(info);
    }
    // 设置长度，留白
    setFixedWidth(oneSecondNoScaleStep * maxTime / 1000 + 200);
    update();
}

void RuleWidget::seekToTimeUI(DMTime t) {
    DMTime currentT = std::max(DMTime(0), std::min(t, mDuration - 1));
    setCurentTime(currentT);
    emit GlobalMsgMgr::getInstance().seekToTimeByRule(currentT);
}

void RuleWidget::setCurentTime(const DMTime &v) {
    if (mCurrentTime == v) {
        return;
    }
    mCurrentTime = v;
    updateRule();
}

void RuleWidget::paintEvent(QPaintEvent *event) {
    // 初始化画笔
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen({104, 104, 104, 255});
    auto font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);

    // 绘制事件
    for (auto &ruleSliceInfo : mRuleSliceInfos) {
        auto rect = ruleSliceInfo.rect;
        painter.fillRect(rect, {104, 104, 104, 255});
        painter.drawText(QPointF(rect.x() + 4, 10 + rect.y()), ruleSliceInfo.text);
    }
    // 进度条
    auto startX = oneSecondNoScaleStep * mCurrentTime / 1000;
    auto progressRect = QRect(QPoint(startX, 0), QSize(1, this->height()));
    painter.fillRect(progressRect, {4, 82, 200, 255});
}

void RuleWidget::resizeEvent(QResizeEvent *event) {
    emit sizeChanged(event->size());
    QWidget::resizeEvent(event);
}

void RuleWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        float xPos = event->pos().x() / oneSecondNoScaleStep;
        seekToTimeUI(xPos * 1000);
        mLeftButtonPressed = true;
    }

    QWidget::mousePressEvent(event);
}

void RuleWidget::mouseMoveEvent(QMouseEvent *event) {
    if (mLeftButtonPressed) {
        float xPos = event->pos().x() / oneSecondNoScaleStep;
        seekToTimeUI(xPos * 1000);
    }
    QWidget::mouseMoveEvent(event);
}

void RuleWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        mLeftButtonPressed = false;
    }
    QWidget::mouseReleaseEvent(event);
}

DividingRule::DividingRule(QWidget *parent) :
    QScrollArea(parent) {
    mRuleWidget = new RuleWidget(this);

    this->setFixedHeight(30);
    this->setWidgetResizable(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏横向滚动条
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 隐藏竖向滚动条
    this->setFrameShape(QFrame::NoFrame);
    this->horizontalScrollBar()->setSliderPosition(0);
    this->verticalScrollBar()->setSliderPosition(0);
    this->verticalScrollBar()->setDisabled(true);
    this->setWidget(mRuleWidget);
    // 绑定信号
    QObject::connect(mRuleWidget, &RuleWidget::sizeChanged, [=](QSize size) {
        emit sceneWidthChangedSignal(size.width());
    });
}

DividingRule::~DividingRule() {
}

RuleWidget *DividingRule::getRuleWidget() {
    return mRuleWidget;
}

} // namespace DM