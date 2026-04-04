#include <QtTest>
#include <QToolButton>
#include <QScrollArea>
#include <QDir>
#include <QFile>
#include "../Controllers/GalleryController.h"
#include "../Models/FileManager.h"

class TestGalleryController : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_constructorCreatesWidget();
    void test_refreshWithNoVideosShowsEmptyGrid();
    void test_refreshWithVideosShowsThumbnails();
    void test_thumbnailClickEmitsVideoSelected();
    void test_backRequestedSignalEmitted();
    void test_refreshClearsPreviousThumbnails();

private:
    GalleryController *controller;
    FileManager *fileManager;
    QStringList createdFiles; // track temp files to clean up

    void createFakeVideo(const QString &path);
};

void TestGalleryController::initTestCase()
{
    fileManager = new FileManager();
    controller = new GalleryController(fileManager);
    QVERIFY(controller != nullptr);
}

void TestGalleryController::cleanupTestCase()
{
    for (const QString &f : createdFiles)
        QFile::remove(f);

    delete controller;
    delete fileManager;
}

void TestGalleryController::createFakeVideo(const QString &path)
{
    QFile f(path);
    f.open(QIODevice::WriteOnly);
    f.write("fake mp4 data");
    f.close();
    createdFiles.append(path);
}

void TestGalleryController::test_constructorCreatesWidget()
{
    QVERIFY(controller->layout() != nullptr);
}

void TestGalleryController::test_refreshWithNoVideosShowsEmptyGrid()
{
    // Remove any pre-existing videos from the test directory
    QDir dir(fileManager->getVideoDirectory());
    QStringList existing = dir.entryList({"*.mp4"}, QDir::Files);
    for (const QString &f : existing)
        dir.remove(f);

    controller->refresh();
    // Should not crash with an empty directory
}

void TestGalleryController::test_refreshWithVideosShowsThumbnails()
{
    QString path = fileManager->getVideoDirectory() + "/test_video.mp4";
    createFakeVideo(path);

    controller->refresh();

    // There should be at least one QToolButton in the gallery after refresh
    QList<QToolButton *> buttons = controller->findChildren<QToolButton *>();
    // One button is the back button, the rest are thumbnails
    int thumbnailButtons = 0;
    for (QToolButton *btn : buttons) {
        if (btn->text() != "← Back" && btn->text() != "Back")
            thumbnailButtons++;
    }
    QVERIFY(thumbnailButtons >= 1);
}

void TestGalleryController::test_thumbnailClickEmitsVideoSelected()
{
    QString path = fileManager->getVideoDirectory() + "/test_signal.mp4";
    createFakeVideo(path);

    controller->refresh();

    QSignalSpy spy(controller, &GalleryController::videoSelected);

    // Click the first non-back thumbnail button
    for (QToolButton *btn : controller->findChildren<QToolButton *>()) {
        if (btn->text() != "← Back" && btn->text() != "Back") {
            QTest::mouseClick(btn, Qt::LeftButton);
            break;
        }
    }

    QCOMPARE(spy.count(), 1);
    // The emitted path should be an absolute path to an mp4
    QString emittedPath = spy.at(0).at(0).toString();
    QVERIFY(emittedPath.endsWith(".mp4"));
}

void TestGalleryController::test_backRequestedSignalEmitted()
{
    QSignalSpy spy(controller, &GalleryController::backRequested);

    for (QToolButton *btn : controller->findChildren<QToolButton *>()) {
        if (btn->text() == "← Back" || btn->text() == "Back") {
            QTest::mouseClick(btn, Qt::LeftButton);
            break;
        }
    }

    QCOMPARE(spy.count(), 1);
}

void TestGalleryController::test_refreshClearsPreviousThumbnails()
{
    // Refresh twice — the second call should not double up buttons
    controller->refresh();
    int countAfterFirst = controller->findChildren<QToolButton *>().count();

    controller->refresh();
    int countAfterSecond = controller->findChildren<QToolButton *>().count();

    QCOMPARE(countAfterFirst, countAfterSecond);
}

QTEST_MAIN(TestGalleryController)
#include "test_GalleryController.moc"
