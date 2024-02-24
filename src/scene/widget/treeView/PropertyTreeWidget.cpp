
#include "PropertyTreeWidget.h"
#include "CompleterLineEdit.h"
#include "utils/GlobalMsgMgr.h"
#include <QHeaderView>
#include <QCheckBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QPushButton>

namespace DM {

PropertyTreeWidget::PropertyTreeWidget(QWidget *parent) :
    QTreeWidget(parent) {
    setAlternatingRowColors(false);
    setFrameStyle(QFrame::NoFrame); // 去掉边框
    setHeaderHidden(true);
    // 每列自动分配列宽
    this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->setExpandsOnDoubleClick(false);
    this->setStyleSheet(QString("QWidget {\n"
                                " font:   10px;\n"
                                " color:  white;\n"
                                "}\n"
                                "QTreeView::item{\n"
                                " height: 30px;\n"
                                "}\n"
                                "QTreeView::branch {\n"
                                " image: none; \n"
                                "}\n"
                                "QLineEdit {\n"
                                "   border-with:0;\n"
                                "   border-style:outset;\n"
                                "}\n"
                                "QDoubleSpinBox {\n"
                                "  border: none;\n"
                                "}\n"
                                "QDoubleSpinBox::up-button {\n"
                                "  width:0px;\n"
                                "}\n"
                                "QDoubleSpinBox::down-button {\n"
                                "  width:0px;\n"
                                "}"));
    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    this->setColumnCount(2);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(this, &QTreeWidget::itemClicked, [=](QTreeWidgetItem *item, int column) {
        item->setExpanded(!item->isExpanded());
    });
}

PropertyTreeWidget::~PropertyTreeWidget() {
}

void PropertyTreeWidget::updateUI(std::shared_ptr<Footage> footage) {
    this->clear();
    mFootage = footage;
    if (mFootage == nullptr) {
        return;
    }
    // 图层基本属性
    QTreeWidgetItem *baseItem = new QTreeWidgetItem(QStringList() << "图层属性");
    this->addTopLevelItem(baseItem);
    updateBasePropertyUI(baseItem);

    // 文本相关属性
    if (mFootage->getFootageType() == EFootageType::EPAG_FOOTAGE) {
        QTreeWidgetItem *pagItem = new QTreeWidgetItem(this, QStringList() << "文本属性");
        this->addTopLevelItem(pagItem);
        updatePagPropertyUI(pagItem);
    }

    // 视频相关属性
    if (mFootage->getFootageType() == EFootageType::EVIDEO_FOOTAGE) {
        QTreeWidgetItem *videoItem = new QTreeWidgetItem(this, QStringList() << "视频属性");
        this->addTopLevelItem(videoItem);
    }

    // this->expandAll();
}

void PropertyTreeWidget::updateBasePropertyUI(QTreeWidgetItem *parent) {
    if (mFootage == nullptr || mFootage->getFootageLayer() == nullptr) {
        return;
    }

    auto footageLayer = mFootage->getFootageLayer();

    // 可见性
    QTreeWidgetItem *visibleItem = new QTreeWidgetItem(parent, QStringList() << "显示");
    QCheckBox *checkBox = new QCheckBox(this);
    checkBox->setCheckState(mFootage->footageVisible() ? Qt::Checked : Qt::Unchecked);
    this->setItemWidget(visibleItem, 1, checkBox);
    QObject::connect(checkBox, &QCheckBox::stateChanged, [this, footageLayer](int v) {
        mFootage->footageVisible() = (v != Qt::Unchecked);
        // 需要更新属性
        updateSceneProperty();
    });

    // srt 缩放旋转平移
    QTreeWidgetItem *transItem = new QTreeWidgetItem(parent, QStringList() << "变换");
    QTreeWidgetItem *scaleItem = new QTreeWidgetItem(transItem, QStringList() << "缩放");
    addVectorPropertyUI(scaleItem, {{&footageLayer->scale().x, -2.0, 2.0, "x轴"}, {&footageLayer->scale().y, -2.0, 2.0, "y轴"}});
    QTreeWidgetItem *rotateItem = new QTreeWidgetItem(transItem, QStringList() << "旋转");
    // addVectorPropertyUI(rotateItem, {{&footageLayer->orientation().x, 0.0, 360.0, "角度"}});
    QTreeWidgetItem *translateItem = new QTreeWidgetItem(transItem, QStringList() << "平移");
    addVectorPropertyUI(translateItem, {{&footageLayer->translate().x, -2000.0, 2000.0, "x轴"}, {&footageLayer->translate().y, -2000.0, 2000.0, "y轴"}});
}

void PropertyTreeWidget::updatePagPropertyUI(QTreeWidgetItem *parent) {
    // 富文本信息
    for (auto &replaceSlice : std::dynamic_pointer_cast<PAGFootage>(mFootage)->getReplaceInfo()) {
        addDocumentPropertyUI(parent, replaceSlice);
    }
}

void PropertyTreeWidget::addDocumentPropertyUI(QTreeWidgetItem *parent, std::shared_ptr<ReplaceSlice> replaceSlice) {
    QTreeWidgetItem *wgtItem = new QTreeWidgetItem(parent, QStringList() << "文本内容");

    int editableIndex = replaceSlice->editableIndex.has_value() ? replaceSlice->editableIndex.value() : 0;

    auto layerDocument = std::static_pointer_cast<PAGFootage>(mFootage)->getPAGFile()->getTextData(editableIndex);
    // document
    if (layerDocument == nullptr) {
        return;
    }

    // 字体
    QTreeWidgetItem *fontItem = new QTreeWidgetItem(wgtItem, QStringList() << "字体属性");
    // addDocumentFontPropertyUI(fontItem, {layerDocument->fontFamily, layerDocument->fontStyle}, replaceSlice->fontFamily, replaceSlice->fontStyle);
    addDocumentFloatPropertyUI(fontItem, 0, layerDocument->fontSize, "大小", replaceSlice->fontSize, 1, 200);
    addDocumentBoolPropertyUI(fontItem, layerDocument->fauxBold, "加粗", replaceSlice->fauxBold);
    addDocumentBoolPropertyUI(fontItem, layerDocument->fauxItalic, "斜体", replaceSlice->fauxItalic);

    addDocumentTextPropertyUI(wgtItem, layerDocument->text, "文本", replaceSlice->text);
    // 填充色
    addDocumentBoolPropertyUI(wgtItem, layerDocument->applyFill, "填充", replaceSlice->applyFill);
    addDocumentColorPropertyUI(wgtItem, layerDocument->fillColor, "填充色", replaceSlice->fillColor);

    // 描边色
    addDocumentBoolPropertyUI(wgtItem, layerDocument->applyStroke, "描边", replaceSlice->applyStroke);
    addDocumentColorPropertyUI(wgtItem, layerDocument->strokeColor, "描边色", replaceSlice->strokeColor);

    addDocumentFloatPropertyUI(wgtItem, 1, layerDocument->baselineShift, "基线位移", replaceSlice->baselineShift, -100.0, 100.0);
    // 框文本
    auto boxTextItem = addDocumentBoolPropertyUI(wgtItem, layerDocument->boxText, "框文本", replaceSlice->boxText);
    // Point
    if (!replaceSlice->boxTextPos.has_value()) {
        replaceSlice->boxTextPos = layerDocument->boxTextPos;
    }
    addDocumentFloatPropertyUI(boxTextItem, 1, "x", replaceSlice->boxTextPos.value().x, -1000.0, 1000.0);
    addDocumentFloatPropertyUI(boxTextItem, 1, "y", replaceSlice->boxTextPos.value().y, -2000.0, 2000.0);
    if (!replaceSlice->boxTextSize.has_value()) {
        replaceSlice->boxTextSize = layerDocument->boxTextSize;
    }
    addDocumentFloatPropertyUI(boxTextItem, 1, "w", replaceSlice->boxTextSize.value().x, -1000.0, 1000.0);
    addDocumentFloatPropertyUI(boxTextItem, 1, "h", replaceSlice->boxTextSize.value().y, -2000.0, 2000.0);
}

QTreeWidgetItem *PropertyTreeWidget::addDocumentTextPropertyUI(QTreeWidgetItem *parent, const std::string &defaultV, const QString &name, std::optional<std::string> &v) {
    QTreeWidgetItem *wgtItem = new QTreeWidgetItem(parent, QStringList() << name);
    QLineEdit *textEdit = new QLineEdit(this);
    textEdit->setAlignment(Qt::AlignCenter);
    textEdit->setText(v.has_value() ? QString::fromStdString(v.value()) : QString::fromStdString(defaultV));
    this->setItemWidget(wgtItem, 1, textEdit);
    QObject::connect(textEdit, &QLineEdit::textChanged, [this, &v](const QString &str) {
        v = str.toStdString();
        updateSceneProperty();
    });
    return wgtItem;
}

QTreeWidgetItem *PropertyTreeWidget::addDocumentBoolPropertyUI(QTreeWidgetItem *parent, const bool &defaultV, const QString &name, std::optional<bool> &v) {
    QTreeWidgetItem *wgtItem = new QTreeWidgetItem(parent, QStringList() << name);
    QWidget *emptyWgt = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(emptyWgt);
    emptyWgt->setLayout(layout);

    QCheckBox *checkBox = new QCheckBox(emptyWgt);
    bool check = v.has_value() ? v.value() : defaultV;
    checkBox->setCheckState(check ? Qt::Checked : Qt::Unchecked);
    layout->addStretch();
    layout->addWidget(checkBox);

    this->setItemWidget(wgtItem, 1, emptyWgt);
    QObject::connect(checkBox, &QCheckBox::stateChanged, [this, &v](int value) {
        v = value != Qt::Unchecked;
        // 需要更新属性
        updateSceneProperty();
    });
    return wgtItem;
}

QTreeWidgetItem *PropertyTreeWidget::addDocumentFloatPropertyUI(QTreeWidgetItem *parent, int decimals, const float &defaultV, const QString &name, std::optional<float> &v, double min, double max) {
    auto [wgtItem, doubleSlider, doubleSpinBox] = createDoubleTreeWidgetItem(parent, decimals, name, min, max);
    doubleSpinBox->setValue(v.has_value() ? double(v.value()) : double(defaultV));
    doubleSlider->setValue(v.has_value() ? double(v.value()) : double(defaultV));
    QObject::connect(doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this, &v](double value) {
        v = value;
        // 需要更新属性
        updateSceneProperty();
    });
    return wgtItem;
}

