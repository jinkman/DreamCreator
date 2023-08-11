#include "EditorWindow.h"

#include <QMenuBar>
#include <QCloseEvent>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSplitter>
#include <QToolButton>
#include "common/Common.h"
#include "scene/widget/DividingRule.h"

namespace DM {

EditorWindow::EditorWindow(QWidget *parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags) {
    setObjectName("EditorWindow");

    this->setStyleSheet(QString("background-color: rgb(0, 0, 0)"));

    setupContentWgt();

    setupPlayerWgt();

    setupPropertyWgt();

    setupTrackWgt();

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

void EditorWindow::setupContentWgt() {
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

    mContentWgt = new ContentWindow(this);
    auto titleLabel = mContentWgt->getTitleWidget();
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

    auto mainLayout = mContentWgt->getLayout();
    mainLayout->setContentsMargins(7, 7, 0, 0);
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

void EditorWindow::setupPlayerWgt() {
    mVideoPlayer = new VideoPlayer();
}

void EditorWindow::setupPropertyWgt() {
    mPropertyWidget = new ContentWindow(this);
    auto titleLabel = mPropertyWidget->getTitleWidget();
    QHBoxLayout *layout = new QHBoxLayout(titleLabel);
    QLabel *lable = new QLabel("属性", titleLabel);
    lable->setStyleSheet(QString("QLabel{\n"
                                 "  font: 12px;\n"
                                 "  color:white;\n"
                                 "}"));

    layout->addWidget(lable);
    layout->addStretch();
    titleLabel->setLayout(layout);

    QLabel *centerLabel = new QLabel(mPropertyWidget);
    centerLabel->setMinimumWidth(200);
    centerLabel->setStyleSheet(QString("QLabel{\n"
                                       "    background-color:rgb(27, 27, 28);\n"
                                       "    border-bottom-left-radius:15;\n"
                                       "    border-bottom-right-radius:15;\n"
                                       "}"));
    auto mainLayout = mPropertyWidget->getLayout();
    mainLayout->setContentsMargins(0, 7, 7, 0);
    mainLayout->addWidget(centerLabel);
}

void EditorWindow::setupTrackWgt() {
    mTrackEditor = new ContentWindow(this);
    auto titleLabel = mTrackEditor->getTitleWidget();
    titleLabel->setMinimumHeight(40);
    titleLabel->setMaximumHeight(40);
    QHBoxLayout *layout = new QHBoxLayout(titleLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    // 放置按钮
    for (int i = 0; i < 6; i++) {
        QToolButton *btn = new QToolButton(titleLabel);
        btn->setFixedSize(30, 30);
        btn->setIcon(QIcon(getQLocalPath("icon/play.png")));
        btn->setIconSize(QSize(20, 20));
        btn->setStyleSheet(QString("QToolButton{\n"
                                   "    color:white;\n"
                                   "    background:transparent;\n"
                                   "}"
                                   "QToolTip{\n"
                                   "    color:white;\n"
                                   "    background-color:rgb(7, 7, 9);\n"
                                   "}"));
        btn->setToolTip("提示信息");
        layout->addWidget(btn);
    }
    layout->addStretch();
    titleLabel->setLayout(layout);

    auto mainLayout = mTrackEditor->getLayout();
    mainLayout->setContentsMargins(7, 7, 7, 7);
    QLabel *trackLabel = new QLabel(mTrackEditor);
    trackLabel->setMinimumHeight(200);
    trackLabel->setStyleSheet(QString("QLabel{\n"
                                      "    background-color:rgb(27, 27, 28);\n"
                                      "    border-bottom-left-radius:15;\n"
                                      "    border-bottom-right-radius:15;\n"
                                      "}"));
    QVBoxLayout *trackLayout = new QVBoxLayout(trackLabel);
    trackLayout->setContentsMargins(0, 0, 0, 0);
    // 布局
    DividingRule *rule = new DividingRule(trackLabel);
    trackLayout->addWidget(rule);
    trackLayout->addStretch();

    trackLabel->setLayout(trackLayout);
    mainLayout->addWidget(trackLabel);
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
    splitterMain->addWidget(mContentWgt);
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