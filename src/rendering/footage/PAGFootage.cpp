#include "PAGFootage.h"
#include "rendering/layer/PAGLayer.h"

namespace DM {

ReplaceSlice::~ReplaceSlice() {
}

// 序列化
void ReplaceSlice::loadFromJson(const nlohmann::json &jObject) {
    serialization<int>(jObject, editableIndex, "editableIndex");
    // 加载doc
    serialization<bool>(jObject, applyFill, "applyFill");
    serialization<bool>(jObject, applyStroke, "applyStroke");
    serialization<float>(jObject, baselineShift, "baselineShift");
    serialization<bool>(jObject, boxText, "boxText");
    serializationPt(jObject, boxTextPos, "boxTextPos");
    serializationPt(jObject, boxTextSize, "boxTextSize");
    serialization<float>(jObject, firstBaseLine, "firstBaseLine");
    serialization<bool>(jObject, fauxBold, "fauxBold");
    serialization<bool>(jObject, fauxItalic, "fauxItalic");
    serializationColor(jObject, fillColor, "fillColor");
    serialization<std::string>(jObject, fontFamily, "fontFamily");
    serialization<std::string>(jObject, fontStyle, "fontSpec");
    serialization<float>(jObject, fontSize, "fontSize");
    serializationColor(jObject, strokeColor, "strokeColor");
    serialization<bool>(jObject, strokeOverFill, "strokeOverFill");
    serialization<float>(jObject, strokeWidth, "strokeWidth");
    serialization<std::string>(jObject, text, "text");
    serialization<float>(jObject, leading, "leading");
    serialization<float>(jObject, tracking, "tracking");
    serializationColor(jObject, backgroundColor, "backgroundColor");
    serialization<uint8_t>(jObject, backgroundAlpha, "backgroundAlpha");
    serialization<pag::Enum>(jObject, direction, "direction");
    // 排版
    serialization<std::string>(jObject, mAlignType, "alignType");
}

// 替换doc
void ReplaceSlice::replaceDoc(std::shared_ptr<pag::TextDocument> textDoc) {
    deSerialization<bool>(applyFill, textDoc->applyFill);
    deSerialization<bool>(applyStroke, textDoc->applyStroke);
    deSerialization<float>(baselineShift, textDoc->baselineShift);
    deSerialization<bool>(boxText, textDoc->boxText);
    deSerialization<pag::Point>(boxTextPos, textDoc->boxTextPos);
    deSerialization<pag::Point>(boxTextSize, textDoc->boxTextSize);
    deSerialization<float>(firstBaseLine, textDoc->firstBaseLine);
    deSerialization<bool>(fauxBold, textDoc->fauxBold);
    deSerialization<bool>(fauxItalic, textDoc->fauxItalic);
    deSerialization<pag::Color>(fillColor, textDoc->fillColor);
    deSerialization<std::string>(fontFamily, textDoc->fontFamily);
    deSerialization<std::string>(fontStyle, textDoc->fontStyle);
    deSerialization<float>(fontSize, textDoc->fontSize);
    deSerialization<pag::Color>(strokeColor, textDoc->strokeColor);
    deSerialization<bool>(strokeOverFill, textDoc->strokeOverFill);
    deSerialization<float>(strokeWidth, textDoc->strokeWidth);
    deSerialization<std::string>(text, textDoc->text);
    deSerialization<float>(leading, textDoc->leading);
    deSerialization<float>(tracking, textDoc->tracking);
    deSerialization<pag::Color>(backgroundColor, textDoc->backgroundColor);
    deSerialization<uint8_t>(backgroundAlpha, textDoc->backgroundAlpha);
    deSerialization<pag::Enum>(direction, textDoc->direction);

    // 设置对齐方式
    if (mAlignType.has_value() && mAlignType.value() == "Center") {
        textDoc->justification = pag::ParagraphJustification::CenterJustify;
    } else if (mAlignType.has_value() && mAlignType.value() == "Left") {
        textDoc->justification = pag::ParagraphJustification::LeftJustify;
    } else if (mAlignType.has_value() && mAlignType.value() == "Right") {
        textDoc->justification = pag::ParagraphJustification::RightJustify;
    }
}

std::shared_ptr<Footage> PAGFootage::createPAGFootageByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) {
    std::shared_ptr<PAGFootage> pagFootage(new PAGFootage(obj, rtNode));
    return pagFootage;
}

