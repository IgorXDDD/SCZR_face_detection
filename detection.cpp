#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
using namespace std;
// using namespace cv;
void detectAndDisplay( cv::Mat frame );
cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eyes_cascade;

int main( int argc, const char** argv )
{
    cv::String face_cascade_name = string("haarcascades/haarcascade_frontalface_alt.xml");


    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) )
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };
    int camera_device = 0;
    cv::VideoCapture capture;

    //-- 2. Read the video stream
    capture.open( camera_device );
    if ( ! capture.isOpened() )
    {
        cout << "--(!)Error opening video capture\n";
        return -1;
    }
    cv::Mat frame;
    while ( capture.read(frame) )
    {
        if( frame.empty() )
        {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }
        //-- 3. Apply the classifier to the frame
        detectAndDisplay( frame );
        if( cv::waitKey(10) == 27 )
        {
            break; // escape
        }
    }
    return 0;
}



void detectAndDisplay( cv::Mat frame )
{
    cv::Mat frame_gray;
    cv::Mat crop;
    cv::Mat res;
    cv::Mat gray;
    int filenumber =1;
    cvtColor( frame, frame_gray, cv::COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    std::vector<cv::Rect> faces;
    std::vector<std::pair<std::string,cv::Mat>> faces_file;
    face_cascade.detectMultiScale( frame_gray, faces );

    cv::Rect roi_b, roi_c;
    

    //kazda z twarzy dodamy do wektora
    for ( size_t i = 0; i < faces.size(); i++ )
    {

        roi_b.x = faces[i].x;
        roi_b.y = faces[i].y;
        roi_b.width = (faces[i].width);
        roi_b.height = (faces[i].height);


        crop = frame(roi_b);
        resize(crop, res, cv::Size(128, 128), 0, 0, cv::INTER_LINEAR); // This will be needed later while saving images
        // cvtColor(crop, gray, cv::COLOR_BGR2GRAY); // Convert cropped image to Grayscale

        // Form a filename
        string filename = "";
        stringstream ssfn;
        ssfn << filenumber << ".jpg";
        filename = ssfn.str();
        filenumber++;

         //imwrite(filename, crop);
         faces_file.push_back(std::make_pair(filename,crop));

        // to tylko do wyswietlania zielonego prostokata wokol twarzy
        cv::Point pt1(faces[i].x-2, faces[i].y-2); // Display detected faces on main window
        cv::Point pt2((faces[i].x + faces[i].height+1), (faces[i].y + faces[i].width+1));
        cv::rectangle(frame, pt1, pt2, cv::Scalar(0, 255, 0), 2, 8, 0); 

    }
    // zapisywanie do pliku kazdej z twarzy
    // for(auto i = faces_file.begin(); i!= faces_file.end(); i++)
    // {
    //     imwrite(i->first,i->second);
    // }
    //wyswietlanie
    imshow( "Capture - Face detection", frame );
}