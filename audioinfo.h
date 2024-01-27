#ifndef AUDIOINFO_H
#define AUDIOINFO_H

#include <QObject>
#include <QUrl>
#include <qqml.h>

class AudioInfo : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged FINAL)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString authorName READ authorName WRITE setAuthorName NOTIFY authorNameChanged FINAL)
    Q_PROPERTY(QUrl imageSource READ imageSource WRITE setImageSource NOTIFY imageSourceChanged FINAL)
    Q_PROPERTY(QUrl videoSource READ videoSource WRITE setVideoSource NOTIFY videoSourceChanged FINAL)
    Q_PROPERTY(QUrl audioSource READ audioSource WRITE setAudioSource NOTIFY audioSourceChanged REQUIRED FINAL)

public:
    explicit AudioInfo(QObject *parent = nullptr);

    const QString& title() const;
    void setTitle(const QString &newTitle);

    const QString& authorName() const;
    void setAuthorName(const QString &newAuthorName);

    const QUrl& imageSource() const;
    void setImageSource(const QUrl &newImageSource);

    const QUrl& videoSource() const;
    void setVideoSource(const QUrl &newVideoSource);

    const QUrl& audioSource() const;
    void setAudioSource(const QUrl &newAudioSource);

    qint64 position() const;
    void setPosition(qint64 newPosition);

    auto initialize(const QString& title,
                    const QString& authorName,
                    const QUrl& audioSource,
                    const QUrl& imageSource,
                    const QUrl& videoSource = QUrl()) -> void;

signals:
    void titleChanged();
    void authorNameChanged();
    void imageSourceChanged();
    void videoSourceChanged();
    void audioSourceChanged();
    void positionChanged();

private:
    QString m_title;
    QString m_authorName;
    QUrl m_imageSource;
    QUrl m_videoSource;
    QUrl m_audioSource;
    qint64 m_position = 0;
};

#endif // AUDIOINFO_H
