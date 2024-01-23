#include "scene/WindowManager.h"
#include <QSurfaceFormat>
#include <QApplication>
#include <pag/pag.h>
#include "common/Common.h"

int main(int argc, char **argv) {
    // 注册默认回退字体
    pag::PAGFont::SetFallbackFontPaths({DM::getLocalPath("fonts/NotoSansCJKsc-Regular.woff")}, {0});

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
