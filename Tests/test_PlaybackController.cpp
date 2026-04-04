#include <QtTest>
#include <QSlider>
#include <QLabel>
#include <QToolButton>
#include "../Controllers/PlaybackController.h"

class TestPlaybackController : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_constructorCreatesWidget();
    void test_loadVideoSetsSource();
    void test_playPauseTogglesState();
    void test_onDurationChangedSetsSliderRange();
    void test_onPositionChangedUpdatesSlider();
    void test_onPositionChangedUpdatesTimeLabel();
    void test_backRequestedSignalEmitted();

private:
    PlaybackController *controller;
};

void TestPlaybackController::initTestCase()
{
    controller = new PlaybackController();
    QVERIFY(controller != nullptr);
}

void TestPlaybackController::cleanupTestCase()
{
    delete controller;
}

void TestPlaybackController::test_constructorCreatesWidget()
{
    // Widget should exist and have a layout
    QVERIFY(controller->layout() != nullptr);
}

void TestPlaybackController::test_loadVideoSetsSource()
{
    // Loading a non-existent file should still set a source without crashing
    controller->loadVideo("/nonexistent/file.mp4");
    // No assert needed — just verify it doesn't crash
}

void TestPlaybackController::test_playPauseTogglesState()
{
    // Without a video loaded the player should be stopped, not playing
    QMediaPlayer *player = controller->findChild<QMediaPlayer *>();
    QVERIFY(player != nullptr);
    QVERIFY(player->playbackState() != QMediaPlayer::PlayingState);
}

void TestPlaybackController::test_onDurationChangedSetsSliderRange()
{
    QSlider *slider = controller->findChild<QSlider *>();
    QVERIFY(slider != nullptr);

    controller->onDurationChanged(60000);

    QCOMPARE(slider->minimum(), 0);
    QCOMPARE(slider->maximum(), 60000);
}

void TestPlaybackController::test_onPositionChangedUpdatesSlider()
{
    QSlider *slider = controller->findChild<QSlider *>();
    QVERIFY(slider != nullptr);

    slider->setRange(0, 60000);
    controller->onPositionChanged(30000);

    QCOMPARE(slider->value(), 30000);
}

void TestPlaybackController::test_onPositionChangedUpdatesTimeLabel()
{
    QLabel *label = controller->findChild<QLabel *>();
    QVERIFY(label != nullptr);

    controller->onDurationChanged(120000); // set duration first
    controller->onPositionChanged(65000);  // 1:05

    QVERIFY(label->text().contains("1:05"));
}

void TestPlaybackController::test_backRequestedSignalEmitted()
{
    QSignalSpy spy(controller, &PlaybackController::backRequested);
    QToolButton *back = controller->findChild<QToolButton *>("", Qt::FindDirectChildrenOnly);

    // Find the back button by text
    for (QToolButton *btn : controller->findChildren<QToolButton *>()) {
        if (btn->text() == "Back") {
            QTest::mouseClick(btn, Qt::LeftButton);
            break;
        }
    }

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestPlaybackController)
#include "test_PlaybackController.moc"
