#include <thread>
#include <mutex>

template <class _T> class customAtomic
{
	_T _data;
	std::mutex _mutex;

public:
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

	operator _T()
	{
		_mutex.lock();
		_T result = _data;
		_mutex.unlock();
		return result;
	}

	customAtomic<_T> &operator++()
	{
		_data++;
		return *this;
	}

	customAtomic ()
	{
		_mutex.lock();
		_data = _T();
		_mutex.unlock();
	}
	customAtomic (const _T &d)
	{
		_mutex.lock();
		_data = d;
		_mutex.unlock();
	}
	~customAtomic ()
	{}
	customAtomic (const customAtomic <_T> &other)
	{
		_mutex.lock();
		_data = other._data;
		_mutex.unlock();
	}
	customAtomic (customAtomic <_T> &&other)
	{
		_mutex.lock();
		_data = other._data;
		_mutex.unlock();
	}
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