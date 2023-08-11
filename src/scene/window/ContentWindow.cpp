#include "ContentWindow.h"

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

void ContentWindow::setupWindow() {
    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setSpacing(0);
    mTitleWidget = new QLabel(this);
    mTitleWidget->setMinimumSize(mTitleWidget->minimumWidth(), 50);
    mTitleWidget->setMaximumSize(mTitleWidget->maximumWidth(), 50);
    mTitleWidget->setStyleSheet(QString("QLabel{\n"
                                        "   background-color:rgb(41, 41, 45);\n"
                                        "   border-top-left-radius:15;\n"
                                        "   border-top-right-radius:15;\n"
                                        "}"));
    mMainLayout->addWidget(mTitleWidget);
}

} // namespace DM