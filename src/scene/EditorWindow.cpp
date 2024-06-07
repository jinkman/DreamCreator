#include "EditorWindow.h"
#include <QMenuBar>
#include <QCloseEvent>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include "window/PropertyWindow.h"
#include "window/ResourcesWindow.h"
#include "window/PlayerWindow.h"
#include "window/TrackWindow.h"
#include "utils/GlobalMsgMgr.h"

namespace DM {

EditorWindow::EditorWindow(QWidget *parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags) {
    setObjectName("EditorWindow");

    setupDockWidget();

    setupMenuBar();
    this->setStyleSheet(QString("background-color: rgb(0, 0, 0)"));
}

EditorWindow::~EditorWindow() {
}

void EditorWindow::openScene(const QString &jstr) {
    if (mWindowMap.count("playerWin")) {
        static_cast<PlayerWindow *>(mWindowMap["playerWin"])->initSceneByFile(jstr);
    }
}

void EditorWindow::setupMenuBar() {
    QMenu *aboutMenu = menuBar()->addMenu(tr("&文件"));
    if (mWindowMap.count("playerWin")) {
        auto actionSave = aboutMenu->addAction(tr("保存"), static_cast<PlayerWindow *>(mWindowMap["playerWin"]), &PlayerWindow::saveFile);
        auto actionExport = aboutMenu->addAction(tr("导出"), static_cast<PlayerWindow *>(mWindowMap["playerWin"]), &PlayerWindow::exportFile);
        actionSave->setShortcut(Qt::CTRL | Qt::Key_S);
        actionExport->setShortcut(Qt::CTRL | Qt::Key_E);
    }
}

void EditorWindow::setupDockWidget() {
    QSplitter *splitterMain = new QSplitter(Qt::Vertical, this); // 水平布置
    // 添加QSplitter水平布局 1.素材、内容窗口；2.播放器窗口；3.调节窗口;
    QSplitter *splitterUp = new QSplitter(Qt::Horizontal, splitterMain); // 水平布置
    // 初始化窗口
    ResourcesWindow *mResWin = new ResourcesWindow(splitterUp);
    PlayerWindow *mPlayerWin = new PlayerWindow(splitterUp);
    PropertyWindow *mPropertyWin = new PropertyWindow(splitterUp);
    TrackWindow *mTrackWin = new TrackWindow(splitterMain);

    mWindowMap["resourcesWin"] = mResWin;
    mWindowMap["playerWin"] = mPlayerWin;
    mWindowMap["propertyWin"] = mPropertyWin;
    mWindowMap["trackWin"] = mTrackWin;

    splitterUp->addWidget(mWindowMap["resourcesWin"]);
    splitterUp->addWidget(mWindowMap["playerWin"]);
    splitterUp->addWidget(mWindowMap["propertyWin"]);
    splitterUp->setChildrenCollapsible(false);

    splitterMain->addWidget(splitterUp);
    splitterMain->addWidget(mWindowMap["trackWin"]);
    splitterMain->setChildrenCollapsible(false);

    this->setCentralWidget(splitterMain);
    splitterMain->setStyleSheet(QString("QSplitter::handle {\n"
                                        "   background-color:rgba(0,0,0,0);\n"
                                        "   width: 1px;\n"
                                        "}"));
}

void EditorWindow::closeEvent(QCloseEvent *e) {
    e->accept();
    for (auto itr : mWindowMap) {
        itr.second->closeScene();
    }
    emit closeSignal();
}
} // namespace DM