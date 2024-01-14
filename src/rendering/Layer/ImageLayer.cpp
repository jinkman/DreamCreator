#include "ImageLayer.h"
#include "RootNode.h"

namespace DM {
std::shared_ptr<ImageLayer> ImageLayer::creatImageLayerByJson(const nlohmann::json &obj, RootNode &rootNode) {
    std::shared_ptr<ImageLayer> image(new ImageLayer(obj, rootNode));
    return image;
}

ImageLayer::~ImageLayer() {
    GLUtils::Get()->deleteTexture(texture);
}

void ImageLayer::drawInternal() {
    // 绘制包围盒
    GLUtils::Get()->renderQuadInternal(getBoundBox());
}

void ImageLayer::preDraw() {
    auto imageShader = getRootNode().getImageShader();
    imageShader->use();
    imageShader->setTexture("tex", 0, texture.id);
    imageShader->setMat4("model", getLayerMatrix());
}

void ImageLayer::postDraw() {
    getRootNode().getImageShader()->unUse();
}

ImageLayer::ImageLayer(const nlohmann::json &obj, RootNode &rootNode) :
    Layer(obj, rootNode) {
    mType = ELayerType::EIMAGE_LAYER;
    if (obj.contains("path")) {
        updateImagePath(obj["path"].get<std::string>());
    }
}

void ImageLayer::updateImagePath(const std::string &path) {
    mImagePath = path;
    if (!texture) {
        auto realPath = getRootNode().getRealFilePath(mImagePath);
        texture = GLUtils::Get()->loadTexture(realPath);
    }
    updateBoundBox(Rect::MakeWH(texture.width, texture.height));
}

void ImageLayer::updateImageData(uint8_t *data, const int &wid, const int &hei,
                                 const int &nChannle) {
    if (!texture) {
        texture = GLUtils::Get()->createTexture(data, wid, hei, nChannle);
    } else if (texture.width != wid || texture.height != hei || texture.nChannle != nChannle) {
        GLUtils::Get()->deleteTexture(texture);
        texture = GLUtils::Get()->createTexture(data, wid, hei, nChannle);
    } else {
        GLUtils::Get()->bindDataToTexture(data, texture);
    }
    updateBoundBox(Rect::MakeWH(texture.width, texture.height));
}

void ImageLayer::releaseResources() {
    Layer::releaseResources();
    if (texture) {
        GLUtils::Get()->deleteTexture(texture);
    }
}

} // namespace DM