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

private:
    void setupTitle();

    void setupPlayer();

    void setupBottomWgt();

    void setTimeToLabel(QLabel *label, DMTime t);

private slots:

    void flushNextFrame();

private:
    // 时长
    QLabel *mLabelPlayTime = nullptr;
    QLabel *mLabelTotolTime = nullptr;
    OpenGLWidget *mOpenGLWgt = nullptr;
    // 播放定时器
    QPushButton *mPlayBtn = nullptr;
    QTimer *mTimer = nullptr;
    // 时间间隔
    float mTimerStep = 0.0f;
};

} // namespace DM

#endif // LAYER_WINDOW_H