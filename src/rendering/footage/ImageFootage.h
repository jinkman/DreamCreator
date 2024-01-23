#ifndef IMAGEFOOTAGE_H
#define IMAGEFOOTAGE_H

#include "Footage.h"

namespace DM {

class ImageFootage : public Footage {
public:
    static std::shared_ptr<Footage> createImageFootageByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

    ~ImageFootage();

    void flush(DMTime t) override;

    void updateResources(const std::string &path) override;

protected:
    ImageFootage(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);
};

} // namespace DM

#endif // IMAGEFOOTAGE_H