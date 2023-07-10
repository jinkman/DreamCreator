#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include "Layer/RootNode.h"

namespace DM {
// 2.继承相关类
class OpenGLWidget : public QOpenGLWidget {
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);

    ~OpenGLWidget();

    void initSceneByFile(const QString &jstr);

    void closeScene();

    void setProgress(float v);

    float getProgress();

protected:
    // 3.重载相关虚函数
    virtual void initializeGL();

    virtual void resizeGL(int w, int h);

    virtual void paintGL();

private:
    float mProgress = 0.0f;
    // rootNode
    std::shared_ptr<RootNode> mRootNode = nullptr;
};

} // namespace DM

#endif // OPENGLWIDGET_H