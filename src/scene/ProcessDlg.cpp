#include "ProcessDlg.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace DM {

ProgressDlg::ProgressDlg(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint) {
    setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);

    setWindowTitle(tr("进度"));

    setupLayout();

    setProgress(0.0f);

    connect(stopBtn, &QPushButton::clicked, this, [=] {
        bCancel = true;
    });
}

ProgressDlg::~ProgressDlg() {
}

void ProgressDlg::setProgress(float v) {
    v = v > 1.0f ? 1.0f : v;
    v = v < 0.0f ? 0.0f : v;
    if (progressBar) {
        int progress = static_cast<int>(v * 100.0f);
        progressBar->setValue(progress);
    }
}

void ProgressDlg::setupLayout() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QWidget *widget0 = new QWidget(this);
    QHBoxLayout *hBoxLayout0 = new QHBoxLayout(widget0);
    // 进度条

    progressBar = new QProgressBar(this);
    // 设置样式
    progressBar->setStyleSheet(QString("QProgressBar::chunk{\n"
                                       "    border-radius:11px;\n"
                                       "    background:qlineargradient(spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 #01FAFF,stop:1 #26B4FF);\n"
                                       "}\n"
                                       "QProgressBar{\n"
                                       "    height:22px;\n"
                                       "    text-align:center;\n"
                                       "    font-size:14px;\n"
                                       "    color:white;\n"
                                       "    border-radius:11px;\n"
                                       "    background:#1D5573;\n"
                                       "}\n"));

    progressBar->setRange(0, 100);
    progressBar->setOrientation(Qt::Horizontal); // 水平方向
    progressBar->setMinimumSize(QSize(300, 20));

    mainLayout->addWidget(progressBar);

    stopBtn = new QPushButton(this);
    stopBtn->setText(tr("Stop"));

    hBoxLayout0->addStretch();
    hBoxLayout0->addWidget(stopBtn);
    hBoxLayout0->addStretch();
    mainLayout->addWidget(widget0);
}

bool ProgressDlg::isCancel() const {
    return bCancel;
}

} // namespace DM