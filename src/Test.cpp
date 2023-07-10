#include "scene/WindowManager.h"
#include <QSurfaceFormat>
#include <QApplication>

int main(int argc, char **argv) {
    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    DM::WindowManager winManager;
    auto ret = app.exec();
    return ret;
}
