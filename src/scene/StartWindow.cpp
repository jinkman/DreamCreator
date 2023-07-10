#include "StartWindow.h"
#include "rendering/OpenglWidget.h"

#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QToolBar>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <filesystem>

namespace DM {
StartWindow::StartWindow(QWidget *parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags) {
    this->resize(800, 600);
    setObjectName("StartWindow");
    OpenGLWidget *openglWgt = new OpenGLWidget(this);
    setCentralWidget(openglWgt);

    setupMenuBar();

    setupToolBar();

    setupDockWidget();
}

void StartWindow::setupMenuBar() {
    QMenu *aboutMenu = menuBar()->addMenu(tr("&文件"));
    auto actionOpen = aboutMenu->addAction(tr("打开"), this, &StartWindow::openFile);
    actionOpen->setShortcut(Qt::CTRL | Qt::Key_O);
}

void StartWindow::setupToolBar() {
}

void StartWindow::setupDockWidget() {
    QDockWidget *dock = new QDockWidget("", this);
    QWidget *lEmptyWidget = new QWidget(this);
    dock->setTitleBarWidget(lEmptyWidget);
    dock->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    QPalette pal;
    pal.setColor(QPalette::Window, QColor::fromRgb(27, 27, 27, 255));
    dock->setAutoFillBackground(true);
    dock->setPalette(pal);
    this->setStyleSheet("QMainWindow::separator {height:0;width:0;}");

    // 添加垂直布局
    QWidget *wgt = new QWidget(dock);
    QVBoxLayout *vBoxLayout = new QVBoxLayout(wgt);

    QPushButton *buttonOpen = new QPushButton("打开文件");
    buttonOpen->setParent(this);
    buttonOpen->setFixedSize(200, 100);
    buttonOpen->setStyleSheet(QString::fromUtf8("QPushButton{\n"
                                                "font: bold 24px;\n"
                                                "border-radius: 15;\n"
                                                "color: rgb(255, 255, 255);\n"
                                                "}\n"
                                                "QPushButton::hover{\n"
                                                "background-color: rgb(40, 40, 40);\n"
                                                "}\n"
                                                "QPushButton::!hover{\n"
                                                "background-color: rgb(36, 36, 36);\n"
                                                "}\n"
                                                ""));
    vBoxLayout->addWidget(buttonOpen);
    vBoxLayout->addStretch();
    dock->setWidget(wgt);
    // 连接信号
    connect(buttonOpen, SIGNAL(clicked()), this, SLOT(openFile()));
}

void StartWindow::closeEvent(QCloseEvent *e) {
    e->accept();
}

void StartWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "/",
        tr("Scene (*.json *.txt)")); // 多组扩展名用双分号";;"隔开

    if (!fileName.isEmpty()) {
        // 读取json
        if (std::filesystem::exists(fileName.toStdString().c_str())) {
            emit openSceneSignal(fileName);
            return;
        }
    }
    QMessageBox::warning(this, "内容错误", QString("json 文件文件不存在: %1").arg(fileName), QMessageBox::Yes);
}

} // namespace DM