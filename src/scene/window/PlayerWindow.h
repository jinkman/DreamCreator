#ifndef LAYER_WINDOW_H
#define LAYER_WINDOW_H

#include <QPushButton>
#include "ContentWindow.h"
#include "scene/widget/OpenGLWidget.h"
#include "utils/Types.h"
#include <QTimer>

namespace DM {

class PlayerWindow : public ContentWindow {
    Q_OBJECT
public:
    explicit PlayerWindow(QWidget *parent = 0);

    ~PlayerWindow();

    void initSceneByFile(const QString &str);

    virtual void closeScene() override;

public slots:
    void saveFile();

    void exportFile();

    void onPlayerProgressUpdate(Player *scenePlayer);

private:
    void setupTitle();

    void setupPlayer();

    void setupBottomWgt();

    void setTimeToLabel(QLabel *label, DMTime t);

private slots:

    void flushFrame();

    void flushProgress();

private:
    // 时长
    QLabel *mLabelPlayTime = nullptr;
    QLabel *mLabelTotolTime = nullptr;
    OpenGLWidget *mOpenGLWgt = nullptr;
    // 播放定时器，两个定时器。一个定时刷新画面，一个定时设置进度
    QPushButton *mPlayBtn = nullptr;
    QTimer *mTimerFlushFrame = nullptr;
    QTimer *mTimerFlushProgress = nullptr;
    // 时间间隔
    float mTimerStep = 0.0f;
    qint64 mLastTimestamp = 0;
};

} // namespace DM

#endif // LAYER_WINDOW_H