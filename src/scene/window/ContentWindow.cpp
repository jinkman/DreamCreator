#include "ContentWindow.h"
#include <QPainter>
#include <QPainterPath>

namespace DM {

ContentWindow::ContentWindow(QWidget *parent, Qt::WindowFlags flags) :
    QWidget(parent, flags) {
    setupWindow();
}

ContentWindow::~ContentWindow() {
}

QLabel *ContentWindow::getTitleWidget() {
    return mTitleWidget;
}

QVBoxLayout *ContentWindow::getLayout() {
    return mMainLayout;
}

void ContentWindow::closeScene() {
}

void ContentWindow::setupWindow() {
    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setSpacing(0);
    mTitleWidget = new QLabel(this);
    mTitleWidget->setFixedHeight(50);
    mTitleWidget->setMinimumWidth(300);
    mTitleWidget->setStyleSheet(QString("QLabel{\n"
                                        "   background-color:rgb(41, 41, 45);\n"
                                        "   border-top-left-radius:15px;\n"
                                        "   border-top-right-radius:15px;\n"
                                        "}"));
    mMainLayout->addWidget(mTitleWidget);
    mMainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mMainLayout);
}

void ContentWindow::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

} // namespace DM