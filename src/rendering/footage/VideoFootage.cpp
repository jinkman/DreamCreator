#include "VideoFootage.h"
#include "rendering/layer/ImageLayer.h"

namespace DM {

std::shared_ptr<Footage> VideoFootage::createVideoFootageByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) {
    std::shared_ptr<VideoFootage> videoFootage(new VideoFootage(obj, rtNode));
    return videoFootage;
}

VideoFootage::VideoFootage(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) :
    Footage(obj, rtNode) {
    // 解析
    mFootageType = EFootageType::EVIDEO_FOOTAGE;
    mResStartTime = obj["resStartTime"].get<DMTime>();
    mResEndTime = obj["resEndTime"].get<DMTime>();
    mLayerJson["type"] = "video";
}

VideoFootage::~VideoFootage() {
}

void VideoFootage::flush(DMTime t) {
    Footage::flush(t);
    if (t < startTime() || t >= endTime()) { // 不可见
        mLayer->visible() = false;
        return;
    }

    mLayer->visible() = footageVisible();
    double ratio = double(t - startTime()) / double(duration());
    DMTime interceptTime = mResStartTime + DMTime((mResEndTime - mResStartTime) * ratio);

    // 时间转帧
    DMFrame frame = timeToFrame(interceptTime, mFrameRate);
    if (frame == mLastFrame) {
        return;
    }

    // 跳帧
    if (frame - mLastFrame != 1) { // 设置开始帧
        int stepFrame = frame - mLastFrame;
        if (stepFrame < 0 || stepFrame > 10) { // 读取空帧，比跳帧划算
            mVideoCap.set(cv::CAP_PROP_POS_FRAMES, frame);
        } else {
            for (int i = 0; i < stepFrame - 1; i++) {
                mVideoCap.read(matCache);
            }
        }
    }

    mVideoCap.read(matCache);
    mLastFrame = frame;
    if (matCache.empty()) { // 读取空帧不算错误
        return;
    }
    cv::cvtColor(matCache, matCache, cv::COLOR_BGRA2RGBA);
    // 更新
    std::static_pointer_cast<ImageLayer>(mLayer)->updateImageData(matCache.data, matCache.cols, matCache.rows, matCache.channels());
}

void VideoFootage::updateResources(const std::string &path) {
    Footage::updateResources(path);

    std::string localPath = getResourcesLocalPath();
    mVideoCap.open(localPath);
    if (!mVideoCap.isOpened()) {
        return;
    }
    mFrameRate = mVideoCap.get(cv::CAP_PROP_FPS); // 帧率
    mLastFrame = -1;
    // 创建图层
    if (mLayer == nullptr) {
        mLayer = ImageLayer::creatImageLayerByJson(mLayerJson, *mRootNode);
        mRootNode->getRootComposition()->addLayer(mLayer);
    }
}

DMTime VideoFootage::getResStartTime() const {
    return mResStartTime;
}

DMTime VideoFootage::getResEndTime() const {
    return mResEndTime;
}

} // namespace DM