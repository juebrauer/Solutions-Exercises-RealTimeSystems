/// File: scheduler_edf.cpp
///
/// Task: Earliest Deadline First (EDF) scheduler
///
/// ---
/// by Prof. Dr. Jürgen Brauer, www.juergenbrauer.org

#include "scheduler_edf.h"



scheduler_edf::scheduler_edf(int quantum) : scheduler(quantum)
{
   cout << "Earliest Deadline First (EDF) scheduler generated." << endl;
}


/// go through all tasks and choose the one
/// with the highest priority to be executed next
///
/// According to EDF scheduling
/// the task that has the nearest deadline has
/// the highest priority

int scheduler_edf::choose_next_task()
{
   task* task_earliest_deadline = all_tasks[0];
   for (unsigned int nr = 1; nr < all_tasks.size(); nr++)
   {
      if (all_tasks[nr]->next_deadline_absolute <
          task_earliest_deadline->next_deadline_absolute)

         task_earliest_deadline = all_tasks[nr];
   }

   // return the task id
   return task_earliest_deadline->id;
}



bool scheduler_edf::schedulability_test(deque<task*> task_list)
{
   cout << "\nSchedulability test:\n";
   cout << "----------------------\n";

   // get number <n> of tasks
   int n = (int)task_list.size();

   double u = compute_cpu_utilization_u(task_list);

   // compute limit for secure schedulability
   // 
   // note:
   // for deadlines = period lengths
   // EDF can schedule the set of task with fullfilling
   // the deadlines up to a CPU utlization of 100%!
   //
   // see: https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling
   double limit = 1.0;

   // is the CPU utilization below or equal to this limit for secure schedulability?
   cout << "   CPU utilization u                     = " << u << "\n";
   cout << "   limit for u for secure schedulability = " << limit << "\n";
   cout << "\n=> the specified set of " << n << " tasks can be scheduled ..\n.";
   if (u <= limit)
   {
      cout << "... with a guarantee that all deadlines will be met =)\n";
      return true;
   }
   else
   {
      cout << "... without (!) any guarantee that are deadlines will be met =(\n";
      return false;
   }

} // schedulability_test
