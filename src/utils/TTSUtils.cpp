#include "TTSUtils.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QUuid>
#include <QFile>

namespace DM {

// 长文本感情预测
const QString longEmotionSubmitUrl = "https://openspeech.bytedance.com/api/v1/tts_async_with_emotion/submit";
const QString longEmotionQueryUrl = "https://openspeech.bytedance.com/api/v1/tts_async_with_emotion/query";
const QString ttsUrl = "https://openspeech.bytedance.com/api/v1/tts";

const QString appid = "id";
const QString accessToken = "token";

TTSUtils &TTSUtils::getInstance() {
    static TTSUtils instance;
    return instance;
}

TTSUtils::~TTSUtils() {
}

TTSUtils::TTSUtils() :
    QObject(nullptr) {
}

void TTSUtils::getTTs(const QString &text, const std::string &filePath, TTSRecordFunc &recorder) {
    QNetworkRequest request(ttsUrl);
    // 添加HTTP头
    QString authorization = QString("Bearer;") + accessToken;
    request.setRawHeader("Authorization", authorization.toUtf8());
    request.setRawHeader("Content-Type", "application/json");

    QUuid uuid = QUuid::createUuid();
    QString uuidString = uuid.toString();

    QJsonDocument doc;
    QJsonObject jsonData;
    QJsonObject app;
    app.insert("appid", appid);
    app.insert("token", accessToken);
    app.insert("cluster", "volcano_tts");
    jsonData.insert("app", app);
    QJsonObject user;
    user.insert("uid", uuidString);
    jsonData.insert("user", user);
    QJsonObject audio;
    audio.insert("voice_type", "BV701_streaming");
    audio.insert("encoding", "mp3");
    audio.insert("speed_ratio", 1);
    audio.insert("volume_ratio", 1);
    audio.insert("pitch_ratio", 1);
    jsonData.insert("audio", audio);
    QJsonObject requestJson;
    requestJson.insert("text", text);
    requestJson.insert("reqid", uuidString);
    requestJson.insert("text_type", "plain");
    requestJson.insert("operation", "query");
    requestJson.insert("with_frontend", 1);
    requestJson.insert("frontend_type", "unitTson");
    jsonData.insert("request", requestJson);
    doc.setObject(jsonData);

    QNetworkAccessManager *manager = new QNetworkAccessManager(nullptr);
    connect(manager, &QNetworkAccessManager::finished, this, [this, &recorder, filePath, manager](QNetworkReply *reply) {
        if (reply->error()) {
            qDebug() << reply->errorString();
            // 清理资源
            reply->deleteLater();
            QJsonDocument jsonDocument = QJsonDocument::fromJson("{}");
            recorder(jsonDocument);
            return;
        }

        QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
        auto data = jsonDocument["data"].toString();

        QByteArray byteData = QByteArray::fromBase64(data.toUtf8());
        QFile file(QString::fromStdString(filePath));
        file.open(QIODevice::WriteOnly);
        // 写入数据并关闭文件
        file.write(byteData);
        file.close();
        manager->deleteLater();
        // 成功
        recorder(jsonDocument);
    });

    manager->post(request, doc.toJson());
}

void TTSUtils::getTTsLongEmotion(const QString &text, TTSRecordFunc &recorder) {
    QNetworkRequest request(longEmotionSubmitUrl);
    // 添加HTTP头
    request.setRawHeader("Resource-Id", "volc.tts_async.emotion");
    QString authorization = QString("Bearer;") + accessToken;
    request.setRawHeader("Authorization", authorization.toUtf8());
    request.setRawHeader("Content-Type", "application/json");

    QJsonDocument doc;
    QJsonObject jsonData;
    jsonData.insert("appid", appid);
    jsonData.insert("text", text);
    jsonData.insert("format", "mp3");
    jsonData.insert("voice_type", "BV701_streaming");
    jsonData.insert("sample_rate", 24000);
    jsonData.insert("volume", 1.2);
    jsonData.insert("speed", 0.9);
    jsonData.insert("pitch", 1.1);
    jsonData.insert("enable_subtitle", 1);
    doc.setObject(jsonData);

    QNetworkAccessManager *manager = new QNetworkAccessManager(nullptr);
    connect(manager, &QNetworkAccessManager::finished, this, [this, &recorder, manager](QNetworkReply *reply) {
        onAsyncNetworkLongEmotionPostReply(reply, recorder);
        manager->deleteLater();
    });

    manager->post(request, doc.toJson());
}

void TTSUtils::onAsyncNetworkLongEmotionPostReply(QNetworkReply *reply, TTSRecordFunc &recorder) {
    if (reply->error()) {
        // 清理资源
        reply->deleteLater();
        QJsonDocument jsonDocument = QJsonDocument::fromJson("{}");
        recorder(jsonDocument);
        return;
    }
    QString answer = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(answer.toLocal8Bit().data());
    if (jsonDocument["task_status"] != 0) { // 调用失败
        recorder(jsonDocument);
        return;
    }
    // 发送成功
    auto taskID = jsonDocument["task_id"].toString();
    // 定时触发查询任务
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, timer, taskID, &recorder]() {
        postQureyLongEmotion(taskID, recorder, timer);
    });
    // 10ms刷新一次
    timer->start(100);
}

bool TTSUtils::onAsyncNetworkLongEmotionGetReply(QNetworkReply *reply, TTSRecordFunc &recorder) {
    if (reply->error()) {
        reply->deleteLater();
        QJsonDocument jsonDocument = QJsonDocument::fromJson("{}");
        recorder(jsonDocument);
        return false;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
    // 查询不到结果
    if (jsonDocument.object().contains("audio_url")) {
        recorder(jsonDocument);
        return true;
    }
    return false;
}

void TTSUtils::postQureyLongEmotion(const QString &taskID, TTSRecordFunc &recorder, QTimer *timer) {
    timer->stop();
    QUrl queryUrl(longEmotionQueryUrl);
    QUrlQuery query;
    query.addQueryItem("appid", appid);
    query.addQueryItem("task_id", taskID);
    queryUrl.setQuery(query);

    QNetworkRequest request(queryUrl);
    // 添加HTTP头
    request.setRawHeader("Resource-Id", "volc.tts_async.emotion");
    QString authorization = QString("Bearer;") + accessToken;
    request.setRawHeader("Authorization", authorization.toUtf8());
    request.setRawHeader("Content-Type", "application/json");

    QNetworkAccessManager *manager = new QNetworkAccessManager(nullptr);

    connect(manager, &QNetworkAccessManager::finished, this, [this, timer, &recorder, manager](QNetworkReply *reply) {
        auto ret = onAsyncNetworkLongEmotionGetReply(reply, recorder);
        if (!ret) {
            timer->start(100);
        }
        manager->deleteLater();
    });

    manager->get(request);
}

} // namespace DM
