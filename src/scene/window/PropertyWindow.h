#ifndef PROPERTYWINDOW_H
#define PROPERTYWINDOW_H

#include "ContentWindow.h"
#include "rendering/footage/Footage.h"
#include "scene/widget/treeView/PropertyTreeWidget.h"

namespace DM {

class PropertyWindow : public ContentWindow {
    Q_OBJECT
public:
    explicit PropertyWindow(QWidget *parent = 0);

    ~PropertyWindow();

public slots:
    void clickUpFootageSlot(Footage *footage);

private:
    void setup();

    void updateScene();

private:
    PropertyTreeWidget *mTreeBrowser = nullptr;
};

} // namespace DM

#endif // PROPERTYWINDOW_H