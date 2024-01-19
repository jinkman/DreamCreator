#ifndef VIDEOFOOTAGE_H
#define VIDEOFOOTAGE_H

#include "Footage.h"
#include "rendering/layer/ImageLayer.h"
#include <opencv2/opencv.hpp>

namespace DM {

class VideoFootage : public Footage {
public:
    static std::shared_ptr<Footage> createVideoFootageByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

    ~VideoFootage();

    void flush(DMTime t) override;

    void updateResources(const std::string &path) override;

    DMTime getResStartTime() const;

    DMTime getResEndTime() const;

protected:
    VideoFootage(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

private:
    // 资源截取 开始结束时间
    DMTime mResStartTime = 0;
    DMTime mResEndTime = 0;
    // 图层
    std::shared_ptr<ImageLayer> mImageLayer = nullptr;
    // 内容
    float mFrameRate = 25.0f;
    // 缓存一帧
    DMFrame mLastFrame = -1;
    cv::VideoCapture mVideoCap;
    cv::Mat matCache;
    // srt
    nlohmann::json mLayerTransform;
};

} // namespace DM

#endif // VIDEOFOOTAGE_H