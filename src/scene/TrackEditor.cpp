#include "TrackEditor.h"
#include <QLabel>
#include <QToolButton>
#include "common/Common.h"
#include <QEvent>

namespace DM {

TrackEditor::TrackEditor(QWidget *parent) :
    QWidget(parent) {
    grabGesture(Qt::PinchGesture);
    setAttribute(Qt::WA_AcceptTouchEvents, true);

    setupLayout();

    setupTopWgt();

    setupTrack();
}

TrackEditor::~TrackEditor() {
}

void TrackEditor::setupLayout() {
    // 垂直布局
    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setContentsMargins(7, 7, 7, 7);
    mMainLayout->setSpacing(0);
    this->setLayout(mMainLayout);
}

void TrackEditor::setupTopWgt() {
    QLabel *topLabel = new QLabel(this);
    topLabel->setMinimumSize(minimumWidth(), 40);
    topLabel->setMaximumHeight(40);
    topLabel->setStyleSheet(QString("QLabel{\n"
                                    "   background-color:rgb(41, 41, 45);\n"
                                    "   border-top-left-radius:15;\n"
                                    "   border-top-right-radius:15;\n"
                                    "}"));
    QHBoxLayout *layout = new QHBoxLayout(topLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    // 放置按钮
    for (int i = 0; i < 6; i++) {
        QToolButton *btn = new QToolButton(topLabel);
        btn->setFixedSize(30, 30);
        btn->setIcon(QIcon(getQLocalPath("icon/play.png")));
        btn->setIconSize(QSize(20, 20));
        btn->setStyleSheet(QString("QToolButton{\n"
                                   "    color:white;\n"
                                   "    background:transparent;\n"
                                   "}"
                                   "QToolTip{\n"
                                   "    color:white;\n"
                                   "    background-color:rgb(7, 7, 9);\n"
                                   "}"));
        btn->setToolTip("提示信息");
        layout->addWidget(btn);
    }
    layout->addStretch();
    topLabel->setLayout(layout);
    mMainLayout->addWidget(topLabel);
}

void TrackEditor::setupTrack() {
    QLabel *bottomLabel = new QLabel(this);
    bottomLabel->setMinimumHeight(200);
    bottomLabel->setStyleSheet(QString("QLabel{\n"
                                       "    background-color:rgb(30, 30, 31);\n"
                                       "    border-bottom-left-radius:15;\n"
                                       "    border-bottom-right-radius:15;\n"
                                       "}"));
    mMainLayout->addWidget(bottomLabel);
}

bool TrackEditor::event(QEvent *event) {
    switch (event->type()) {
    case QEvent::Gesture: {
        return gestureEvent(static_cast<QGestureEvent *>(event));
    }
    default:
        break;
    }

    return QWidget::event(event);
}

bool TrackEditor::gestureEvent(QGestureEvent *event) {
    if (QGesture *pinch = event->gesture(Qt::PinchGesture)) {
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    }

    return true;
}

void TrackEditor::pinchTriggered(QPinchGesture *gesture) {
    static float scale = 1.0f;
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    if (changeFlags & QPinchGesture::ScaleFactorChanged) {
        auto scaleFactor = gesture->property("scaleFactor").toReal();
        DMLOG << "value:" << scaleFactor;
        scale += (scaleFactor - 1.0f) * 0.1f;
        DMLOG << "scale" << scale;
    }
}

void TrackEditor::wheelEvent(QWheelEvent *event) {
    DMLOG << event->angleDelta();
}

} // namespace DM