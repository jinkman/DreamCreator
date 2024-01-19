#ifndef TRACKWINDOW_H
#define TRACKWINDOW_H

#include <QScrollArea>
#include "ContentWindow.h"
#include "rendering/Player.h"
#include "scene/widget/ContainerWidget.h"
#include "scene/widget/DividingRule.h"

namespace DM {

class TrackWindow : public ContentWindow {
    Q_OBJECT
public:
    explicit TrackWindow(QWidget *parent = 0);

    ~TrackWindow();

private:
    void setupTitle();

    void setupContentLayout();

public slots:
    void updateTrackInfo(Player *scenePlayer);

private:
    ContainerWidget *mTrackControllerManager = nullptr;
    ContainerWidget *mTrackVisitorManager = nullptr;
    DividingRule *mDividingRule = nullptr;
};

} // namespace DM

#endif // TRACKWINDOW_H