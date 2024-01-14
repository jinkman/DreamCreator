
#ifndef CONTAINER_WIDGET_H
#define CONTAINER_WIDGET_H
#include <QScrollArea>
#include <QVBoxLayout>

namespace DM {

class ContainerWidget : public QScrollArea {
    Q_OBJECT
public:
    explicit ContainerWidget(QWidget *parent = nullptr);

    ~ContainerWidget();

    QVBoxLayout *getMainLayout();

    void clear();

    void resetScrollValue();

private:
    void setupWidget();

private:
    // 垂直布局
    QVBoxLayout *mMainLayout = nullptr;
};
} // namespace DM

#endif // CONTAINER_WIDGET_H
