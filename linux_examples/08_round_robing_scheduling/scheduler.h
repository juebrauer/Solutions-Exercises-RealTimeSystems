#pragma once

#include <iostream>

#include <deque>

using namespace std;


struct task
{
  int  id;
  int  time_needed;
  int  time_computed;
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

    void              add_new_task(int time_needed);

    bool              all_tasks_finished();

    void              show_status();

    task*             get_task_with_id(int id);

    void              delete_task(task* t);

    virtual int       choose_next_task() = 0;

};
