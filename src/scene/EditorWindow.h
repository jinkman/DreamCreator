#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include "rendering/OpenglWidget.h"

namespace DM {
class EditorWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = {});

    void openScene(const QString &jstr);

signals:
    void closeSignal();

private:
    void setupMenuBar();

    void setupDockWidget();

    void closeEvent(QCloseEvent *e) override;

private slots:
    void saveFile();

    void exportFile();

private:
    OpenGLWidget *openglWgt = nullptr;
};

} // namespace DM

#endif // EDITORWINDOW_H
