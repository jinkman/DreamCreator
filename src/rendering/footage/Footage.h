#ifndef FOOTAGE_H
#define FOOTAGE_H

#include "utils/Types.h"
#include "rendering/layer/RootNode.h"

namespace DM {

class Footage {
public:
    static std::shared_ptr<Footage> createFootageByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

    virtual ~Footage();

    virtual void flush(DMTime t) = 0;

    DMTime startTime() const;

    DMTime endTime() const;

    DMTime duration() const;

    void updateStartTime(DMTime t);

    void updateEndTime(DMTime t);

    virtual void updateResources(const std::string &path) = 0;

protected:
    Footage(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

protected:
    // 画面开始结束时间
    DMTime mStartTime = 0;
    DMTime mEndTime = 0;
    // 根结点
    std::shared_ptr<RootNode> mRootNode = nullptr;
};

} // namespace DM

#endif // FOOTAGE_H