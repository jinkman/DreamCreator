#include "PropertyWidget.h"
#include <QLabel>

namespace DM {

PropertyWidget::PropertyWidget(QWidget *parent) :
    QWidget(parent) {
    setupWidget();

    setupTopWgt();

    setupCenterWgt();
}

PropertyWidget::~PropertyWidget() {
}

void PropertyWidget::setupWidget() {
    // 垂直布局 三部分
    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setContentsMargins(0, 7, 7, 0);
    mMainLayout->setSpacing(0);
    this->setLayout(mMainLayout);
    this->setMinimumHeight(300);
}

void PropertyWidget::setupTopWgt() {
    // 放置label
    QLabel *topLabel = new QLabel(this);
    topLabel->setMinimumSize(topLabel->minimumWidth(), 50);
    topLabel->setMaximumSize(topLabel->maximumWidth(), 50);
    QHBoxLayout *layout = new QHBoxLayout(topLabel);
    topLabel->setContentsMargins(0, 0, 0, 0);
    topLabel->setStyleSheet(QString("QLabel{\n"
                                    "   background-color:rgb(41, 41, 45);\n"
                                    "   border-top-left-radius:15;\n"
                                    "   border-top-right-radius:15;\n"
                                    "}"));

    QLabel *lable = new QLabel("属性", topLabel);
    lable->setStyleSheet(QString("QLabel{\n"
                                 "  font: 12px;\n"
                                 "  color:white;\n"
                                 "}"));

    layout->addWidget(lable);

    topLabel->setLayout(layout);
    mMainLayout->addWidget(topLabel);
}

void PropertyWidget::setupCenterWgt() {
    QLabel *centerLabel = new QLabel(this);
    centerLabel->setMinimumWidth(200);
    centerLabel->setStyleSheet(QString("QLabel{\n"
                                       "    background-color:rgb(27, 27, 28);\n"
                                       "    border-bottom-left-radius:15;\n"
                                       "    border-bottom-right-radius:15;\n"
                                       "}"));
    mMainLayout->addWidget(centerLabel);
}

} // namespace DM