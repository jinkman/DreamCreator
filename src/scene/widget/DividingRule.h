#ifndef DIVIDINGRULE_H
#define DIVIDINGRULE_H

#include <QWidget>
#include <QList>
#include "utils/Types.h"

namespace DM {

// 刻度尺相关
class DividingRule : public QWidget {
    Q_OBJECT
public:
    explicit DividingRule(QWidget *parent = 0);

    ~DividingRule();

    void setDuration(const DMTime &v);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    // 缩放值
    float mScale = 1.0f;
    QList<QRect> rectList;
    int showSize = 20; // 页面显示20位
    QPoint m_clickPoint;
    bool m_bClick = false;
    // 总时长
    DMTime mDuration = 0;

    // 影响项:1.中心点影响缩放;2.mDuration影响总长度;3.缩放值为1.0刚好放下mDuration;
    float mDelta = 0.0f; // 控制放大缩小
};

} // namespace DM

#endif // DIVIDINGRULE_H