QTreeWidgetItem *PropertyTreeWidget::addDocumentFloatPropertyUI(QTreeWidgetItem *parent, int decimals, const QString &name, float &v, double min, double max) {
    auto [wgtItem, doubleSlider, doubleSpinBox] = createDoubleTreeWidgetItem(parent, decimals, name, min, max);
    doubleSpinBox->setValue(v);
    doubleSlider->setValue(v);
    QObject::connect(doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this, &v](double value) {
        v = value;
        // 需要更新属性
        updateSceneProperty();
    });
    return wgtItem;
}

QTreeWidgetItem *PropertyTreeWidget::addDocumentColorPropertyUI(QTreeWidgetItem *parent, const pag::Color &defaultV, const QString &name, std::optional<pag::Color> &v) {
    QTreeWidgetItem *wgtItem = new QTreeWidgetItem(parent, QStringList() << name);
    QWidget *emptyWgt = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(emptyWgt);
    emptyWgt->setLayout(layout);

    QPushButton *btn = new QPushButton(emptyWgt);
    pag::Color useColor = v.has_value() ? v.value() : defaultV;
    btn->setStyleSheet(QString("background-color: rgb(%1, %2, %3)").arg(useColor.red).arg(useColor.green).arg(useColor.blue));
    btn->setFixedSize(QSize(40, 20));
    layout->addStretch();
    layout->addWidget(btn);
    this->setItemWidget(wgtItem, 1, emptyWgt);
    QObject::connect(btn, &QPushButton::clicked, [this, btn, &v]() {
        QColor color = QColorDialog::getColor(Qt::white, btn);
        if (!color.isValid()) {
            return;
        }
        v = {uint8_t(color.red()), uint8_t(color.green()), uint8_t(color.blue())};
        btn->setStyleSheet(QString("background-color: rgb(%1, %2, %3)").arg(v.value().red).arg(v.value().green).arg(v.value().blue));
        updateSceneProperty();
    });
    return wgtItem;
}

