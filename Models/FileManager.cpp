#include "FileManager.h"
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>

FileManager::FileManager()
{
    videosPath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + "/CameraApp";
    photosPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/CameraApp";
    
    // Create directories if they don't exist
    QDir().mkpath(videosPath);
    QDir().mkpath(photosPath);
}

QString FileManager::getVideoDirectory() const
{
    return videosPath;
}

QString FileManager::getNextVideoFilePath()
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");
    return videosPath + "/video_" + timestamp + ".mp4";
}

QStringList FileManager::listSavedVideos()
{
    QDir dir(videosPath);
    return dir.entryList({"*.mp4"}, QDir::Files | QDir::NoDotAndDotDot);
}

QString FileManager::getPhotoDirectory() const
{
    return photosPath;
}

QString FileManager::getNextPhotoFilePath()
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");
    return photosPath + "/photo_" + timestamp + ".png";
}