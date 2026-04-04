#include "PlaybackController.h"
#include <QVideoWidget>
#include <QToolButton>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTime>

PlaybackController::PlaybackController(QWidget *parent)
    : QWidget(parent)
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    videoWidget = new QVideoWidget(this);
    player->setVideoOutput(videoWidget);
    
    // buttons
    backButton = new QToolButton(this);
    backButton->setText("Back");
    playPauseButton = new QToolButton(this);
    playPauseButton->setIcon(QIcon(":/icons/icons/play.png"));
    seekSlider = new QSlider(Qt::Horizontal, this);
    timeLabel = new QLabel(this);
    timeLabel->setText("0:00");

    // arrange layout
    auto *controls = new QHBoxLayout();
    controls->addWidget(backButton);
    controls->addWidget(playPauseButton);
    controls->addWidget(seekSlider);
    controls->addWidget(timeLabel);

    auto *root = new QVBoxLayout(this);
    root->addWidget(videoWidget, 1);  // stretch factor 1 = takes all remaining space
    root->addLayout(controls);

    // event handlers
    connect(backButton, &QToolButton::clicked, this, &PlaybackController::backRequested);
    connect(playPauseButton, &QToolButton::clicked, this, &PlaybackController::onPlayPauseClicked);
    connect(seekSlider, &QSlider::sliderMoved, this, &PlaybackController::onSliderMoved);
    connect(player, &QMediaPlayer::durationChanged, this, &PlaybackController::onDurationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &PlaybackController::onPositionChanged);
    connect(player, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        if (state == QMediaPlayer::PlayingState)
            playPauseButton->setIcon(QIcon(":/icons/icons/pause.png"));
        else
            playPauseButton->setIcon(QIcon(":/icons/icons/play.png"));
    });
}

void PlaybackController::loadVideo(const QString &filePath)
{
    player->setSource(QUrl::fromLocalFile(filePath));
    player->play();
}

void PlaybackController::onPlayPauseClicked()
{
    if (player->playbackState() == QMediaPlayer::PlayingState)
        player->pause();
    else
        player->play();
}

void PlaybackController::onDurationChanged(qint64 duration)
{
    seekSlider->setRange(0, static_cast<int>(duration));
    formattedDuration = formatTime(duration);
}

void PlaybackController::onPositionChanged(qint64 position)
{
    seekSlider->blockSignals(true);
    seekSlider->setSliderPosition(static_cast<int>(position));
    seekSlider->blockSignals(false);

    timeLabel->setText(formatTime(position) + " / " + formattedDuration);
}

QString PlaybackController::formatTime(qint64 ms) const
{
    return QTime(0, 0).addMSecs(ms).toString("m:ss");
}

void PlaybackController::onSliderMoved(int position)
{
    player->setPosition(static_cast<qint64>(position));
}