// 字体
QTreeWidgetItem *PropertyTreeWidget::addDocumentFontPropertyUI(QTreeWidgetItem *parent, const pag::PAGFont &defaultFont, std::optional<std::string> &fontFamily, std::optional<std::string> &fontStyle) {
    QTreeWidgetItem *wgtItem = new QTreeWidgetItem(parent, QStringList() << "字体");
    QWidget *emptyWgt = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(emptyWgt);
    emptyWgt->setLayout(layout);

    pag::PAGFont useFont(fontFamily.has_value() ? fontFamily.value() : defaultFont.fontFamily, fontStyle.has_value() ? fontStyle.value() : defaultFont.fontStyle);

    QStringList fontFamilyList;
    // auto fontLists = MogicFontUtils::getInstance().getFontConfigList();
    // for (auto &font : fontLists) {
    //     fontFamilyList << QString::fromStdString(font.first);
    // }

    CompleterLineEdit *fontFamilyEdit = new CompleterLineEdit(fontFamilyList, QString::fromStdString(useFont.fontFamily), emptyWgt);

    QStringList fontStyleList;
    // for (auto &font : fontLists[useFont.fontFamily]) {
    //     fontStyleList << QString::fromStdString(font);
    // }

    CompleterLineEdit *fontStyleEdit = new CompleterLineEdit(fontStyleList, QString::fromStdString(useFont.fontStyle), emptyWgt);
    layout->addWidget(fontFamilyEdit);
    layout->addWidget(fontStyleEdit);

    QObject::connect(fontFamilyEdit, &CompleterLineEdit::textChoiceFinished, [fontStyleEdit](const QString &str) {
        QStringList fontStyleList;
        // auto fontLists = MogicFontUtils::getInstance().getFontConfigList();
        // for (auto &font : fontLists[str.toStdString()]) {
        //     fontStyleList << QString::fromStdString(font);
        // }
        fontStyleEdit->updateStringList(fontStyleList);
    });

    QObject::connect(fontStyleEdit, &CompleterLineEdit::textChoiceFinished, [this, fontFamilyEdit, &fontFamily, &fontStyle](const QString &str) {
        fontFamily = fontFamilyEdit->text().toStdString();
        fontStyle = str.toStdString();
        // 更新字体属性
        updateSceneProperty();
    });

    this->setItemWidget(wgtItem, 1, emptyWgt);

    return wgtItem;
}

