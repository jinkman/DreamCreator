#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include <QVBoxLayout>
#include <QWidget>
#include "common/Common.h"

namespace DM {

static const std::vector<
    std::tuple<QString, QString, QString>>
    contentItem = {
        {"媒体", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
        {"音频", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
        {"文本", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
        {"贴纸", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
        {"特效", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
        {"转场", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
        {"滤镜", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
        {"调节", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")},
        {"模板", getQLocalPath("icon/play.png"), getQLocalPath("icon/play.png")}};

class ContentWidget : public QWidget {
    Q_OBJECT
public:
    explicit ContentWidget(QWidget *parent = 0);

    ~ContentWidget();

private:
    void setupWidget();

    void setupTopWgt();

    void setupBottomWgt();

private:
    QVBoxLayout *mMainLayout = nullptr;
};

} // namespace DM

#endif // CONTENTWIDGET_H