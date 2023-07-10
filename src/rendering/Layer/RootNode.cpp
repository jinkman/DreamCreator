#include "RootNode.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include "common/Common.h"

namespace DM {

std::shared_ptr<RootNode> RootNode::creatRootNodeByFile(const QString &jstr) {
    std::filesystem::path filePath(jstr.toStdString());
    std::ifstream ifs(filePath.c_str());
    if (!ifs.is_open()) {
        return nullptr;
    }
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();
    auto obj = nlohmann::json::parse(ss.str(), nullptr, false);
    auto box_array = nlohmann::json::array();
    box_array.push_back(0);
    box_array.push_back(0);
    box_array.push_back(obj["width"].get<int>());
    box_array.push_back(obj["height"].get<int>());
    obj["boundBox"] = box_array;
    std::shared_ptr<RootNode> rootNode(new RootNode(obj, filePath.parent_path()));
    return rootNode;
}

RootNode::~RootNode() {
}

RootNode::RootNode(const nlohmann::json &obj, const std::string &path) :
    mRelativePath(path) {
    // 解析
    mWidth = obj["width"].get<unsigned int>();
    mHeight = obj["height"].get<unsigned int>();
    if (obj.contains("backGroundClr")) {
        mBgColor = GLColor::Deserialization(obj["backGroundClr"]);
    }
    // 初始化着色器
    initShaders();
    mRootCom = CompositionLayer::creatCompositionLayerByJson(obj, *this);
}

void RootNode::draw() {
    preDraw();
    if (mRootCom->visible()) {
        mRootCom->drawInternal();
    }
    postDraw();
}

unsigned int RootNode::width() const {
    return mWidth;
}

unsigned int RootNode::height() const {
    return mHeight;
}

std::string RootNode::getRealFilePath(const std::string &path) {
    std::filesystem::path filePath(path);
    auto realPath = filePath.is_relative() ? mRelativePath / filePath : filePath;
    return realPath.make_preferred().string();
}

std::shared_ptr<Shader> RootNode::getImageShader() {
    return mImageShader;
}

void RootNode::preDraw() {
    // 设置窗口颜色
    GLUtils::Get()->cleanColor(0, mBgColor);
    mRootCom->preDraw();
}

void RootNode::postDraw() {
    mRootCom->postDraw();
}

void RootNode::initShaders() {
    if (mImageShader == nullptr) {
        mImageShader = std::make_shared<Shader>(getLocalPath("shaders/ImageLayer.vert").c_str(), getLocalPath("shaders/ImageLayer.frag").c_str());
        mImageShader->use();
        mImageShader->setMat4("projection", glm::ortho(0.0f, float(width()), 0.0f, float(height()), -100.0f, 100.0f));
        mImageShader->setMat4("model", glm::mat4(1.0f));
        mImageShader->unUse();
    }
}

} // namespace DM