#ifndef GLUTILS_H
#define GLUTILS_H

#include <QOpenGLFunctions_3_3_Core>
#include "Types.h"

namespace DM {

struct GLTexture {
    operator bool() const {
        return id != 0 && width != 0 && height != 0;
    }
    unsigned int id = 0;
    int width = 0;
    int height = 0;
    GLenum format = GL_RGBA;
};

// 2.继承相关类
class GLUtils : public QOpenGLFunctions_3_3_Core {
public:
    static std::shared_ptr<GLUtils> Get();

    GLTexture createTexture(const std::string &path);

    void deleteTexture(GLTexture &texture);

    void cleanColor(const unsigned int &fbo, const GLColor &clr);

    void renderQuadInternal(const Rect &boundBox);

    void setViewPort(const Rect &vp);

    ~GLUtils();

protected:
    GLUtils();
    GLUtils(const GLUtils &) = delete;
    const GLUtils &operator=(const GLUtils &) = delete;

private:
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;
};

} // namespace DM

#endif // GLUTILS_H