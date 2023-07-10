#ifndef TRACK_H
#define TRACK_H

#include "utils/Types.h"
#include "rendering/layer/RootNode.h"

namespace DM {

class Track {
public:
    static std::shared_ptr<Track> createTrackByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

    virtual ~Track();

    virtual void flush(DMTime t) = 0;

protected:
    Track(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);
};

} // namespace DM

#endif // PLAYER_H