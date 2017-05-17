/// File: scheduler_rms.h
///
/// Task: Interface of Rate Monotonic Scheduler
///
/// ---
/// by Prof. Dr. Jürgen Brauer, www.juergenbrauer.org


#pragma once

#include <iostream>

#include "scheduler.h"

using namespace std;

/// RMS: Rate Monotonic Scheduler
/// see https://en.wikipedia.org/wiki/Rate-monotonic_scheduling

class scheduler_rms : public scheduler
{
public:

            scheduler_rms(int quantum);

  int       choose_next_task();


  /// new methods

  bool      schedulability_test(deque<task*> task_list);

  ///

};

