#include "audiosearchmodel.h"
#include "audioinfo.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>

const QString& k_requestUrl = "https://api.jamendo.com/v3.0/tracks/";
const QString& k_clientId = "9f3a55de";

AudioSearchModel::AudioSearchModel(QObject *parent)
    : QAbstractListModel{parent}
{}

QHash<int, QByteArray> AudioSearchModel::roleNames() const
{
    QHash<int, QByteArray> res;

    res[(int)Role::AudioAuthorName] = "audioAuthorName";
    res[(int)Role::AudioTitle] = "audioTitle";
    res[(int)Role::AudioSource] = "audioSource";
    res[(int)Role::AudioImageSource] = "audioImageSource";

    return res;
}

int AudioSearchModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_musicList.length();
}

QVariant AudioSearchModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() >= 0 && index.row() <= m_musicList.length()) {
        AudioInfo* audioInfo = m_musicList[index.row()];

        switch ((Role)role) {
        case Role::AudioTitle:
            return audioInfo->title();
        case Role::AudioAuthorName:
            return audioInfo->authorName();
        case Role::AudioSource:
            return audioInfo->audioSource();
        case Role::AudioImageSource:
            return audioInfo->imageSource();
        }
    }

    return {};
}

void AudioSearchModel::searchSong(const QString &name)
{
    if (name.trimmed().isEmpty())
        return;

    if (m_reply) {
        m_reply->abort();
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    QUrlQuery query;
    query.addQueryItem("client_id", k_clientId);
    query.addQueryItem("namesearch", name);
    query.addQueryItem("format", "json");

    setIsSearching(true);
    m_reply = m_networkManager.get(QNetworkRequest(k_requestUrl + "?" + query.toString()));
    connect(m_reply, &QNetworkReply::finished, this, &AudioSearchModel::parseData);
    qDebug() << "Searching song" << name;
}

void AudioSearchModel::parseData()
{
    if (m_reply->error() != QNetworkReply::NoError) {
        if (m_reply->error() != QNetworkReply::OperationCanceledError)
            qCritical() << "An error occured in song query:" << m_reply->errorString();
        return;
    }

    beginResetModel();

    qDeleteAll(m_musicList);
    m_musicList.clear();

    QByteArray data = m_reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    QJsonObject headers = jsonDoc["headers"].toObject();

    if (headers["status"].toString() == "success") {
        QJsonArray results = jsonDoc["results"].toArray();

        for (const auto& result: results) {
            QJsonObject entry = result.toObject();

            if (!entry["audiodownload_allowed"].toBool()) {
                qWarning() << "Song" << entry["name"].toString() << "does not allow download!";
                continue;
            }

            AudioInfo* audioInfo = new AudioInfo(this);
            audioInfo->initialize(entry["name"].toString(),
                                  entry["artist_name"].toString(),
                                  entry["audiodownload"].toString(),
                                  entry["album_image"].toString());
            m_musicList << audioInfo;
        }
    } else {
        qCritical() << "status not success:" << headers["error_message"].toString() << headers["status"].toString();
    }

    endResetModel();

    m_reply->deleteLater();
    m_reply = nullptr;
    setIsSearching(false);
}

bool AudioSearchModel::isSearching() const
{
    return m_isSearching;
}

void AudioSearchModel::setIsSearching(bool newIsSearching)
{
    if (m_isSearching == newIsSearching)
        return;
    m_isSearching = newIsSearching;
    emit isSearchingChanged();
}
