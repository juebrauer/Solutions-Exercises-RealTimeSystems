/// File: scheduler_edf.h
///
/// Task: Interface of Earliest Deadline First Scheduler
///
/// ---
/// by Prof. Dr. Juergen Brauer, www.juergenbrauer.org


#pragma once

#include <iostream>

#include "scheduler.h"

using namespace std;

/// EDF: Earliest Deadline First
/// see https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling

class scheduler_edf : public scheduler
{
public:

            scheduler_edf(int quantum);

  int       choose_next_task();


  /// new methods

  bool      schedulability_test(deque<task*> task_list);

  ///

};

