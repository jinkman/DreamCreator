#include "PropertyWindow.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTreeView>
#include <common/Common.h>

namespace DM {
/*
- `QVariant::Invalid`：无效的 `QVariant` 类型。
- `QVariant::Bool`：布尔类型。
- `QVariant::Int`：整数类型。
- `QVariant::UInt`：无符号整数类型。
- `QVariant::LongLong`：长整型。
- `QVariant::ULongLong`：无符号长整型。
- `QVariant::Double`：双精度浮点数。
- `QVariant::Char`：字符类型。
- `QVariant::Map`：映射类型。
- `QVariant::List`：列表类型。
- `QVariant::String`：字符串类型。
- `QVariant::StringList`：字符串列表类型。
- `QVariant::ByteArray`：字节数组类型。
- `QVariant::BitArray`：位数组类型。
- `QVariant::Date`：日期类型。
- `QVariant::Time`：时间类型。
- `QVariant::DateTime`：日期时间类型。
- `QVariant::Url`：URL 类型。
- `QVariant::Locale`：区域设置信息。
- `QVariant::RegExp`：正则表达式类型。
- `QVariant::Uuid`：UUID 类型。
*/

static std::vector<std::tuple<QString, QVariant::Type, bool>> basePropertyList = {
    {"图层ID", QVariant::Int, false},
    {"图层名", QVariant::String, false},
    {"图层类型", QVariant::String, false},
    {"资源链接", QVariant::String, false},
    {"图层开始", QVariant::Int, false},
    {"图层结束", QVariant::Int, false},
    {"资源开始", QVariant::Int, false},
    {"资源结束", QVariant::Int, false},
    {"显示", QVariant::Bool, false},
    {"图层变换", QVariant::Int, false},
    {"特效列表", QVariant::Int, false}};

PropertyWindow::PropertyWindow(QWidget *parent) :
    ContentWindow(parent) {
    this->setStyleSheet(QString("background-color:rgb(27, 27, 28);\n"));
    setup();
    getLayout()->setContentsMargins(3, 6, 6, 3);
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

    auto mainLayout = getLayout();
    mTreeBrowser = new QtTreePropertyBrowser(this);
    // 设置样式
    // 取消颜色隔行差异
    mTreeBrowser->setResizeMode(QtTreePropertyBrowser::ResizeMode::Interactive);
    mTreeBrowser->setAlternatingRowColors(false);
    mTreeBrowser->setHeaderVisible(false);
    mTreeBrowser->setStyleSheet(QString("QTreeWidget {\n"
                                        "  font: 12px;\n"
                                        "  color:white;\n"
                                        "  border: none;\n"
                                        "}\n"));
    // 隐藏滚动条
    QTreeView *treeView = mTreeBrowser->findChild<QTreeView *>();
    if (treeView) {
        treeView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    mainLayout->addWidget(mTreeBrowser);

    mEditManager = new QtVariantPropertyManager(mTreeBrowser);
    QtVariantEditorFactory *varFactory = new QtVariantEditorFactory(mTreeBrowser);
    // 为管理器设置工厂
    mTreeBrowser->setFactoryForManager(mEditManager, varFactory);
}

void PropertyWindow::updateScene() {
    emit updateSceneSignal();
}

void PropertyWindow::clickUpFootageSlot(std::shared_ptr<Footage> footage) {
    mEditManager->clear();
    mTreeBrowser->clear();
    if (footage == nullptr) {
        return;
    }

    for (auto &baseProperty : basePropertyList) {
        auto item = mEditManager->addProperty(std::get<1>(baseProperty), std::get<0>(baseProperty));
        mTreeBrowser->addProperty(item);
    }
}

} // namespace DM