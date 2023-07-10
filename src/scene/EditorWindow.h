#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include "VideoPlayer.h"
#include "ContentWidget.h"
#include "PropertyWidget.h"
#include "TrackEditor.h"

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
    // 初始化菜单栏
    void setupMenuBar();

    // 初始化内容
    void setupDockWidget();

    void closeEvent(QCloseEvent *e) override;

private:
    // 内容、播放器、属性、轨道
    ContentWidget *mContentWidget = nullptr;
    VideoPlayer *mVideoPlayer = nullptr;
    PropertyWidget *mPropertyWidget = nullptr;
    TrackEditor *mTrackEditor = nullptr;
};

} // namespace DM

#endif // EDITORWINDOW_H
