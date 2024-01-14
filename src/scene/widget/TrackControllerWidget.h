
#ifndef TRACK_CONTROLLER_WIDGET_H
#define TRACK_CONTROLLER_WIDGET_H
#include <QWidget>
#include <QHBoxLayout>

namespace DM {

class TrackControllerWidget : public QWidget {
    Q_OBJECT
public:
    explicit TrackControllerWidget(QWidget *parent = nullptr);

    ~TrackControllerWidget();

private:
    QHBoxLayout *mLayout = nullptr;
};
} // namespace DM

#endif // TRACK_CONTROLLER_WIDGET_H
