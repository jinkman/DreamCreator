#include "OpenglWidget.h"
#include <QTimer>

namespace DM {
OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent) {
}

OpenGLWidget::~OpenGLWidget() {
}

void OpenGLWidget::initSceneByFile(const QString &jstr) {
    mRootNode = RootNode::creatRootNodeByFile(jstr);
    setFixedSize(mRootNode->width(), mRootNode->height());
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(16);
}

void OpenGLWidget::closeScene() {
    // 释放场景
    mRootNode = nullptr;
}

void OpenGLWidget::setProgress(float v) {
    mProgress = v;
}

float OpenGLWidget::getProgress() {
    return mProgress;
}

void OpenGLWidget::initializeGL() {
}

void OpenGLWidget::resizeGL(int w, int h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void OpenGLWidget::paintGL() {
    // 绘制
    if (mRootNode) {
        mRootNode->draw();
    }
}

} // namespace DM