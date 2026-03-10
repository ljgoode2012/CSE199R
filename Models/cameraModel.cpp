#include "CameraModel.h"
#include <QVideoFrame>
#include <QAudioInput>
#include <QMediaCaptureSession>
#include <QUrl>
#include <QMutexLocker>
#include <algorithm>

CameraModel::CameraModel(QObject *parent)
    : QObject(parent),
      currentEffect(Default),
      recordingActive(false),
      camera(nullptr),
      audioInput(nullptr),
      session(nullptr),
      videoSink(nullptr),
      recorder(nullptr)
{
    // Initialize camera and media capture
    camera = new QCamera(this);
    session = new QMediaCaptureSession(this);
    session->setCamera(camera);
    
    // Add audio input
    audioInput = new QAudioInput(this);
    session->setAudioInput(audioInput);
    
    // Setup video sink for frame capture
    videoSink = new QVideoSink(this);
    session->setVideoOutput(videoSink);
    
    // Create recorder
    recorder = new QMediaRecorder(this);
    session->setRecorder(recorder);
    
    // Connect video frame signal to our slot
    connect(videoSink, &QVideoSink::videoFrameChanged, this, &CameraModel::onVideoFrame);
    
    // Connect recorder state changes
    connect(recorder, &QMediaRecorder::recorderStateChanged, this, &CameraModel::onRecorderStateChanged);
}

CameraModel::~CameraModel()
{
    if (recorder && recordingActive) {
        recorder->stop();
    }
}

void CameraModel::startCamera()
{
    if (camera) {
        camera->start();
    }
}

void CameraModel::stopCamera()
{
    if (camera) {
        camera->stop();
    }
}

void CameraModel::onVideoFrame(const QVideoFrame &frame)
{
    if (!frame.isValid()) return;
    
    // Convert frame to QImage
    QImage image = frame.toImage();
    
    // Apply current effect
    switch (currentEffect) {
        case Pixelated:
            image = applyPixelation(image);
            break;
        case Negative:
            image = applyNegative(image);
            break;
        case Greyscale:
            image = applyGreyscale(image);
            break;
        case Default:
        default:
            // No processing needed for default
            break;
    }
    
    // Store and notify
    {
        QMutexLocker locker(&frameMutex);
        currentFrame = image;
    }
    
    notifyFrameUpdated();
}

void CameraModel::applyEffect(EffectType effect)
{
    currentEffect = effect;
}

QImage CameraModel::applyPixelation(const QImage &image)
{
    if (image.isNull()) return image;
    
    // Scale down to 32x32, then scale back up for pixelation effect
    QImage lowRes = image.scaled(32, 32, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    return lowRes.scaled(image.width(), image.height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
}

QImage CameraModel::applyNegative(const QImage &image)
{
    if (image.isNull()) return image;
    
    QImage result = image;
    result.invertPixels(QImage::InvertRgb);
    return result;
}

QImage CameraModel::applyGreyscale(const QImage &image)
{
    if (image.isNull()) return image;
    
    return image.convertToFormat(QImage::Format_Grayscale8);
}

void CameraModel::startRecording(const QString &filepath)
{
    if (!recorder || recordingActive) return;
    
    // Set output location
    recorder->setOutputLocation(QUrl::fromLocalFile(filepath));
    recordingActive = true;
    recorder->record();
}

void CameraModel::stopRecording()
{
    if (!recorder || !recordingActive) return;
    
    recorder->stop();
    recordingActive = false;
    notifyRecordingStateChanged();
}

void CameraModel::captureFrame(const QString &filepath)
{
    QMutexLocker locker(&frameMutex);
    if (!currentFrame.isNull()) {
        currentFrame.save(filepath, "JPEG", 90);
    }
}

QImage CameraModel::getCurrentFrame() const
{
    QMutexLocker locker(&frameMutex);
    return currentFrame;
}

void CameraModel::subscribeToVideo(IVideoObserver *observer)
{
    if (observer) {
        videoObservers.push_back(observer);
    }
}

void CameraModel::subscribeToRecording(IRecordingObserver *observer)
{
    if (observer) {
        recordingObservers.push_back(observer);
    }
}

void CameraModel::unsubscribeFromVideo(IVideoObserver *observer)
{
    auto it = std::find(videoObservers.begin(), videoObservers.end(), observer);
    if (it != videoObservers.end()) {
        videoObservers.erase(it);
    }
}

void CameraModel::unsubscribeFromRecording(IRecordingObserver *observer)
{
    auto it = std::find(recordingObservers.begin(), recordingObservers.end(), observer);
    if (it != recordingObservers.end()) {
        recordingObservers.erase(it);
    }
}

void CameraModel::notifyFrameUpdated()
{
    QImage frame = getCurrentFrame();
    for (auto observer : videoObservers) {
        observer->onFrameUpdated(frame);
    }
}

void CameraModel::notifyRecordingStateChanged()
{
    for (auto observer : recordingObservers) {
        observer->onRecordingStateChanged(recordingActive);
    }
}

void CameraModel::onRecorderStateChanged(QMediaRecorder::RecorderState state)
{
    if (state == QMediaRecorder::RecordingState) {
        recordingActive = true;
    } else {
        recordingActive = false;
    }
    notifyRecordingStateChanged();
}

void CameraModel::onRecorderError(QMediaRecorder::Error error, const QString &errorString)
{
    // Error handling can be delegated to UI if needed
}