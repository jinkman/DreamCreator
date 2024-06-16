#include "RootNode.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include <limits>

namespace DM {

static const char IMAGE_FRAG_SHADER[] = {
#include "shaders/ImageLayer.frag"
};

static const char IMAGE_VERT_SHADER[] = {
#include "shaders/ImageLayer.vert"
};

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
    std::shared_ptr<RootNode> rootNode(new RootNode(obj));
    // rootNode->setRootPath(filePath.parent_path());
    return rootNode;
}

std::shared_ptr<RootNode> RootNode::creatRootNodeByJson(const nlohmann::json &obj) {
    std::shared_ptr<RootNode> rootNode(new RootNode(obj));
    return rootNode;
}

RootNode::~RootNode() {
    mRootCom->releaseResources();
}

RootNode::RootNode(const nlohmann::json &obj) {
    // 解析
    mWidth = obj["width"].get<unsigned int>();
    mHeight = obj["height"].get<unsigned int>();
    if (obj.contains("backgroundColor")) {
        mBgColor = GLColor::deserialization(obj["backgroundColor"]);
    }
    mRootCom = CompositionLayer::creatCompositionLayerByJson(obj, *this);
    initialize();
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

Shader *RootNode::getImageShader() {
    return mImageShader.get();
}

CompositionLayer* RootNode::getRootComposition() {
    return mRootCom.get();
}

void RootNode::setRootPath(const std::filesystem::path &path) {
    mRelativePath = path;
}

std::filesystem::path RootNode::getRootPath() const {
    return mRelativePath;
}

void RootNode::preDraw() {
    mImageShader->use();
    mImageShader->setBool("bFlipY", false);
    mImageShader->setMat4("projection", glm::ortho(0.0f, float(width()), 0.0f, float(height()), std::numeric_limits<float>::min(), std::numeric_limits<float>::max()));
    mImageShader->setMat4("model", glm::mat4(1.0f));
    mImageShader->unUse();
    GLUtils::Get()->setViewPort(Rect::MakeWH(float(width()), float(height())));
    // 设置窗口颜色;
    GLUtils::Get()->cleanColor(mBgColor);
    mRootCom->preDraw();
}

void RootNode::postDraw() {
    mRootCom->postDraw();
}

void RootNode::initialize() {
    GLUtils::Get();
    mImageShader = std::make_shared<Shader>(IMAGE_VERT_SHADER, IMAGE_FRAG_SHADER);
}

} // namespace DM