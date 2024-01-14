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
    int nChannle = 4;
};

struct GLFrameBuffer {
    GLTexture tex;
    unsigned int fboId = 0;
    int width = 0;
    int height = 0;

    bool isValid() const {
        return width != 0 && height != 0;
    }
};

// 2.继承相关类
class GLUtils : public QOpenGLFunctions_3_3_Core {
public:
    static std::shared_ptr<GLUtils> Get();

    GLTexture loadTexture(const std::string &path);

    GLTexture createTexture(uint8_t *data, const int &wid, const int &hei,
                            const int &nChannle);

    void bindDataToTexture(uint8_t *data, GLTexture &texture);

    void deleteTexture(GLTexture &texture);

    void cleanColor(const GLColor &clr);

    void renderQuadInternal(const Rect &boundBox);

    void setViewPort(const Rect &vp);

    GLFrameBuffer createFrameBuffer(const int &wid, const int &hei, const int &nChannle);

    void deleteFrameBuffer(GLFrameBuffer &fbo);

    void bindFrameBuffer(const GLFrameBuffer &fbo);

    void bindFrameBuffer(const unsigned int &fboid);

    void readPixels(const GLFrameBuffer &fbo, unsigned char *dstData);

    int getBindFboId();

    Rect getCurrentViewPort();

    ~GLUtils();

protected:
    GLUtils();
    GLUtils(const GLUtils &) = delete;
    const GLUtils &operator=(const GLUtils &) = delete;

private:
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;
    unsigned int quadVAOSync = 0;
    unsigned int quadVBOSync = 0;
};

} // namespace DM

#endif // GLUTILS_H