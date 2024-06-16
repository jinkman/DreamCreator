#include "FootageWidget.h"
#include "DividingRule.h"
#include "utils/GlobalMsgMgr.h"
#include "rendering/footage/PAGFootage.h"
#include "rendering/footage/ImageFootage.h"
#include "rendering/footage/VideoFootage.h"
#include <QPainter>
#include <QLabel>
#include <QIcon>
#include <QScrollBar>
#include <QMouseEvent>

namespace DM {

const float footageWidth = 50.0f;

ImageLabel::ImageLabel(QWidget *parent) :
    QLabel(parent) {
    this->setStyleSheet(QString("QLabel{\n"
                                "   color : white;\n"
                                "   font-size : 10px;\n"
                                "}\n"));
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
    QWidget(parent) {
    setupWidget();
    this->setFixedHeight(footageWidth);
    this->setStyleSheet(QString("QWidget{\n"
                                "   border-radius: 4px;\n"
                                "   background-color: rgb(0, 77, 88);\n"
                                "}\n"));
}

FootageWidget::~FootageWidget() {
}

void FootageWidget::updateFootage(Footage *footage) {
    mFootage = footage;
    int totalWidth = std::ceil(mFootage->duration() / 1000.0f * oneSecondNoScaleStep);
    this->setFixedWidth(totalWidth);
    auto remoteUrl = mFootage->getResourcesLocalPath();
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

Footage *FootageWidget::getFootage() {
    return mFootage;
}

void FootageWidget::setupPAGFootage(Footage *footage) {
    auto pagFootage = static_cast<PAGFootage *>(footage);
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
        int oneWidth = getOneWidth(footageWid, footageHei);
        int num = std::ceil(float(this->width()) / float(oneWidth));

        auto resStartTime = pagFootage->getResStartTime();
        auto resEndTime = pagFootage->getResEndTime();
        auto oldProgress = pagFootage->getProgress();

        std::vector<QPixmap> mapList(num);
        for (int index = 0; index < num; index++) {
            int offsetWidth = index * oneWidth;
            DMTime offsetTime = resStartTime + (resEndTime - resStartTime) * (float(offsetWidth) / float(this->width()));
            double progress = double(1000 * offsetTime) / double(pagFile->duration());
            pagFootage->setPAGProgressAndFlush(progress);
            cv::Mat frame(footageHei, footageWid, CV_8UC4, cv::Scalar(0, 0, 0, 0));
            pagFootage->readPixels(pag::ColorType::RGBA_8888, pag::AlphaType::Premultiplied, (unsigned char *)frame.data, footageWid * 4);
            QImage qImg((uchar *)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGBA8888);
            QPixmap pixmap = QPixmap::fromImage(qImg);
            // 重置大小
            pixmap = pixmap.scaled(QSize(oneWidth, this->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            mapList[index] = std::move(pixmap);
        }
        createImageLabel(mapList, oneWidth);
        pagFootage->setPAGProgressAndFlush(oldProgress);
    }
}

void FootageWidget::setupImageFootage(Footage *footage) {
    auto localPath = footage->getResourcesLocalPath();
    QPixmap pixmap(QString::fromStdString(localPath));
    if (pixmap.isNull()) {
        // 错误处理
        this->setToolTip("video open error:" + QString::fromStdString(footage->getResourcesLocalPath()));
        return;
    }
    int footageWid = pixmap.width();  // 获取宽度
    int footageHei = pixmap.height(); // 获取高度
    int oneWidth = getOneWidth(footageWid, footageHei);
    pixmap = pixmap.scaled(QSize(oneWidth, this->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    createImageLabel({std::move(pixmap)}, oneWidth);
}

void FootageWidget::setupVideoFootage(Footage *footage) {
    // 打开视频
    auto videoFootage = static_cast<VideoFootage *>(footage);
    auto localPath = videoFootage->getResourcesLocalPath();
    // 指定图片路径
    // 使用imread函数读取图片
    cv::Mat frame = cv::imread(localPath, cv::IMREAD_COLOR);

    if (!frame.empty()) {
        int oneWidth = getOneWidth(frame.cols, frame.rows);
        QImage::Format format = (frame.channels() == 3) ? QImage::Format_RGB888 : QImage::Format_Grayscale8;
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB); // 如果是彩色图像，需要从BGR转至RGB
        QImage qImg((uchar *)frame.data, frame.cols, frame.rows, frame.step, format);
        QPixmap pixmap = QPixmap::fromImage(qImg);
        // 重置大小
        pixmap = pixmap.scaled(QSize(oneWidth, this->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        createImageLabel({std::move(pixmap)}, oneWidth);
    } else {
        cv::VideoCapture cap(localPath);
        if (!cap.isOpened()) {
            // 错误处理
            this->setToolTip("video open error:" + QString::fromStdString(videoFootage->getResourcesLocalPath()));
            return;
        }
        cv::Mat frame;
        // 获取视频帧，根据宽高比处理
        int footageWid = cap.get(cv::CAP_PROP_FRAME_WIDTH);  // 获取宽度
        int footageHei = cap.get(cv::CAP_PROP_FRAME_HEIGHT); // 获取高度
        int oneWidth = getOneWidth(footageWid, footageHei);
        int num = std::ceil(float(this->width()) / float(oneWidth));

        auto resStartTime = videoFootage->getResStartTime();
        auto resEndTime = videoFootage->getResEndTime();
        DMTime lastTime = 0;
        std::vector<QPixmap> mapList(num);
        for (int i = 0; i < num; i++) {
            int offsetWidth = i * oneWidth;
            // 确定时长
            DMTime offsetTime = resStartTime + (resEndTime - resStartTime) * (float(offsetWidth) / float(this->width()));
            if (lastTime - offsetTime != 0) { // 设置开始帧
                cap.set(cv::CAP_PROP_POS_MSEC, offsetTime);
                lastTime = offsetTime;
            }
            // 读取视频一帧
            cap.read(frame);

            QImage::Format format = (frame.channels() == 3) ? QImage::Format_RGB888 : QImage::Format_Grayscale8;
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB); // 如果是彩色图像，需要从BGR转至RGB
            QImage qImg((uchar *)frame.data, frame.cols, frame.rows, frame.step, format);
            QPixmap pixmap = QPixmap::fromImage(qImg);
            // 重置大小
            pixmap = pixmap.scaled(QSize(oneWidth, this->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            mapList[i] = std::move(pixmap);
        }
        cap.release();
        createImageLabel(mapList, oneWidth);
    }
}

void FootageWidget::setupWidget() {
    mMainLayout = new QHBoxLayout(this);
    mMainLayout->setContentsMargins(0, 0, 0, 0);
    mMainLayout->setSpacing(0);
    this->setLayout(mMainLayout);
}

void FootageWidget::enterEvent(QEvent *event) {
    for (auto label : mLabelVec) {
        label->setIsIn(true);
    }
    QWidget::enterEvent(event);
}

void FootageWidget::leaveEvent(QEvent *event) {
    for (auto label : mLabelVec) {
        label->setIsIn(false);
    }
    QWidget::leaveEvent(event);
}

void FootageWidget::mousePressEvent(QMouseEvent *event) {
    // 点击事件
    if (event->button() == Qt::LeftButton) {
        emit GlobalMsgMgr::getInstance().clickUpFootageSignal(getFootage());
    }
}

int FootageWidget::getOneWidth(const int &footageWid, const int &footageHei) {
    float ratioh = float(this->height() / float(footageHei));
    return std::ceil(footageWid * ratioh);
}

void FootageWidget::createImageLabel(const std::vector<QPixmap> &mapList, int oneWidth) {
    int num = std::ceil(float(this->width()) / float(oneWidth));
    int lastWidth = 0;
    for (int i = 0; i < num; i++) {
        int mapIndex = std::min(i, int(mapList.size() - 1));
        auto &map = mapList.at(mapIndex);
        ImageLabel *label = new ImageLabel(this);
        int width = (lastWidth + oneWidth) > this->width() ? this->width() - lastWidth : oneWidth;
        lastWidth += width;
        label->setFixedSize(width, this->height());
        // 裁剪图片
        label->setPixmap(width == oneWidth ? map : map.copy(QRect(0, 0, width, map.height())));
        label->setAlignment(Qt::AlignCenter);
        label->setScaledContents(true);
        mMainLayout->addWidget(label);
        mLabelVec.emplace_back(label);
    }
}

} // namespace DM