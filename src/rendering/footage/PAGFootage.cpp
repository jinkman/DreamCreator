#include "PAGFootage.h"
#include "rendering/layer/PAGLayer.h"
#include "utils/LogUtils.h"

namespace DM {

std::shared_ptr<Footage> PAGFootage::createPAGFootageByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) {
    std::shared_ptr<PAGFootage> pagFootage(new PAGFootage(obj, rtNode));
    return pagFootage;
}

PAGFootage::PAGFootage(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) :
    Footage(obj, rtNode) {
    // 解析
    mFootageType = EFootageType::EPAG_FOOTAGE;
    mResStartTime = obj["resStartTime"].get<DMTime>();
    mResEndTime = obj["resEndTime"].get<DMTime>();
}

PAGFootage::~PAGFootage() {
}

void PAGFootage::flush(DMTime t) {
    if (t < startTime() || t >= endTime()) { // 不可见
        mLayer->visible() = false;
        return;
    }
    std::static_pointer_cast<PAGLayer>(mLayer)->updatePagFile(nullptr);

    mLayer->visible() = true;
    double ratio = double(t - startTime()) / double(duration());
    DMTime interceptTime = mResStartTime + DMTime((mResEndTime - mResStartTime) * ratio);
    auto pagPlayer = std::static_pointer_cast<PAGLayer>(mLayer)->getPlayer();
    double progress = double(1000 * interceptTime) / double(pagPlayer->getComposition()->duration());
    DMLOG << progress;
    pagPlayer->setProgress(progress);
    pag::BackendSemaphore bs;
    pagPlayer->flushAndSignalSemaphore(&bs);
    pagPlayer->wait(bs);
}

void PAGFootage::updateResources(const std::string &path) {
    Footage::updateResources(path);
    // 创建图层
    if (mLayer == nullptr) {
        nlohmann::json layerJson = nlohmann::json::object();
        layerJson["type"] = "image";
        layerJson["scaleMode"] = 2;
        if (!mLayerTransform.is_null()) {
            layerJson["transform"] = mLayerTransform;
        }
        mLayer = PAGLayer::creatPAGLayerByJson(layerJson, *mRootNode);
        mRootNode->getRootComposition()->addLayer(mLayer);
    }
    std::string localPath = getResourcesLocalPath();
    std::static_pointer_cast<PAGLayer>(mLayer)->updatePagFile(pag::PAGFile::Load(localPath));
}

DMTime PAGFootage::getResStartTime() const {
    return mResStartTime;
}

DMTime PAGFootage::getResEndTime() const {
    return mResEndTime;
}

} // namespace DM