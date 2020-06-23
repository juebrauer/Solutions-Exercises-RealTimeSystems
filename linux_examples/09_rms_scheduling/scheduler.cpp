/// File: scheduler_rms.h
///
/// Task: Implementation of base scheduler class
///
/// ---
/// by Prof. Dr. Juergen Brauer, www.juergenbrauer.org

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


void scheduler::add_new_task(int id, int time_needed, int period)
{
  task* t = new task;
  t->id            = id;
  t->time_needed   = time_needed;
  t->time_computed = 0;
  t->period        = period;

  all_tasks.push_back( t );

  cout << "added task with id=" << t->id << " that will have to compute for " << t->time_needed << " msecs. Period time: " << t->period << " ms\n";
}


void scheduler::show_status()
{
  int N = (int) all_tasks.size();
  cout << "\nThere are " << N << " tasks that have not yet finished their computation." << endl;
  for (int i = 0; i < N; i++)
  {
    cout << "\tTask #" << all_tasks[i]->id << " : Time computed=" << all_tasks[i]->time_computed << ", Time needed: " <<
      all_tasks[i]->time_needed << endl;
  }
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
