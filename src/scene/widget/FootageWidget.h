#ifndef FOOTAGE_WIDGET_H
#define FOOTAGE_WIDGET_H

#include <QScrollArea>
#include <QLabel>
#include <QHBoxLayout>
#include "rendering/footage/Footage.h"
namespace DM {

class ImageLabel : public QLabel {
    Q_OBJECT
public:
    ImageLabel(QWidget *parent = 0);

    ~ImageLabel();

    void setIsIn(bool v);

private:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    bool mIsIn;
};

class FootageWidget : public QScrollArea {
    Q_OBJECT
public:
    FootageWidget(QWidget *parent = 0);
    ~FootageWidget();

    void updateFootage(std::shared_ptr<Footage> footage);

    std::shared_ptr<Footage> getFootage();

signals:
    void clickUpFootageSignal(std::shared_ptr<Footage> foogate);

protected:
    void setupSubtitleFootage(std::shared_ptr<Footage> footage);

    void setupImageFootage(std::shared_ptr<Footage> footage);

    void setupVideoFootage(std::shared_ptr<Footage> footage);

    void setupWidget();

    virtual void paintEvent(QPaintEvent *event) override;

    virtual void enterEvent(QEvent *event) override;

    virtual void leaveEvent(QEvent *event) override;

    virtual void mousePressEvent(QMouseEvent *event) override;

private:
    QHBoxLayout *mMainLayout = nullptr;
    std::vector<ImageLabel *> mLabelVec;
    std::shared_ptr<Footage> mFootage = nullptr;
};

} // namespace DM

#endif // FOOTAGE_WIDGET_H