#include "EditorWindow.h"

#include <QMenuBar>
#include <QCloseEvent>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>

namespace DM {

EditorWindow::EditorWindow(QWidget *parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags) {
    setObjectName("EditorWindow");

    this->setStyleSheet(QString("background-color: rgb(0, 0, 0)"));

    mContentWidget = new ContentWidget(this);

    mVideoPlayer = new VideoPlayer();

    mPropertyWidget = new PropertyWidget(this);

    mTrackEditor = new TrackEditor(this);

    setupMenuBar();

    setupDockWidget();
}

EditorWindow::~EditorWindow() {
    if (mVideoPlayer != nullptr) {
        delete mVideoPlayer;
        mVideoPlayer = nullptr;
    }
}

void EditorWindow::openScene(const QString &jstr) {
    mVideoPlayer->initSceneByFile(jstr);
}

void EditorWindow::setupMenuBar() {
    QMenu *aboutMenu = menuBar()->addMenu(tr("&文件"));
    auto actionSave = aboutMenu->addAction(tr("保存"), mVideoPlayer, &VideoPlayer::saveFile);
    auto actionExport = aboutMenu->addAction(tr("导出"), mVideoPlayer, &VideoPlayer::exportFile);
    actionSave->setShortcut(Qt::CTRL | Qt::Key_S);
    actionExport->setShortcut(Qt::CTRL | Qt::Key_E);
}

void EditorWindow::setupDockWidget() {
    QPalette pal;
    pal.setColor(QPalette::Window, QColor::fromRgb(27, 27, 27, 255));

    // 上
    QDockWidget *dockTop = new QDockWidget("", this);
    QWidget *lEmptyWidgetLeft = new QWidget(this);
    dockTop->setTitleBarWidget(lEmptyWidgetLeft);
    dockTop->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
    dockTop->setAllowedAreas(Qt::TopDockWidgetArea);
    addDockWidget(Qt::TopDockWidgetArea, dockTop);

    // 添加QSplitter水平布局 1.素材、内容窗口；2.播放器窗口；3.调节窗口;
    QSplitter *splitterMain = new QSplitter(Qt::Horizontal, dockTop); // 水平布置
    splitterMain->addWidget(mContentWidget);
    splitterMain->addWidget(mVideoPlayer);
    splitterMain->addWidget(mPropertyWidget);
    splitterMain->setChildrenCollapsible(false);
    dockTop->setWidget(splitterMain);

    // 下 1.轨道窗口
    QDockWidget *dockBottom = new QDockWidget("", this);
    QWidget *lEmptyWidgetDown = new QWidget(this);
    dockBottom->setTitleBarWidget(lEmptyWidgetDown);
    dockBottom->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
    dockBottom->setAllowedAreas(Qt::BottomDockWidgetArea);
    dockBottom->setWidget(mTrackEditor);
    addDockWidget(Qt::BottomDockWidgetArea, dockBottom);

    dockTop->setAutoFillBackground(true);
    dockTop->setPalette(pal);
    dockBottom->setAutoFillBackground(true);
    dockBottom->setPalette(pal);
}

void EditorWindow::closeEvent(QCloseEvent *e) {
    e->accept();
    emit closeSignal();
    mVideoPlayer->resetScene();
}

} // namespace DM