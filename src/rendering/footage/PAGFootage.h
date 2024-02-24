#ifndef PAGFOOTAGE_H
#define PAGFOOTAGE_H

#include "Footage.h"
#include <pag/pag.h>

namespace DM {

enum class EPAGType {
    EUNKNOW_FOOTAGE = 0,
    EPAG_SUBTITLE = 1,
    EPAG_COMPOSITION = 2
};

template <typename T>
inline void serialization(const nlohmann::json &mObj, std::optional<T> &v, const std::string &name) {
    if (mObj.contains(name)) {
        v = mObj[name].get<T>();
    }
}

template <typename T>
inline void deSerialization(const std::optional<T> &v, T &obj) {
    if (v.has_value()) {
        obj = v.value();
    }
}

inline void serializationPt(const nlohmann::json &mObj, std::optional<pag::Point> &v, const std::string &name) {
    if (mObj.contains(name)) {
        v->x = mObj.at(name)[0].get<float>();
        v->y = mObj.at(name)[1].get<float>();
    }
}

inline void serializationColor(const nlohmann::json &mObj, std::optional<pag::Color> &v, const std::string &name) {
    if (mObj.contains(name)) {
        auto colorList = mObj.at(name);
        if (colorList.is_array() && colorList.size() == 3) {
            auto r = mObj.at(name).at(0).get<int>();
            auto g = mObj.at(name).at(1).get<int>();
            auto b = mObj.at(name).at(2).get<int>();
            v = {uint8_t(r), uint8_t(g), uint8_t(b)};
        }
    }
}

class ReplaceSlice {
public:
    ReplaceSlice() = default;
    ~ReplaceSlice();

    void loadFromJson(const nlohmann::json &jObject);

    void replaceDoc(std::shared_ptr<pag::TextDocument> textDoc);

public:
    std::optional<int> editableIndex;
    // text doc;
    std::optional<bool> applyFill;
    std::optional<bool> applyStroke;
    std::optional<float> baselineShift;
    std::optional<bool> boxText;
    std::optional<pag::Point> boxTextPos;
    std::optional<pag::Point> boxTextSize;
    std::optional<float> firstBaseLine;
    std::optional<bool> fauxBold;
    std::optional<bool> fauxItalic;
    std::optional<pag::Color> fillColor;
    std::optional<std::string> fontFamily;
    std::optional<std::string> fontStyle;
    std::optional<float> fontSize;
    std::optional<pag::Color> strokeColor;
    std::optional<bool> strokeOverFill;
    std::optional<float> strokeWidth;
    std::optional<std::string> text;
    std::optional<float> leading;
    std::optional<float> tracking;
    std::optional<pag::Color> backgroundColor;
    std::optional<uint8_t> backgroundAlpha;
    std::optional<pag::Enum> direction;
    std::optional<std::string> mAlignType; // 对齐方式
};

class PAGFootage : public Footage {
public:
    static std::shared_ptr<Footage> createPAGFootageByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

    ~PAGFootage();

    void flush(DMTime t) override;

    void updateResources(const std::string &path) override;

    DMTime getResStartTime() const;

    DMTime getResEndTime() const;

    EPAGType getPAGType() const;

    double getProgress();

    void setPAGProgressAndFlush(double v);

    bool readPixels(pag::ColorType colorType, pag::AlphaType alphaType, void *dstPixels, size_t dstRowBytes);

    std::shared_ptr<pag::PAGFile> getPAGFile();

    std::vector<std::shared_ptr<ReplaceSlice>> getReplaceInfo() const;

protected:
    PAGFootage(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

private:
    // 资源截取 开始结束时间
    DMTime mResStartTime = 0;
    DMTime mResEndTime = 0;
    EPAGType mPagType = EPAGType::EUNKNOW_FOOTAGE;
    std::vector<std::shared_ptr<ReplaceSlice>> mRepalceInfoList;
};

} // namespace DM

#endif // PAGFOOTAGE_H