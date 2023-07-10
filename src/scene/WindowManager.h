#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "StartWindow.h"
#include "EditorWindow.h"

namespace DM {

class WindowManager : public QWidget {
    Q_OBJECT
public:
    WindowManager(QWidget *parent = nullptr);

    ~WindowManager();

private:
    StartWindow startWindow{};
    EditorWindow editorWindow{};

private slots:
    void openScene(const QString &jstr);
};
} // namespace DM

#endif // WindowManager
