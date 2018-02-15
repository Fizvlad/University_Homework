#include <thread>
#include <mutex>

template <class _T> class customAtomic
{
	_T _data;
	mutable std::recursive_mutex _mutex;

public:
	_T load () const // Return value
	{
		_mutex.lock();
		_T result = _data;
		_mutex.unlock();
		return result;
	}

	void store (const _T __data) // Save value
	{
		_mutex.lock();
		_data = __data;
		_mutex.unlock();
	}

	_T exchange (const _T __data) // Exchange value
	{
		_mutex.lock();
		_T result = _data;
		_data = __data;
		_mutex.unlock();
		return result;
	}

    template <typename _return_t, typename... _Args_t> _return_t call (_return_t (_T::*__function) (_Args_t...), _Args_t... __Args) // Call for method
    {
        _mutex.lock();
        _return_t result = (_data.*__function)(__Args...);
        _mutex.unlock();
        return result;
    }

    template <typename _func_t, typename... _Args_t> auto apply (_func_t __function, _Args_t... __Args) -> decltype(__function(_data, __Args...)) // Call for function(_data, _Args)
    {
        _mutex.lock();
        auto result = __function(_data, __Args...);
        _mutex.unlock();
        return result;
    }

	operator _T () // Changing type
	{
		_mutex.lock();
		_T result = _data;
		_mutex.unlock();
		return result;
	}

	// Increment
	customAtomic <_T> & operator++ ()
	{
	    _mutex.lock();
		++_data;
		_mutex.unlock();
		return *this;
	}
	customAtomic <_T> operator++ (int)
	{
	    _mutex.lock();
	    _T oldData = _data;
	    ++_data;
		_mutex.unlock();
		return customAtomic <_T> (oldData);
	}

	// Constructor. No need to mute
	customAtomic () = delete;
	customAtomic (const _T __d) : _data(__d)
	{}
	customAtomic (const customAtomic <_T> &__other) : _data(__other.load())
	{}
	customAtomic (customAtomic <_T> &&__other) : _data(__other.load())
	{}

	// Destructor
	~customAtomic () = default;

	// operator=
	customAtomic & operator= (const customAtomic <_T> &__other)
	{
		_mutex.lock();
		_data = __other.load();
		_mutex.unlock();
		return *this;
	}
	customAtomic & operator= (customAtomic <_T> &&__other)
	{
		_mutex.lock();
		_data = __other.load();
		_mutex.unlock();
		return *this;
	}
};
