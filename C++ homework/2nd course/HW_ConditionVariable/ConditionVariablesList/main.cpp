#include <iostream> // console
#include <thread> // threads
#include <mutex> // mutex
#include <cstdlib> // rand
#include <chrono> // sleep_for
#include <ctime> // time

#include "conditionList.h"

using namespace std;
using condition_list::conditionList;

void listener (mutex *globalMutex, conditionList <int> *condList, size_t *recievedNumbers, size_t maxNumbersAmount, size_t workersAmount, size_t *finishedWorkers_)
{
    {
        lock_guard <mutex> lockGuard(*globalMutex);
        cout << "L! New listener created. Id: " << this_thread::get_id() << endl;
    }
    while (*recievedNumbers < maxNumbersAmount) {
        size_t finishedWorkers = *finishedWorkers_; // Checking amount of finished workers
        auto uniqueLock = condList->getLock(); // Getting locked mutex
        condList->condition_variable.wait_for(uniqueLock, chrono::milliseconds(finishedWorkers == workersAmount ? 0 : 10000)); // Mutex unlocked. Not waiting if all workers have finished
        // Woke up. Mutex locked
        if (uniqueLock.owns_lock()) uniqueLock.unlock(); // Unlocking mutex
        {
            lock_guard <mutex> lockGuard(*globalMutex);
            cout << "L! Listener " << this_thread::get_id() << " woke up" << endl;
        }
        if (condList->size() == 0) continue; // Proceeding
        try {
            int data = condList->consume_back(true); // Getting data and deleting it
            {
                lock_guard <mutex> lockGuard(*globalMutex);
                cout << "L- Listener " << this_thread::get_id() << " consumed data: " << data << endl;
                (*recievedNumbers)++;
            }
            srand(time(0));
            this_thread::sleep_for(chrono::milliseconds(rand() % 15000 + 5000)); // Working for random time
            {
                lock_guard <mutex> lockGuard(*globalMutex);
                cout << "L= Listener " << this_thread::get_id() << " finished work"<< endl;
            }
        } catch (out_of_range) {
            // Have a low chance of this error
            // It is possible because we have several atomic operations:
            // 1. Checking size
            // 2. Getting data
            // Possible error scenario:
            // 1. Thread1: Checking size (size = 1)
            // 2. Thread2: Checking size (size = 1)
            // 3. Thread1: Consuming data (size = 0)
            // 4. Thread2: Trying to get data (size = 0) => Error!
            lock_guard <mutex> lockGuard(*globalMutex);
            cout << "L? Listener " << this_thread::get_id() << " got no data :(" << endl;
        }
    }
    {
        lock_guard <mutex> lockGuard(*globalMutex);
        cout << "Lx Listener " << this_thread::get_id() << " ended work" << endl;
    }
}

void worker (mutex *globalMutex, conditionList <int> *condList, int workerNum, size_t toProduce, size_t *finishedWorkers)
{
    {
        lock_guard <mutex> lockGuard(*globalMutex);
        cout << " W! New worker created. Id: " << this_thread::get_id() << endl;
    }
    // Working
    for (size_t i = 0; i < toProduce; i++) {
        {
            lock_guard <mutex> lockGuard(*globalMutex);
            cout << " W+ Worker " << this_thread::get_id() << " started working on number" << endl;
        }
        srand(time(0));
        this_thread::sleep_for(chrono::milliseconds(rand() % 9500 + 500)); // Working for random time
        int number = workerNum * 1000 + i + 1; // Number
        condList->push_back(number);
        condList->notify_all();
        {
            lock_guard <mutex> lockGuard(*globalMutex);
            cout << " W+ Worker " << this_thread::get_id() << " worked out number: " << number << endl;
        }
    }
    {
        lock_guard <mutex> lockGuard(*globalMutex);
        cout << " Wx Worker " << this_thread::get_id() << " ended work" << endl;
        (*finishedWorkers)++;
    }
}

using namespace std;

int main()
{
    mutex globalMutex;
    conditionList <int> condList;
    size_t recievedNumbers = 0; // Shared
    size_t finishedWorkers = 0; // Shared

    size_t WORKERS_AMOUNT = 3;
    size_t EACH_WORKER_LIMIT = 10;

    cout << "All threads will be created in 2 seconds" << endl;

    thread t1(listener, &globalMutex, &condList, &recievedNumbers, EACH_WORKER_LIMIT * WORKERS_AMOUNT, WORKERS_AMOUNT, &finishedWorkers);
    this_thread::sleep_for(chrono::milliseconds(300));
    thread t2(listener, &globalMutex, &condList, &recievedNumbers, EACH_WORKER_LIMIT * WORKERS_AMOUNT, WORKERS_AMOUNT, &finishedWorkers);
    this_thread::sleep_for(chrono::milliseconds(300));
    thread t3(listener, &globalMutex, &condList, &recievedNumbers, EACH_WORKER_LIMIT * WORKERS_AMOUNT, WORKERS_AMOUNT, &finishedWorkers);
    this_thread::sleep_for(chrono::milliseconds(300));
    thread t4(listener, &globalMutex, &condList, &recievedNumbers, EACH_WORKER_LIMIT * WORKERS_AMOUNT, WORKERS_AMOUNT, &finishedWorkers);
    this_thread::sleep_for(chrono::milliseconds(300));
    thread t5(listener, &globalMutex, &condList, &recievedNumbers, EACH_WORKER_LIMIT * WORKERS_AMOUNT, WORKERS_AMOUNT, &finishedWorkers);
    this_thread::sleep_for(chrono::milliseconds(300));
    thread t6(listener, &globalMutex, &condList, &recievedNumbers, EACH_WORKER_LIMIT * WORKERS_AMOUNT, WORKERS_AMOUNT, &finishedWorkers);

    thread w1(worker, &globalMutex, &condList, 1, EACH_WORKER_LIMIT, &finishedWorkers);
    this_thread::sleep_for(chrono::milliseconds(300));
    thread w2(worker, &globalMutex, &condList, 2, EACH_WORKER_LIMIT, &finishedWorkers);
    this_thread::sleep_for(chrono::milliseconds(300));
    thread w3(worker, &globalMutex, &condList, 3, EACH_WORKER_LIMIT, &finishedWorkers);

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
