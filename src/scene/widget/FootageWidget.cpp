#include "FootageWidget.h"
#include <QPainter>
#include <QLabel>
#include <QIcon>
#include <QScrollBar>
#include <QMouseEvent>
#include <common/Common.h>
#include "DividingRule.h"
// #include "rendering/footage/PAGFootage.h"
// #include "rendering/footage/ImageFootage.h"
// #include "rendering/footage/VideoFootage.h"

namespace DM {

ImageLabel::ImageLabel(QWidget *parent) :
    QLabel(parent) {
}

ImageLabel::~ImageLabel() {
}

void ImageLabel::setIsIn(bool v) {
    if (v == mIsIn) {
        return;
    }
    mIsIn = v;
    this->update();
}

void ImageLabel::paintEvent(QPaintEvent *event) {
    QLabel::paintEvent(event);
    if (mIsIn) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(this->rect(), {27, 27, 28, 100});
    }
}

FootageWidget::FootageWidget(QWidget *parent) :
    QScrollArea(parent) {
    setAttribute(Qt::WA_TranslucentBackground);
    setupWidget();

    this->setFixedHeight(50);
    this->setWidgetResizable(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏横向滚动条
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 隐藏竖向滚动条
    this->setFrameShape(QFrame::NoFrame);
    this->horizontalScrollBar()->setSliderPosition(0);
    this->verticalScrollBar()->setSliderPosition(0);
    this->horizontalScrollBar()->setDisabled(true);
    this->setStyleSheet(QString("QWidget{\n"
                                "   border-radius: 4px;\n"
                                "   background-color: rgb(0, 77, 88);\n"
                                "}\n"
                                "QLabel{\n"
                                "   color : white;\n"
                                "   font-size : 10px;\n"
                                "}\n"));
}

FootageWidget::~FootageWidget() {
}

void FootageWidget::updateFootage(std::shared_ptr<Footage> footage) {
    // mFootage = footage;
    // int totalWidth = std::ceil(mFootage->duration() / 1000.0f * oneSecondNoScaleStep);
    // this->setFixedWidth(totalWidth);
    // auto remoteUrl = mFootage->getResourcesRealPath();
    // // 提示内容
    // this->setToolTip(QString::fromStdString(remoteUrl));
    // switch (mFootage->getFootageType()) {
    // case EFootageType::EIMAGE_FOOTAGE:
    //     setupImageFootage(mFootage);
    //     break;
    // case EFootageType::EPAG_FOOTAGE:
    //     setupSubtitleFootage(mFootage);
    //     break;
    // case EFootageType::EVIDEO_FOOTAGE:
    //     setupVideoFootage(mFootage);
    //     break;

    // default:
    //     break;
    // }
}

std::shared_ptr<Footage> FootageWidget::getFootage() {
    return mFootage;
}

void FootageWidget::setupSubtitleFootage(std::shared_ptr<Footage> footage) {
    // auto replaceText = std::static_pointer_cast<PAGFootage>(footage)->getReplaceText();
    // ImageLabel *label = new ImageLabel(this);
    // label->setFixedSize(this->size());
    // label->setText(QString::fromStdString(replaceText));
    // label->setAlignment(Qt::AlignCenter);
    // label->setWordWrap(true);
    // mMainLayout->addWidget(label);
    // mLabelVec.emplace_back(label);
}

void FootageWidget::setupImageFootage(std::shared_ptr<Footage> footage) {
    // auto localPath = footage->getResourcesLocalPath();
    // QPixmap pixmap(QString::fromStdString(localPath));
    // if (pixmap.isNull()) {
    //     // 错误处理
    //     this->setToolTip("video open error:" + QString::fromStdString(footage->getResourcesRealPath()));
    //     return;
    // }
    // int footageWid = pixmap.width();  // 获取宽度
    // int footageHei = pixmap.height(); // 获取高度
    // float whRatio = float(footageWid) / float(footageHei);
    // int oneWidth = std::ceil(oneSecondNoScaleStep * std::min(whRatio, 1.0f));
    // int num = std::ceil(float(this->width()) / float(oneWidth));
    // // 重置大小
    // pixmap = pixmap.scaled(QSize(oneWidth, this->height()) * 4, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // for (int i = 0; i < num; i++) {
    //     ImageLabel *label = new ImageLabel(this);
    //     label->setFixedSize(oneWidth, this->height());
    //     label->setPixmap(pixmap);
    //     label->setAlignment(Qt::AlignCenter);
    //     label->setScaledContents(true);
    //     mMainLayout->addWidget(label);
    //     mLabelVec.emplace_back(label);
    // }
}

void FootageWidget::setupVideoFootage(std::shared_ptr<Footage> footage) {
    // 打开视频
    // auto videoFootage = std::static_pointer_cast<VideoFootage>(footage);
    // auto localPath = videoFootage->getResourcesLocalPath();
    // cv::VideoCapture cap(localPath);
    // if (!cap.isOpened()) {
    //     // 错误处理
    //     this->setToolTip("video open error:" + QString::fromStdString(videoFootage->getResourcesRealPath()));
    //     return;
    // }
    // cv::Mat frame;
    // // 获取视频帧，根据宽高比处理
    // int footageWid = cap.get(cv::CAP_PROP_FRAME_WIDTH);  // 获取宽度
    // int footageHei = cap.get(cv::CAP_PROP_FRAME_HEIGHT); // 获取高度
    // float whRatio = float(footageWid) / float(footageHei);
    // int oneWidth = std::ceil(oneSecondNoScaleStep * std::min(whRatio, 1.0f));
    // int num = std::ceil(float(this->width()) / float(oneWidth));

    // auto resStartTime = videoFootage->getResStartTime();
    // auto resEndTime = videoFootage->getResEndTime();
    // int lastTime = 0;
    // for (int i = 0; i < num; i++) {
    //     ImageLabel *label = new ImageLabel(this);
    //     label->setFixedSize(oneWidth, this->height());
    //     int offsetWidth = i * oneWidth;
    //     // 确定时长
    //     auto offsetTime = resStartTime + (resEndTime - resStartTime) * (float(offsetWidth) / float(this->width()));
    //     if (lastTime - offsetTime != 0) { // 设置开始帧
    //         cap.set(cv::CAP_PROP_POS_MSEC, offsetTime);
    //         lastTime = offsetTime;
    //     }
    //     // 读取视频一帧
    //     cap.read(frame);

    //     QImage::Format format = (frame.channels() == 3) ? QImage::Format_RGB888 : QImage::Format_Grayscale8;
    //     cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB); // 如果是彩色图像，需要从BGR转至RGB
    //     QImage qImg((uchar *)frame.data, frame.cols, frame.rows, frame.step, format);
    //     QPixmap pixmap = QPixmap::fromImage(qImg);
    //     // 重置大小
    //     pixmap = pixmap.scaled(label->size() * 4, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //     label->setPixmap(pixmap);
    //     label->setScaledContents(true);
    //     label->setAlignment(Qt::AlignCenter);
    //     mMainLayout->addWidget(label);
    //     mLabelVec.emplace_back(label);
    // }
    // cap.release();
}

void FootageWidget::setupWidget() {
    QWidget *mainWidget = new QWidget(this);
    mMainLayout = new QHBoxLayout(mainWidget);
    mMainLayout->setContentsMargins(0, 0, 0, 0);
    mMainLayout->setSpacing(0);
    mainWidget->setLayout(mMainLayout);
    this->setWidget(mainWidget);
}

void FootageWidget::paintEvent(QPaintEvent *event) {
    QScrollArea::paintEvent(event);
}

void FootageWidget::enterEvent(QEvent *event) {
    for (auto label : mLabelVec) {
        label->setIsIn(true);
    }
    QScrollArea::enterEvent(event);
}

void FootageWidget::leaveEvent(QEvent *event) {
    for (auto label : mLabelVec) {
        label->setIsIn(false);
    }
    QScrollArea::enterEvent(event);
}

void FootageWidget::mousePressEvent(QMouseEvent *event) {
    // 点击事件
    if (event->button() == Qt::LeftButton) {
        emit clickUpFootageSignal(getFootage());
    }
}

} // namespace DM