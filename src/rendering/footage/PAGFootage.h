#ifndef PAGFOOTAGE_H
#define PAGFOOTAGE_H

#include "Footage.h"

namespace DM {

class PAGFootage : public Footage {
public:
    static std::shared_ptr<Footage> createPAGFootageByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

    ~PAGFootage();

    void flush(DMTime t) override;

    void updateResources(const std::string &path) override;

    DMTime getResStartTime() const;

    DMTime getResEndTime() const;

protected:
    PAGFootage(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

private:
    // 资源截取 开始结束时间
    DMTime mResStartTime = 0;
    DMTime mResEndTime = 0;
};

} // namespace DM

#endif // PAGFOOTAGE_H