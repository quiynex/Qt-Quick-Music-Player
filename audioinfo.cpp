#include "audioinfo.h"

AudioInfo::AudioInfo(QObject *parent)
    : QObject{parent}
{}

const QString& AudioInfo::title() const
{
    return m_title;
}

void AudioInfo::setTitle(const QString &newTitle)
{
    if (m_title == newTitle)
        return;
    m_title = newTitle;
    emit titleChanged();
}

const QString& AudioInfo::authorName() const
{
    return m_authorName;
}

void AudioInfo::setAuthorName(const QString &newAuthorName)
{
    if (m_authorName == newAuthorName)
        return;
    m_authorName = newAuthorName;
    emit authorNameChanged();
}

const QUrl& AudioInfo::imageSource() const
{
    return m_imageSource;
}

void AudioInfo::setImageSource(const QUrl &newImageSource)
{
    if (m_imageSource == newImageSource)
        return;
    m_imageSource = newImageSource;
    emit imageSourceChanged();
}

const QUrl& AudioInfo::videoSource() const
{
    return m_videoSource;
}

void AudioInfo::setVideoSource(const QUrl &newVideoSource)
{
    if (m_videoSource == newVideoSource)
        return;
    m_videoSource = newVideoSource;
    emit videoSourceChanged();
}

const QUrl& AudioInfo::audioSource() const
{
    return m_audioSource;
}

void AudioInfo::setAudioSource(const QUrl &newAudioSource)
{
    if (m_audioSource == newAudioSource)
        return;
    m_audioSource = newAudioSource;
    emit audioSourceChanged();
}

qint64 AudioInfo::position() const
{
    return m_position;
}

void AudioInfo::setPosition(qint64 newPosition)
{
    if (m_position == newPosition)
        return;
    m_position = newPosition;
    emit positionChanged();
}

void AudioInfo::initialize(const QString &title, const QString &authorName, const QUrl &audioSource, const QUrl &imageSource, const QUrl &videoSource)
{
    setAudioSource(audioSource);
    setImageSource(imageSource);
    setVideoSource(videoSource);
    setAuthorName(authorName);
    setTitle(title);
}
