/// File: scheduling_simulation.cpp
///
/// Task: Simulates tasks coming in
///       that have to be scheduled for CPU computation time
///
/// ---
/// by Prof. Dr. Juergen Brauer, www.juergenbrauer.org

#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

#include "scheduler_rms.h"
#include "math_functions.h"


int main()
{
  deque<task*>    info_all_periodic_tasks;
  vector<int>     nr_instances_started;
  vector<string>  infos_per_timestep;

  // generate a RMS scheduler
  int quantum = 10;
  scheduler_rms s( quantum );

  // ask for number of tasks, execution times and periods
  int nr_tasks, execution_time, period;
  cout << "\nPlease enter number of tasks to schedule: ";
  cin  >> nr_tasks;
  for (int i = 0; i < nr_tasks; i++)
  {
    cout << "\n\tEnter execution time for task " << i << " in ms : ";
    cin  >> execution_time;
    cout << "\tEnter period for task " << i << " in ms : ";
    cin  >> period;    

    // store information about this task in order to be able
    // to restart it all <period> ms
    task* t = new task();
    t->time_needed = execution_time;
    t->period      = period;
    info_all_periodic_tasks.push_back( t );    
    nr_instances_started.push_back( 0 );
  }


  s.schedulability_test(info_all_periodic_tasks);

  // compute kgV (Least Common Multiple) of period times
  int simulation_end_time =
     LCM(info_all_periodic_tasks[0]->period, info_all_periodic_tasks[1]->period);
  for (int i = 2; i < nr_tasks; i++)
  {
     // get the i-th task
     task* t = info_all_periodic_tasks[i];
     simulation_end_time = LCM(simulation_end_time, info_all_periodic_tasks[i]->period);
  }
  printf("\nLCM of all tasks period lengths is %d. So I will simulate up to time=%d ms...\n",
     simulation_end_time, simulation_end_time);


  cout << "\nPress a key to start the simulation!" << endl;
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { }
  getchar();
  

  // simulate until we are finished
  int simulation_time = 0;
  char info_str[5000];
  bool deadline_violation = false;
  while ((deadline_violation == false) && (simulation_time < simulation_end_time))
  {
    cout << "\nSimulation time : " << simulation_time << "\n";
    cout << "Quantum = " << quantum << "\n";
    for (int i = 0; i < nr_tasks; i++)
    {
      // get the i-th task
      task* t = info_all_periodic_tasks[i];
      printf("Periodic task #%d : exec_time=%d ms, period=%d ms\n", i, t->time_needed, t->period);
    }
    cout << "------------------------------------\n\n";

    // start periodic tasks
    for (int i = 0; i < nr_tasks; i++)
    {
      // get the i-th task
      task* t = info_all_periodic_tasks[i];

      // how many instances should we have started of this task till now?
      int nr_instances_should_have_started = (simulation_time / t->period) + 1;

      // how many instances do we have started already?
      if (nr_instances_started[i] < nr_instances_should_have_started)
      {
        // start new instance of this task now!
        s.add_new_task( i, t->time_needed, t->period );

        //cout << "Started new instance of task " << i << " (period time: ) " << t->period << "\n";

        nr_instances_started[i]++;
      }

    } // for (all periodic tasks)


    // prepare a string that contains the information which tasks waited
    char tasks_waiting[1000];
    sprintf(tasks_waiting, " ");
    for (unsigned int i = 0; i < s.get_task_list().size(); i++)
      sprintf(tasks_waiting, "%s%d ", tasks_waiting, s.get_task_list()[i]->id);


    ////////////////////////
    // deadline violation?
    ////////////////////////
    for (unsigned int i = 0; i < s.get_task_list().size(); i++)
    {
       task* t1 = s.get_task_list()[i];
       for (unsigned int j = 0; j < s.get_task_list().size(); j++)
       {
          task* t2 = s.get_task_list()[j];
          if ((t1->id == t2->id) && (i != j))
          {
             cout << "\nDeadline of task " << t1->id << " was violated! --> simulation will cancel.\n";
             printf("\a\a\a"); // beep, beep, beep
             deadline_violation = true;
             break;
          }
       } // for (j)

       if (deadline_violation)
          break;
    } // for (i)

    
    // are there any tasks at all?
    if (s.get_task_list().size() > 0)
    {

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
      cout << "\n*** Scheduler choosed task #" << t->id << " ***\n";

      // show status of all tasks
      s.show_status();

      // is the chosen task finished?
      if (t->time_computed == t->time_needed)
      {
        cout << "Task #" << t->id << " has now finished its computation!" << endl;

        // erase task from task vector/list
        s.delete_task(t);
      }

      // prepare new time step info string          
      sprintf(info_str, "Time %d : choosed task #%d (tasks that waited were: %s)", simulation_time, task_id, tasks_waiting);

      // time goes by ...
      simulation_time += computation_time;
      

    } // if (there are any tasks in the task list at all currently)
    else
    {
      sprintf(info_str, "Time %d : no tasks in task queue!", simulation_time);      
      simulation_time += quantum;
    }
    infos_per_timestep.push_back(info_str);

    
    // show infos for all time steps so far
    cout << "\n";
    for (unsigned int i = 0; i < infos_per_timestep.size(); i++)
      cout << infos_per_timestep[i].c_str() << endl;


    getchar();
           
  } // while (continue scheduling simulation)
  
  
  cout << "Simulation finished at time step " << simulation_time << endl;
  cout << "Result: ";
  if (deadline_violation)
     cout << ":( A deadline was violated!\n";
  else
     cout << ":) No deadline was violated\n";
  cout << "Press any key to exit." << endl;
  
  cout << "Simulation finished. Press key to exit." << endl;
  while ((c = getchar()) != '\n' && c != EOF) { }
  getchar();
  
} // main
