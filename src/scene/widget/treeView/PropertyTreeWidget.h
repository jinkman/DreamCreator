
#ifndef PROPERTY_TREE_WIDGET_H
#define PROPERTY_TREE_WIDGET_H

#include "rendering/footage/PAGFootage.h"
#include "DoubleSliderWidget.h"
#include <QTreeWidget>
#include <QDoubleSpinBox>
#include <QTreeWidgetItem>
#include <QLineEdit>
#include <QCompleter>
#include <QPaintEvent>

namespace DM {

struct DoubleSliderInfo {
    float *data = nullptr;
    double minV = 0.0;
    double maxV = 100.0;
    QString name;
};

class PropertyTreeWidget : public QTreeWidget {
    Q_OBJECT
public:
    explicit PropertyTreeWidget(QWidget *parent = nullptr);

    ~PropertyTreeWidget();

    void updateUI(Footage *footage);

private:
    void updateBasePropertyUI(QTreeWidgetItem *parent);

    // 文本相关属性
    void updatePagPropertyUI(QTreeWidgetItem *parent);

    // 添加文本属性ui
    void addDocumentPropertyUI(QTreeWidgetItem *parent, std::shared_ptr<ReplaceSlice> replaceSlice);

    // 文本编辑
    QTreeWidgetItem *addDocumentTextPropertyUI(QTreeWidgetItem *parent, const std::string &defaultV, const QString &name, std::optional<std::string> &v);

    // 复选框
    QTreeWidgetItem *addDocumentBoolPropertyUI(QTreeWidgetItem *parent, const bool &defaultV, const QString &name, std::optional<bool> &v);

    // 滑条
    QTreeWidgetItem *addDocumentFloatPropertyUI(QTreeWidgetItem *parent, int decimals, const float &defaultV, const QString &name, std::optional<float> &v, double min, double max);

    QTreeWidgetItem *addDocumentFloatPropertyUI(QTreeWidgetItem *parent, int decimals, const QString &name, float &v, double min, double max);

    // 颜色
    QTreeWidgetItem *addDocumentColorPropertyUI(QTreeWidgetItem *parent, const pag::Color &defaultV, const QString &name, std::optional<pag::Color> &v);

    // 字体
    QTreeWidgetItem *addDocumentFontPropertyUI(QTreeWidgetItem *parent, const pag::PAGFont &defaultFont, std::optional<std::string> &fontFamily, std::optional<std::string> &fontStyle);

    void addVectorPropertyUI(QTreeWidgetItem *parent, std::vector<DoubleSliderInfo> info);

    void addQuatPropertyUI(QTreeWidgetItem *parent, glm::quat &quat);

private:
    std::tuple<QTreeWidgetItem *, DoubleSliderWidget *, QDoubleSpinBox *> createDoubleTreeWidgetItem(QTreeWidgetItem *parent, int decimals, const QString &name, double min, double max);

    void updateSceneProperty();

private:
    Footage *mFootage = nullptr;
};
} // namespace DM

#endif // PROPERTY_TREE_WIDGET_H
