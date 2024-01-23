#include "ImageFootage.h"
#include "rendering/layer/ImageLayer.h"
#include "common/stb_image.h"

namespace DM {

std::shared_ptr<Footage> ImageFootage::createImageFootageByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) {
    std::shared_ptr<ImageFootage> imageFootage(new ImageFootage(obj, rtNode));
    return imageFootage;
}

ImageFootage::ImageFootage(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) :
    Footage(obj, rtNode) {
    // 解析
    mFootageType = EFootageType::EIMAGE_FOOTAGE;
    mLayerJson["type"] = "image";
}

ImageFootage::~ImageFootage() {
}

void ImageFootage::flush(DMTime t) {
    if (t < startTime() || t >= endTime()) { // 不可见
        mLayer->visible() = false;
        return;
    }

    mLayer->visible() = true;
}

void ImageFootage::updateResources(const std::string &path) {
    Footage::updateResources(path);

    std::string localPath = getResourcesLocalPath();
    // 创建图层
    if (mLayer == nullptr) {
        mLayer = ImageLayer::creatImageLayerByJson(mLayerJson, *mRootNode);
        mRootNode->getRootComposition()->addLayer(mLayer);
    }

    stbi_set_flip_vertically_on_load(false);
    int width = 0;
    int height = 0;
    int channle = 0;
    unsigned char *data = stbi_load(localPath.c_str(), &width, &height, &channle, 0);
    if (data) {
        std::static_pointer_cast<ImageLayer>(mLayer)->updateImageData(data, width, height, channle);
    }
    stbi_image_free(data);
}

} // namespace DM