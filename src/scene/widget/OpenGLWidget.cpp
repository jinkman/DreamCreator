#include "OpenGLWidget.h"
#include <QMessageBox>
#include <QCoreApplication>
#include "scene/widget/ProcessDlg.h"
#include "utils/GlobalMsgMgr.h"

namespace DM {

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent) {
    connect(&GlobalMsgMgr::getInstance(), &GlobalMsgMgr::exportFile, this, &OpenGLWidget::exportVideo);
    connect(&GlobalMsgMgr::getInstance(), &GlobalMsgMgr::seekToTimeByRule, this, &OpenGLWidget::seekToTime);
}

OpenGLWidget::~OpenGLWidget() {
    closeScene();
}

void OpenGLWidget::initSceneByFile(const QString &jstr) {
    // 绑定至当前context
    makeCurrent();
    std::filesystem::path filePath(jstr.toStdString());
    std::ifstream ifs(filePath.c_str());
    if (!ifs.is_open()) {
        QMessageBox::warning(this, "内容错误", QString("场景文件文件无法打开: %1").arg(QString::fromStdString(filePath)), QMessageBox::Yes);
        return;
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    auto obj = nlohmann::json::parse(ss.str(), nullptr, false);
    if (obj.is_discarded()) {
        QMessageBox::warning(this, "内容错误", QString("非法的场景文件: %1").arg(QString::fromStdString(filePath)), QMessageBox::Yes);
        return;
    }
    mPlayer = Player::createPlayerByJson(obj, filePath.parent_path());
}

void OpenGLWidget::closeScene() {
    // 释放场景
    mPlayer = nullptr;
}

void OpenGLWidget::exportVideo(const QString &str) {
    std::filesystem::path path = str.toStdString();
    if (mPlayer == nullptr) {
        return;
    }
    // 导出视频
    ProgressDlg *progressDlg = new ProgressDlg(this);
    progressDlg->show();
    auto progressRecord = [this, &progressDlg](float v) -> bool {
        // 设置进度
        progressDlg->setProgress(v);
        QCoreApplication::processEvents();
        return !progressDlg->isCancel();
    };
    mPlayer->exportVideo(path, progressRecord);
    delete progressDlg;
}

Player *OpenGLWidget::getScenePlayer() {
    return mPlayer.get();
}

void OpenGLWidget::initializeGL() {
}

void OpenGLWidget::resizeGL(int w, int h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void OpenGLWidget::paintGL() {
    // 绘制
    GLUtils::Get()->bindFrameBuffer(defaultFramebufferObject());
    GLUtils::Get()->cleanColor({27.0f / 255.0f, 27.0f / 255.0f, 28.0f / 255.0f, 1.0f});
    if (mPlayer && mPlayer->isValid()) {
        mPlayer->flush();
        // 发送全局信号
        emit GlobalMsgMgr::getInstance().flushOneFrame(mPlayer.get());
    }
}

void OpenGLWidget::seekToTime(const DMTime &t) {
    if (mPlayer == nullptr) {
        return;
    }
    mPlayer->setProgress(float(t) / float(mPlayer->duration()));
    update();
}

} // namespace DM