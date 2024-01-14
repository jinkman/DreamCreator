#include "GLUtils.h"
#include "common/stb_image.h"
#include <mutex>
#include "LogUtils.h"

namespace DM {

static std::shared_ptr<GLUtils> singleton = nullptr;
static std::mutex singletonMutex;

GLUtils::GLUtils() {
    initializeOpenGLFunctions();
}

GLUtils::~GLUtils() {
}

std::shared_ptr<GLUtils> GLUtils::Get() {
    if (singleton == nullptr) {
        std::unique_lock<std::mutex> lock(singletonMutex);
        if (singleton == nullptr) {
            std::shared_ptr<GLUtils> temp(new GLUtils());
            singleton = temp;
        }
    }
    singleton->initializeOpenGLFunctions();
    return singleton;
}

GLTexture GLUtils::loadTexture(const std::string &path) {
    stbi_set_flip_vertically_on_load(true);
    GLTexture texture;
    glGenTextures(1, &texture.id);
    unsigned char *data = stbi_load(path.c_str(), &texture.width, &texture.height, &texture.nChannle, 0);
    if (data) {
        if (texture.nChannle == 1)
            texture.format = GL_RED;
        else if (texture.nChannle == 3)
            texture.format = GL_RGB;
        else if (texture.nChannle == 4)
            texture.format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, texture.format, texture.width, texture.height, 0, texture.format, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        texture.id = 0;
        stbi_image_free(data);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

GLTexture GLUtils::createTexture(uint8_t *data, const int &wid, const int &hei,
                                 const int &nChannle) {
    GLTexture texture;
    glGenTextures(1, &texture.id);
    texture.width = wid;
    texture.height = hei;
    texture.nChannle = nChannle;
    if (nChannle == 1)
        texture.format = GL_RED;
    else if (nChannle == 3)
        texture.format = GL_RGB;
    else if (nChannle == 4)
        texture.format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, texture.format, texture.width, texture.height, 0, texture.format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

void GLUtils::bindDataToTexture(uint8_t *data, GLTexture &texture) {
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, texture.format, texture.width, texture.height, 0, texture.format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLUtils::deleteTexture(GLTexture &texture) {
    glDeleteTextures(1, &texture.id);
    // reset
    texture = {0, 0, 0, GL_RGBA};
}

void GLUtils::cleanColor(const GLColor &clr) {
    glClearColor(clr.r, clr.g, clr.b, clr.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLUtils::renderQuadInternal(const Rect &boundBox) {
    float quadVertices[] = {
        boundBox.left, boundBox.bottom, 0.0f, 1.0f, boundBox.left, boundBox.top, 0.0f, 0.0f,
        boundBox.right, boundBox.top, 1.0f, 0.0f, boundBox.left, boundBox.bottom, 0.0f, 1.0f,
        boundBox.right, boundBox.top, 1.0f, 0.0f, boundBox.right, boundBox.bottom, 1.0f, 1.0f};

    if (quadVAO == 0) {
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_DYNAMIC_DRAW);
    }
    glBindVertexArray(quadVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void GLUtils::setViewPort(const Rect &vp) {
    glViewport(vp.left, vp.top, vp.width(), vp.height());
}

GLFrameBuffer GLUtils::createFrameBuffer(const int &wid, const int &hei, const int &nChannle) {
    GLFrameBuffer fbo;
    fbo.width = wid;
    fbo.height = hei;
    glGenFramebuffers(1, &fbo.fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo.fboId);
    fbo.tex = createTexture(nullptr, wid, hei, nChannle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo.tex.id, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        DMLOG << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return fbo;
}

void GLUtils::deleteFrameBuffer(GLFrameBuffer &fbo) {
    deleteTexture(fbo.tex);
    glDeleteFramebuffers(1, &(fbo.fboId));
    fbo.width = 0;
    fbo.height = 0;
    fbo.fboId = -1;
}

void GLUtils::bindFrameBuffer(const GLFrameBuffer &fbo) {
    bindFrameBuffer(fbo.fboId);
}

void GLUtils::bindFrameBuffer(const unsigned int &fboid) {
    glBindFramebuffer(GL_FRAMEBUFFER, fboid);
}

void GLUtils::readPixels(const GLFrameBuffer &fbo, unsigned char *dstData) {
    bindFrameBuffer(fbo);
    glReadPixels(0, 0, fbo.width, fbo.height, fbo.tex.format, GL_UNSIGNED_BYTE, dstData);
}

int GLUtils::getBindFboId() {
    int fbo = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
    return fbo;
}

Rect GLUtils::getCurrentViewPort() {
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    return Rect::MakeXYWH(float(viewport[0]), float(viewport[1]), float(viewport[2]), float(viewport[3]));
}

} // namespace DM