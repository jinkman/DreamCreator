#include "PlayerWindow.h"
#include <QResizeEvent>
#include <QFileDialog>
#include "scene/widget/ProcessDlg.h"
#include "utils/GlobalMsgMgr.h"
#include <QDateTime>

namespace DM {

PlayerWindow::PlayerWindow(QWidget *parent) :
    ContentWindow(parent) {
    setupTitle();

    setupPlayer();

    setupBottomWgt();
    getLayout()->setContentsMargins(3, 6, 3, 3);
    // 绑定相关事件
    connect(&GlobalMsgMgr::getInstance(), &GlobalMsgMgr::updateProgress, this, &PlayerWindow::onPlayerProgressUpdate);

    connect(&GlobalMsgMgr::getInstance(), &GlobalMsgMgr::updateProperty, this, &PlayerWindow::flushFrame);
}

PlayerWindow::~PlayerWindow() {
}

void PlayerWindow::initSceneByFile(const QString &str) {
    mOpenGLWgt->initSceneByFile(str);
    // 设置总时长
    setTimeToLabel(mLabelTotolTime, mOpenGLWgt->getScenePlayer()->duration());
    setTimeToLabel(mLabelPlayTime, 0);
    // 播放
    mTimerStep = 1000.0f / mOpenGLWgt->getScenePlayer()->getVideoInfo().fps;
    if (mTimerFlushFrame == nullptr) {
        mTimerFlushFrame = new QTimer(this);
        connect(mTimerFlushFrame, SIGNAL(timeout()), this, SLOT(flushFrame()));
    }

    if (mTimerFlushProgress == nullptr) {
        mTimerFlushProgress = new QTimer(this);
        connect(mTimerFlushProgress, SIGNAL(timeout()), this, SLOT(flushProgress()));
    }
    emit GlobalMsgMgr::getInstance().initSceneFinished(mOpenGLWgt -> getScenePlayer());
}

void PlayerWindow::closeScene() {
    if (mTimerFlushFrame->isActive()) {
        mTimerFlushFrame->stop();
    }
    if (mTimerFlushProgress->isActive()) {
        mTimerFlushProgress->stop();
    }
    mOpenGLWgt->closeScene();
    setTimeToLabel(mLabelTotolTime, 0);
    setTimeToLabel(mLabelPlayTime, 0);
}

void PlayerWindow::setupTitle() {
    auto titleLabel = getTitleWidget();

    QHBoxLayout *layout = new QHBoxLayout(titleLabel);
    QLabel *lable = new QLabel("播放器", titleLabel);
    lable->setStyleSheet(QString("QLabel{\n"
                                 "  font: 12px;\n"
                                 "  color:white;\n"
                                 "}"));

    QPushButton *btn = new QPushButton(titleLabel);
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
                           .arg(":/data/icon/menuOpen_normal.png")
                           .arg(":/data/icon/menuOpen_notNormal.png"));
    btn->setFixedSize(20, 20);

    layout->addWidget(lable);
    layout->addStretch();
    layout->addWidget(btn);
    titleLabel->setLayout(layout);
}

void PlayerWindow::setupPlayer() {
    mOpenGLWgt = new OpenGLWidget(this);
    mOpenGLWgt->setMinimumSize(300, 300);
    getLayout()->addWidget(mOpenGLWgt);
}

void PlayerWindow::setupBottomWgt() {
    // 放置播放按钮相关
    QLabel *bottomLabel = new QLabel(this);
    bottomLabel->setMinimumWidth(300);
    bottomLabel->setFixedHeight(40);
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
    mPlayBtn = new QPushButton(bottomLabel);
    mPlayBtn->setStyleSheet(QString("QPushButton{\n"
                                    "    background:transparent;\n"
                                    "}\n"));
    mPlayBtn->setFixedSize(20, 20);
    mPlayBtn->setIcon(QIcon(":/data/icon/play.png"));
    connect(mPlayBtn, &QPushButton::clicked, this, [=]() {
        mPlayBtn->setIcon(QIcon(mTimerFlushFrame->isActive() ? ":/data/icon/play.png" : ":/data/icon/pause.png"));
        mTimerFlushFrame->isActive() ? mTimerFlushFrame->stop() : mTimerFlushFrame->start(mTimerStep);
        mTimerFlushProgress->isActive() ? mTimerFlushProgress->stop() : mTimerFlushProgress->start(1000.0f / 60.0f);
        mLastTimestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    });

    layout->addWidget(mLabelPlayTime);
    layout->addWidget(mLabelTotolTime);
    layout->addWidget(mPlayBtn);
    layout->addStretch();
    bottomLabel->setLayout(layout);

    getLayout()->addWidget(bottomLabel);
}

void PlayerWindow::saveFile() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("josn (*.josn)"));
    if (!fileName.isEmpty()) {
        emit GlobalMsgMgr::getInstance().saveFile(fileName);
    }
}

void PlayerWindow::exportFile() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Video"), "", tr("Video (*.mp4)"));
    if (!fileName.isEmpty()) {
        emit GlobalMsgMgr::getInstance().exportFile(fileName);
    }
}

void PlayerWindow::onPlayerProgressUpdate(Player *scenePlayer) {
    setTimeToLabel(mLabelTotolTime, scenePlayer->duration());
    setTimeToLabel(mLabelPlayTime, scenePlayer->currentTime());
}

void PlayerWindow::setTimeToLabel(QLabel *label, DMTime t) {
    int hours = t / 3600000;
    int minutes = (t % 3600000) / 60000;
    int seconds = (t % 60000) / 1000;
    int mSeconds = t % 1000;
    label->setText(QString("%1:%2:%3:%4").arg(hours, 2, 10, QLatin1Char('0')).arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0')).arg(mSeconds, 3, 10, QLatin1Char('0')));
}

void PlayerWindow::flushFrame() {
    mOpenGLWgt->update();
}

void PlayerWindow::flushProgress() {
    auto scenePlayer = mOpenGLWgt->getScenePlayer();
    if (scenePlayer == nullptr) {
        return;
    }
    auto currentTimestap = QDateTime::currentDateTime().toMSecsSinceEpoch();
    auto step = currentTimestap - mLastTimestamp;
    mLastTimestamp = currentTimestap;
    scenePlayer->setProgress(scenePlayer->getProgress() + float(step) / float(scenePlayer->getVideoInfo().duration));
    emit GlobalMsgMgr::getInstance().updateProgress(scenePlayer);
}

} // namespace DM