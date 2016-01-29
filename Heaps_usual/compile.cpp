#include "heaps.h"
#include <queue>

int main()
{
	LeftHeap h;
	std::priority_queue<int> q;
	for (int i = 0; i < 30000; i++)
	{
		int tip = rand() % 2;
		if (tip == 1)
		{
			int x = rand() % 1000;
			h.insert(x);
			q.push(x);
		}
		if (tip == 2)
		{
			assert(h.isEmpty() == q.empty());
			if (!q.empty())
			{
				assert(h.extractMin() == q.top());
				q.pop();
			}
		}
	}
	return 0;
}
