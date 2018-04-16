/// Simple Speed Limit Assistant
///
/// using Template Matching Approach on Multiple Scales
///
/// and
///
/// multiple threads in order to distribute computation
/// on multiple cores
///
/// ---
/// by Prof. Dr. Jürgen Brauer, www.juergenbrauer.org
///
/// note regarding std::ref --> http://jakascorner.com/blog/2016/01/arguments.html

#include "opencv2/opencv.hpp"
#include <conio.h>
#include <thread>   // for std:thread

using namespace cv;
using namespace std;

#define MATCH_THRESHOLD 0.50


void search_template(Mat frame, Mat img_template, double template_scale,
                     Rect* found_rect, double* match_value, bool* found)
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
   *found_rect = Rect(maxLoc, scaled_template.size());
   *match_value = maxVal;

   // 5. did we find it?
   if (*match_value >= MATCH_THRESHOLD)
      *found = true;
   else
      *found = false;

} // search_template


struct Detection {
   double   scale;
   Rect     found_rect;
   double   match_value;   
};


int main()
{
   printf("TrafficSignRecognition3\n");

   string videofilename1 = "test_videos\\traffic_sign_speed_limit_60.mp4";
   string videofilename2 = "V:\\01_job\\12_datasets\\traffic_scenes\\speed_limit_60_sign\\speed_limit_60_appears_two_times_scene1.mp4";
   string videofilename3 = "V:\\01_job\\12_datasets\\traffic_scenes\\speed_limit_60_sign\\speed_limit_60_appears_three_times_scene2.mp4";
   string template_filename = "test_videos\\template_speed_limit_60.png";

   string videofilename = videofilename1;
   VideoCapture cap(videofilename);
   if (!cap.isOpened()) {
      printf("Error! Could not open video file %s\n", videofilename.c_str());
      _getch();
      return -1;
   }

   Mat img_template = imread(template_filename);
   imshow("template", img_template);

   double speed_limit = -1.0;
   char txt[100];
   double WCET = -1.0;
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
      Rect found_rect[100];
      double match_value[100];
      bool found[100];
      clock_t tic, toc;
      double computation_time;
      vector<Detection> detections;
      vector<thread*> my_threads;

      /// ----------------------------------------------------------------
      tic = clock();
      int thread_nr = 0;
      bool USE_MULTIPLE_THREADS = true;
      for (double scale = 0.2f; scale < 1.0f; scale += 0.1)
      {
         if (USE_MULTIPLE_THREADS)
         {
            thread* t = new thread(search_template,
               frame, img_template, scale,
               &found_rect[thread_nr],
               &match_value[thread_nr],
               &found[thread_nr]
               );
            my_threads.push_back(t);
         }
         else
         {
            search_template(frame, img_template, scale,
                            &found_rect[thread_nr],
                            &match_value[thread_nr],
                            &found[thread_nr]
                           );
         }
         thread_nr++;
      }

      if (USE_MULTIPLE_THREADS)
         for (int i = 0; i < thread_nr; i++)
            my_threads[i]->join();

      thread_nr = 0;
      for (double scale = 0.2f; scale < 1.0f; scale += 0.1)
      {
         if (found[thread_nr])
         {
            Detection d;
            d.scale = scale;
            d.found_rect = found_rect[thread_nr];
            d.match_value = match_value[thread_nr];            
            detections.push_back( d );
         }
         thread_nr++;
      }      
      toc = clock();
      computation_time = (double)(toc - tic) / CLOCKS_PER_SEC;

      // found larger computation time than before?      
      if ((WCET == -1) || (computation_time > WCET))
      {
         WCET = computation_time;
      }
      /// ----------------------------------------------------------------


      // 3. show all detections
      for (int i=0; i<detections.size(); i++)
      {
         Detection d = detections[i];

         rectangle(frame, d.found_rect, Scalar(0, 255, 0), 2);

         // show scale and match value of that detection         
         sprintf_s(txt, "s=%.1f m=%.2f", d.scale, d.match_value);
         putText(frame,
            txt,
            Point(d.found_rect.tl().x, d.found_rect.br().y + 15),
            FONT_HERSHEY_SIMPLEX, 0.5, // font face and scale
            CV_RGB(0, 255, 0),
            1); // line thickness and type

         speed_limit = 60;
      }


      // 4. show computation time needed
      sprintf_s(txt, "Computation time: %.2f sec (%.2f fps). WCET: %.2f sec (%.2f fps)",
         computation_time,
         1.0 / computation_time,
         WCET,
         1.0 / WCET);
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
         Point(10, 20),
         FONT_HERSHEY_SIMPLEX, 0.7, // font face and scale
         CV_RGB(255, 255, 0),
         1); // line thickness and type


      // 6. show video frame with annotations
      imshow("UKSLA (University of Applied Sciences Kempten Speed Limit Assistant) "
             "by Prof. Dr. Jürgen Brauer", frame);


      // 7. if speed limit found, wait for user key press
      //    else continue with video
      if (detections.size() == 0)
         waitKey(1);
      else
         //waitKey(0); // wait for keypress
         waitKey(1);   // do not wait for keypress


      // 8. save detection images in order to create a video?
      if (0)
      {
         int nr_times_to_write = 1;
         if (detections.size() > 0)
            nr_times_to_write = 40;

         for (int i = 0; i < nr_times_to_write; i++)
         {
            char fname[500];
            sprintf_s(fname, "V:\\tmp\\img%04d.png", img_save_counter++);
            imwrite(fname, frame);
         }
      }

   } // while (the video still has frames for us)

} // main