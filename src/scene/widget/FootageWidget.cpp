#include "FootageWidget.h"
#include <QPainter>
#include <QLabel>
#include <QIcon>
#include <QScrollBar>
#include <QMouseEvent>
#include <common/Common.h>
#include "DividingRule.h"
#include "rendering/footage/PAGFootage.h"
// #include "rendering/footage/ImageFootage.h"
#include "rendering/footage/VideoFootage.h"

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
        painter.fillRect(this->rect(), {255, 255, 255, 100});
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
    mFootage = footage;
    int totalWidth = std::ceil(mFootage->duration() / 1000.0f * oneSecondNoScaleStep);
    this->setFixedWidth(totalWidth);
    auto remoteUrl = mFootage->getResourcesOriginPath();
    // 提示内容
    this->setToolTip(QString::fromStdString(remoteUrl));
    switch (mFootage->getFootageType()) {
    case EFootageType::EIMAGE_FOOTAGE:
        setupImageFootage(mFootage);
        break;
    case EFootageType::EPAG_FOOTAGE:
        setupPAGFootage(mFootage);
        break;
    case EFootageType::EVIDEO_FOOTAGE:
        setupVideoFootage(mFootage);
        break;

    default:
        break;
    }
}

std::shared_ptr<Footage> FootageWidget::getFootage() {
    return mFootage;
}

void FootageWidget::setupPAGFootage(std::shared_ptr<Footage> footage) {
    auto pagFootage = std::static_pointer_cast<PAGFootage>(footage);
    auto pagFile = pagFootage->getPAGFile();
    // 字幕则展示文本
    if (pagFootage->getPAGType() == EPAGType::EPAG_SUBTITLE) {
        auto layers = pagFile->getLayersByEditableIndex(0, pag::LayerType::Text);
        ImageLabel *label = new ImageLabel(this);
        label->setFixedSize(this->size());
        if (!layers.empty()) {
            auto firstTextLayer = std::static_pointer_cast<pag::PAGTextLayer>(layers.at(0));
            label->setText(QString::fromStdString(firstTextLayer->text()));
        } else {
            this->setToolTip("pagfile have no editable text layer" + QString::fromStdString(footage->getResourcesOriginPath()));
        }
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);
        mMainLayout->addWidget(label);
        mLabelVec.emplace_back(label);
    } else { // 组合则展示内容
        auto footageWid = pagFile->width();
        auto footageHei = pagFile->height();
        auto resStartTime = pagFootage->getResStartTime();
        auto resEndTime = pagFootage->getResEndTime();
        auto oldProgress = pagFootage->getProgress();
        initImageFrameByRecorder(footageWid, footageHei, [this, pagFootage, pagFile, &resStartTime, &resEndTime, &footageWid, &footageHei](int num, int index, int oneWidth) -> cv::Mat {
            cv::Mat frame(footageHei, footageWid, CV_8UC4, cv::Scalar(0, 0, 0, 0));
            int offsetWidth = index * oneWidth;
            auto offsetTime = resStartTime + (resEndTime - resStartTime) * (float(offsetWidth) / float(this->width()));
            double progress = double(1000 * offsetTime) / double(pagFile->duration());
            pagFootage->setPAGProgressAndFlush(progress);
            pagFootage->readPixels(pag::ColorType::RGBA_8888, pag::AlphaType::Premultiplied, (unsigned char *)frame.data, footageWid * 4);
            return frame;
        });
        pagFootage->setPAGProgressAndFlush(oldProgress);
    }
}

void FootageWidget::setupImageFootage(std::shared_ptr<Footage> footage) {
    auto localPath = footage->getResourcesLocalPath();
    QPixmap pixmap(QString::fromStdString(localPath));
    if (pixmap.isNull()) {
        // 错误处理
        this->setToolTip("video open error:" + QString::fromStdString(footage->getResourcesOriginPath()));
        return;
    }
    int footageWid = pixmap.width();  // 获取宽度
    int footageHei = pixmap.height(); // 获取高度
    float whRatio = float(footageWid) / float(footageHei);
    int oneWidth = std::ceil(oneSecondNoScaleStep * std::min(whRatio, 1.0f));
    int num = std::ceil(float(this->width()) / float(oneWidth));
    // 重置大小
    pixmap = pixmap.scaled(QSize(oneWidth, this->height()) * 4, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    for (int i = 0; i < num; i++) {
        ImageLabel *label = new ImageLabel(this);
        label->setFixedSize(oneWidth, this->height());
        label->setPixmap(pixmap);
        label->setAlignment(Qt::AlignCenter);
        label->setScaledContents(true);
        mMainLayout->addWidget(label);
        mLabelVec.emplace_back(label);
    }
}

void FootageWidget::setupVideoFootage(std::shared_ptr<Footage> footage) {
    // 打开视频
    auto videoFootage = std::static_pointer_cast<VideoFootage>(footage);
    auto localPath = videoFootage->getResourcesLocalPath();
    cv::VideoCapture cap(localPath);
    if (!cap.isOpened()) {
        // 错误处理
        this->setToolTip("video open error:" + QString::fromStdString(videoFootage->getResourcesOriginPath()));
        return;
    }

    // 获取视频帧，根据宽高比处理
    int footageWid = cap.get(cv::CAP_PROP_FRAME_WIDTH);  // 获取宽度
    int footageHei = cap.get(cv::CAP_PROP_FRAME_HEIGHT); // 获取高度
    auto resStartTime = videoFootage->getResStartTime();
    auto resEndTime = videoFootage->getResEndTime();
    initImageFrameByRecorder(footageWid, footageHei, [this, &cap, &resStartTime, &resEndTime](int num, int index, int oneWidth) -> cv::Mat {
        cv::Mat frame;
        int offsetWidth = index * oneWidth;
        auto offsetTime = resStartTime + (resEndTime - resStartTime) * (float(offsetWidth) / float(this->width()));
        cap.set(cv::CAP_PROP_POS_MSEC, offsetTime);
        // 读取视频一帧
        cap.read(frame);
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB); // 如果是彩色图像，需要从BGR转至RGB
        return frame;
    });

    cap.release();
}

void FootageWidget::setupWidget() {
    QWidget *mainWidget = new QWidget(this);
    mMainLayout = new QHBoxLayout(mainWidget);
    mMainLayout->setContentsMargins(0, 0, 0, 0);
    mMainLayout->setSpacing(0);
    mainWidget->setLayout(mMainLayout);
    this->setWidget(mainWidget);
}

void FootageWidget::initImageFrameByRecorder(int footageWid, int footageHei, FrameRecordFunc frameRecord) {
    float whRatio = float(footageWid) / float(footageHei);
    int oneWidth = std::ceil(oneSecondNoScaleStep * std::min(whRatio, 1.0f));
    int num = std::ceil(float(this->width()) / float(oneWidth));
    for (int i = 0; i < num; i++) {
        ImageLabel *label = new ImageLabel(this);
        label->setFixedSize(oneWidth, this->height());
        cv::Mat frame = frameRecord(num, i, oneWidth);
        QImage::Format format = (frame.channels() == 3) ? QImage::Format_RGB888 : QImage::Format_RGBA8888;
        QImage qImg((uchar *)frame.data, frame.cols, frame.rows, frame.step, format);
        QPixmap pixmap = QPixmap::fromImage(qImg);
        // 重置大小
        pixmap = pixmap.scaled(label->size() * 4, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(pixmap);
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);
        mMainLayout->addWidget(label);
        mLabelVec.emplace_back(label);
    }
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