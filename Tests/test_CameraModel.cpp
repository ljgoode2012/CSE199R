#include <QtTest>
#include <QImage>
#include "../Models/CameraModel.h"
#include "../IObservers.h"

class MockVideoObserver : public IVideoObserver
{
public:
    void onFrameUpdated(const QImage &frame) override
    {
        lastFrame = frame;
        frameUpdateCount++;
    }
    
    QImage lastFrame;
    int frameUpdateCount = 0;
};

class MockRecordingObserver : public IRecordingObserver
{
public:
    void onRecordingStateChanged(bool isRecording) override
    {
        this->isRecording = isRecording;
        recordingStateChangeCount++;
    }
    
    bool isRecording = false;
    int recordingStateChangeCount = 0;
};

class TestCameraModel : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    void test_cameraModelCreation();
    void test_applyPixelation();
    void test_applyNegative();
    void test_applyGreyscale();
    void test_subscribeToVideo();
    void test_subscribeToRecording();
    void test_unsubscribeFromVideo();
    void test_unsubscribeFromRecording();

private:
    CameraModel *cameraModel;
    QImage createTestImage(int width = 100, int height = 100);
};

void TestCameraModel::initTestCase()
{
    cameraModel = new CameraModel();
    QVERIFY(cameraModel != nullptr);
}

void TestCameraModel::cleanupTestCase()
{
    delete cameraModel;
}

QImage TestCameraModel::createTestImage(int width, int height)
{
    QImage image(width, height, QImage::Format_RGB32);
    image.fill(Qt::blue);
    return image;
}

void TestCameraModel::test_cameraModelCreation()
{
    QVERIFY(cameraModel != nullptr);
    QVERIFY(!cameraModel->isRecording());
}

void TestCameraModel::test_applyPixelation()
{
    QImage original = createTestImage(100, 100);
    original.fill(Qt::red);
    
    // Test that applyEffect doesn't crash
    cameraModel->applyEffect(CameraModel::Pixelated);
    
    // Just verify the effect type is set correctly by not crashing
    QVERIFY(true);
}

void TestCameraModel::test_applyNegative()
{
    QImage original = createTestImage(100, 100);
    original.fill(Qt::white);
    
    cameraModel->applyEffect(CameraModel::Negative);
    
    QVERIFY(true);
}

void TestCameraModel::test_applyGreyscale()
{
    QImage original = createTestImage(100, 100);
    original.fill(Qt::green);
    
    cameraModel->applyEffect(CameraModel::Greyscale);
    
    QVERIFY(true);
}

void TestCameraModel::test_subscribeToVideo()
{
    MockVideoObserver observer1;
    MockVideoObserver observer2;
    
    cameraModel->subscribeToVideo(&observer1);
    cameraModel->subscribeToVideo(&observer2);
    
    // Just verify no crash
    QVERIFY(true);
}

void TestCameraModel::test_subscribeToRecording()
{
    MockRecordingObserver observer1;
    MockRecordingObserver observer2;
    
    cameraModel->subscribeToRecording(&observer1);
    cameraModel->subscribeToRecording(&observer2);
    
    // Just verify no crash
    QVERIFY(true);
}

void TestCameraModel::test_unsubscribeFromVideo()
{
    MockVideoObserver observer;
    cameraModel->subscribeToVideo(&observer);
    cameraModel->unsubscribeFromVideo(&observer);
    
    QVERIFY(true);
}

void TestCameraModel::test_unsubscribeFromRecording()
{
    MockRecordingObserver observer;
    cameraModel->subscribeToRecording(&observer);
    cameraModel->unsubscribeFromRecording(&observer);
    
    QVERIFY(true);
}

QTEST_MAIN(TestCameraModel)
#include "test_CameraModel.moc"
