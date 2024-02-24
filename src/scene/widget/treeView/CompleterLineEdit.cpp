#include "CompleterLineEdit.h"
#include <QCompleter>
#include <QEvent>
#include <QApplication>

namespace DM {
CompleterLineEdit::CompleterLineEdit(const QStringList &list, const QString &text, QWidget *parent) :
    QLineEdit(parent) {
    setCompleter(new QCompleter(list, this));
    setAlignment(Qt::AlignCenter);
    completer()->setCaseSensitivity(Qt::CaseInsensitive);
    mOldText = text;
    this->setText(text);

    QObject::connect(this, &QLineEdit::textChanged, [this](const QString &str) {
        completer()->setCompletionMode(str.isEmpty() ? QCompleter::UnfilteredPopupCompletion : QCompleter::PopupCompletion);
        if (str.isEmpty()) {
            QApplication::postEvent(this, new QEvent(QEvent::User));
        }
    });

    QObject::connect(this, &QLineEdit::editingFinished, [this]() {
        QStringListModel *model = qobject_cast<QStringListModel *>(this->completer()->model());
        if (model == nullptr) {
            return;
        }
        QString curText = this->text();
        if (!model->stringList().contains(curText)) {
            this->setText(mOldText);
        } else {
            this->mOldText = curText;
            // emit signal
            emit textChoiceFinished(curText);
        }
    });
}

CompleterLineEdit::~CompleterLineEdit() {
}

void CompleterLineEdit::updateStringList(const QStringList &list) {
    completer()->setModel(new QStringListModel(list, completer()));
    QString curText = text();
    if (!list.contains(curText)) {
        setText(list.at(0));
    }
    mOldText = text();
    emit textChoiceFinished(text());
}

void CompleterLineEdit::focusInEvent(QFocusEvent *e) {
    QLineEdit::focusInEvent(e);
    completer()->complete();
}

void CompleterLineEdit::customEvent(QEvent *e) {
    QLineEdit::customEvent(e);
    completer()->complete();
}
} // namespace DM