#include "playercontroller.h"
#include "audioinfo.h"
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QDebug>

PlayerController::PlayerController(QObject *parent)
    : QAbstractListModel{parent}
{
    const auto& mediaOutputs = QMediaDevices::audioOutputs();
    if (!mediaOutputs.empty()) {
        m_mediaPlayer.setAudioOutput(new QAudioOutput(&m_mediaPlayer));
    }

    connect(&m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &PlayerController::onMediaStatusChanged);
    connect(&m_mediaPlayer, &QMediaPlayer::durationChanged, this, &PlayerController::durationChanged);
    connect(&m_mediaPlayer, &QMediaPlayer::positionChanged, this, &PlayerController::onPositionChanged);

    addAudio("Eine Kleine Nachtmusik",
             "Wolfgang Amadeus Mozart",
             QUrl("qrc:/MusicPlayer/assets/audio/eine_kleine_nachtmusik.mp3"),
             QUrl("qrc:/MusicPlayer/assets/images/quin_dog_11.jpg"));

    addAudio("Symphone No. 5",
             "Ludwig Van Beethoven",
             QUrl("qrc:/MusicPlayer/assets/audio/symphony_no_5.mp3"),
             QUrl("qrc:/MusicPlayer/assets/images/quin_moyu.png"));

    addAudio("Air on the G String",
             "Johann Sebastian Bach",
             QUrl("qrc:/MusicPlayer/assets/audio/air_on_the_g_string.mp3"),
             QUrl("qrc:/MusicPlayer/assets/images/quin_kawaii.png"));
}

bool PlayerController::playing() const
{
    return m_playing;
}

int PlayerController::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_musicList.length();
}

QVariant PlayerController::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && index.row() >= 0 && index.row() <= m_musicList.length()) {
        AudioInfo* audioInfo = m_musicList[index.row()];

        switch ((Role)role) {
        case AudioTitleRole:
            return audioInfo->title();
        case AudioAuthorNameRole:
            return audioInfo->authorName();
        case AudioSourceRole:
            return audioInfo->audioSource();
        case AudioImageSourceRole:
            return audioInfo->imageSource();
        case AudioVideoSourceRole:
            return audioInfo->videoSource();
        }
    }

    return {};
}

QHash<int, QByteArray> PlayerController::roleNames() const
{
    QHash<int, QByteArray> res;

    res[AudioAuthorNameRole] = "audioAuthorName";
    res[AudioTitleRole] = "audioTitle";
    res[AudioSourceRole] = "audioSource";
    res[AudioImageSourceRole] = "audioImageSource";
    res[AudioVideoSourceRole] = "audioVideoSource";

    return res;
}

void PlayerController::switchToNextSong()
{
    if (m_currSongIndex < 0)
        return;

    if (m_currSongIndex + 1 >= m_musicList.length())
        m_currSongIndex = 0;
    else
        m_currSongIndex++;

    switchToAudioByIndex(m_currSongIndex);
}

void PlayerController::playPause()
{
    m_playing = !m_playing;

    if (m_playing)
        m_mediaPlayer.play();
    else
        m_mediaPlayer.pause();

    emit playingChanged();
}

void PlayerController::switchToPrevSong()
{
    if (m_currSongIndex < 0)
        return;

    if (m_currSongIndex > 0)
        m_currSongIndex--;
    else
        m_currSongIndex = m_musicList.length() - 1;

    switchToAudioByIndex(m_currSongIndex);
}

void PlayerController::changeAudioSource(const QUrl &source, qint64 position)
{
    m_ready = false;
    m_mediaPlayer.stop();
    m_mediaPlayer.setSource(source);

    qDebug() << "Song on position" << position;
    qDebug() << "Song" << m_currentSong->title() << "seekable?" << m_mediaPlayer.isSeekable();

    if (m_playing)
        m_mediaPlayer.play();
}

void PlayerController::addAudio(const QString &title, const QString &authorName, const QUrl &audioSource, const QUrl &imageSource, const QUrl &videoSource)
{
    beginInsertRows(QModelIndex(), m_musicList.length(), m_musicList.length());

    AudioInfo* audioInfo = new AudioInfo(this);
    audioInfo->initialize(title, authorName, audioSource, imageSource, videoSource);

    m_musicList << audioInfo;

    if (m_musicList.length() == 1)
        switchToAudioByIndex(0);

    endInsertRows();
}

void PlayerController::removeAudio(int index)
{
    if (index < 0 || index >= m_musicList.length())
        return;

    beginRemoveRows(QModelIndex(), index, index);

    AudioInfo* toRemove = m_musicList[index];
    if (toRemove == m_currentSong) {
        if (m_musicList.length() > 1) {
            if (index != 0)
                switchToAudioByIndex(index - 1);
            else
                switchToAudioByIndex(index + 1);
        } else {
            switchToAudioByIndex(-1);
        }
    }

    m_musicList.removeAt(index);
    toRemove->deleteLater();

    endRemoveRows();
}

void PlayerController::switchToAudioByIndex(int index)
{
    if (index < 0 || index >= m_musicList.length()) {
        m_currSongIndex = -1;
        setCurrentSong(nullptr);
    } else {
        if (setCurrentSong(m_musicList[index]))
            m_currSongIndex = index;
    }
}

void PlayerController::slideToPosition(qint64 position)
{
    if (!m_currentSong) return;

    if (m_mediaPlayer.mediaStatus() == QMediaPlayer::MediaStatus::BufferedMedia) {
        m_mediaPlayer.setPosition(position);
    } else {
        qDebug() << "Cannot slide to position" << position << "since media not buffered!";
    }
}

void PlayerController::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::MediaStatus::BufferedMedia) {
        if (m_currentSong) {
            m_mediaPlayer.setPosition(m_currentSong->position());
            qDebug() << "Song set to position" << m_currentSong->position();
            m_ready = true;
        }
    } else if (status == QMediaPlayer::MediaStatus::EndOfMedia) {
        m_mediaPlayer.setPosition(0);
        switchToNextSong();
    }
}

void PlayerController::onPositionChanged(qint64 newPosition)
{
    if (m_currentSong && m_ready)
        m_currentSong->setPosition(newPosition);
}

AudioInfo *PlayerController::currentSong() const
{
    return m_currentSong;
}

bool PlayerController::setCurrentSong(AudioInfo *newCurrentSong)
{
    if (m_currentSong == newCurrentSong)
        return false;

    m_currentSong = newCurrentSong;
    emit currentSongChanged();

    if (m_currentSong) {
        changeAudioSource(m_currentSong->audioSource(), m_currentSong->position());
    } else {
        m_mediaPlayer.stop();
        m_mediaPlayer.setSource(QUrl());
        m_playing = false;
        emit playingChanged();
    }

    return true;
}

void PlayerController::setPlaying(bool newPlaying)
{
    if (m_playing == newPlaying)
        return;
    m_playing = newPlaying;
    emit playingChanged();
}
