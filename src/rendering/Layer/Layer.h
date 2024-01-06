#ifndef LAYER_H
#define LAYER_H

#include "utils/GLUtils.h"
#include "common/Shader.h"
#include "utils/Types.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DM {

enum class ELayerType {
    EUNKNOW,
    EROOT_NODE,
    ECOMPOSITION_LAYER,
    EIMAGE_LAYER
};

class RootNode;
class Layer {
public:
    virtual ~Layer();

    virtual void drawInternal() = 0;

    virtual void preDraw();

    virtual void postDraw();

    glm::mat4 getLayerMatrix();

    glm::vec3 &scale();

    glm::quat &orientation();

    glm::vec3 &translate();

    bool &visible();

    ELayerType getLayerType();

    Rect getBoundBox() const;

    void updateBoundBox(const Rect &rc);

    RootNode &getRootNode();

protected:
    Layer(const nlohmann::json &obj, RootNode &rootNode);

    glm::mat4 ApplyScaleMode(int scaleMode, int sourceWidth, int sourceHeight,
                             int targetWidth, int targetHeight);

    ELayerType mType = ELayerType::EUNKNOW;

private:
    glm::vec3 mScale = glm::vec3(1.0f);
    glm::quat mOrientation = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
    glm::vec3 mTranslate = glm::vec3(0.0f);

    Rect mBoundBox = Rect::MakeEmpty();

    int mScaleMode = DMScaleMode::LetterBox;

    bool bVisible = true;

    RootNode &mRootNode;
};

} // namespace DM

#endif // LAYER_H