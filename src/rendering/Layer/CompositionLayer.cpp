#include "CompositionLayer.h"
#include "ImageLayer.h"

namespace DM {
std::shared_ptr<CompositionLayer> CompositionLayer::creatCompositionLayerByJson(const nlohmann::json &obj, RootNode &rootNode) {
    std::shared_ptr<CompositionLayer> com(new CompositionLayer(obj, rootNode));
    return com;
}

CompositionLayer::~CompositionLayer() {
}

CompositionLayer::CompositionLayer(const nlohmann::json &obj, RootNode &rootNode) :
    Layer(obj, rootNode) {
    mType = ELayerType::ECOMPOSITION_LAYER;
    if (obj.contains("children")) {
        for (auto &child : obj["children"]) {
            auto type = child["type"].get<std::string>();
            if (type == "image") {
                mLayers.emplace_back(ImageLayer::creatImageLayerByJson(child, rootNode));
            } else if (type == "composition") {
                mLayers.emplace_back(CompositionLayer::creatCompositionLayerByJson(child, rootNode));
            }
        }
    }
}

void CompositionLayer::drawInternal() {
    for (auto &ly : mLayers) {
        ly->preDraw();
        if (ly->visible()) {
            ly->drawInternal();
        }
        ly->postDraw();
    }
}

void CompositionLayer::preDraw() {
    Layer::preDraw();
}

void CompositionLayer::postDraw() {
    Layer::postDraw();
}

// 图层管理
void CompositionLayer::addLayer(std::shared_ptr<Layer> ly) {
    mLayers.push_back(ly);
}

void CompositionLayer::addLayerAtIndex(std::shared_ptr<Layer> ly, int index) {
    if (index < 0 || index >= numberChildren()) {
        addLayer(ly);
    }
    mLayers.insert(mLayers.begin() + index, ly);
}

int CompositionLayer::getLayerIndex(std::shared_ptr<Layer> ly) {
    int index = -1;
    for (auto &layer : mLayers) {
        index++;
        if (layer == ly) {
            break;
        }
    }
    return index;
}

std::shared_ptr<Layer> CompositionLayer::getLayerAtIndex(int index) {
    if (index < 0 || index >= numberChildren()) {
        return nullptr;
    }
    return mLayers.at(index);
}

void CompositionLayer::removeLayer(std::shared_ptr<Layer> ly) {
    auto pos = std::find(mLayers.begin(), mLayers.end(), ly);
    if (pos != mLayers.end()) {
        mLayers.erase(pos);
    }
}

void CompositionLayer::removeLayerAtIndex(int index) {
    if (index < 0 || index >= numberChildren()) {
        return;
    }
    mLayers.erase(mLayers.begin() + index);
}

void CompositionLayer::removeAllLayer() {
    mLayers.clear();
}

int CompositionLayer::numberChildren() const {
    return static_cast<int>(mLayers.size());
}

} // namespace DM