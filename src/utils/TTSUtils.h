#ifndef TTS_UTILS_H
#define TTS_UTILS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QTimer>
#include <QNetworkAccessManager>

namespace DM {

typedef std::function<void(QJsonDocument qdoc)> TTSRecordFunc;

class TTSUtils : public QObject {
    Q_OBJECT
public:
    static TTSUtils &getInstance();

    ~TTSUtils();

    void getTTsLongEmotion(const QString &text, TTSRecordFunc &recorder);

    void getTTs(const QString &text, const std::string &filePath, TTSRecordFunc &recorder);

private slots:
    void onAsyncNetworkLongEmotionPostReply(QNetworkReply *reply, TTSRecordFunc &recorder);

    bool onAsyncNetworkLongEmotionGetReply(QNetworkReply *reply, TTSRecordFunc &recorderr);

protected:
    void postQureyLongEmotion(const QString &taskID, TTSRecordFunc &recorder, QTimer *timer);

private:
    TTSUtils();
    TTSUtils(const TTSUtils &) = delete;
    const TTSUtils &operator=(const TTSUtils &) = delete;
};

} // namespace DM

#endif // GLOBAM_MSG_MGR_H