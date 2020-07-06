/// scheduler.h: Scheduler base class
///
/// by Prof. Dr. Juergen Brauer
/// University of Applied Sciences Kempten

#pragma once

#include <iostream>

#include <deque>

using namespace std;


struct task
{
  int  id;
  int  time_needed;
  int  time_computed;
  int  next_deadline_absolute;
  int  period;
  int  laxity;
};



// class is abstract, i.e. no instantiation possible
// it serves only as a generic interface for all scheduler classes
class scheduler
{
  protected:

    int               quantum;

    deque<task*>      all_tasks;

    int               next_task_id;

    int               current_time;

    
  public:
                      scheduler(int quantum);

    void              set_time(int t);

    void              add_new_task(int id, int time_need, int period, int current_time);

    bool              all_tasks_finished();

    void              show_status();

    task*             get_task_with_id(int id);

    void              delete_task(task* t);

    virtual int       choose_next_task() = 0;

    deque<task*>      get_task_list();

    double            compute_cpu_utilization_u(deque<task*> task_list);

    virtual bool      schedulability_test(deque<task*> task_list) = 0;

};
