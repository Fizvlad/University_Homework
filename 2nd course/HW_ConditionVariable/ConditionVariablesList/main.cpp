#include <iostream>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <chrono>
#include <atomic>

#include "conditionList.h"

using namespace std;
using condition_list::conditionList;

void listener (mutex *globalMutex, conditionList <int> *condList, size_t *recievedNumbers, size_t maxNumbersAmount)
{
    {
        lock_guard <mutex> lockGuard(*globalMutex);
        cout << "! New listener created. Id: " << this_thread::get_id() << endl;
    }
    unique_lock <decltype(condList->mutex)> uniqueLock(condList->mutex);
    while (*recievedNumbers < maxNumbersAmount) {
        {
            lock_guard <mutex> lockGuard(*globalMutex);
            cout << "| Listener " << this_thread::get_id() << " is waiting" << endl;
        }
        auto wakeUpReason = condList->condition_variable.wait_for(uniqueLock, chrono::milliseconds(2000));
        if (wakeUpReason == cv_status::timeout && condList->size() == 0) continue; // Proceeding
        {
            lock_guard <mutex> lockGuard(*globalMutex);
            cout << "! Listener " << this_thread::get_id() << " woke up" << endl;
        }
        try {
            int data = condList->consume_back(true);
            {
                lock_guard <mutex> lockGuard(*globalMutex);
                cout << "- Listener " << this_thread::get_id() << " received data: " << data << endl;
                (*recievedNumbers)++;
                cout << "  Received numbers amount: " << *recievedNumbers << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(1000)); // Timeout
        } catch (out_of_range) {
            lock_guard <mutex> lockGuard(*globalMutex);
            cout << "? Listener " << this_thread::get_id() << " got no data :(" << endl;
        }
    }
    {
        lock_guard <mutex> lockGuard(*globalMutex);
        cout << "x Listener " << this_thread::get_id() << " ended work" << endl;
    }
}

void worker (mutex *globalMutex, conditionList <int> *condList, int workerNum, size_t toProduce)
{
    {
        lock_guard <mutex> lockGuard(*globalMutex);
        cout << "! New worker created. Id: " << this_thread::get_id() << endl;
    }
    // Working
    for (size_t i = 0; i < toProduce; i++) {
        this_thread::sleep_for(chrono::milliseconds(1000)); // Timeout
        int number = workerNum * 1000 + i + 1;
        condList->push_back(number);
        condList->notify_one();
        {
            lock_guard <mutex> lockGuard(*globalMutex);
            cout << "+ Worker " << this_thread::get_id() << " worked out number: " << number << endl;
        }
    }
    {
        lock_guard <mutex> lockGuard(*globalMutex);
        cout << "x Worker " << this_thread::get_id() << " ended work" << endl;
    }
}

using namespace std;

int main()
{
    mutex globalMutex;
    conditionList <int> condList;
    size_t recievedNumbers = 0; // Shared

    size_t WORKERS_AMOUNT = 3;
    size_t EACH_WORKER_LIMIT = 10;

    thread t1(listener, &globalMutex, &condList, &recievedNumbers, EACH_WORKER_LIMIT * WORKERS_AMOUNT);
    thread t2(listener, &globalMutex, &condList, &recievedNumbers, EACH_WORKER_LIMIT * WORKERS_AMOUNT);
    thread t3(listener, &globalMutex, &condList, &recievedNumbers, EACH_WORKER_LIMIT * WORKERS_AMOUNT);
    thread t4(listener, &globalMutex, &condList, &recievedNumbers, EACH_WORKER_LIMIT * WORKERS_AMOUNT);
    thread t5(listener, &globalMutex, &condList, &recievedNumbers, EACH_WORKER_LIMIT * WORKERS_AMOUNT);
    thread t6(listener, &globalMutex, &condList, &recievedNumbers, EACH_WORKER_LIMIT * WORKERS_AMOUNT);

    thread w1(worker,   &globalMutex, &condList, 1, EACH_WORKER_LIMIT);
    this_thread::sleep_for(chrono::milliseconds(300));
    thread w2(worker,   &globalMutex, &condList, 2, EACH_WORKER_LIMIT);
    this_thread::sleep_for(chrono::milliseconds(300));
    thread w3(worker,   &globalMutex, &condList, 3, EACH_WORKER_LIMIT);


    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    w1.join();
    w2.join();
    w3.join();

    cout << "List size: " << condList.size() << endl;
    cout << "Received numbers amount: " << recievedNumbers << endl;
    return 0;
}
