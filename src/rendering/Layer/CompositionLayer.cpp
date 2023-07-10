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
    for (auto &child : obj["children"]) {
        auto type = child["type"].get<std::string>();
        if (type == "image") {
            mLayers.emplace_back(ImageLayer::creatImageLayerByJson(child, rootNode));
        } else if (type == "composition") {
            mLayers.emplace_back(CompositionLayer::creatCompositionLayerByJson(child, rootNode));
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

} // namespace DM