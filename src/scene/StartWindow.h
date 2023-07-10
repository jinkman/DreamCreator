#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QMainWindow>

namespace DM {

class StartWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = {});

signals:
    void openSceneSignal(const QString &jstr);

private:
    void setupMenuBar();

    void setupToolBar();

    void setupDockWidget();

    void closeEvent(QCloseEvent *e) override;
private slots:
    void openFile();
};

} // namespace DM

#endif // STARTWINDOW_H
