#ifndef FOOTAGE_TRACK_H
#define FOOTAGE_TRACK_H

#include "Track.h"
#include "rendering/footage/Footage.h"

namespace DM {

class FootageTrack : public Track {
public:
    static std::shared_ptr<Track> createFootageTrackByJson(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

    ~FootageTrack();

    virtual void flush(DMTime t) override;

    std::vector<std::shared_ptr<Footage>> getFootages();

protected:
    FootageTrack(const nlohmann::json &obj, std::shared_ptr<RootNode> rtNode);

private:
    // 画面内容
    std::vector<std::shared_ptr<Footage>> mFootages;
};

} // namespace DM

#endif // PLAYER_H