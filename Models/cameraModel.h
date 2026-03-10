#ifndef CAMERAMODEL_H
#define CAMERAMODEL_H

#include <QObject>
#include <QImage>
#include <QCamera>
#include <QMediaRecorder>
#include <QMediaCaptureSession>
#include <QAudioInput>
#include <QVideoSink>
#include <QVideoFrame>
#include <QMutex>
#include <vector>
#include "IObservers.h"

class CameraModel : public QObject
{
    Q_OBJECT

public:
    enum EffectType { Default, Greyscale, Negative, Pixelated };
    
    explicit CameraModel(QObject *parent = nullptr);
    ~CameraModel();
    
    // Camera operations
    void startCamera();
    void stopCamera();
    void applyEffect(EffectType effect);
    
    // Recording operations
    void startRecording(const QString &filepath);
    void stopRecording();
    
    // Snapshot
    void captureFrame(const QString &filepath);
    
    // Observers
    void subscribeToVideo(IVideoObserver *observer);
    void subscribeToRecording(IRecordingObserver *observer);
    void unsubscribeFromVideo(IVideoObserver *observer);
    void unsubscribeFromRecording(IRecordingObserver *observer);
    
    QImage getCurrentFrame() const;
    bool isRecording() const { return recordingActive; }

private slots:
    void onVideoFrame(const QVideoFrame &frame);
    void onRecorderStateChanged(QMediaRecorder::RecorderState state);
    void onRecorderError(QMediaRecorder::Error error, const QString &errorString);

private:
    void notifyFrameUpdated();
    void notifyRecordingStateChanged();
    
    QImage applyPixelation(const QImage &image);
    QImage applyNegative(const QImage &image);
    QImage applyGreyscale(const QImage &image);
    
    QImage currentFrame;
    EffectType currentEffect;
    bool recordingActive;
    
    // Qt multimedia components
    QCamera *camera;
    QAudioInput *audioInput;
    QMediaCaptureSession *session;
    QVideoSink *videoSink;
    QMediaRecorder *recorder;
    
    // Thread-safe frame access
    mutable QMutex frameMutex;
    
    // Observer lists
    std::vector<IVideoObserver*> videoObservers;
    std::vector<IRecordingObserver*> recordingObservers;
};

#endif