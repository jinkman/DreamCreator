#ifndef ROOTNODE_H
#define ROOTNODE_H

#include "CompositionLayer.h"

namespace DM {
class RootNode {
public:
    static std::shared_ptr<RootNode> creatRootNodeByFile(const QString &jstr);

    ~RootNode();

    void draw();

    unsigned int width() const;

    unsigned int height() const;

    std::string getRealFilePath(const std::string &path);

    std::shared_ptr<Shader> getImageShader();

protected:
    RootNode(const nlohmann::json &obj,const std::string & path);

private:
    void preDraw();

    void postDraw();

    void initShaders();

private:
    unsigned int mWidth = 0;
    unsigned int mHeight = 0;
    GLColor mBgColor = {1.0f, 1.0f, 1.0f, 1.0f};
    std::filesystem::path mRelativePath = "";
    // 着色器
    std::shared_ptr<Shader> mImageShader = nullptr;
    // root
    std::shared_ptr<CompositionLayer> mRootCom = nullptr;
};

} // namespace DM

#endif // IMAGELAYER_H