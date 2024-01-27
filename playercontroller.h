#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QMediaPlayer>
#include <QObject>
#include <QAbstractListModel>

class AudioInfo;

class PlayerController : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(AudioInfo* currentSong READ currentSong WRITE setCurrentSong NOTIFY currentSongChanged FINAL)
    Q_PROPERTY(bool playing READ playing NOTIFY playingChanged FINAL)

public:
    enum Role {
        AudioTitleRole = Qt::UserRole + 1,
        AudioAuthorNameRole,
        AudioSourceRole,
        AudioImageSourceRole,
        AudioVideoSourceRole
    };

    explicit PlayerController(QObject *parent = nullptr);

    bool playing() const;
    void setPlaying(bool newPlaying);

    AudioInfo *currentSong() const;
    bool setCurrentSong(AudioInfo *newCurrentSong);

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

public slots:
    void switchToNextSong();
    void playPause();
    void switchToPrevSong();
    auto changeAudioSource(const QUrl& source, qint64 position = 0) -> void;
    auto addAudio(const QString& title,
                  const QString& authorName,
                  const QUrl& audioSource,
                  const QUrl& imageSource,
                  const QUrl& videoSource = QUrl()) -> void;
    auto removeAudio(int index) -> void;
    auto switchToAudioByIndex(int index) -> void;
    auto slideToPosition(qint64 position) -> void;

signals:
    void playingChanged();
    void currentSongChanged();

    auto durationChanged(qint64 newDuration) -> void;

private slots:
    auto onMediaStatusChanged(QMediaPlayer::MediaStatus status) -> void;
    auto onPositionChanged(qint64 newPosition) -> void;

private:
    int m_currSongIndex = 0;
    bool m_playing = false;
    QMediaPlayer m_mediaPlayer;

    bool m_ready = false;

    QList<AudioInfo*> m_musicList;
    AudioInfo *m_currentSong = nullptr;
};

#endif // PLAYERCONTROLLER_H
