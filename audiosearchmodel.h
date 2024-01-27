#ifndef AUDIOSEARCHMODEL_H
#define AUDIOSEARCHMODEL_H

#include <QAbstractListModel>
#include <QNetworkAccessManager>

class AudioInfo;

class AudioSearchModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool isSearching READ isSearching NOTIFY isSearchingChanged FINAL)

public:
    enum class Role {
        AudioTitle = Qt::UserRole + 1,
        AudioAuthorName,
        AudioSource,
        AudioImageSource,
    };

    explicit AudioSearchModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    bool isSearching() const;
    void setIsSearching(bool newIsSearching);

public slots:
    auto searchSong(const QString& name) -> void;
    auto parseData() -> void;

signals:
    void isSearchingChanged();

private:
    QList<AudioInfo*> m_musicList;

    QNetworkAccessManager m_networkManager;
    QNetworkReply* m_reply = nullptr;
    bool m_isSearching;
};

#endif // AUDIOSEARCHMODEL_H
