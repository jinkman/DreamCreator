#include "PropertyWindow.h"
#include "utils/GlobalMsgMgr.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTreeView>

namespace DM {

PropertyWindow::PropertyWindow(QWidget *parent) :
    ContentWindow(parent) {
    this->setStyleSheet(QString("background-color:rgb(27, 27, 28);\n"));
    setup();
    getLayout()->setContentsMargins(3, 6, 6, 3);

    connect(&GlobalMsgMgr::getInstance(), &GlobalMsgMgr::clickUpFootageSignal, this, &PropertyWindow::clickUpFootageSlot);
}

PropertyWindow::~PropertyWindow() {
}

void PropertyWindow::setup() {
    auto titleLabel = getTitleWidget();
    QHBoxLayout *layout = new QHBoxLayout(titleLabel);
    QLabel *lable = new QLabel("属性", titleLabel);
    lable->setStyleSheet(QString("QLabel{\n"
                                 "  font: 12px;\n"
                                 "  color:white;\n"
                                 "}"));

    layout->addWidget(lable);
    layout->addStretch();
    titleLabel->setLayout(layout);

    mTreeBrowser = new PropertyTreeWidget(this);
    getLayout()->addWidget(mTreeBrowser);
    this->setMinimumWidth(300);
}

void PropertyWindow::updateScene() {
}

void PropertyWindow::clickUpFootageSlot(Footage *footage) {
    mTreeBrowser->updateUI(footage);
}

} // namespace DM