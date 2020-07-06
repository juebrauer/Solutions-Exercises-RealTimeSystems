/// scheduler_llf.h:  Least Laxity First (LLF) scheduler
///
/// by Prof. Dr. Juergen Brauer
/// University of Applied Sciences Kempten

#pragma once

#include <iostream>

#include "scheduler.h"

using namespace std;

/// LLF: Least Laxity First scheduler
/// see https://de.wikipedia.org/wiki/Least_Laxity_First

class scheduler_llf : public scheduler
{
public:

            scheduler_llf(int quantum);

  int       choose_next_task();

  bool      schedulability_test(deque<task*> task_list);


};

