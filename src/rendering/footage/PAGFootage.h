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

protected:
    PAGFootage(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

private:
    // 资源截取 开始结束时间
    DMTime mResStartTime = 0;
    DMTime mResEndTime = 0;
    EPAGType mPagType = EPAGType::EUNKNOW_FOOTAGE;
};

} // namespace DM

#endif // PAGFOOTAGE_H