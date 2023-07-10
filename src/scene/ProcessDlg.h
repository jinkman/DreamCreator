#ifndef PROGRESSDLG_H
#define PROGRESSDLG_H

#include <QDialog>

#include <QProgressBar>
#include <QPushButton>

namespace DM {

class ProgressDlg : public QDialog {
    Q_OBJECT
public:
    ProgressDlg(QWidget *parent = 0);
    ~ProgressDlg();

    void setProgress(float v);

    bool isCancel() const;

private:
    void setupLayout();

private:
    QProgressBar *progressBar = nullptr;
    QPushButton *stopBtn = nullptr;
    bool bCancel = false;
};

} // namespace DM

#endif // PROGRESSDLG_H