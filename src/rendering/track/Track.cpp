#include "Track.h"
#include "FootageTrack.h"

namespace DM {

std::shared_ptr<Track> Track::createTrackByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) {
    std::shared_ptr<Track> track = nullptr;
    std::string type = obj["trackType"].get<std::string>();
    if (type == "footage") {
        track = FootageTrack::createFootageTrackByJson(obj, rtNode);
    }
    return track;
}

Track::Track(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) {
    // 初始化镜头
}

Track::~Track() {
}

} // namespace DM