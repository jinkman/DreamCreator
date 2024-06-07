#include "scene/WindowManager.h"
#include <QSurfaceFormat>
#include <QApplication>
#include <pag/pag.h>
#include <filesystem>

int main(int argc, char **argv) {
    std::filesystem::path appPath(argv[0]);
    // 注册默认回退字体
    std::filesystem::path fontPath = appPath.parent_path() / "../Resources/fonts/";
    pag::PAGFont::SetFallbackFontPaths({(fontPath / "NotoSansCJKsc-Regular.woff").string()}, {0});

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