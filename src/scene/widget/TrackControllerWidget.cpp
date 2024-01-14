#include "TrackControllerWidget.h"
#include <QLabel>
#include <QToolButton>
#include <common/Common.h>

namespace DM {

TrackControllerWidget::TrackControllerWidget(QWidget *parent) :
    QWidget(parent) {
    mLayout = new QHBoxLayout(this);
    this->setLayout(mLayout);
    mLayout->setContentsMargins(0, 0, 0, 0);
    mLayout->setSpacing(0);

    QLabel *label = new QLabel(this);
    label->setStyleSheet(QString("QLabel{\n"
                                 "    background-color:rgb(45, 45, 46);\n"
                                 "    border-radius: 10px;\n"
                                 "}"));
    mLayout->addWidget(label);
    QHBoxLayout *layout = new QHBoxLayout(label);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    for (int i = 0; i < 3; i++) {
        QToolButton *btn = new QToolButton(this);
        btn->setFixedSize(25, 25);
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
    label->setLayout(layout);
    this->setFixedHeight(50);
}

TrackControllerWidget::~TrackControllerWidget() {
}

} // namespace DM