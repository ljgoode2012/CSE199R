#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QStringList>

class FileManager
{
public:
    FileManager();
    
    // Video file operations
    QString getVideoDirectory() const;
    QString getNextVideoFilePath();  // Generates unique timestamped filename
    QStringList listSavedVideos();
    
    // Photo operations
    QString getPhotoDirectory() const;
    QString getNextPhotoFilePath();   // Generates unique timestamped filename
    
private:
    QString videosPath;
    QString photosPath;
};

#endif