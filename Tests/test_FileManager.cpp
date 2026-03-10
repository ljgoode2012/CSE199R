#include <QtTest>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include "../Models/FileManager.h"

class TestFileManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    
    void test_getVideoDirectory();
    void test_getPhotoDirectory();
    void test_getNextVideoFilePath();
    void test_getNextPhotoFilePath();
    void test_uniqueFilePaths();

private:
    FileManager *fileManager;
};

void TestFileManager::initTestCase()
{
    fileManager = new FileManager();
    QVERIFY(fileManager != nullptr);
}

void TestFileManager::cleanupTestCase()
{
    delete fileManager;
}

void TestFileManager::test_getVideoDirectory()
{
    QString videoDir = fileManager->getVideoDirectory();
    QVERIFY(!videoDir.isEmpty());
    QVERIFY(videoDir.contains("CSE199R") || videoDir.contains("Videos"));
}

void TestFileManager::test_getPhotoDirectory()
{
    QString photoDir = fileManager->getPhotoDirectory();
    QVERIFY(!photoDir.isEmpty());
    QVERIFY(photoDir.contains("CSE199R") || photoDir.contains("Pictures"));
}

void TestFileManager::test_getNextVideoFilePath()
{
    QString filepath = fileManager->getNextVideoFilePath();
    QVERIFY(!filepath.isEmpty());
    QVERIFY(filepath.endsWith(".mp4"));
    QVERIFY(filepath.contains("video_"));
}

void TestFileManager::test_getNextPhotoFilePath()
{
    QString filepath = fileManager->getNextPhotoFilePath();
    QVERIFY(!filepath.isEmpty());
    QVERIFY(filepath.endsWith(".png"));
    QVERIFY(filepath.contains("photo_"));
}

void TestFileManager::test_uniqueFilePaths()
{
    // Get video paths - with timestamps they should differ
    QString path1 = fileManager->getNextVideoFilePath();
    // Wait for timestamp to change (at least 1 second for timestamp precision)
    QTest::qWait(1100);
    QString path2 = fileManager->getNextVideoFilePath();
    
    QVERIFY(path1 != path2);
    
    // Same for photos
    QString photoPath1 = fileManager->getNextPhotoFilePath();
    QTest::qWait(1100);
    QString photoPath2 = fileManager->getNextPhotoFilePath();
    
    QVERIFY(photoPath1 != photoPath2);
}

QTEST_MAIN(TestFileManager)
#include "test_FileManager.moc"
