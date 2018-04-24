/// File: scheduling_simulation.cpp
///
/// Task: Simulates tasks coming in
///       that have to be scheduled for CPU computation time
///
/// ---
/// by Prof. Dr. Jürgen Brauer, www.juergenbrauer.org


#include <conio.h>
#include <iostream>
#include <time.h>

#include "scheduler_roundrobin.h"

int main()
{
  srand((unsigned int)time(NULL));

  // generate a random scheduler
  int quantum = 50;
  scheduler_roundrobin s(quantum);

  // generate 5 tasks with random length of needed computation times
  const int N = 5;
  for (int i = 0; i < N; i++)
  {
    s.add_new_task((rand() % 5 + 1) * 100); // time is 100,200,300,400,or 500 msec
  }

  // simulate until we are finished
  int simulation_time = 0;
  while (s.all_tasks_finished() == false)
  {
    cout << "\nSimulation time : " << simulation_time << endl;
    cout << "Quantum = " << quantum << endl;

    /////////////////////////////////////////////////////////////////
    // ask the scheduler to choose a task that shall be computed next
    int task_id = s.choose_next_task();
    /////////////////////////////////////////////////////////////////


    // get that task
    task* t = s.get_task_with_id(task_id);
    
    // how much time has this task still to compute?
    int computation_time_left = t->time_needed - t->time_computed;

    // is the rest computation < quantum?
    int computation_time = quantum;
    if (computation_time_left < quantum)
      computation_time = computation_time_left;

    // update time computed so far for this task
    t->time_computed += computation_time;

    // output our decision:
    cout << "Scheduler choosed task with id=" << t->id << endl;
    
    // show status of all tasks
    s.show_status();

    // is the chosen task finished?
    if (t->time_computed >= t->time_needed)
    {
      cout << "Task with id=" << t->id << " is finished with its computation!" << endl;

      // erase task from task vector/list
      s.delete_task(t);
    }

    // time goes by ...
    simulation_time += computation_time;

    _getch();
  }

  cout << "\n\nAll tasks finished their computation." << endl;
  cout << "Simulation finished. Press key to exit."    << endl;
  _getch();

} // main
