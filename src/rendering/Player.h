#ifndef PLAYER_H
#define PLAYER_H

#include "track/Track.h"
#include "layer/RootNode.h"

namespace DM {

struct VideoProperty {
    static VideoProperty deserialization(const nlohmann::json &obj) {
        VideoProperty videoProperty;
        videoProperty.width = obj["width"].get<int>();
        videoProperty.height = obj["height"].get<int>();
        videoProperty.fps = obj["fps"].get<float>();
        videoProperty.duration = obj["duration"].get<DMTime>();
        videoProperty.bkClr = GLColor::deserialization(obj["backgroundColor"]);
        return std::move(videoProperty);
    }

    int width = 0;
    int height = 0;
    float fps = 25.0f;
    DMTime duration = 0;
    GLColor bkClr = {0.0f, 0.0f, 0.0f, 0.0f};
};

typedef std::function<bool(float v)> ProgressRecordFunc;

class Player {
public:
    static std::shared_ptr<Player> createPlayerByJson(const nlohmann::json &obj, const std::filesystem::path &path);

    ~Player();

    void setProgress(float v);

    float getProgress();

    void flush();

    DMTime duration() const;

    DMTime currentTime() const;

    // 管理轨道
    int getTrackSize();

    void addTrackToPlayer(std::shared_ptr<Track> track);

    void addTrackToPlayerAtIndex(std::shared_ptr<Track> track, int index);

    void removeTrack(std::shared_ptr<Track> track);

    void removeTrackAtIndex(int index);

    VideoProperty getVideoInfo() const;

    bool isValid() const;

    std::filesystem::path getRootPath() const;

    void exportVideo(const std::filesystem::path &path, ProgressRecordFunc recordFunc);

protected:
    Player(const nlohmann::json &obj, const std::filesystem::path &path);

private:
    float mProgress = 0.0f;
    // 节点信息
    std::shared_ptr<RootNode> rtNode = nullptr;
    // 轨道信息
    std::vector<std::shared_ptr<Track>> mTracks;
    // 时长信息
    VideoProperty mVideoProperty;

    GLFrameBuffer mScreenFbo;
};

} // namespace DM

#endif // PLAYER_H