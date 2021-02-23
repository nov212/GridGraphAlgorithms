#pragma once
#include <queue>

template <typename T, typename priority> class PriorityQueue
{
private: 
	typedef std::pair<priority, T> element;
	std::priority_queue<element, std::vector<element>> pqueue;
public:
	void push(const T&, priority)
	{
		pqueue.emplace(pr, item);
	}
	bool empty()
	{
		return pqueue.empty();
	}

	T top()
	{	
		T item = pqueue.top().second;
		return item;
	}
};

