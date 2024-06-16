#ifndef GLOBAM_MSG_MGR_H
#define GLOBAM_MSG_MGR_H

#include <QObject.h>
#include <mutex>
#include "rendering/footage/Footage.h"

namespace DM {

class Player;
class GlobalMsgMgr : public QObject {
    Q_OBJECT
public:
    static GlobalMsgMgr &getInstance() {
        static GlobalMsgMgr instance;
        return instance;
    }

    ~GlobalMsgMgr() {
    }

signals:

    // 初始化场景成功
    void initSceneFinished(Player *);

    // 刷新一帧
    void flushOneFrame(Player *);

    // 设置进度
    void updateProgress(Player *);

    // 属性框更新
    void updateProperty();

    // 更新场景
    void updateScene();

    // 轨道浏览器更新
    void updateTrack();

    // 导出文件
    void exportFile(const QString &);

    // 保存场景
    void saveFile(const QString &);

    // 轨道画面点击事件
    void clickUpFootageSignal(Footage *);

    // 关闭场景，退出
    void closeScene();

    // 加载协议成功
    void loadProtocolFinish(const nlohmann::json &, const QString &);

    // 进度条seek
    void seekToTimeByRule(const DMTime &t);

private:
    GlobalMsgMgr(QObject *parent = nullptr) :
        QObject(parent) {
    }

    GlobalMsgMgr(const GlobalMsgMgr &) = delete;
    const GlobalMsgMgr &operator=(const GlobalMsgMgr &) = delete;
};

} // namespace DM

#endif // GLOBAM_MSG_MGR_H