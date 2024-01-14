#include "ResourcesWindow.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QSplitter>
#include <QLabel>
#include <common/Common.h>
#include <QDebug>

namespace DM {

ResourcesWindow::ResourcesWindow(QWidget *parent) :
    ContentWindow(parent) {
    setup();
    getLayout()->setContentsMargins(6, 6, 3, 3);
}

ResourcesWindow::~ResourcesWindow() {
}

void ResourcesWindow::setup() {
    static const std::vector<
        std::tuple<QString, QString, QString>>
        contentItem = {
            {"媒体", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
            {"音频", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
            {"文本", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
            {"贴纸", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
            {"特效", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
            {"转场", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
            {"滤镜", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
            {"调节", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
            {"模板", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")}};

    auto titleLabel = getTitleWidget();
    titleLabel->setContentsMargins(10, 0, 10, 0);
    QHBoxLayout *layout = new QHBoxLayout(titleLabel);
    layout->setContentsMargins(0, 5, 0, 0);
    // 放置按钮
    for (auto &content : contentItem) {
        QToolButton *btn = new QToolButton(titleLabel);
        btn->setFixedSize(50, 50);
        btn->setIcon(QIcon(std::get<1>(content)));
        btn->setIconSize(QSize(20, 20));
        btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn->setStyleSheet(QString("QToolButton{\n"
                                   "    color:white;\n"
                                   "    background:transparent;\n"
                                   "}"));
        btn->setText(std::get<0>(content));
        layout->addWidget(btn);
    }
    layout->addStretch();

    auto mainLayout = getLayout();
    QSplitter *splitterMain = new QSplitter(Qt::Horizontal, this); // 水平布置
    splitterMain->setHandleWidth(1);
    splitterMain->setStyleSheet(QString("QSplitter::handle{\n"
                                        "   background-color:rgb(37, 37, 40);\n"
                                        "}\n"));
    QLabel *leftLabel = new QLabel(splitterMain);
    leftLabel->setMinimumWidth(100);
    leftLabel->setStyleSheet(QString("QLabel{\n"
                                     "  background-color:rgb(37, 37, 40);\n"
                                     "  border-bottom-left-radius:15;\n"
                                     "}"));
    QLabel *centerLabel = new QLabel(splitterMain);
    centerLabel->setMinimumSize(300, 300);
    centerLabel->setStyleSheet(QString("QLabel{\n"
                                       "  background-color:rgb(27, 27, 28);\n"
                                       "  border-bottom-right-radius:15;\n"
                                       "}"));
    splitterMain->addWidget(leftLabel);
    splitterMain->addWidget(centerLabel);
    splitterMain->setChildrenCollapsible(false);
    mainLayout->addWidget(splitterMain);
}

} // namespace DM