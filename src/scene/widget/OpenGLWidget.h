#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include "rendering/Player.h"

namespace DM {
class OpenGLWidget : public QOpenGLWidget {
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);

    ~OpenGLWidget();

    void initSceneByFile(const QString &jstr);

    void closeScene();

    void exportVideo(const QString &str);

    Player *getScenePlayer();

protected:
    // 3.重载相关虚函数
    virtual void initializeGL() override;

    virtual void resizeGL(int w, int h) override;

    virtual void paintGL() override;

private slots:

    void seekToTime(const DMTime &t);

private:
    // player
    std::shared_ptr<Player> mPlayer = nullptr;
};

} // namespace DM

#endif // OPENGLWIDGET_H