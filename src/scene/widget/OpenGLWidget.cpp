#include "OpenGLWidget.h"
#include <QMessageBox>
#include "scene/ProcessDlg.h"
#include <QCoreApplication>

namespace DM {

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent) {
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
    setMinimumSize(300, 300);
    setMaximumSize(mPlayer->getVideoInfo().width, mPlayer->getVideoInfo().height);
}

void OpenGLWidget::closeScene() {
    // 释放场景
    mPlayer = nullptr;
}

void OpenGLWidget::exportVideo(const std::filesystem::path &path) {
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

std::shared_ptr<Player> OpenGLWidget::getScenePlayer() {
    return mPlayer;
}

void OpenGLWidget::setContentSize(QSize size) {
    float scaleX = float(size.width()) / float(mPlayer->getVideoInfo().width);
    float scaleY = float(size.height()) / float(mPlayer->getVideoInfo().height);
    // 黑边处理
    float scaleV = scaleX < scaleY ? scaleX : scaleY;
    this->setFixedSize(scaleV * mPlayer->getVideoInfo().width, scaleV * mPlayer->getVideoInfo().height);
}

void OpenGLWidget::initializeGL() {
}

void OpenGLWidget::resizeGL(int w, int h) {
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void OpenGLWidget::paintGL() {
    // 绘制
    if (mPlayer && mPlayer->isValid()) {
        mPlayer->flush();
    }
}

} // namespace DM