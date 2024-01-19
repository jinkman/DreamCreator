#include "FootageTrack.h"

namespace DM {

std::shared_ptr<Track> FootageTrack::createFootageTrackByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) {
    std::shared_ptr<FootageTrack> footageTrack(new FootageTrack(obj, rtNode));
    return footageTrack;
}

FootageTrack::FootageTrack(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode) :
    Track(obj, rtNode) {
    mFootages.reserve(obj["footages"].size());
    for (auto &footageObj : obj["footages"]) {
        mFootages.push_back(Footage::createFootageByJson(footageObj, rtNode));
    }
}

FootageTrack::~FootageTrack() {
}

void FootageTrack::flush(DMTime t) {
    for (auto &footage : mFootages) {
        footage->flush(t);
    }
}

std::vector<std::shared_ptr<Footage>> FootageTrack::getFootages() {
    return mFootages;
}

} // namespace DM