PAGFootage::PAGFootage(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) :
    Footage(obj, rtNode) {
    // 解析
    mFootageType = EFootageType::EPAG_FOOTAGE;
    mResStartTime = obj["resStartTime"].get<DMTime>();
    mResEndTime = obj["resEndTime"].get<DMTime>();
    mPagType = obj.contains("pagType") && obj["pagType"].get<std::string>() == "subtitle" ? EPAGType::EPAG_SUBTITLE : EPAGType::EPAG_COMPOSITION;
    mLayerJson["type"] = "pag";
    mRepalceInfoList.clear();
    if (obj.contains("texts")) {
        for (auto &textJson : obj["texts"]) {
            std::shared_ptr<ReplaceSlice> repalceInfo = std::make_shared<ReplaceSlice>();
            repalceInfo->loadFromJson(textJson);
            mRepalceInfoList.emplace_back(repalceInfo);
        }
    }
}

PAGFootage::~PAGFootage() {
}

void PAGFootage::flush(DMTime t) {
    Footage::flush(t);
    if (!timelineVisible()) {
        return;
    }
    // 需要替换内容
    auto pagFile = getPAGFile();
    for (auto &replaceSlice : mRepalceInfoList) {
        int editableIndex = replaceSlice->editableIndex.has_value() ? replaceSlice->editableIndex.value() : 0;
        auto doc = pagFile->getTextData(editableIndex);
        if (doc == nullptr) {
            continue;
        }
        replaceSlice->replaceDoc(doc);
        pagFile->replaceText(editableIndex, doc);
    }

    double ratio = double(t - startTime()) / double(duration());
    DMTime interceptTime = mResStartTime + DMTime((mResEndTime - mResStartTime) * ratio);
    auto pagPlayer = std::static_pointer_cast<PAGLayer>(mLayer)->getPAGPlayer();
    double progress = double(1000 * interceptTime) / double(pagPlayer->getComposition()->duration());
    setPAGProgressAndFlush(progress);
}

void PAGFootage::updateResources(const std::string &path) {
    Footage::updateResources(path);
    // 创建图层
    if (mLayer == nullptr) {
        mLayer = PAGLayer::creatPAGLayerByJson(mLayerJson, *mRootNode);
        mRootNode->getRootComposition()->addLayer(mLayer);
    }
    std::string localPath = getResourcesLocalPath();
    std::static_pointer_cast<PAGLayer>(mLayer)->updatePagFile(pag::PAGFile::Load(localPath));
}

DMTime PAGFootage::getResStartTime() const {
    return mResStartTime;
}

DMTime PAGFootage::getResEndTime() const {
    return mResEndTime;
}

EPAGType PAGFootage::getPAGType() const {
    return mPagType;
}

double PAGFootage::getProgress() {
    auto pagPlayer = std::static_pointer_cast<PAGLayer>(mLayer)->getPAGPlayer();
    return pagPlayer->getProgress();
}

void PAGFootage::setPAGProgressAndFlush(double v) {
    auto pagPlayer = std::static_pointer_cast<PAGLayer>(mLayer)->getPAGPlayer();
    pagPlayer->setProgress(v);
    pagPlayer->flush();
}

bool PAGFootage::readPixels(pag::ColorType colorType, pag::AlphaType alphaType, void *dstPixels, size_t dstRowBytes) {
    auto pagPlayer = std::static_pointer_cast<PAGLayer>(mLayer)->getPAGPlayer();
    return pagPlayer->getSurface()->readPixels(colorType, alphaType, dstPixels, dstRowBytes);
}

pag::PAGFile *PAGFootage::getPAGFile() {
    return std::static_pointer_cast<PAGLayer>(mLayer)->getPAGFile();
}

std::vector<std::shared_ptr<ReplaceSlice>> PAGFootage::getReplaceInfo() const {
    return mRepalceInfoList;
}

} // namespace DM