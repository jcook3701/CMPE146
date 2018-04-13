#ifndef WATCHDOG_HPP_
#define WATCHDOG_HPP_

#include "scheduler_task.hpp"
#include "shared_handles.h"
#include "uart3.hpp"
//#include "rn_xv_task.hpp"
#include "FreeRTOS.h"
#include "semphr.h"

class producer_task : public scheduler_task
{
    public :
        producer_task();
        bool run(void *p);
};

class consumer_task : public scheduler_task
{
    public :
        consumer_task();
        bool run(void *p);
};

class watchdog_task : public scheduler_task
{
    public :
        watchdog_task();
        bool run(void *p);
};

#endif /* WATCHDOG_HPP_ */
