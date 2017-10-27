#include <thread>
#include <mutex>

template <class _T> class customAtomic
{
	_T _data;
	std::mutex _mutex;

public:
    // Load, store, exchange
	_T load ()
	{
		_mutex.lock();
		_T result = _data;
		_mutex.unlock();
		return result;
	}
	void store(_T data)
	{
		_mutex.lock();
		_data = data;
		_mutex.unlock();
	}
	_T exchange(_T data)
	{
		_mutex.lock();
		_T result = _data;
		_data = data;
		_mutex.unlock();
		return result;
	}

	// Changing type
	operator _T()
	{
		_mutex.lock();
		_T result = _data;
		_mutex.unlock();
		return result;
	}

	// Increment
	customAtomic<_T> &operator++()
	{
	    _mutex.lock();
		_data++;
		_mutex.unlock();
		return *this;
	}
	customAtomic<_T> &operator++(int)
	{
	    _mutex.lock();
	    customAtomic<_T> result(++_data);
		_mutex.unlock();
		return *this;
	}

	// Constructor. No need to mute
	customAtomic ()
	{
		_data = _T();
	}
	customAtomic (const _T &d)
	{
		_data = d;
	}
	customAtomic (const customAtomic <_T> &other)
	{
		_data = other._data;
	}
	customAtomic (customAtomic <_T> &&other)
	{
		_data = other._data;
	}

	// Destructor
	~customAtomic ()
	{}

	// operator=
	customAtomic &operator= (const customAtomic <_T> &other)
	{
		_mutex.lock();
		_data = other._data;
		_mutex.unlock();
		return *this;
	}
	customAtomic &operator= (customAtomic <_T> &&other)
	{
		_mutex.lock();
		_data = other._data;
		_mutex.unlock();
		return *this;
	}
};
