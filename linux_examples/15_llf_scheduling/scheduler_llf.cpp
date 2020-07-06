/// scheduler_llf.cpp: Least Laxity First (LLF) scheduler
///
/// by Prof. Dr. Juergen Brauer
/// University of Applied Sciences Kempten

#include "scheduler_llf.h"


scheduler_llf::scheduler_llf(int quantum) : scheduler(quantum)
{
  cout << "Least Laxity First (LLF) scheduler generated." << endl;
}


/// go through all tasks and choose the one
/// with the highest priority to be executed next
///
/// According to LLF scheduling
/// the task that has the smallest slack time has
/// the highest priority

int scheduler_llf::choose_next_task()
{  
  task* task_least_laxity = NULL;
  for (unsigned int nr = 0; nr < all_tasks.size(); nr++)
  {
    // get next task
    task* t = all_tasks[nr];

    // compute laxity for that task
    t->laxity = t->next_deadline_absolute - current_time - 
      (t->time_needed - t->time_computed);

    // smaller laxity found?
    if ((task_least_laxity == NULL) || (t->laxity < task_least_laxity->laxity))
      task_least_laxity = t;
  }
  
  // return the task id
  return task_least_laxity->id;
}


bool scheduler_llf::schedulability_test(deque<task*> task_list)
{
  cout << "\nSchedulability test:\n";
  cout << "----------------------\n";

  // get number <n> of tasks
  int n = task_list.size();

  double u = compute_cpu_utilization_u( task_list );
    
  // compute limit for secure schedulability
  // 
  // note:
  // for deadlines = period lengths
  // LLF can schedule the set of task with fullfilling
  // the deadlines up to a CPU utlization of 100%!
  //
  // see: https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling
  double limit = 1.0;

  // is the CPU utilization below or equal to this limit for secure schedulability?
  cout << "   CPU utilization u                     = " << u     << "\n";
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