#include "PAGLayer.h"
#include "RootNode.h"

namespace DM {
std::shared_ptr<PAGLayer> PAGLayer::creatPAGLayerByJson(const nlohmann::json &obj, RootNode &rootNode) {
    std::shared_ptr<PAGLayer> pag(new PAGLayer(obj, rootNode));
    return pag;
}

PAGLayer::~PAGLayer() {
    GLUtils::Get()->deleteTexture(texture);
}

void PAGLayer::drawInternal() {
    // 绘制包围盒
    GLUtils::Get()->renderQuadInternal(getBoundBox());
}

void PAGLayer::preDraw() {
    // 读取数据
    auto surface = mPlayer->getSurface();
    size_t byteSize = surface->width() * surface->height() * 4;
    size_t byteWidth = surface->width() * 4;
    if (mImageData.size() != byteSize) {
        mImageData.resize(byteSize);
    }
    // 读
    surface->readPixels(pag::ColorType::RGBA_8888, pag::AlphaType::Premultiplied, mImageData.data(), byteWidth);
    // 写
    GLUtils::Get()->bindDataToTexture(mImageData.data(), texture);
    auto imageShader = getRootNode().getImageShader();
    imageShader->use();
    imageShader->setTexture("tex", 0, texture.id);
    imageShader->setMat4("model", getLayerMatrix());
}

void PAGLayer::postDraw() {
    getRootNode().getImageShader()->unUse();
}

PAGLayer::PAGLayer(const nlohmann::json &obj, RootNode &rootNode) :
    Layer(obj, rootNode) {
    mType = ELayerType::EPAG_LAYER;
    mPlayer = std::make_shared<pag::PAGPlayer>();
    if (obj.contains("path")) {
        // 创建pagFile
        std::string filePath = obj["path"].get<std::string>();
        updatePagFile(pag::PAGFile::Load(filePath));
    }
}

void PAGLayer::updatePagFile(std::shared_ptr<pag::PAGFile> pagFile) {
    if (pagFile == nullptr) {
        return;
    }
    mPagFile = pagFile;
    mPlayer->setComposition(pagFile);
    auto wid = mPlayer->getComposition()->width();
    auto hei = mPlayer->getComposition()->height();
    bool needUpdateSur = false;
    if (!texture) {
        texture = GLUtils::Get()->createTexture(nullptr, wid, hei, 4);
    } else if (texture.width != wid || texture.height != hei) {
        GLUtils::Get()->deleteTexture(texture);
        texture = GLUtils::Get()->createTexture(nullptr, wid, hei, 4);
        needUpdateSur = true;
    }
    // 是否需要创建surface
    std::shared_ptr<pag::PAGSurface> sur = mPlayer->getSurface();
    if (needUpdateSur || sur == nullptr) {
        sur = pag::PAGSurface::MakeOffscreen(texture.width, texture.height);
        // pag::GLTextureInfo pagTexInfo;
        // pagTexInfo.id = texture.id;
        // sur = pag::PAGSurface::MakeFrom({pagTexInfo, texture.width, texture.height}, pag::ImageOrigin::TopLeft, true);
        mPlayer->setSurface(sur);
    }
    updateBoundBox(Rect::MakeWH(texture.width, texture.height));
}

std::shared_ptr<pag::PAGPlayer> PAGLayer::getPAGPlayer() {
    return mPlayer;
}

std::shared_ptr<pag::PAGFile> PAGLayer::getPAGFile() {
    return mPagFile;
}

void PAGLayer::releaseResources() {
    Layer::releaseResources();
    if (texture) {
        GLUtils::Get()->deleteTexture(texture);
    }
}

} // namespace DM