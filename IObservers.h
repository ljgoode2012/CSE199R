#ifndef IOBSERVERS_H
#define IOBSERVERS_H

#include <QImage>

// Observer interface for video frame updates
class IVideoObserver
{
public:
    virtual ~IVideoObserver() = default;
    virtual void onFrameUpdated(const QImage &frame) = 0;
};

// Observer interface for recording state changes
class IRecordingObserver
{
public:
    virtual ~IRecordingObserver() = default;
    virtual void onRecordingStateChanged(bool isRecording) = 0;
};

#endif // IOBSERVERS_H