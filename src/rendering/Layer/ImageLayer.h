#ifndef IMAGELAYER_H
#define IMAGELAYER_H

#include "Layer.h"

namespace DM {
class ImageLayer : public Layer {
public:
    static std::shared_ptr<ImageLayer> creatImageLayerByJson(const nlohmann::json &obj, RootNode &rootNode);

    ~ImageLayer();

    virtual void drawInternal() override;

    virtual void preDraw() override;

    virtual void postDraw() override;

    void updateImagePath(const std::string &path);

protected:
    ImageLayer(const nlohmann::json &obj, RootNode &rootNode);

    std::string mImagePath = "";

    GLTexture texture;
};

} // namespace DM

#endif // IMAGELAYER_H