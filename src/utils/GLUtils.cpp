#include "GLUtils.h"
#include "common/stb_image.h"
#include <mutex>

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
    return singleton;
}

GLTexture GLUtils::createTexture(const std::string &path) {
    stbi_set_flip_vertically_on_load(true);
    GLTexture texture;
    glGenTextures(1, &texture.id);
    int nrComponents = 0;
    unsigned char *data = stbi_load(path.c_str(), &texture.width, &texture.height, &nrComponents, 0);
    if (data) {
        if (nrComponents == 1)
            texture.format = GL_RED;
        else if (nrComponents == 3)
            texture.format = GL_RGB;
        else if (nrComponents == 4)
            texture.format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture.id);
        glTexImage2D(GL_TEXTURE_2D, 0, texture.format, texture.width, texture.height, 0, texture.format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        texture.id = 0;
        stbi_image_free(data);
    }

    return texture;
}

void GLUtils::deleteTexture(GLTexture &texture) {
    glDeleteTextures(1, &texture.id);
    // reset
    texture = {0, 0, 0, GL_RGBA};
}

void GLUtils::cleanColor(const unsigned int &fbo, const GLColor &clr) {
    GLint oldFbo = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(clr.r, clr.g, clr.b, clr.a);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
}

void GLUtils::renderQuadInternal(const Rect &boundBox) {
    float quadVertices[] = {
        boundBox.left, boundBox.bottom, 0.0f, 0.0f, 1.0f, boundBox.left, boundBox.top, 0.0f, 0.0f, 0.0f,
        boundBox.right, boundBox.top, 0.0f, 1.0f, 0.0f, boundBox.left, boundBox.bottom, 0.0f, 0.0f, 1.0f,
        boundBox.right, boundBox.top, 0.0f, 1.0f, 0.0f, boundBox.right, boundBox.bottom, 0.0f, 1.0f, 1.0f};

    if (quadVAO == 0) {
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_DYNAMIC_DRAW);
    }
    glBindVertexArray(quadVAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void GLUtils::setViewPort(const Rect &vp) {
    glViewport(vp.left, vp.top, vp.right, vp.bottom);
}

} // namespace DM