#ifndef DIVIDINGRULE_H
#define DIVIDINGRULE_H

#include <QScrollArea>
#include <QList>
#include "utils/Types.h"

namespace DM {

const float oneSecondNoScaleStep = 50.0f;

// 刻度尺相关
class RuleWidget : public QWidget {
    Q_OBJECT
public:
    explicit RuleWidget(QWidget *parent = 0);

    ~RuleWidget();

    void setDuration(const DMTime &v);

    void setCurentTime(const DMTime &v);

    // 坐标刻度信息
    struct RuleSliceInfo {
        /* data */
        QString text = "";         // 展示内容
        DMTime time = 0;           // 时间
        QRect rect = {0, 0, 0, 0}; // 位置
    };

signals:
    void sizeChanged(const QSize &);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    virtual void resizeEvent(QResizeEvent *event) override;

    virtual void mousePressEvent(QMouseEvent *event) override;

    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    // 更新坐标尺
    void updateRule();

    void seekToTimeUI(DMTime t);

private:
    DMTime mDuration = 0;    // 总时长
    DMTime mCurrentTime = 0; // 当前时长
    std::vector<RuleSliceInfo> mRuleSliceInfos;
    bool mLeftButtonPressed = false;
};

// 刻度尺相关
class DividingRule : public QScrollArea {
    Q_OBJECT
public:
    explicit DividingRule(QWidget *parent = 0);

    ~DividingRule();

    RuleWidget *getRuleWidget();

signals:
    void sceneWidthChangedSignal(int v);

protected:
    RuleWidget *mRuleWidget = nullptr;
};

} // namespace DM

#endif // DIVIDINGRULE_H
