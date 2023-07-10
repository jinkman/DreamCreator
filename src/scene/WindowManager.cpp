#include "WindowManager.h"

namespace DM {

WindowManager::WindowManager(QWidget *parent) :
    QWidget(parent) {
    startWindow.show();
    editorWindow.hide();
    connect(&startWindow, SIGNAL(openSceneSignal(const QString &)), this, SLOT(openScene(const QString &)));
    connect(&editorWindow, SIGNAL(closeSignal()), &startWindow, SLOT(show()));
}

WindowManager::~WindowManager() {
}

void WindowManager::openScene(const QString &jstr) {
    startWindow.hide();
    editorWindow.show();
    editorWindow.openScene(jstr);
}

} // namespace DM