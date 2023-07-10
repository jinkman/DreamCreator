#include "EditorWindow.h"

#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPalette>
#include <QLabel>

namespace DM {

EditorWindow::EditorWindow(QWidget *parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags) {
    setObjectName("EditorWindow");
    openglWgt = new OpenGLWidget(this);
    setCentralWidget(openglWgt);

    setupMenuBar();

    setupDockWidget();
}

void EditorWindow::openScene(const QString &jstr) {
    openglWgt->initSceneByFile(jstr);
}

void EditorWindow::setupMenuBar() {
    QMenu *aboutMenu = menuBar()->addMenu(tr("&文件"));
    auto actionSave = aboutMenu->addAction(tr("保存"), this, &EditorWindow::saveFile);
    auto actionExport = aboutMenu->addAction(tr("导出"), this, &EditorWindow::exportFile);
    actionSave->setShortcut(Qt::CTRL | Qt::Key_S);
    actionExport->setShortcut(Qt::CTRL | Qt::Key_E);
}

void EditorWindow::setupDockWidget() {
    // 左
    QDockWidget *dockLeft = new QDockWidget("", this);
    QWidget *lEmptyWidgetLeft = new QWidget(this);
    dockLeft->setTitleBarWidget(lEmptyWidgetLeft);
    dockLeft->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
    dockLeft->setAllowedAreas(Qt::LeftDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, dockLeft);

    QPalette pal;
    pal.setColor(QPalette::Window, QColor::fromRgb(27, 27, 27, 255));
    dockLeft->setAutoFillBackground(true);
    dockLeft->setPalette(pal);
    this->setStyleSheet("QMainWindow::separator {height:0;width:0;}");

    // 添加垂直布局
    QWidget *wgt = new QWidget(dockLeft);
    QVBoxLayout *vBoxLayout = new QVBoxLayout(wgt);

    QLabel *label = new QLabel();
    label->setParent(this);
    label->setFixedSize(200, 100);
    label->setStyleSheet(QString::fromUtf8("QLabel{\n"
                                           "font: bold 24px;\n"
                                           "border-radius: 15;\n"
                                           "color: rgb(255, 255, 255);\n"
                                           "}\n"
                                           "QLabel::hover{\n"
                                           "background-color: rgb(40, 40, 40);\n"
                                           "}\n"
                                           "QLabel::!hover{\n"
                                           "background-color: rgb(36, 36, 36);\n"
                                           "}\n"
                                           ""));
    vBoxLayout->addWidget(label);
    vBoxLayout->addStretch();
    dockLeft->setWidget(wgt);

    // 下
    QDockWidget *dockDown = new QDockWidget("", this);
    QWidget *lEmptyWidgetDown = new QWidget(this);
    dockDown->setTitleBarWidget(lEmptyWidgetDown);
    dockDown->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
    dockDown->setAllowedAreas(Qt::BottomDockWidgetArea);
    addDockWidget(Qt::BottomDockWidgetArea, dockDown);
}

void EditorWindow::closeEvent(QCloseEvent *e) {
    e->accept();
    emit closeSignal();
    openglWgt->closeScene();
}

void EditorWindow::saveFile() {
    QMessageBox::about(this, tr("About saveFile"), "saveFile");
}

void EditorWindow::exportFile() {
    QMessageBox::about(this, tr("About exportFile"), "exportFile");
}

} // namespace DM