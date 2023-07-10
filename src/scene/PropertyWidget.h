#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <QVBoxLayout>
#include <QWidget>

namespace DM {

class PropertyWidget : public QWidget {
    Q_OBJECT
public:
    explicit PropertyWidget(QWidget *parent = 0);

    ~PropertyWidget();

private:
    void setupWidget();

    void setupTopWgt();

    void setupCenterWgt();

private:
    QVBoxLayout *mMainLayout = nullptr;
};

} // namespace DM

#endif // PROPERTYWIDGET_H