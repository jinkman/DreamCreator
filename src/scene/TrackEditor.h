#ifndef TRACKEDITOR_H
#define TRACKEDITOR_H

#include "OpenGLWidget.h"
#include <QVBoxLayout>
#include <QGestureEvent>

namespace DM {

class TrackEditor : public QWidget {
    Q_OBJECT
public:
    explicit TrackEditor(QWidget *parent = 0);

    ~TrackEditor();

private:
    void setupLayout();

    void setupTopWgt();

    void setupTrack();

    bool event(QEvent *e) override;

    bool gestureEvent(QGestureEvent *event);

    void pinchTriggered(QPinchGesture *gesture);

    void wheelEvent(QWheelEvent *event) override;

private:
    QVBoxLayout *mMainLayout = nullptr;
};

} // namespace DM

#endif // TRACKEDITOR_H