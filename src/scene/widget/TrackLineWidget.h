
#ifndef TRACK_LINE_WIDGET_H
#define TRACK_LINE_WIDGET_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <rendering/track/FootageTrack.h>
#include "FootageWidget.h"

namespace DM {

class TrackLineWidget : public QGraphicsView {
    Q_OBJECT
public:
    explicit TrackLineWidget(Track *track, int sceneWidth, QWidget *parent = nullptr);

    ~TrackLineWidget();

public slots:
    void updateSceneWidthSlot(int v);

protected:
    void setupTrackLine();

private:
    QGraphicsScene *mScene = nullptr;
    QHBoxLayout *mLayout = nullptr;
    Track *mTrack = nullptr;
};
} // namespace DM

#endif // TRACK_LINE_WIDGET_H
