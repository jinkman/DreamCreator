#include "TrackWindow.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <common/Common.h>
#include "scene/widget/TrackControllerWidget.h"
#include "scene/widget/TrackLineWidget.h"
#include <QSplitter>
#include <QScrollBar>
#include "utils/GlobalMsgMgr.h"

namespace DM {

TrackWindow::TrackWindow(QWidget *parent) :
    ContentWindow(parent) {
    this->setStyleSheet("background-color:rgb(27, 27, 28)");
    this->setMinimumHeight(250);

    setupTitle();

    setupContentLayout();

    getLayout()->setContentsMargins(6, 3, 6, 6);
    connect(&GlobalMsgMgr::getInstance(), &GlobalMsgMgr::initSceneFinished, this, &TrackWindow::updateTrackInfo);
}

TrackWindow::~TrackWindow() {
}

void TrackWindow::setupTitle() {
    // 初始化ui
    auto titleLabel = getTitleWidget();
    titleLabel->setFixedHeight(30);
    QHBoxLayout *layout = new QHBoxLayout(titleLabel);
    layout->setContentsMargins(10, 0, 0, 0);
    // 放置按钮
    for (int i = 0; i < 6; i++) {
        QToolButton *btn = new QToolButton(titleLabel);
        btn->setFixedSize(25, 25);
        btn->setIcon(QIcon(getQLocalPath("icon/play.png")));
        btn->setIconSize(QSize(20, 20));
        btn->setStyleSheet(QString("QToolButton{\n"
                                   "    color:white;\n"
                                   "    background:transparent;\n"
                                   "}"
                                   "QToolTip{\n"
                                   "    color:black;\n"
                                   "    background-color:rgb(255, 255, 255);\n"
                                   "}"));
        btn->setToolTip("提示信息");
        layout->addWidget(btn);
    }
    layout->addStretch();
    titleLabel->setLayout(layout);

    mTrackControllerManager = new ContainerWidget(this);
    mTrackControllerManager->setFixedWidth(100);
    mTrackVisitorManager = new ContainerWidget(this);

    // 绑定滚动
    QObject::connect(mTrackControllerManager->verticalScrollBar(), &QScrollBar::valueChanged, [=](int value) {
        if (mTrackVisitorManager->verticalScrollBar()->value() != value)
            mTrackVisitorManager->verticalScrollBar()->setValue(value);
    });

    QObject::connect(mTrackVisitorManager->verticalScrollBar(), &QScrollBar::valueChanged, [=](int value) {
        if (mTrackControllerManager->verticalScrollBar()->value() != value)
            mTrackControllerManager->verticalScrollBar()->setValue(value);
    });

    QSplitter *outerSplitter = new QSplitter(Qt::Horizontal, this);
    getLayout()->addWidget(outerSplitter);
    outerSplitter->addWidget(mTrackControllerManager);

    QSplitter *rightSplitter = new QSplitter(Qt::Vertical, outerSplitter);
    // 添加时间戳
    mDividingRule = new DividingRule(rightSplitter);
    rightSplitter->addWidget(mDividingRule);
    rightSplitter->addWidget(mTrackVisitorManager);
    rightSplitter->setStretchFactor(1, 1);
    rightSplitter->setHandleWidth(1);
    rightSplitter->handle(1)->setDisabled(true);

    outerSplitter->addWidget(rightSplitter);
    outerSplitter->setStretchFactor(1, 1);
    outerSplitter->setChildrenCollapsible(false);
    // 禁止拖拽
    outerSplitter->handle(1)->setDisabled(true);
    outerSplitter->setHandleWidth(1);

    outerSplitter->setStyleSheet(QString("QSplitter::handle {\n"
                                         "    background-color:rgb(170,170,170);\n"
                                         "}"));

    mTrackControllerManager->getMainLayout()->setContentsMargins(5, mDividingRule->height(), 5, 100 + rightSplitter->handleWidth());
}

void TrackWindow::setupContentLayout() {
}

void TrackWindow::updateTrackInfo(Player *scenePlayer) {
    mTrackControllerManager->clear();
    mTrackVisitorManager->clear();
    if (scenePlayer == nullptr) {
        return;
    }
    auto tracks = scenePlayer->getTracks();
    std::vector<QScrollBar *> scrollBarVec = {mDividingRule->horizontalScrollBar()};
    for (auto track : tracks) {
        TrackControllerWidget *controllerWgt = new TrackControllerWidget(mTrackControllerManager);
        mTrackControllerManager->getMainLayout()->addWidget(controllerWgt);
        auto trackLineWgt = new TrackLineWidget(track, mDividingRule->widget()->width(), mTrackVisitorManager);
        // 绑定长度
        QObject::connect(mDividingRule, &DividingRule::sceneWidthChangedSignal, trackLineWgt, &TrackLineWidget::updateSceneWidthSlot);
        // 绑定点击事件
        // QObject::connect(trackLineWgt, &TrackLineWidget::clickUpFootageSignal, this, &TrackWindow::clickUpFootageSignal);
        mTrackVisitorManager->getMainLayout()->addWidget(trackLineWgt);
        scrollBarVec.push_back(trackLineWgt->horizontalScrollBar());
    }
    mTrackControllerManager->getMainLayout()->addStretch();
    mTrackVisitorManager->getMainLayout()->addStretch();
    mTrackControllerManager->resetScrollValue();
    mTrackVisitorManager->resetScrollValue();

    // 共享滚动事件
    for (auto &scrollBar : scrollBarVec) {
        QObject::connect(scrollBar, &QScrollBar::valueChanged, [=](int value) {
            for (auto &bar : scrollBarVec) {
                if (bar->value() != value) {
                    bar->setValue(value);
                }
            }
        });
    }
}

} // namespace DM