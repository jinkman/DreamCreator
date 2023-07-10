#include "VideoPlayer.h"
#include <QProgressBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QPalette>
#include <QHBoxLayout>
#include <QPushButton>
#include "common/Common.h"
#include <QResizeEvent>

namespace DM {

VideoPlayer::VideoPlayer(QWidget *parent) :
    QWidget(parent) {
    setupLayout();

    setupTopWgt();

    setupPlayer();

    setupBottomWgt();
}

VideoPlayer::~VideoPlayer() {
}

void VideoPlayer::initSceneByFile(const QString &str) {
    mOpenGLWgt->initSceneByFile(str);
    mOpenGLWgt->setContentSize(mHCenterWgt->size());
    // 设置总时长
    setTimeToLabel(mLabelTotolTime, mOpenGLWgt->getScenePlayer()->duration());
    // 播放
    mTimerStep = 1000.0f / mOpenGLWgt->getScenePlayer()->getVideoInfo().fps;
    if (mTimer == nullptr) {
        mTimer = new QTimer(this);
        connect(mTimer, SIGNAL(timeout()), this, SLOT(flushOneFrame()));
    }
}

void VideoPlayer::resetScene() {
    if (mTimer->isActive()) {
        mTimer->stop();
    }
    mOpenGLWgt->closeScene();
}

void VideoPlayer::setupLayout() {
    // 垂直布局 三部分
    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setContentsMargins(0, 7, 0, 0);
    mMainLayout->setSpacing(0);
    this->setLayout(mMainLayout);
}

void VideoPlayer::setupTopWgt() {
    // 放置label
    QLabel *topLabel = new QLabel(this);
    topLabel->setMinimumSize(300, 50);
    topLabel->setMaximumHeight(50);
    QHBoxLayout *layout = new QHBoxLayout(topLabel);
    topLabel->setContentsMargins(0, 0, 0, 0);
    topLabel->setStyleSheet(QString("QLabel{\n"
                                    "   background-color:rgb(41, 41, 45);\n"
                                    "   border-top-left-radius:15;\n"
                                    "   border-top-right-radius:15;\n"
                                    "}"));

    QLabel *lable = new QLabel("播放器", topLabel);
    lable->setStyleSheet(QString("QLabel{\n"
                                 "  font: 12px;\n"
                                 "  color:white;\n"
                                 "}"));

    QPushButton *btn = new QPushButton(topLabel);
    btn->setStyleSheet(QString("QPushButton{\n"
                               "    background:transparent;\n"
                               "    border-image:url(%1);\n"
                               "}\n"
                               "QPushButton::hover{\n"
                               "    color: rgb(40, 40, 40);\n"
                               "    border-image:url(%2);\n"
                               "}\n"
                               "QPushButton::!hover{\n"
                               "    color: rgb(255, 255, 255);\n"
                               "}\n")
                           .arg(getQLocalPath("icon/menuOpen_normal.png"))
                           .arg(getQLocalPath("icon/menuOpen_notNormal.png")));
    btn->setFixedSize(20, 20);

    layout->addWidget(lable);
    layout->addStretch();
    layout->addWidget(btn);

    topLabel->setLayout(layout);
    mMainLayout->addWidget(topLabel);
}

void VideoPlayer::setupPlayer() {
    mOpenGLWgt = new OpenGLWidget(this);
    // 水平垂直居中
    mHCenterWgt = new QWidget(this);
    mHCenterWgt->setMinimumSize(300, 300);
    mHCenterWgt->setStyleSheet(QString("QWidget{\n"
                                       "    background-color:rgb(27, 27, 28);\n"
                                       "}"));
    QHBoxLayout *hLayout = new QHBoxLayout(mHCenterWgt);
    QWidget *vCenterWgt = new QWidget(mHCenterWgt);
    QVBoxLayout *vLayout = new QVBoxLayout(vCenterWgt);
    // 水平
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->addStretch();
    vLayout->addWidget(mOpenGLWgt);
    vLayout->addStretch();
    // 垂直
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->addStretch();
    hLayout->addWidget(vCenterWgt);
    hLayout->addStretch();
    mMainLayout->addWidget(mHCenterWgt);
}

void VideoPlayer::setupBottomWgt() {
    // 放置播放按钮相关
    QLabel *bottomLabel = new QLabel(this);
    bottomLabel->setMinimumSize(300, 40);
    bottomLabel->setMaximumHeight(40);
    QHBoxLayout *layout = new QHBoxLayout(bottomLabel);
    bottomLabel->setContentsMargins(0, 0, 0, 0);
    bottomLabel->setStyleSheet(QString("QLabel{\n"
                                       "    background-color:rgb(41, 41, 45);\n"
                                       "    border-bottom-left-radius:15;\n"
                                       "    border-bottom-right-radius:15;\n"
                                       "}"));

    // 1.Label;2.Label;3.PushButton
    mLabelPlayTime = new QLabel(bottomLabel);
    setTimeToLabel(mLabelPlayTime, 0);
    mLabelPlayTime->setStyleSheet(QString("QLabel{\n"
                                          " font: 10px;\n"
                                          " color:rgb(0, 193, 205);\n"
                                          "}"));
    mLabelPlayTime->setFixedSize(70, 20);
    mLabelTotolTime = new QLabel(bottomLabel);
    setTimeToLabel(mLabelTotolTime, 0);
    mLabelTotolTime->setStyleSheet(QString("QLabel{\n"
                                           "    font: 10px;\n"
                                           "    color:rgb(255, 255, 255);\n"
                                           "}"));
    mLabelTotolTime->setFixedSize(70, 20);
    // 播放按钮
    QPushButton *btn = new QPushButton(bottomLabel);
    btn->setStyleSheet(QString("QPushButton{\n"
                               "    background:transparent;\n"
                               "}\n"));
    btn->setFixedSize(20, 20);
    btn->setIcon(QIcon(getQLocalPath("icon/play.png")));
    connect(btn, &QPushButton::clicked, this, [=]() {
        btn->setIcon(QIcon(mTimer->isActive() ? getQLocalPath("icon/play.png") : getQLocalPath("icon/pause.png")));
        mTimer->isActive() ? mTimer->stop() : mTimer->start(mTimerStep);
    });

    layout->addWidget(mLabelPlayTime);
    layout->addWidget(mLabelTotolTime);
    layout->addWidget(btn);
    layout->addStretch();
    bottomLabel->setLayout(layout);
    mMainLayout->addWidget(bottomLabel);
}

void VideoPlayer::saveFile() {
    QMessageBox::about(this, tr("About saveFile"), "saveFile");
}

void VideoPlayer::exportFile() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Video"), "", tr("Video (*.mp4)"));
    if (!fileName.isEmpty()) {
        mOpenGLWgt->exportVideo(fileName.toStdString());
    }
}

void VideoPlayer::setTimeToLabel(QLabel *label, DMTime t) {
    int hours = t / 3600000;
    int minutes = (t % 3600000) / 60000;
    int seconds = (t % 60000) / 1000;
    int mSeconds = t % 1000;
    label->setText(QString("%1:%2:%3:%4").arg(hours, 2, 10, QLatin1Char('0')).arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0')).arg(mSeconds, 3, 10, QLatin1Char('0')));
}

void VideoPlayer::resizeEvent(QResizeEvent *event) {
    if (mOpenGLWgt && mOpenGLWgt->getScenePlayer() && mHCenterWgt) {
        mOpenGLWgt->setContentSize(mHCenterWgt->size());
    }
}

void VideoPlayer::flushOneFrame() {
    auto scenePlayer = mOpenGLWgt->getScenePlayer();
    if (scenePlayer == nullptr) {
        return;
    }
    mOpenGLWgt->update();
    setTimeToLabel(mLabelPlayTime, scenePlayer->currentTime());
    // 下一帧
    scenePlayer->setProgress(scenePlayer->getProgress() + 1.0f / float(timeToFrame(scenePlayer->getVideoInfo().duration, scenePlayer->getVideoInfo().fps)));
}

} // namespace DM