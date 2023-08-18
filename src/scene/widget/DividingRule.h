#ifndef DIVIDINGRULE_H
#define DIVIDINGRULE_H

#include <QWidget>
#include <QList>
#include "utils/Types.h"
#include <glm/gtc/matrix_transform.hpp>

namespace DM {

// 刻度尺相关
class DividingRule : public QWidget {
    Q_OBJECT
public:
    explicit DividingRule(QWidget *parent = 0);

    ~DividingRule();

    void setDuration(const DMTime &v);

    void setCurentTime(const DMTime &v);

    // 坐标刻度信息
    struct RuleScaleInfo {
        /* data */
        QString text = "";         // 展示内容
        DMTime time = 0;           // 时间
        QRect rect = {0, 0, 0, 0}; // 位置
    };

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

    // 更新坐标尺
    void updateRule();

private:
    // 拖拽
    QPoint m_clickPoint = {0, 0};
    bool m_bClick = false;
    // 总时长
    DMTime mDuration = 0;    // 总时长
    DMTime mCurrentTime = 0; // 当前时间戳
    int mCurrentPos = 0;     // 当前中心点位置
    // 刻度尺内容
    std::vector<RuleScaleInfo> mRuleScaleInfos;
};

} // namespace DM

#endif // DIVIDINGRULE_H
