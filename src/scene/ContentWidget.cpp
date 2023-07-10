#include "ContentWidget.h"
#include <QLabel>
#include <QToolButton>
#include <QSplitter>

namespace DM {

ContentWidget::ContentWidget(QWidget *parent) :
    QWidget(parent) {
    setupWidget();

    setupTopWgt();

    setupBottomWgt();
}

ContentWidget::~ContentWidget() {
}

void ContentWidget::setupWidget() {
    // 垂直布局 三部分
    mMainLayout = new QVBoxLayout(this);
    mMainLayout->setContentsMargins(7, 7, 0, 0);
    mMainLayout->setSpacing(0);
    this->setLayout(mMainLayout);
}

void ContentWidget::setupTopWgt() {
    // 放置label
    QLabel *topLabel = new QLabel(this);
    topLabel->setMinimumSize(480, 50);
    topLabel->setMaximumHeight(50);
    QHBoxLayout *layout = new QHBoxLayout(topLabel);
    layout->setContentsMargins(0, 5, 0, 0);
    topLabel->setContentsMargins(10, 0, 10, 0);
    topLabel->setStyleSheet(QString("QLabel{\n"
                                    "   background-color:rgb(41, 41, 45);\n"
                                    "   border-top-left-radius:15;\n"
                                    "   border-top-right-radius:15;\n"
                                    "}"));

    // 放置按钮
    for (auto &content : contentItem) {
        QToolButton *btn = new QToolButton(topLabel);
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
    topLabel->setLayout(layout);
    mMainLayout->addWidget(topLabel);
}

void ContentWidget::setupBottomWgt() {
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

    mMainLayout->addWidget(splitterMain);
}

} // namespace DM