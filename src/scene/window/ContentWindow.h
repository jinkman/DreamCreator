#ifndef CONTENTWINDOW_H
#define CONTENTWINDOW_H

#include <QVBoxLayout>
#include <QLabel>

namespace DM {

class ContentWindow : public QWidget {
    Q_OBJECT
public:
    explicit ContentWindow(QWidget *parent = 0, Qt::WindowFlags flags = {});

    ~ContentWindow();

    QLabel *getTitleWidget();

    QVBoxLayout *getLayout();

protected:
    void setupWindow();

private:
    QLabel *mTitleWidget = nullptr;
    QVBoxLayout *mMainLayout = nullptr;
};

} // namespace DM

#endif // CONTENTWINDOW_H