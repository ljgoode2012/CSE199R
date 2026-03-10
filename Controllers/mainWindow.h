#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QMutex>
#include <memory>
#include <vector>
#include <rlottie.h>
#include "Models/CameraModel.h"
#include "Models/FileManager.h"
#include "IObservers.h"
#include "ui_mainWindow.h"

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
    // Recording mode
    void onEffectChanged();
    void onRecordClicked();
    void onCaptureClicked();
    void onVideoFrame(const QVideoFrame &frame);
    void toggleRecording();
    void snapPicture();
    void onRecorderStateChanged(QMediaRecorder::RecorderState state);
    void onRecorderError(QMediaRecorder::Error error, const QString &errorString);
    void reactToToggle(bool checked);

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
};

#endif