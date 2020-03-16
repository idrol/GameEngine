// Patrick Dahlin 09.02.2020
// sleepy af dont look
#pragma once


#define QUEUE_DEFAULT_SIZE 25

/*
Basic circular queue implementation
Enqueueing is O(n) unless expansion is disabled, then O(1)
Dequeueing is O(1)

!!UNLESS the queue needs to resize,
then a memcpy is ran on all data
which means O(n)? worst case for 
enqueueing, dunno...!!
The worst case scenario can be avoided
by disabling allow_expansion, this
however means enqueuing can fail
*/
template<typename T>
class Queue {
	public:

	Queue(bit allow_expansion, u32 size) {
		if(size == 0) size = 1;
		m_data = new T[size];
		memset(&m_data[0], 0, sizeof(T)*size);
		m_size = size;
		m_count = 0;
		m_front = 0;
		m_back = 0;
		m_allow_resize = allow_expansion;
	}

	bool Enqueue(T obj) {
		if (m_count == m_size)
			if (!m_allow_resize)
				return false;
			else
				_resize(m_size * 2); // Assume allocations succeed :DD

		// we somehow didn't manage to allocate a bigger buffer, hmmmmmmmmmmmmm
		if(m_front == m_back && m_count != 0)
			return false;

		// By this point we can assume that the enqueued element will fit

		// Enqueue to the front and dequeue from the back
		// My brain works this way okay
		auto next_pos = m_front + 1;
		// Incase we pass the front of our circular queue, go to the beginning
		if(next_pos >= m_size)
			next_pos = 0;

		m_data[m_front] = obj;
		m_front = next_pos;
		m_count++;
		return true;
	}

	bool Dequeue(T& output) {
		if(m_back == m_front || m_count == 0) return false;
		output = m_data[m_back];
		m_back++;
		// incase the tail passes the front of our circular buffer
		if(m_back >= m_size) m_back = 0;

		m_count--;
		return true;
	}

	u32 Count() const { return m_count; }

	void Clear() { m_front = m_back = m_count = 0; }

private:

	void _resize(u32 newsize) {

		/*
		
		We can have either of these 3 cases when it comes
		to the buffer pointers
		[b] is for back pointer
		[f] is for front pointer

		|[b]---------------------------------[f]|
		

		|-----------------------------[f][b]----|

		_______

		Start by copying from back pointer as far forward as possible.
		The upper limit is either front pointer or end of buffer, whichever is smaller
		but greater than back pointer.
		- This fixes case nr1 completely.
		- In case nr2 it copies parts of the buffer.

		*/
		assert_le(m_size,newsize,"Called resize with same or smaller size, what");
		auto tmp = new T[newsize];
		memset(&tmp[0],0,newsize*sizeof(T));
		auto count = m_size;

		// case nr 2
		if (m_front < m_back)
		{
			// calculate length of tail
			auto tail_len = m_size-m_back;
			memcpy_s(&tmp[0], newsize * sizeof(T), &m_data[m_back], tail_len * sizeof(T));
			// Copy over head from the beginning of source
			memcpy_s(&tmp[tail_len], (newsize-tail_len) * sizeof(T), &m_data[0], m_front * sizeof(T));
		}
		// case nr 1
		else
		{
			// hard copy since head is ahed of tail
			memcpy_s(&tmp[0], newsize * sizeof(T), &m_data[0], m_size * sizeof(T));
		}
		m_front = m_count;
		m_back = 0;
		delete[] m_data;
		m_data = tmp;
		m_size = newsize;
	}

	T *m_data;
	u32 m_size;
	u32 m_count;
	u32 m_front, m_back;
	bit m_allow_resize;
};

