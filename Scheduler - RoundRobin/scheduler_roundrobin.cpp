#include "scheduler_roundrobin.h"


scheduler_roundrobin::scheduler_roundrobin(int quantum) : scheduler(quantum)
{
  cout << "round robin scheduler generated." << endl;
}


int scheduler_roundrobin::choose_next_task()
{
  // get task from the front of the queue
  task* t = all_tasks.front();

  // remove that task from the front ...
  all_tasks.pop_front();

  // ... and insert it at the end of the queue
  all_tasks.push_back( t );

  // return the task id
  return t->id;  
}
