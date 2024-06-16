#ifndef PAGLAYER_H
#define PAGLAYER_H

#include "Layer.h"
#include <pag/pag.h>

namespace DM {
class PAGLayer : public Layer {
public:
    static std::shared_ptr<PAGLayer> creatPAGLayerByJson(const nlohmann::json &obj, RootNode &rootNode);

    ~PAGLayer();

    virtual void drawInternal() override;

    virtual void preDraw() override;

    virtual void postDraw() override;

    void updatePagFile(std::shared_ptr<pag::PAGFile> pagFile);

    pag::PAGPlayer *getPAGPlayer();

    pag::PAGFile *getPAGFile();

protected:
    PAGLayer(const nlohmann::json &obj, RootNode &rootNode);

    virtual void releaseResources() override;

    std::string mImagePath = "";

    GLTexture texture;

    std::shared_ptr<pag::PAGPlayer> mPlayer = nullptr;

    std::shared_ptr<pag::PAGFile> mPagFile = nullptr;

    std::vector<unsigned char> mImageData;
};

} // namespace DM

#endif // PAGLAYER_H