#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
// using namespace std;
void detectFaces( cv::Mat frame, cv::CascadeClassifier face_cascade);

void startCapturing();
void create_video();


int main( int argc, const char** argv )
{
    startCapturing();
}



void startCapturing()
{
    cv::String face_cascade_name = std::string("haarcascades/haarcascade_frontalface_alt.xml");
    //-- 1. Load the cascades
    cv::CascadeClassifier face_cascade;
    if( !face_cascade.load( face_cascade_name ) )
    {
        std::cout << "--(!)Error loading face cascade\n";
    };

    int camera_device = 0;
    cv::VideoCapture capture;

    //-- 2. Read the video stream
    capture.open( camera_device );
    if ( ! capture.isOpened() )
    {
        std::cout << "--(!)Error opening video capture\n";
    }
    cv::Mat frame;
    while ( capture.read(frame) )
    {
        if( frame.empty() )
        {
            std::cout << "--(!) No captured frame -- Break!\n";
            break;
        }
        //-- 3. Apply the classifier to the frame
        detectFaces( frame, face_cascade);
        if( cv::waitKey(10) == 27 )
        {
            break; // escape
        }
    }
}



void detectFaces( cv::Mat frame, cv::CascadeClassifier face_cascade)
{
    cv::Mat frame_gray;
    cv::Mat crop;
    cv::Mat res;
    int filenumber =1;
    cvtColor( frame, frame_gray, cv::COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    std::vector<cv::Rect> faces;
    // std::vector<std::pair<std::string,cv::Mat>> faces_file;
    face_cascade.detectMultiScale( frame_gray, faces );

    cv::Rect face_frame;
    

    //kazda z twarzy dodamy do wektora
    for ( size_t i = 0; i < faces.size(); i++ )
    {

        face_frame.x = faces[i].x;
        face_frame.y = faces[i].y;
        face_frame.width = (faces[i].width);
        face_frame.height = (faces[i].height);

        crop = frame(face_frame);
        resize(crop, res, cv::Size(128, 128), 0, 0, cv::INTER_LINEAR); // This will be needed later while saving images
        // cvtColor(crop, gray, cv::COLOR_BGR2GRAY); // Convert cropped image to Grayscale

        // Form a filename
        // string filename = "";
        // stringstream ssfn;
        // ssfn << filenumber << ".jpg";
        // filename = ssfn.str();
        // filenumber++;

        // imwrite(filename, crop);
        //  faces_file.push_back(std::make_pair(filename,crop));

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