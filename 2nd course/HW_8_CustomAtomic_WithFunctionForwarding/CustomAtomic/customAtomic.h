#include <thread>
#include <mutex>

template <typename _T> class customAtomic
{
	_T _data;
	mutable std::recursive_mutex _mutex;

public:
    // Load, store, exchange
	_T load () const
	{
		_mutex.lock();
		_T result = _data;
		_mutex.unlock();
		return result;
	}
	void store (const _T __data)
	{
		_mutex.lock();
		_data = __data;
		_mutex.unlock();
	}
	_T exchange (const _T __data)
	{
		_mutex.lock();
		_T result = _data;
		_data = __data;
		_mutex.unlock();
		return result;
	}

	// Changing type
	operator _T ()
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
		_data++;
		_mutex.unlock();
		return *this;
	}
	customAtomic <_T> operator++ (int)
	{
	    _mutex.lock();
	    _T oldData = _data;
	    _data++;
		_mutex.unlock();
		return customAtomic <_T> (oldData);
	}

	// Constructor. No need to mute
	customAtomic () = delete;
	customAtomic (const _T __d) : _data(__d)
	{}
	customAtomic (const customAtomic <_T> &__other)
	{
		_data = __other.load();
	}
	customAtomic (customAtomic <_T> &&__other)
	{
		_data = __other.load();
	}

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
