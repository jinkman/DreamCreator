#ifndef DIVIDINGRULE_H
#define DIVIDINGRULE_H

#include <QWidget>

namespace DM {

// 刻度尺相关
class DividingRule : public QWidget {
    Q_OBJECT
public:
    explicit DividingRule(QWidget *parent = 0);

    ~DividingRule();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    // 缩放值
    float mScale = 1.0f;
};

} // namespace DM

#endif // DIVIDINGRULE_H