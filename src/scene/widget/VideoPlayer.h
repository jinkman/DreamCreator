#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "OpenGLWidget.h"
#include "scene/window/ContentWindow.h"
#include <QTimer>

namespace DM {

class VideoPlayer : public ContentWindow {
    Q_OBJECT
public:
    explicit VideoPlayer(QWidget *parent = 0);

    ~VideoPlayer();

    void initSceneByFile(const QString &str);

    void resetScene();

public slots:
    void saveFile();

    void exportFile();

private:
    void setupTitle();

    void setupPlayer();

    void setupBottomWgt();

    void setTimeToLabel(QLabel *label, DMTime t);

    void resizeEvent(QResizeEvent *event) override;

private slots:
    void flushOneFrame(); // 更新场景

private:
    // 垂直布局
    OpenGLWidget *mOpenGLWgt = nullptr;
    QWidget *mHCenterWgt = nullptr;
    // 时长
    QLabel *mLabelPlayTime = nullptr;
    QLabel *mLabelTotolTime = nullptr;
    // 播放定时器
    QTimer *mTimer = nullptr;
    // 时间间隔
    float mTimerStep = 0.0f;
};

} // namespace DM

#endif // VIDEOPLAYER_H