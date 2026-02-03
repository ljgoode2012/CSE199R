#include <iostream>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <QtWidgets>
#include <QApplication>
#include <QLabel>
#include <QImage>
#include <QTimer>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

using namespace cv;
using namespace std;

class VideoWidget : public QWidget {
    Q_OBJECT
private:
    QLabel *label;
    VideoCapture camera;
    QTimer *timer;
    bool isBlackWhite = false;

public:
    VideoWidget() {
        cout << "VideoWidget constructor starting..." << endl;
        label = new QLabel();
        camera.open(0);
        
        if (!camera.isOpened()) {
            label->setText("Error: Cannot open camera!");
            resize(400, 100);
            return;
        }
        
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &VideoWidget::updateFrame);
        timer->start(30);  // Update every 30ms
        
        resize(800, 600);
        setWindowTitle("Video Feed");

        // Main layout
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(5, 5, 5, 5);
        
        QRadioButton *blackWhite = new QRadioButton("Black/White");
        QRadioButton *colorMode = new QRadioButton("Color");

        
        connect(blackWhite, &QRadioButton::toggled, this, &VideoWidget::setBlackWhite);
        mainLayout->addWidget(label);
        mainLayout->addStretch();
        mainLayout->addWidget(blackWhite);
        mainLayout->addWidget(colorMode); 
        
        setLayout(mainLayout);
    }

    private slots:
    void updateFrame() {
        Mat frame;
        camera >> frame;
        
        if (frame.empty()) {
            cout << "Frame is empty!" << endl;
            label->setText("No camera feed");
            return;
        }
        
        if (isBlackWhite) {
            applyBlackWhite(frame);
        } else {
            cvtColor(frame, frame, COLOR_BGR2RGB);
        }

        QImage image = QImage(frame.data, frame.cols, frame.rows, 
                             frame.step, QImage::Format_RGB888);
        label->setPixmap(QPixmap::fromImage(image).scaledToWidth(750, Qt::SmoothTransformation));
    }
    void applyBlackWhite(Mat &frame) {
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        cvtColor(frame, frame, COLOR_GRAY2RGB);
    }
    void setBlackWhite(bool checked) {
        isBlackWhite = checked;
    }
};


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VideoWidget window;
    window.show();
    return app.exec();
}

#include "demo.moc"




