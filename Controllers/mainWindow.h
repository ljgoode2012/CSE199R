#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QMutex>
#include <QStackedWidget>
#include <memory>
#include <vector>
#include <rlottie.h>
#include "Models/CameraModel.h"
#include "Models/FileManager.h"
#include "IObservers.h"
#include "ui_mainWindow.h"
#include "Controllers/GalleryController.h"
#include "Controllers/PlaybackController.h"

class MainWindow : public QMainWindow, 
                   public IVideoObserver,
                   public IRecordingObserver
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    
    // Observer implementations
    void onFrameUpdated(const QImage &frame) override;
    void onRecordingStateChanged(bool isRecording) override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onEffectChanged();
    void onRecordClicked();
    void onCaptureClicked();
    void showPlayback(const QString &fullpath);
    void showCamera();
    void showGallery();

private:
    void setupConnections();
    void displayFrame(const QImage &frame);
    void initReactionMenu();
    void initReaction(const QString &reactionPath);
    void renderReaction(QImage &image);

    Ui::MainWindow ui;
    
    // Models
    CameraModel *cameraModel;
    FileManager *fileManager;
    
    // Video display
    QLabel *videoLabel;
    
    // Rlottie animation
    std::unique_ptr<rlottie::Animation> anim;
    size_t animFrame;
    size_t animTotalFrames;
    bool reactionEnabled;
    
    // Recording state
    QString currentRecordingPath;

    // playback feature
    GalleryController  *galleryController;
    PlaybackController *playbackController;
    QStackedWidget *stackedWidget;
};

#endif