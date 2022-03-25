#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/objdetect/detection_based_tracker.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/cuda.hpp>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
using namespace std;
using namespace cv;
int maini(int argc, char** argv) {
    // declares all required variables
    Rect roi;
    // create a tracker object
    Ptr<Tracker> tracker = TrackerGOTURN::create();
    // set input video
    VideoCapture video("video.mp4");
    VideoWriter video_written("tracked_video.mp4", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(video.get(3), video.get(4)));
    // get bounding box
    // Exit if video is not opened
    if (!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return EXIT_FAILURE;
    }
    Mat frame;
    if (!video.read(frame))
    {
        cout << "Cannot read video file" << endl;
        return EXIT_FAILURE;
    }
    // Define initial boundibg box 
    Rect bbox(287, 23, 86, 320);
    // Uncomment the line below to select a different bounding box 
    //bbox = selectROI(frame, false); [/cpp]
    tracker->init(frame, bbox);
    while (video.read(frame))
    {
        // Start timer
        double timer = (double)getTickCount();

        // Update the tracking result
        bool ok = tracker->update(frame, bbox);

        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / ((double)getTickCount() - timer);

        if (ok)
        {
            // Tracking success : Draw the tracked object
            rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
        }
        else
        {
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
        }

        // Display tracker type on frame
        putText(frame, "GOTURN Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

        // Display FPS on frame
        
        putText(frame, "FPS : " + std::to_string(fps), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

        // Display frame.
        imshow("Tracking", frame);
        video_written.write(frame);
        // Exit if ESC pressed.
        if (waitKey(1) == 27) break;
    }
    return 0;
}