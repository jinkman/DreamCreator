#ifndef RESOURCESWINDOW_H
#define RESOURCESWINDOW_H

#include <QPushButton>
#include "ContentWindow.h"

namespace DM {

class ResourcesWindow : public ContentWindow {
    Q_OBJECT
public:
    explicit ResourcesWindow(QWidget *parent = 0);

    ~ResourcesWindow();

private:
    void setup();

private:
};

} // namespace DM

#endif // RESOURCESWINDOW_H