#include "Layer.h"
#include "RootNode.h"

namespace DM {

Layer::~Layer() {
}

Layer::Layer(const nlohmann::json &obj, RootNode &rootNode) :
    mRootNode(rootNode) {
    // 解析
    if (obj.contains("boundBox")) {
        auto boxObj = obj["boundBox"];
        mBoundBox = Rect::MakeLTRB(boxObj[0].get<float>(), boxObj[1].get<float>(), boxObj[2].get<float>(), boxObj[3].get<float>());
    }
    // 缩放模式
    if (obj.contains("scaleMode")) {
        mScaleMode = obj["scaleMode"].get<int>();
    }
    // transform
    if (obj.contains("transform")) {
        auto transformObj = obj["transform"];
        // 缩放
        mScale.x = transformObj["scale"][0].get<float>();
        mScale.y = transformObj["scale"][1].get<float>();
        mScale.z = transformObj["scale"][2].get<float>();
        // 旋转
        mOrientation = glm::angleAxis(glm::radians(transformObj["orientation"][0].get<float>()), glm::vec3(transformObj["orientation"][1].get<float>(), transformObj["orientation"][2].get<float>(), transformObj["orientation"][3].get<float>()));
        // 位移
        mTranslate.x = transformObj["translate"][0].get<float>();
        mTranslate.y = transformObj["translate"][1].get<float>();
        mTranslate.z = transformObj["translate"][2].get<float>();
    }
}

void Layer::preDraw() {
}

void Layer::postDraw() {
}

glm::mat4 Layer::getLayerMatrix() {
    glm::mat4 matrix = glm::mat4(1.0f);
    // 绕锚点旋转缩放
    glm::vec3 centerPt = glm::vec3(getBoundBox().centerX(), getBoundBox().centerY(), 0.0f);
    matrix = glm::translate(glm::mat4(1.0f), -centerPt) * matrix;
    matrix = glm::scale(glm::mat4(1.0f), scale()) * matrix;
    matrix = glm::mat4_cast(orientation()) * matrix;
    matrix = glm::translate(glm::mat4(1.0f), centerPt) * matrix;
    // 缩放模式
    matrix = Layer::ApplyScaleMode(mScaleMode, getBoundBox().width(), getBoundBox().height(), getRootNode().width(), getRootNode().height()) * matrix;
    // 位移
    matrix = glm::translate(glm::mat4(1.0f), translate()) * matrix;
    return matrix;
}

glm::vec3 &Layer::scale() {
    return mScale;
}

glm::quat &Layer::orientation() {
    return mOrientation;
}

glm::vec3 &Layer::translate() {
    return mTranslate;
}

bool &Layer::visible() {
    return bVisible;
}

ELayerType Layer::getLayerType() {
    return mType;
}

Rect Layer::getBoundBox() const {
    return mBoundBox;
}

void Layer::updateBoundBox(const Rect &rc) {
    mBoundBox = rc;
}

RootNode &Layer::getRootNode() {
    return mRootNode;
}

glm::mat4 Layer::ApplyScaleMode(int scaleMode, int sourceWidth, int sourceHeight,
                                int targetWidth, int targetHeight) {
    glm::mat4 matrix(1.0f);
    if (scaleMode == DMScaleMode::NoneValue || sourceWidth <= 0 || sourceHeight <= 0 || targetWidth <= 0 || targetHeight <= 0) {
        return matrix;
    }
    auto scaleX = targetWidth * 1.0 / sourceWidth;
    auto scaleY = targetHeight * 1.0 / sourceHeight;
    switch (scaleMode) {
    case DMScaleMode::Stretch: {
        matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, 1.0f)) * matrix;
    } break;
    case DMScaleMode::Zoom: {
        auto scale = std::max(scaleX, scaleY);
        matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f)) * matrix;
        if (scaleX > scaleY) {
            matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, (targetHeight - sourceHeight * scale) * 0.5f, 0.0f)) * matrix;
        } else {
            matrix = glm::translate(glm::mat4(1.0f), glm::vec3((targetWidth - sourceWidth * scale) * 0.5f, 0.0f, 0.0f)) * matrix;
        }
    } break;
    default: {
        auto scale = std::min(scaleX, scaleY);
        matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f)) * matrix;
        if (scaleX < scaleY) {
            matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, (targetHeight - sourceHeight * scale) * 0.5f, 0.0f)) * matrix;
        } else {
            matrix = glm::translate(glm::mat4(1.0f), glm::vec3((targetWidth - sourceWidth * scale) * 0.5f, 0.0f, 0.0f)) * matrix;
        }
    } break;
    }
    return matrix;
}

void Layer::releaseResources() {
}

} // namespace DM
