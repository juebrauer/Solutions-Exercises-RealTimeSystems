/// Least Laxity First (LLF) Scheduler simulation
///
/// by Prof. Dr. J�rgen Brauer
/// University of Applied Sciences Kempten

#include <iostream>
#include <vector>
#include <algorithm>    // std::max
#include <stdio.h>

using namespace std;

#include "scheduler_llf.h"
#include "math_functions.h"
#include <time.h>


int main()
{
  srand(time(NULL));

  while (true)
  {
    system("clear");

    deque<task*>    info_all_periodic_tasks;
    vector<int>     nr_instances_started;
    vector<string>  infos_per_timestep;

    // generate a LLF scheduler
    int quantum = 1;
    scheduler_llf s(quantum);

    // randomly generate some tasks
    // with random:
    //  - execution_time
    //  - period
    /*
    int nr_tasks = rand() % 4 + 2; // we will generate 2-5 tasks
    printf("\nI will generate %d random periodic tasks for the following LLF scheduling simulation...\n", nr_tasks);
    for (int i = 0; i < nr_tasks; i++)
    {
      // store information about this task in order to be able
      // to restart it all <period> ms
      task* t = new task();

      // execution time will be 1-5 time slots
      t->time_needed = (rand() % 5 + 1) * quantum;

      // compute deadline, which is in this simulation assumbed to be the same as the period
      // 2-6 time slots more than execution time needed:
      t->period = t->time_needed + (rand() % 5 + 2) * quantum;

      // store info 
      info_all_periodic_tasks.push_back(t);
      nr_instances_started.push_back(0);

      // output info
      printf("\tTask %d: execution time=%d ms,  relative deadline=period length=%d ms\n",
        i,
        t->time_needed,
        t->period);
    }*/

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

    // how much CPU utilization will be there for the randomly
    // generated task set?
    double u = s.compute_cpu_utilization_u(info_all_periodic_tasks);
    printf("\nCPU utilization u for this task set is : %.2f\n", u);

    // compute kgV of period times
    int simulation_end_time =
      kgV(info_all_periodic_tasks[0]->period, info_all_periodic_tasks[1]->period);
    for (int i = 2; i < nr_tasks; i++)
    {
      // get the i-th task
      task* t = info_all_periodic_tasks[i];
      simulation_end_time = kgV(simulation_end_time, info_all_periodic_tasks[i]->period);
    }
    printf("\nkgV of all task period length is %d. So I will simulate up to time=%d ms...\n",
      simulation_end_time, simulation_end_time);

    //s.schedulability_test( info_all_periodic_tasks );

    cout << "\nPress a key to start the simulation!" << endl;
    int c;    
    getchar();


    // simulate until we are finished
    int simulation_time = 0;
    bool deadline_violation = false;
    char info_str[500];

    bool simulate_step_by_step = true; // turn this on or off

    while ((deadline_violation == false) && (simulation_time < simulation_end_time))
    {
      system("clear");

      // inform the scheduler what the current time is
      s.set_time( simulation_time );

      cout << "\nSimulation time : " << simulation_time << "\n";
      cout << "Quantum = " << quantum << "\n";

      // show info about which periodic tasks are there in general
      for (int i = 0; i < nr_tasks; i++)
      {
        // get the i-th task
        task* t = info_all_periodic_tasks[i];
        printf("Periodic task #%d : exec_time=%d ms, period=%d ms\n", i, t->time_needed, t->period);
      }
      cout << "------------------------------------\n\n";

      // start period tasks
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
          s.add_new_task(i, t->time_needed, t->period, simulation_time);

          //cout << "Started new instance of task " << i << " (period time: ) " << t->period << "\n";

          nr_instances_started[i]++;
        }

      } // for (all periodic tasks)


      // prepare a string that contains the information which tasks waited
      char tasks_waiting[500];
      sprintf(tasks_waiting, " ");
      for (unsigned int i = 0; i < s.get_task_list().size(); i++)
        sprintf(tasks_waiting, "%s%d ", tasks_waiting, s.get_task_list()[i]->id);


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
      if (simulate_step_by_step)
      {
        for (unsigned int i = 0; i < infos_per_timestep.size(); i++)
          cout << infos_per_timestep[i].c_str() << endl;
      }
      else
      {
        // show only last 10 entries...
        for (unsigned int i = max(0, (int)infos_per_timestep.size() - 10); i < infos_per_timestep.size(); i++)
          cout << infos_per_timestep[i].c_str() << endl;
      }


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
            deadline_violation = true;
            break;
          }
        } // for (j)

        if (deadline_violation)
          break;
      } // for (i)


      if (simulate_step_by_step)
      {
          //while ((c = getchar()) != '\n' && c != EOF) { }
          getchar();
      }

    } // while (continue scheduling simulation)

    cout << "LLF Simulation finished. Press key to start another one." << endl;

    while ((c = getchar()) != '\n' && c != EOF) { }
    getchar();

  }

} // main
