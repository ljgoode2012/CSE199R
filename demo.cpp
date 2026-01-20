#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

using namespace cv;
using namespace std;

int main() {

    VideoCapture cap(0);
    if(!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    Mat frame;
    int key;
    

    while(true) {
        cap >> frame;
        imshow("Video Frame", frame);
        key = waitKey(30);

        if (key == 27) {  // Exit on 'ESC' key
            break;}
    }

}
