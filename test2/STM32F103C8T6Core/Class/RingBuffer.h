#pragma once

#include <stdint.h>

template<typename T, uint32_t MAX_SIZE>
class RingBuffer
{
private:
	T _buffer[MAX_SIZE];
	uint32_t _head = 0;
	uint32_t _tail = 0;
public:
	RingBuffer()
	{
	}
public:
	bool isEmpty() const { return _head == _tail; }
	bool isFull() const { return _tail == (_head - 1) % MAX_SIZE; }

	int size() const
	{
		if (_tail >= _head)
			return _tail - _head;
		return MAX_SIZE - (_head - _tail);
	}

	void push(T item)
	{
		if (isFull())
		{
			return;
		}
		_buffer[_tail] = item;
		_tail = (_tail + 1) % MAX_SIZE;
	}

	void append(const T* data, int size)
	{
		for(int i = 0; i < size; ++i)
			push(data[i]);
	}

	T pop()
	{
		T ret = _buffer[_head];
		_head = (_head + 1) % MAX_SIZE;
		return ret;
	}

};
