/// OpenCV - mini example
///
/// Simple example for testing whether your
/// installation of the OpenCV library worked
///
///
/// First you need to download, build and install OpenCV for C++ development:
/// See e.g. https://askubuntu.com/questions/1123955/install-opencv4-in-ubuntu-16
///
/// My commands to build OpenCV under Ubuntu 20.04:
/// - go to your source code directory: cd src
/// - get the newest OpenCV code: git clone https://github.com/opencv/opencv.git
/// - prepare a build directory with the OpenCV directory:
///   cd opencv
///   mkdir build
///   cd build
/// - prepare the build: cmake ../
/// - start the build: make (this will take a long time, ca. 60 minutes)
/// - install the binaries just built: sudo make install
///  
/// Note: for compiling this example with g++ type in:
///
///   g++ opencv_mini_example.cpp -o opencv_mini_example `pkg-config --cflags --libs opencv4`
///
/// ---
/// by Prof. Dr.-Ing. Juergen Brauer, www.juergenbrauer.org

#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: opencv_mini_example <image_filename>" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1]);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                // Show our image inside it.

    waitKey(0);             // Wait for a keystroke in the window
    return 0;
}