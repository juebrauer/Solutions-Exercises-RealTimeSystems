/// File: scheduler_rms.cpp
///
/// Task: Implementation of Rate Monotonic Scheduler
///
/// ---
/// by Prof. Dr. Juergen Brauer, www.juergenbrauer.org

#include "scheduler_rms.h"

#include <math.h>


scheduler_rms::scheduler_rms(int quantum) : scheduler(quantum)
{
  cout << "Rate Monotonic Scheduler (RMS) generated." << endl;
}


/// go through all tasks and choose the one
/// with the highest priority to be executed next
///
/// task priority := 1 / period time of task

int scheduler_rms::choose_next_task()
{  
  task* task_max_priority = all_tasks[0];
  for (unsigned int nr = 1; nr < all_tasks.size(); nr++)
  {
    if (all_tasks[nr]->period < task_max_priority->period)
      task_max_priority = all_tasks[nr];
  }
  
  // return the task id
  return task_max_priority->id;
}


bool scheduler_rms::schedulability_test(deque<task*> task_list)
{
   cout << "\nSchedulability test:\n";
   cout << "----------------------\n";

   // get number <n> of tasks
   int n = (int) task_list.size();

   // compute CPU utilization u
   double u = 0.0;
   for (int i = 0; i < n; i++)
   {
      task* t = task_list[i];
      u += (double)t->time_needed / (double)t->period;
   }

   // compute limit for secure schedulability
   double dn = (double)n;
   double nth_root_of_2 = pow(2.0, 1.0 / dn);
   double limit = dn * (nth_root_of_2 - 1.0);

   // is the CPU utilization below or equal to this limit for secure schedulability?
   cout << "   CPU utilization u                     = " << u << "\n";
   cout << "   limit for u for secure schedulability = " << limit << "\n";
   cout << "\n=> the specified set of " << n << " tasks ...\n";
   if (u <= limit)
   {
      cout << "... can be scheduled with a guarantee that all deadlines will be met =)\n";
      return true;
   }
   else
   {
      cout << "... cannot be scheduled with a guarantee that all deadlines will be met =(\n";
      return false;
   }

} // schedulability_test
