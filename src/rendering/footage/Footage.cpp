#include "Footage.h"
#include "VideoFootage.h"

namespace DM {

std::shared_ptr<Footage> Footage::createFootageByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) {
    std::shared_ptr<Footage> footage = nullptr;
    std::string footageType = obj["footageType"].get<std::string>();
    if (footageType == "mp4") {
        footage = VideoFootage::createVideoFootageByJson(obj, rtNode);
    }
    footage->updateResources(obj["resourcesPath"].get<std::string>());
    return footage;
}

Footage::~Footage() {
}

Footage::Footage(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) {
    mRootNode = rtNode;
    mStartTime = obj["startTime"].get<DMTime>();
    mEndTime = obj["endTime"].get<DMTime>();
}

DMTime Footage::startTime() const {
    return mStartTime;
}

DMTime Footage::endTime() const {
    return mEndTime;
}

DMTime Footage::duration() const {
    return mEndTime - mStartTime;
}

void Footage::updateStartTime(DMTime t) {
    mStartTime = t;
}

void Footage::updateEndTime(DMTime t) {
    mEndTime = t;
}

} // namespace DM