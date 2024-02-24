#ifndef COMPETER_LINE_EDIT_H
#define COMPETER_LINE_EDIT_H

#include <QLineEdit>
#include <QStringListModel>
#include <QStringList>
#include <QTimer>

namespace DM {

class CompleterLineEdit : public QLineEdit {
    Q_OBJECT
public:
    explicit CompleterLineEdit(const QStringList &list, const QString &text, QWidget *parent = 0);

    ~CompleterLineEdit();

    void updateStringList(const QStringList &list);

signals:
    void textChoiceFinished(const QString &str);

private:
    void focusInEvent(QFocusEvent *e);
    void customEvent(QEvent *e);

private:
    QString mOldText;
};
} // namespace DM
#endif // COMPETER_LINE_EDIT_H