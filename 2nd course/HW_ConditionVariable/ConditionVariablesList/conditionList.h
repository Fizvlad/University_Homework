#ifndef CONDITIONLIST_H_INCLUDED
#define CONDITIONLIST_H_INCLUDED

#include <list>
#include <mutex>
#include <condition_variable>

namespace condition_list
{
    // conditionList is std::list wrapper which provides functional for threads communication
    template <typename dataType>
    class conditionList
    {
        // Types
        typedef std::mutex                        outerMutexType;
        typedef std::recursive_mutex              innerMutexType;
        typedef std::condition_variable           cvType;
        typedef std::list <dataType>              listType;
        typedef std::unique_lock <outerMutexType> outerUniqueLockType;
        typedef std::unique_lock <innerMutexType> innerUniqueLockType;
        typedef std::lock_guard <outerMutexType>  outerMutexLockGuardType;

        // Members
                listType       list_; // List with data
        mutable innerMutexType mutex_; // Mutex for inner use
        mutable outerMutexType outerMutex_; // Mutex for threads synchronizing
    public:
        // Public members
        mutable cvType         condition_variable; // Condition variable for threads synchronizing

        // Default
        conditionList  () : list_(), mutex_(), outerMutex_(), condition_variable() {};
        ~conditionList () = default;

        // Deleted due to mutex
        conditionList            (const conditionList&) = delete;
        conditionList &operator= (const conditionList&) = delete;
        conditionList            (conditionList&&)      = delete;
        conditionList &operator= (conditionList&&)      = delete;

        // Threads synchronizing
        outerUniqueLockType getLock() const
        {
            return std::move(outerUniqueLockType(outerMutex_));
        }

        // List info getters
        bool empty () const
        {
            innerUniqueLockType uniqueLock(mutex_);
            return list_.empty();
        }

        size_t size () const
        {
            innerUniqueLockType uniqueLock(mutex_);
            return list_.size();
        }

        // Data getters
        dataType front () const
        {
            innerUniqueLockType uniqueLock(mutex_);
            if (list_.size() == 0) throw std::out_of_range("conditionList.front(): Out of range.");
            return list_.front();
        }
        dataType back () const
        {
            innerUniqueLockType uniqueLock(mutex_);
            if (list_.size() == 0) throw std::out_of_range("conditionList.back(): Out of range.");
            return list_.back();
        }

        dataType at (size_t index) const
        {
            innerUniqueLockType uniqueLock(mutex_);
            if (index >= list_.size()) throw std::out_of_range("conditionList.at(): Index is out of range.");
            auto it = list_.cbegin();
            std::advance(it, index);
            return *it;
        }

        // Special data getters
        dataType consume_front (bool ifDelete = false)
        {
            innerUniqueLockType uniqueLock(mutex_);
            dataType result = front();
            if (ifDelete) {
                pop_front();
            }
            return result;
        }
        dataType consume_back (bool ifDelete = false)
        {
            innerUniqueLockType uniqueLock(mutex_);
            dataType result = back();
            if (ifDelete) {
                pop_back();
            }
            return result;
        }

        dataType consume (size_t index, bool ifDelete = false)
        {
            innerUniqueLockType uniqueLock(mutex_);
            dataType result = at(index);
            if (ifDelete) {
                pop(index);
            }
            return result;
        }

        // Data changers
        void push_front (const dataType &data)
        {
            innerUniqueLockType uniqueLock(mutex_);
            list_.push_front(data);
        }
        void push_back (const dataType &data)
        {
            innerUniqueLockType uniqueLock(mutex_);
            list_.push_back(data);
        }

        void pop_front ()
        {
            innerUniqueLockType uniqueLock(mutex_);
            return list_.pop_front();
        }
        void pop_back ()
        {
            innerUniqueLockType uniqueLock(mutex_);
            return list_.pop_back();
        }
        void pop(size_t index)
        {
            innerUniqueLockType uniqueLock(mutex_);
            if (index >= list_.size()) throw std::out_of_range("conditionList.pop(): Index is out of range.");
            auto it = *(list_.begin());
            std::advance(it, index);
            dataType result = *it;
            list_.erase(it);
            return result;
        }

        // Condition variable
        void notify_one () const
        {
            condition_variable.notify_one();
        }

        void notify_all () const
        {
            condition_variable.notify_all();
        }
    };
}

#endif // CONDITIONLIST_H_INCLUDED