void PropertyTreeWidget::addVectorPropertyUI(QTreeWidgetItem *parent, std::vector<DoubleSliderInfo> info) {
    for (auto &item : info) {
        auto [wgtItem, doubleSlider, doubleSpinBox] = createDoubleTreeWidgetItem(parent, 3, item.name, item.minV, item.maxV);
        doubleSpinBox->setValue(item.data == nullptr ? item.minV : double(*item.data));
        doubleSlider->setValue(item.data == nullptr ? item.minV : double(*item.data));
        QObject::connect(doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this, item](double v) {
            if (item.data == nullptr) {
                return;
            }
            *item.data = float(v);
            // 需要更新属性
            updateSceneProperty();
        });
    }
}

std::tuple<QTreeWidgetItem *, DoubleSliderWidget *, QDoubleSpinBox *> PropertyTreeWidget::createDoubleTreeWidgetItem(QTreeWidgetItem *parent, int decimals, const QString &name, double min, double max) {
    QTreeWidgetItem *wgtItem = new QTreeWidgetItem(parent, QStringList() << name);
    // 创建水平布局
    QWidget *emptyWgt = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(emptyWgt);
    emptyWgt->setLayout(layout);

    // 添加滑条
    DoubleSliderWidget *doubleSlider = new DoubleSliderWidget(this);
    doubleSlider->setDecimals(decimals);
    doubleSlider->setRange(min, max); // 范围
    doubleSlider->setOrientation(Qt::Horizontal);
    layout->addWidget(doubleSlider);
    // 添加float编辑框
    QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox(this);
    doubleSpinBox->setRange(min, max); // 范围
    doubleSpinBox->setDecimals(decimals);
    doubleSpinBox->setMaximumWidth(50);
    layout->addWidget(doubleSpinBox);
    this->setItemWidget(wgtItem, 1, emptyWgt);
    QObject::connect(doubleSlider, QOverload<double>::of(&DoubleSliderWidget::valueChanged), doubleSpinBox, &QDoubleSpinBox::setValue);
    QObject::connect(doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), doubleSlider, &DoubleSliderWidget::setValue);
    return {wgtItem, doubleSlider, doubleSpinBox};
}

void PropertyTreeWidget::updateSceneProperty() {
    mFootage->flush(mFootage->currentTime());
    emit GlobalMsgMgr::getInstance().updateProperty();
}

} // namespace DM