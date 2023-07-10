#ifndef COMPOSITIONLAYER_H
#define COMPOSITIONLAYER_H

#include "Layer.h"
#include <vector>

namespace DM {
class CompositionLayer : public Layer {
public:
    static std::shared_ptr<CompositionLayer> creatCompositionLayerByJson(const nlohmann::json &obj, RootNode &rootNode);

    ~CompositionLayer();

    virtual void drawInternal() override;

    virtual void preDraw() override;

    virtual void postDraw() override;

    // 图层管理
    void addLayer(std::shared_ptr<Layer> ly);

    void addLayerAtIndex(std::shared_ptr<Layer> ly, int index);

    int getLayerIndex(std::shared_ptr<Layer> ly);

    std::shared_ptr<Layer> getLayerAtIndex(int index);

    void removeLayer(std::shared_ptr<Layer> ly);

    void removeLayerAtIndex(int index);

    void removeAllLayer();

    int numberChildren() const;

protected:
    CompositionLayer(const nlohmann::json &obj, RootNode &rootNode);

private:
    std::vector<std::shared_ptr<Layer>> mLayers;
};

} // namespace DM
#endif // COMPOSITIONLAYER_H