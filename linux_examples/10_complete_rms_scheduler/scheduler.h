/// File: scheduler.h
///
/// Task: Interface of base scheduler class
///       All specific scheduler algorithms
///       should derive from this class.
///
/// ---
/// by Prof. Dr. Juergen Brauer, www.juergenbrauer.org

#pragma once

#include <iostream>

#include <deque>

using namespace std;


struct task
{
  int  id;
  int  time_needed;
  int  time_computed;
  int  period;
};



// class is abstract, i.e. no instantiation possible
// it serves only as a generic interface for all scheduler classes
class scheduler
{
  protected:

    int               quantum;

    deque<task*>      all_tasks;

    int               next_task_id;

    
  public:
                      scheduler(int quantum);

    void              add_new_task(int id, int time_need, int period);

    bool              all_tasks_finished();

    void              show_status();

    task*             get_task_with_id(int id);

    void              delete_task(task* t);

    virtual int       choose_next_task() = 0;

    deque<task*>      get_task_list();

    /// new methods

    virtual bool      schedulability_test(deque<task*> task_list) = 0;

    ///

};
