#include "mainWindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QPixmap>
#include <QMenu>
#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <rlottie.h>
#include <QRadioButton>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), 
      animFrame(0), animTotalFrames(0), reactionEnabled(false)
{
    ui.setupUi(this);
    initReactionMenu();
    
    // Create video display label in frame
    videoLabel = new QLabel(ui.frame);
    videoLabel->setScaledContents(false);
    auto *layout = new QHBoxLayout(ui.frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(videoLabel);
    
    // Create models
    cameraModel = new CameraModel(this);
    fileManager = new FileManager();
    
    // Subscribe to camera updates
    cameraModel->subscribeToVideo(this);
    cameraModel->subscribeToRecording(this);
    
    // Setup button connections
    setupConnections();
    
    // Start camera
    cameraModel->startCamera();
}

void MainWindow::setupConnections()
{
    // Effect radio buttons
    connect(ui.radioButtonDefault, &QRadioButton::toggled, this, &MainWindow::onEffectChanged);
    connect(ui.radioButtonGrey, &QRadioButton::toggled, this, &MainWindow::onEffectChanged);
    connect(ui.radioButtonNegative, &QRadioButton::toggled, this, &MainWindow::onEffectChanged);
    connect(ui.radioButtonPixel, &QRadioButton::toggled, this, &MainWindow::onEffectChanged);
    
    // Control buttons
    connect(ui.ButtonRecord, &QToolButton::clicked, this, &MainWindow::onRecordClicked);
    connect(ui.ButtonPicture, &QToolButton::clicked, this, &MainWindow::onCaptureClicked);
}

void MainWindow::onFrameUpdated(const QImage &frame)
{
    displayFrame(frame);
}

void MainWindow::displayFrame(const QImage &frame)
{
    if (frame.isNull()) return;
    
    // Apply reactions overlay if enabled
    QImage displayFrame = frame.copy();
    if (reactionEnabled) {
        renderReaction(displayFrame);
    }
    
    // Scale to fit frame
    QPixmap pix = QPixmap::fromImage(displayFrame);
    QPixmap scaled = pix.scaled(ui.frame->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    videoLabel->setPixmap(scaled);
}

void MainWindow::onRecordingStateChanged(bool isRecording)
{
    if (isRecording) {
        ui.ButtonRecord->setIcon(QIcon(":/icons/icons/stop.png"));
    } else {
        ui.ButtonRecord->setIcon(QIcon(":/icons/icons/rec-button.png"));
        if (!currentRecordingPath.isEmpty()) {
            QMessageBox::information(this, "Recorded", QString("Video saved to:\n%1").arg(currentRecordingPath));
            currentRecordingPath.clear();
        }
    }
}

void MainWindow::onEffectChanged()
{
    if (ui.radioButtonDefault->isChecked()) {
        cameraModel->applyEffect(CameraModel::Default);
    } else if (ui.radioButtonGrey->isChecked()) {
        cameraModel->applyEffect(CameraModel::Greyscale);
    } else if (ui.radioButtonNegative->isChecked()) {
        cameraModel->applyEffect(CameraModel::Negative);
    } else if (ui.radioButtonPixel->isChecked()) {
        cameraModel->applyEffect(CameraModel::Pixelated);
    }
}

void MainWindow::onRecordClicked()
{
    if (cameraModel->isRecording()) {
        cameraModel->stopRecording();
    } else {
        currentRecordingPath = fileManager->getNextVideoFilePath();
        cameraModel->startRecording(currentRecordingPath);
    }
}

void MainWindow::onCaptureClicked()
{
    QString filepath = fileManager->getNextPhotoFilePath();
    cameraModel->captureFrame(filepath);
    QMessageBox::information(this, "Saved", QString("Photo saved to:\n%1").arg(filepath));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    displayFrame(cameraModel->getCurrentFrame());
}

void MainWindow::initReactionMenu()
{
    QMenu *reactionMenu = new QMenu(this);
    QAction *successAction = reactionMenu->addAction(QIcon(":/icons/icons/star.png"), "Success");
    QAction *agreeAction = reactionMenu->addAction(QIcon(":/icons/icons/100.png"), "Agree");
    QAction *loveAction = reactionMenu->addAction(QIcon(":/icons/icons/heart.png"), "Love");
    QAction *applauseAction = reactionMenu->addAction(QIcon(":/icons/icons/clap.png"), "Applause");

    connect(successAction, &QAction::triggered, this, [this]() {
        initReaction(QString(":/icons/icons/star-success-transparent.json"));
        reactionEnabled = true;
    });
    connect(agreeAction, &QAction::triggered, this, [this]() {
        initReaction(QString(":/icons/icons/one-hundred.json"));
        reactionEnabled = true;
    });
    connect(loveAction, &QAction::triggered, this, [this]() {
        initReaction(QString(":/icons/icons/heart-grow.json"));
        reactionEnabled = true;
    });
    connect(applauseAction, &QAction::triggered, this, [this]() {
        initReaction(QString(":/icons/icons/clap.json"));
        reactionEnabled = true;
    });

    ui.ButtonReactions->setMenu(reactionMenu);
    ui.ButtonReactions->setPopupMode(QToolButton::InstantPopup);
}

void MainWindow::initReaction(const QString &reactionPath)
{
    QFile reactionFile(reactionPath);
    if (!reactionFile.open(QIODevice::ReadOnly)) {
        return;
    }
    const QByteArray reactionJson = reactionFile.readAll();
    anim = rlottie::Animation::loadFromData(reactionJson.toStdString(), reactionPath.toStdString());
    if (anim) {
        animTotalFrames = anim->totalFrame();
        animFrame = 0;
    }
}

void MainWindow::renderReaction(QImage &image)
{
    if (!anim || !animTotalFrames) {
        return;
    }
    
    const int w = image.width() / 4;
    const int h = image.height() / 2;
    std::vector<uint32_t> pixels(w * h);
    rlottie::Surface surface(pixels.data(), w, h, w * 4);
    anim->renderSync(animFrame, surface);

    QImage overlay(reinterpret_cast<uchar*>(pixels.data()), w, h, QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&image);
    painter.drawImage(QRect(20, 20, w, h), overlay);
    painter.end();

    if (animFrame + 1 < animTotalFrames) {
        ++animFrame;
    } else {
        reactionEnabled = false;
        animFrame = 0;
    }
}

// Stub implementations for observer interface (handled by CameraModel now)
void MainWindow::onVideoFrame(const QVideoFrame &frame) { }
void MainWindow::toggleRecording() { }
void MainWindow::snapPicture() { }
void MainWindow::onRecorderStateChanged(QMediaRecorder::RecorderState state) { }
void MainWindow::onRecorderError(QMediaRecorder::Error error, const QString &errorString) { }
void MainWindow::reactToToggle(bool checked) { }