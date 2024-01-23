#include "Footage.h"
#include "VideoFootage.h"
#include "PAGFootage.h"
#include "ImageFootage.h"

namespace DM {

std::shared_ptr<Footage> Footage::createFootageByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) {
    std::shared_ptr<Footage> footage = nullptr;
    std::string footageType = obj["footageType"].get<std::string>();
    if (footageType == "video") {
        footage = VideoFootage::createVideoFootageByJson(obj, rtNode);
    } else if (footageType == "pag") {
        footage = PAGFootage::createPAGFootageByJson(obj, rtNode);
    } else if (footageType == "image") {
        footage = ImageFootage::createImageFootageByJson(obj, rtNode);
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
    mLayerJson = obj;
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

void Footage::updateResources(const std::string &path) {
    mLocalPath = mRootNode == nullptr ? path : mRootNode->getRealFilePath(path);
    mOriginPath = path;
}

std::string Footage::getResourcesOriginPath() const {
    return mOriginPath;
}

std::string Footage::getResourcesLocalPath() const {
    return mLocalPath;
}

EFootageType Footage::getFootageType() const {
    return mFootageType;
}

} // namespace DM