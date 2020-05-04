/// Simple Speed Limit Assistant
///
/// To build this project type in:
///
/// g++ tsr2.cpp -o tsr2 `pkg-config --cflags --libs opencv4`
///
/// ---
/// by Prof. Dr. Juergen Brauer, www.juergenbrauer.org
///

#include "opencv2/opencv.hpp"
#include <chrono>


using namespace cv;
using namespace std;

#define MATCH_THRESHOLD 0.45


bool search_template(Mat frame, Mat img_template, double template_scale,
   Rect& found_rect, double& match_value, bool& found)
{
   // 1. prepare scaled template
   Mat scaled_template;
   int w = img_template.cols;
   int h = img_template.rows;
   resize(img_template, scaled_template, Size((int)(w*template_scale), (int)(h*template_scale)));

   // 2. define search method
   int match_method = cv::TM_CCOEFF_NORMED; // the larger the value, the better

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

   // 5. did we find it?
   if (match_value >= MATCH_THRESHOLD)
      found = true;
   else
      found = false;

   return found;

} // search_template



struct Detection {
   double   scale;
   Rect     found_rect;
   double   match_value;
};


int main()
{
   printf("TrafficSignRecognition2\n");

   string videofilename = "test_videos/traffic_sign_speed_limit_60.mp4";
   string template_filename = "template_speed_limit_60.png";

   VideoCapture cap(videofilename);
   if (!cap.isOpened()) {
      printf("Error! Could not open video file %s\n", videofilename.c_str());
      return -1;
   }

   Mat img_template = imread(template_filename);
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
      resize(frame, frame, Size(width / 2, height / 2));


      // 2. search for the "speed limit 60" sign
      Rect found_rect;
      double match_value;
      bool found;
      vector<Detection> detections;

      /// ----------------------------------------------------------------
      auto tic = std::chrono::system_clock::now();
      
      int thread_nr = 0;
      
      for (double scale = 0.2f; scale < 1.0f; scale += 0.1)
      {
         search_template(frame, img_template, scale,
                         found_rect, match_value, found);         
         if (found)
         {
            Detection d;
            d.scale = scale;
            d.found_rect = found_rect;
            d.match_value = match_value;
            detections.push_back(d);
         }
         thread_nr++;
      }
      
      auto toc = std::chrono::system_clock::now();
      std::chrono::duration<double> duration = toc - tic;
      double computation_time = duration.count();
      /// ----------------------------------------------------------------


      // 3. show all detections
      for (int i = 0; i<detections.size(); i++)
      {
         Detection d = detections[i];

         rectangle(frame, d.found_rect, Scalar(0, 255, 0), 2);

         // show scale and match value of that detection         
         sprintf(txt, "s=%.1f m=%.2f", d.scale, d.match_value);
         putText(frame,
            txt,
            Point(d.found_rect.tl().x, d.found_rect.br().y + 15),
            FONT_HERSHEY_SIMPLEX, 0.5, // font face and scale
            CV_RGB(0, 255, 0),
            1); // line thickness and type

         speed_limit = 60;
      }


      // 4. show computation time needed
      sprintf(txt, "Computation time: %.2f seconds (%.2f fps)",
         computation_time, 1.0 / computation_time);
      putText(frame,
         txt,
         Point(10, 40),
         FONT_HERSHEY_SIMPLEX, 0.5, // font face and scale
         CV_RGB(255, 255, 0),
         1); // line thickness and type


             // 5. show speed limit
      if (speed_limit == -1)
         sprintf(txt, "unknown speed limit");
      else
         sprintf(txt, "speed limit 60");
      putText(frame,
         txt,
         Point(10, 20),
         FONT_HERSHEY_SIMPLEX, 0.7, // font face and scale
         CV_RGB(255, 255, 0),
         1); // line thickness and type


             // 5. show video frame with annotations
      imshow("UKSLA (University of Applied Sciences Kempten Speed Limit Assistant) "
         "by Prof. Dr. Juergen Brauer", frame);


      // 6. if speed limit found, wait for user key press OR do not wait
      //    else continue with video
      if (detections.size()==0)
         waitKey(1);
      else
         waitKey(1);


      int nr_times_to_write = 1;
      if (detections.size() > 0)
         nr_times_to_write = 40;

      /*
      for (int i = 0; i<nr_times_to_write; i++)
      {
         char fname[500];
         sprintf_s(fname, "~/img%04d.png", img_save_counter++);
         imwrite(fname, frame);
      }
      */

   } // while (the video still has frames for us)

} // main