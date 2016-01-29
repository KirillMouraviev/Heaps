#include <gtest/gtest.h>
#include <queue>
#include <cstdlib>
#include <ctime>
#include "heaps.h"

using namespace std;

int randInt()
{
	return rand() % 2001 - 1000;
}

void addStdHeap(vector<priority_queue<int> > &v, int x)//adding a standard heap to the vector
{
	priority_queue<int> q;
	q.push(-x);
	v.push_back(q);
}

int extractMinFromStdHeap(vector<priority_queue<int> > &v, int i)//extract min from a standard heap in the vector
{
	assert(!v[i].empty());
	int res = -v[i].top();
	v[i].pop();
	return res;
}

void MeldStdHeaps(vector<priority_queue<int> > &v, int i, int j)//merge two standard heaps from the vector
{
	while (!v[j].empty())
	{
		v[i].push(v[j].top());
		v[j].pop();
	}
	swap(v[j], v[v.size() - 1]);
	v.pop_back();
}

template <class T>
void TestTime(std::string heapName)//testing time of working of any heap
{
	cout << CLOCKS_PER_SEC << " per second" << endl;
	srand(time(0));
	int kol = 100000;
	vector<T*> v;
	int startTime = clock();
	for (int i = 0; i < kol; i++)
	{
		int tip = rand() % 4;
		if ((tip == 0) && (rand() % 3 == 0))
		{
			int x = randInt();
			v.push_back(new T(x));
		}
		if (tip == 1)
		{
			if (v.size() > 0)
			{
				int i = rand() % v.size();
				int x = randInt();
				v[i]->insert(x);
			}
		}
		if (tip == 2)
		{
			if ((rand() % 5 == 0) && (v.size() > 0))
			{
				int i = rand() % v.size();
				if (!v[i]->isEmpty())
					v[i]->extractMin();
			}
		}
		if (tip == 3)
		{
			if (v.size() > 1)
			{
				int i = rand() % v.size();
				int j = rand() % v.size();
				while (j == i)
					j = rand() % v.size();
				v[i]->Meld(v[j]);
				swap(v[j], v[v.size() - 1]);
				v.pop_back();
			}
		}
	}
	int finishTime = clock();
	int time = finishTime - startTime;
	//tell how long works this heap
	cout << heapName << " heap made " << kol << " operations in " << time << " * 10^-6 s" << endl;
}

TEST(SkewTimeTest, MultipleRandomTesting)//testing time of working of skew heap
{
	TestTime<SkewHeap>("Skew");
}

TEST(LeftTimeTest, MultipleRandomTesting)//testing time of working of left heap
{
	TestTime<LeftHeap>("Left");
}

TEST(BinomialTimeTest, MultipleRandomTesting)//testing time of working of binomial heap
{
	TestTime<BinomialHeap>("Binomial");
}

template <class T>
void TestCorrectness()//test correctness of working of the heap of class T
{
	srand(time(0));
	int kol = 100000;
	vector<T*> v;
	vector<priority_queue<int> > vv;
	int x = randInt();
	v.push_back(new T(x));
	addStdHeap(vv, x);
	for (int i = 0; i < kol; i++)
	{
		int tip = rand() % 4;
		if (tip == 0)
		{
			int x = randInt();
			v.push_back(new T(x));
			addStdHeap(vv, x);
		}
		if (tip == 1)
		{
			if (v.size() > 0)
			{
				int i = rand() % v.size();
				int x = randInt();
				v[i]->insert(x);
				vv[i].push(-x);
			}
		}
		if (tip == 2)
		{
			if (v.size() > 0)
			{
				int i = rand() % v.size();
				if (!v[i]->isEmpty())
				{
					int myMin = v[i]->extractMin();
					int trueMin = extractMinFromStdHeap(vv, i);
					ASSERT_EQ(myMin, trueMin);
				}
			}
		}
		if (tip == 3)
		{
			if (v.size() > 1)
			{
				int i = rand() % v.size();
				int j = rand() % v.size();
				while (j == i)
					j = rand() % v.size();
				v[i]->Meld(v[j]);
				swap(v[j], v[v.size() - 1]);
				v.pop_back();
				MeldStdHeaps(vv, i, j);
			}
		}
	}
}

TEST(SkewCorrectnessTest, MultipleRandomTesting)//test correctness of skew heap
{
	TestCorrectness<SkewHeap>();
}

TEST(LeftHeapTest, MultipleRandomTesting)//test correctness of left heap
{
	TestCorrectness<LeftHeap>();
}

TEST(BinomialHeapTest, MultipleRandomTesting)//test correctness of binomial heap
{
	TestCorrectness<BinomialHeap>();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  srand(time(0));
  return RUN_ALL_TESTS();
}
