/// File: scheduler_rms.h
///
/// Task: Implementation of base scheduler class
///
/// ---
/// by Prof. Dr. Jürgen Brauer, www.juergenbrauer.org

#pragma once


#include "scheduler.h"


scheduler::scheduler(int quantum)
{
  this->quantum      = quantum;
  next_task_id       = 0;
  cout << "scheduler base class constructor called." << endl;
}


bool scheduler::all_tasks_finished()
{
  if (all_tasks.size() == 0)
    return true;
  else
    return false;
}


void scheduler::add_new_task(int id, int time_needed, int period, int current_time)
{
  task* t = new task;
  t->id            = id;
  t->time_needed   = time_needed;
  t->time_computed = 0;
  t->period        = period;

  // compute and store information about next absolute deadline for this task
  t->next_deadline_absolute = current_time + t->period;

  all_tasks.push_back( t );

  cout << "added task with id=" << t->id << " that will have to compute for " << t->time_needed << " msecs. Period time: " << t->period << " ms\n";
}


void scheduler::show_status()
{
   int N = all_tasks.size();
   cout << "\nThere are " << N << " tasks that have not yet finished their computation." << endl;

   for (int i = 0; i < N; i++)
   {
      printf("\tTask #%d : Time computed=%d, \tTotal time needed: %d, \tNext Deadline: %d\n",
         all_tasks[i]->id,
         all_tasks[i]->time_computed,
         all_tasks[i]->time_needed,
         all_tasks[i]->next_deadline_absolute
         );
   } // for (i)

}


task* scheduler::get_task_with_id(int id)
{
  for (unsigned int i = 0; i < all_tasks.size(); i++)
  {
    if (all_tasks[i]->id == id)
      return all_tasks[i];
  }

  // ups! task with that id does not exist in list!
  return NULL; 
}


void scheduler::delete_task(task* t)
{
  for (unsigned int i = 0; i < all_tasks.size(); i++)
  {
    if (all_tasks[i]->id == t->id)
    {
      all_tasks.erase( all_tasks.begin() + i );
      return;
    }
  }
}



deque<task*> scheduler::get_task_list()
{
  return all_tasks;
}



double scheduler::compute_cpu_utilization_u(deque<task*> task_list)
{
   // get number <n> of tasks
   int n = task_list.size();

   // compute CPU utilization u
   double u = 0.0;
   for (int i = 0; i < n; i++)
   {
      task* t = task_list[i];
      u += (double)t->time_needed / (double)t->period;
   }

   return u;

} // compute_cpu_utilization_u