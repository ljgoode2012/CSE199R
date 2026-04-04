#ifndef GALLERYCONTROLLER_H
#define GALLERYCONTROLLER_H

#include <QWidget>
#include <QStringList>
#include "Models/FileManager.h"

class QScrollArea;
class QGridLayout;

class GalleryController : public QWidget
{
    Q_OBJECT

public:
    explicit GalleryController(FileManager *fileManager, QWidget *parent = nullptr);

    // Call this before showing the gallery to reload videos from disk
    void refresh();

signals:
    void videoSelected(const QString &filePath);
    void backRequested();

private slots:
    void onThumbnailClicked(const QString &filePath);

private:
    void buildGrid();
    QPixmap getThumbnail(const QString &filePath, qint64 *durationMs = nullptr);

    FileManager *fileManager;
    QScrollArea *scrollArea;
    QGridLayout *gridLayout;
};

#endif
