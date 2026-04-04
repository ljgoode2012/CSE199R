#include "GalleryController.h"
#include <QScrollArea>
#include <QGridLayout>
#include <QToolButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDir>
#include <QMediaPlayer>
#include <QVideoSink>
#include <QVideoFrame>
#include <QEventLoop>
#include <QTimer>
#include <QTime>

GalleryController::GalleryController(FileManager *fileManager, QWidget *parent)
    : QWidget(parent), fileManager(fileManager)
{
    auto *backButton = new QToolButton(this);
    backButton->setText("Back");
    connect(backButton, &QToolButton::clicked, this, &GalleryController::backRequested);

    auto *container = new QWidget();
    gridLayout = new QGridLayout(container);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(container);

    auto *root = new QVBoxLayout(this);
    root->addWidget(backButton);
    root->addWidget(scrollArea);
}

void GalleryController::refresh()
{
    while (QLayoutItem *item = gridLayout->takeAt(0))
    {
        delete item->widget();
        delete item;
    }
    buildGrid();
}

void GalleryController::buildGrid()
{
    // get the list of saved videos from fileManager
    QStringList videos = fileManager->listSavedVideos();
    QString dir = fileManager->getVideoDirectory();

    const int columns = 4;
    int row = 0, col = 0;
    for (const QString &filename : videos) {
        QString fullPath = dir + "/" + filename;

        qint64 durationMs = 0;
        QPixmap thumb = getThumbnail(fullPath, &durationMs);

        auto *btn = new QToolButton(this);
        btn->setIcon(QIcon(thumb));
        btn->setIconSize(QSize(160, 90));
        btn->setToolTip(filename);
        connect(btn, &QToolButton::clicked, this, [this, fullPath]() { onThumbnailClicked(fullPath); });

        QString timeText = QTime(0, 0).addMSecs(durationMs).toString("m:ss");
        auto *durationLabel = new QLabel(timeText, this);
        durationLabel->setAlignment(Qt::AlignCenter);

        auto *cell = new QWidget(this);
        auto *cellLayout = new QVBoxLayout(cell);
        cellLayout->setContentsMargins(0, 0, 0, 4);
        cellLayout->setSpacing(2);
        cellLayout->addWidget(btn);
        cellLayout->addWidget(durationLabel);

        gridLayout->addWidget(cell, row, col);
        if (++col >= columns) { col = 0; ++row; }
    }
}

QPixmap GalleryController::getThumbnail(const QString &filePath, qint64 *durationMs)
{
    QMediaPlayer player;
    QVideoSink sink;
    player.setVideoSink(&sink);
    player.setSource(QUrl::fromLocalFile(filePath));

    QImage frame;
    qint64 duration = 0;
    QEventLoop loop;
    connect(&sink, &QVideoSink::videoFrameChanged, &loop, [&](const QVideoFrame &f) {
        frame = f.toImage();
        loop.quit();
    });
    connect(&player, &QMediaPlayer::durationChanged, &loop, [&](qint64 d) {
        duration = d;
    });

    player.play();
    QTimer::singleShot(3000, &loop, &QEventLoop::quit);
    loop.exec();
    player.stop();

    if (durationMs)
        *durationMs = duration;

    if (frame.isNull())
        return QPixmap();
    
    return QPixmap::fromImage(frame).scaled(160, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void GalleryController::onThumbnailClicked(const QString &filePath)
{
    // TODO: emit the videoSelected signal with the given file path
    emit videoSelected(filePath);
}
