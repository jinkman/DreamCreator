#include "TrackLineWidget.h"
#include <QToolButton>
#include "DividingRule.h"
#include <QGraphicsProxyWidget>
#include <QScrollBar>

namespace DM {

TrackLineWidget::TrackLineWidget(Track *track, int sceneWidth, QWidget *parent) :
    QGraphicsView(parent), mTrack(track) {
    mScene = new QGraphicsScene(this);
    mScene->setSceneRect(0, 0, sceneWidth, 50);
    this->setFrameShape(QFrame::NoFrame);

    this->setFixedHeight(50);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏横向滚动条
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 隐藏竖向滚动条
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->verticalScrollBar()->setDisabled(true);

    setupTrackLine();
    this->setScene(mScene);
    this->centerOn(0, 0);
}

TrackLineWidget::~TrackLineWidget() {
}

// 更新场景长度
void TrackLineWidget::updateSceneWidthSlot(int v) {
    mScene->setSceneRect(0, 0, v, 50);
}

void TrackLineWidget::setupTrackLine() {
    auto footageTrack = static_cast<FootageTrack *>(mTrack);
    if (footageTrack == nullptr) {
        return;
    }
    auto footages = footageTrack->getFootages();
    for (auto &footage : footages) {
        FootageWidget *footageWgt = new FootageWidget(nullptr);
        footageWgt->updateFootage(footage.get());
        auto proxy = mScene->addWidget(footageWgt);
        int startX = std::ceil(footage->startTime() / 1000.0f * oneSecondNoScaleStep);
        proxy->setPos(startX, 0);
    }
}

} // namespace DM