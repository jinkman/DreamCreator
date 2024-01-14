#include "ContainerWidget.h"
#include <QScrollBar>

namespace DM {

ContainerWidget::ContainerWidget(QWidget *parent) :
    QScrollArea(parent) {
    this->setWidgetResizable(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏横向滚动条
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 隐藏竖向滚动条
    this->setFrameShape(QFrame::NoFrame);
    setupWidget();
}

ContainerWidget::~ContainerWidget() {
}

QVBoxLayout *ContainerWidget::getMainLayout() {
    return mMainLayout;
}

void ContainerWidget::clear() {
    if (mMainLayout == nullptr || mMainLayout->count() == 0) {
        return;
    }
    // 清空
    QLayoutItem *child = nullptr;
    while ((child = mMainLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
        child = nullptr;
    }
}

void ContainerWidget::resetScrollValue() {
    this->horizontalScrollBar()->setSliderPosition(0);
    this->verticalScrollBar()->setSliderPosition(0);
}

void ContainerWidget::setupWidget() {
    QWidget *emptyEidget = new QWidget(this);
    mMainLayout = new QVBoxLayout(emptyEidget);
    mMainLayout->setContentsMargins(0, 0, 0, 100);
    mMainLayout->setSpacing(20);
    emptyEidget->setLayout(mMainLayout);
    this->setWidget(emptyEidget);
}

} // namespace DM