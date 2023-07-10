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

protected:
    CompositionLayer(const nlohmann::json &obj, RootNode &rootNode);

private:
    std::vector<std::shared_ptr<Layer>> mLayers;
};

} // namespace DM
#endif // COMPOSITIONLAYER_H