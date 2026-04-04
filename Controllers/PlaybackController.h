#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>

class QVideoWidget;
class QToolButton;
class QSlider;
class QLabel;

class PlaybackController : public QWidget
{
    Q_OBJECT

public:
    explicit PlaybackController(QWidget *parent = nullptr);

    // Load and begin playing the given video file
    void loadVideo(const QString &filePath);

signals:
    void backRequested();

private slots:
    void onPlayPauseClicked();
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    void onSliderMoved(int position);

#ifdef UNIT_TEST
    friend class TestPlaybackController;
#endif

private:
    QString formatTime(qint64 ms) const;

    QString formattedDuration;

    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QVideoWidget *videoWidget;
    QToolButton  *backButton;
    QToolButton  *playPauseButton;
    QSlider      *seekSlider;
    QLabel       *timeLabel;
};

#endif
