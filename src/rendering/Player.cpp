#include "Player.h"
#include "common/Common.h"
#include <opencv2/opencv.hpp>

namespace DM {

std::shared_ptr<Player> Player::createPlayerByJson(const nlohmann::json &obj, const std::filesystem::path &path) {
    std::shared_ptr<Player> player(new Player(obj, path));
    return player;
}

Player::Player(const nlohmann::json &obj, const std::filesystem::path &path) {
    mVideoProperty = VideoProperty::deserialization(obj["videoProperty"]);
    // 数据
    auto jRoot = nlohmann::json::object();
    jRoot["width"] = mVideoProperty.width;
    jRoot["height"] = mVideoProperty.height;
    jRoot["backgroundColor"] = mVideoProperty.bkClr.serialization();
    rtNode = RootNode::creatRootNodeByJson(jRoot);
    rtNode->setRootPath(path);
    // 构建轨道
    mTracks.reserve(obj["trackInfo"].size());
    for (auto &trackObj : obj["trackInfo"]) {
        mTracks.push_back(Track::createTrackByJson(trackObj, rtNode));
    }
    initialize();
}

Player::~Player() {
    GLUtils::Get()->deleteFrameBuffer(mOffScreenFbo);
}

void Player::setProgress(float v) {
    mProgress = v > 1.0f ? v - 1.0f : v;
}

float Player::getProgress() {
    return mProgress;
}

void Player::flush() {
    if (!isValid()) {
        return;
    }
    // 离屏渲染
    auto oldViewPort = GLUtils::Get()->getCurrentViewPort();
    auto oldFbo = GLUtils::Get()->getBindFboId();
    GLUtils::Get()->bindFrameBuffer(mOffScreenFbo);
    // 绘制
    flushInternal();
    // 上屏
    GLUtils::Get()->bindFrameBuffer(oldFbo);
    GLUtils::Get()->setViewPort(oldViewPort);
    auto imageShader = rtNode->getImageShader();
    imageShader->use();
    imageShader->setTexture("tex", 0, mOffScreenFbo.tex.id);
    imageShader->setBool("bFlipY", true);
    imageShader->setMat4("projection", glm::ortho(0.0f, oldViewPort.width(), 0.0f, oldViewPort.height()));
    imageShader->setMat4("model", Layer::ApplyScaleMode(DMScaleMode::LetterBox, mOffScreenFbo.width, mOffScreenFbo.height, oldViewPort.width(), oldViewPort.height()));
    GLUtils::Get()->renderQuadInternal(Rect::MakeWH(mOffScreenFbo.width, mOffScreenFbo.height));
    imageShader->unUse();
}

DMTime Player::duration() const {
    return getVideoInfo().duration;
}

DMTime Player::currentTime() const {
    return DMTime(mProgress * duration());
}

int Player::getTrackSize() {
    return int(mTracks.size());
}

void Player::addTrackToPlayer(std::shared_ptr<Track> track) {
    mTracks.push_back(track);
}

void Player::addTrackToPlayerAtIndex(std::shared_ptr<Track> track, int index) {
    if (index < 0 || index > getTrackSize() - 1) {
        addTrackToPlayer(track);
    }
    mTracks.insert(mTracks.begin() + index, track);
}

void Player::removeTrack(std::shared_ptr<Track> track) {
    auto pos = std::find(mTracks.begin(), mTracks.end(), track);
    if (pos != mTracks.end()) {
        mTracks.erase(pos);
    }
}

void Player::removeTrackAtIndex(int index) {
    if (index < 0 || index > getTrackSize() - 1) {
        return;
    }
    mTracks.erase(mTracks.begin() + index);
}

VideoProperty Player::getVideoInfo() const {
    return mVideoProperty;
}

bool Player::isValid() const {
    return rtNode != nullptr && mOffScreenFbo.isValid();
}

std::filesystem::path Player::getRootPath() const {
    return isValid() ? rtNode->getRootPath() : "";
}

void Player::exportVideo(const std::filesystem::path &path, ProgressRecordFunc recordFunc) {
    // 离屏导出
    const DMFrame totalFrame = timeToFrame(mVideoProperty.duration, mVideoProperty.fps);
    auto oldProgress = getProgress();

    if (std::filesystem::exists(path)) {
        std::filesystem::remove(path);
    }

    cv::VideoWriter videoWriter(path, 0x31637661, mVideoProperty.fps, cv::Size(mVideoProperty.width, mVideoProperty.height));
    if (!videoWriter.isOpened()) {
        DMLOG << "create " << path.c_str() << " faild";
        return;
    }

    cv::Mat frame(mVideoProperty.height, mVideoProperty.width, CV_8UC3);
    // 获取默认缓冲
    auto oldFboId = GLUtils::Get()->getBindFboId();
    for (DMFrame frameIndex = 0; frameIndex < totalFrame; frameIndex++) {
        GLUtils::Get()->bindFrameBuffer(mOffScreenFbo);
        GLUtils::Get()->setViewPort(Rect::MakeWH(mVideoProperty.width, mVideoProperty.height));
        float progress = static_cast<float>(frameIndex) / static_cast<float>(totalFrame);
        setProgress(progress);
        flushInternal();
        GLUtils::Get()->readPixels(mOffScreenFbo, frame.data);
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        videoWriter.write(frame);
        if (!recordFunc(progress)) { // 用户输入取消
            break;
        }
    }
    videoWriter.release();
    recordFunc(1.0f);
    // 取消绑定
    GLUtils::Get()->bindFrameBuffer(oldFboId);
    setProgress(oldProgress);
}

void Player::initialize() {
    mOffScreenFbo = GLUtils::Get()->createFrameBuffer(mVideoProperty.width, mVideoProperty.height, 3);
}

void Player::flushInternal() {
    // 更新
    auto cTime = currentTime();
    for (auto track : mTracks) {
        track->flush(cTime);
    }
    rtNode->draw();
}

} // namespace DM