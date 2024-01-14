#ifndef PROPERTYWINDOW_H
#define PROPERTYWINDOW_H

#include "ContentWindow.h"
#include "rendering/footage/Footage.h"
#include "scene/widget/qtpropertybrowser/qttreepropertybrowser.h"
#include "scene/widget/qtpropertybrowser/qtvariantproperty.h"

namespace DM {

class PropertyWindow : public ContentWindow {
    Q_OBJECT
public:
    explicit PropertyWindow(QWidget *parent = 0);

    ~PropertyWindow();

public slots:
    void clickUpFootageSlot(std::shared_ptr<Footage> footage);

signals:
    void updateSceneSignal();

private:
    void setup();

    void updateScene();

private:
    QtTreePropertyBrowser *mTreeBrowser = nullptr;
    QtVariantPropertyManager *mEditManager;
    QMap<QtProperty *, QString> mPropertyToQString;
};

} // namespace DM

#endif // PROPERTYWINDOW_H