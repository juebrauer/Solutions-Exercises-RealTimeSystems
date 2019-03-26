/// Simple Speed Limit Assistant
/// using Template Matching Approach on Multiple Scales
///
/// ---
/// by Prof. Dr. Jürgen Brauer, www.juergenbrauer.org
///

#include "opencv2/opencv.hpp"
#include <conio.h>

using namespace cv;
using namespace std;

#define MATCH_THRESHOLD 0.45


bool search_template(Mat frame, Mat img_template, double template_scale,
                     Rect& found_rect, double& match_value)
{
   // 1. prepare scaled template
   Mat scaled_template;
   int w = img_template.cols;
   int h = img_template.rows;
   resize(img_template, scaled_template, Size((int)(w*template_scale), (int)(h*template_scale)));

   // 2. define search method
   int match_method = CV_TM_CCOEFF_NORMED; // the larger the value, the better

   // 3. search scaled template
   Mat result;
   matchTemplate(frame, scaled_template, result, match_method);
   //imshow("result", result);

   // 4. compute best matching location
   double minVal, maxVal;
   Point minLoc, maxLoc;
   minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);   
   found_rect = Rect(maxLoc, scaled_template.size());
   match_value = maxVal;
   imshow("match result matrix", result);
   
   // 5. did we find it?
   bool found;
   if (match_value >= MATCH_THRESHOLD)
      found = true;
   else
      found = false;

   return found;

} // search_template


int main()
{
   printf("TrafficSignRecognition1\n");


   string videofilename = "test_videos\\traffic_sign_speed_limit_60.mp4";
   string template_filename = "test_videos\\template_speed_limit_60.png";

   VideoCapture cap(videofilename);
   if (!cap.isOpened()) {
      printf("Error! Could not open video file %s\n", videofilename.c_str());
      _getch();
      return -1;
   }

   Mat img_template = imread( template_filename );
   imshow("template", img_template);
         
   double speed_limit = -1.0;
   char txt[100];
   int img_save_counter = 0;
   while (true)
   {

      // 1. get next video frame
      Mat frame;
      cap >> frame;

      if (frame.empty())
         break;

      int width = frame.cols;
      int height = frame.rows;
      resize(frame, frame, Size(width/2, height/2));


      // 2. search for the "speed limit 60" sign
      Rect found_rect;
      double match_value;
      clock_t tic, toc;
      double computation_time;
      tic = clock();
      bool found = search_template(frame, img_template, 0.5,
                                   found_rect, match_value);
      toc = clock();
      computation_time = (double)(toc - tic) / CLOCKS_PER_SEC;


      // 3. show best matching location
      if (found)
      {         
         rectangle(frame, found_rect, Scalar(0, 255, 0), 2);

         // show match value         
         sprintf_s(txt, "%.3f", match_value);
         putText(frame,
            txt,
            Point(found_rect.tl().x, found_rect.br().y+15),
            FONT_HERSHEY_SIMPLEX, 0.5, // font face and scale
            CV_RGB(0, 255, 0),
            1); // line thickness and type

         speed_limit = 60;
      }


      // 4. show computation time needed
      sprintf_s(txt, "Computation time: %.2f seconds (%.2f fps)",
                computation_time, 1.0/computation_time);
      putText(frame,
         txt,
         Point(10, 40),
         FONT_HERSHEY_SIMPLEX, 0.5, // font face and scale
         CV_RGB(255, 255, 0),
         1); // line thickness and type


      // 5. show speed limit
      if (speed_limit == -1)
         sprintf_s(txt, "unknown speed limit");
      else
         sprintf_s(txt, "speed limit 60");                  
      putText(frame,
         txt,
         Point(10,20),
         FONT_HERSHEY_SIMPLEX, 0.7, // font face and scale
         CV_RGB(255, 255, 0),
         1); // line thickness and type


      // 5. show video frame with annotations
      imshow("UKSLA (University of Applied Sciences Kempten Speed Limit Assistant) "
             "by Prof. Dr. Jürgen Brauer", frame);


      // 6. if speed limit found, wait for user key press,
      //    else continue with video
      if (!found)
         waitKey(1);
      else
         waitKey(0);
      int nr_times_to_write = 1;
      if (found)
         nr_times_to_write = 50;

      if (0)
      {
         for (int i=0; i<nr_times_to_write; i++)
         {
            char fname[500];
            sprintf_s(fname, "V:\\tmp\\img%04d.png", img_save_counter++);
            imwrite(fname, frame);
         }
      }

   } // while (the video still has frames for us)

} // main