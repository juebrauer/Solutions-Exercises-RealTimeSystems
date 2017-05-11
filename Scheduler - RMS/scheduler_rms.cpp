/// File: scheduler_rms.cpp
///
/// Task: Implementation of Rate Monotonic Scheduler
///
/// ---
/// by Prof. Dr. Jürgen Brauer, www.juergenbrauer.org

#include "scheduler_rms.h"


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
