#pragma once

#include <iostream>

#include "scheduler.h"

using namespace std;

class scheduler_roundrobin : public scheduler
{
public:

            scheduler_roundrobin(int quantum);

  int       choose_next_task();


};

