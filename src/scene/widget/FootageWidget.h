#ifndef FOOTAGE_WIDGET_H
#define FOOTAGE_WIDGET_H

#include <QScrollArea>
#include <QLabel>
#include <QHBoxLayout>
#include "rendering/footage/Footage.h"
#include <opencv2/opencv.hpp>
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

class FootageWidget : public QWidget {
    Q_OBJECT
public:
    FootageWidget(QWidget *parent = 0);
    ~FootageWidget();

    void updateFootage(Footage *footage);

    Footage *getFootage();

protected:
    void setupPAGFootage(Footage *footage);

    void setupImageFootage(Footage *footage);

    void setupVideoFootage(Footage *footage);

    void setupWidget();

    virtual void enterEvent(QEvent *event) override;

    virtual void leaveEvent(QEvent *event) override;

    virtual void mousePressEvent(QMouseEvent *event) override;

    int getOneWidth(const int &footageWid, const int &footageHei);

    void createImageLabel(const std::vector<QPixmap> &mapList, int oneWidth);

private:
    QHBoxLayout *mMainLayout = nullptr;
    std::vector<ImageLabel *> mLabelVec;
    Footage *mFootage = nullptr;
};

} // namespace DM

#endif // FOOTAGE_WIDGET_H