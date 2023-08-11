#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include "window/ContentWindow.h"
#include "VideoPlayer.h"

namespace DM {
class EditorWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = {});

    ~EditorWindow();

public slots:
    void openScene(const QString &jstr);

signals:
    void closeSignal();

private:
    void setupContentWgt();

    void setupPlayerWgt();

    void setupPropertyWgt();

    void setupTrackWgt();

    // 初始化菜单栏
    void setupMenuBar();

    // 初始化内容
    void setupDockWidget();

    void closeEvent(QCloseEvent *e) override;

private:
    // 内容、播放器、属性、轨道
    ContentWindow *mContentWgt = nullptr;
    VideoPlayer *mVideoPlayer = nullptr;
    ContentWindow *mPropertyWidget = nullptr;
    ContentWindow *mTrackEditor = nullptr;

    // 重新实现
};

} // namespace DM

#endif // EDITORWINDOW_H
