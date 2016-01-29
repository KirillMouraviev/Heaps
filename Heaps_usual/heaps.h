#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cassert>
#include <memory>

const int inf = 1e9;

using std::cout;
using std::endl;
using std::unique_ptr;

class IHeap
{
private:

public:
	virtual void Meld(IHeap* h) = 0;
	virtual void insert(int x) = 0;
	virtual int extractMin() = 0;
	virtual int getMin() = 0;
	virtual bool isEmpty() = 0;
	void stupidMeld(IHeap* h)
	{
		while (!(h->isEmpty()))
		{
			int x = h->extractMin();
			insert(x);
		}
	}
};

class BinomialHeap;

class BTNode
{
	friend class BinomialHeap;
private:
	int key;
	BTNode* child;//left child
	BTNode* next;//right brother
	BTNode* p;//parent
	int deg;//number of children
public:
	BTNode(int _key)
	{
		key = _key;
		child = nullptr;
		next = nullptr;
		p = nullptr;
		deg = 0;
	}
	
	void join(BTNode* t)//join this tree with another tree
	{
		assert(deg == t->deg);
		t->next = child;
		t->p = this;
		child = t;
		deg++;
	}
	
	~BTNode()
	{
		
	}
};

class BinomialHeap: public IHeap
{
	friend class BTNode;
private:
	std::vector<BTNode*> v;//vector of binomial trees that contain elements of the heap
	int min;
public:
	BinomialHeap()//creating an empty heap
	{
		v.clear();
		min = -1;
	}
	
	BinomialHeap(int key)
	{
		v.clear();
		v.push_back(new BTNode(key));
		min = 0;
	}
	
	int getMin()
	{
		return v[min]->key;
	}
	
	void join(BTNode* &a, BTNode* &b)//join two random binomial TREES of one rank in correct order
	{
		if (a->key < b->key)
			a->join(b);
		else
		{
			b->join(a);
			std::swap(a, b);
		}
	}
	
	int f(BTNode* t)//exists or not tree under pointer t
	{
		if (t == nullptr)
			return 0;
		return 1;
	}
	
	int ff(std::vector<BTNode*> &v, int i)//exists or not tree on v[i]
	{
		if (i >= v.size())
			return 0;
		return f(v[i]);
	}
	
	bool isEmpty()
	{
		return (v.size() == 0);
	}
	
	void countMin()//recount minimun after reconstruction of the heap
	{
		min = 0;
		for (int i = 0; i < v.size(); i++)
			if ((v[i] != nullptr) && ((v[min] == nullptr) || (v[min]->key > v[i]->key)))
				min = i;
	}
	
	void Meld(IHeap* h)//meld our heap with another
	{
		BinomialHeap* bh = dynamic_cast<BinomialHeap*>(h);
		if (bh)
		{
			//addition by column for vectors of trees
			BTNode* vume = nullptr;
			for (int i = 0; i < std::max(v.size(), bh->v.size()); i++)
			{
				if (i >= v.size())
					v.push_back(nullptr);
				//how to add trees on the i-th position of vectors
				int sum = f(vume) + ff(v, i) + ff(bh->v, i);
				//if sum = 1 then one tree goes to i-th position and nothing goes to vume
				if (sum == 1)
				{
					if (ff(v, i) == 1)
						v[i] = v[i];
					if (f(vume) == 1)
						v[i] = vume;
					if (ff(bh->v, i) == 1)
						v[i] = bh->v[i];
					vume = nullptr;
				}
				//if sum = 2 then trees merge and go to vume to the next operation, and nothing goes to vector[i]
				if (sum == 2)
				{
					if (f(vume) == 0)
					{
						join(v[i], bh->v[i]);
						vume = v[i];
					}
					if (ff(v, i) == 0)
						join(vume, bh->v[i]);
					if (ff(bh->v, i) == 0)
						join(vume, v[i]);
					v[i] = nullptr;
				}
				//if sum = 3 then one tree (which was on v[i], it exists because sum = 3)
				//goes to v[i] and both of other trees go to vume to the next operation
				if (sum == 3)
					join(vume, bh->v[i]);
			}
			if (vume != nullptr)
				v.push_back(vume);
			countMin();
		}
		else
		{
			stupidMeld(h);
		}
	}
	
	void insert(int x)
	{
		//meld our heap with a heap with only one x
		BinomialHeap* newHeap = new BinomialHeap(x);
		Meld(newHeap);
	}
	
	int extractMin()
	{
		//returns min key
		int res = v[min]->key;
		BTNode* cur = v[min]->child;
		delete v[min];
		//delete tree node with the minimal key and merge all its children to one heap which is called underMin
		v[min] = nullptr;
		if (min + 1 == v.size())
			v.pop_back();
		BinomialHeap* underMin = new BinomialHeap;
		while (cur != nullptr)
		{
			underMin->v.push_back(cur);
			cur = cur->next;
		}
		//reversing underMin because order of children of binomial tree is reversed
		reverse(underMin->v.begin(), underMin->v.end());
		underMin->countMin();
		//meld underMin with our heap to recovery our heap
		Meld(underMin);
		return res;
	}
	
	~BinomialHeap()
	{
	
	}
};

class SkewHeap;

class SkewNode
{
private:
	friend class SkewHeap;
	int key;
	unique_ptr<SkewNode> l;
	unique_ptr<SkewNode> r;
public:
	SkewNode(int x)
	{
		key = x;
		l = nullptr;
		r = nullptr;
	}
};

typedef unique_ptr<SkewNode> skewPtr;

class SkewHeap:public IHeap 
{
protected:
    skewPtr root;
public:
    void balance(skewPtr &v)
    {
		assert(v != nullptr);
		std::swap(v->l, v->r);
	}
 
    SkewHeap()
    {
		root = nullptr;
    }
    
    SkewHeap(int val)
    {
		root.reset(new SkewNode(val));
	}
    
    bool isEmpty()
    {
		return (root == nullptr);
	}
	
	int getMin()
	{
		return root->key;
	}
	
	void merge(skewPtr &a, skewPtr &b)
	{
		if (a == nullptr)
		{
			a = std::move(b);
			return;
		}
		if (b == nullptr)
			return;
		if (a->key > b->key)
			std::swap(a, b);
		if (a->r == nullptr)
			std::swap(a->l, a->r);
		balance(a);
		if (a->r == nullptr)
			a->r = move(b);
		else
			merge(a->r, b);
	}
	
	void insert(int x)
	{
		skewPtr newNode(new SkewNode(x));
		merge(root, newNode);
	}
 
    int extractMin()
    {
		assert(root != nullptr);
		int res = root->key;
		merge(root->l, root->r);
		root = move(root->l);
		return res;
    }
    
    void Meld(IHeap* h)
    {
		SkewHeap* lh = dynamic_cast<SkewHeap*>(h);
		if (lh)
		{
			merge(root, lh->root);
			delete lh;
		}
		else
			stupidMeld(h);
	}
};

class LeftHeap;

class LeftNode
{
private:
	friend class LeftHeap;
	int key;
	unique_ptr<LeftNode> l;
	unique_ptr<LeftNode> r;
	int rank;
public:
	LeftNode(int val)
	{
		key = val;
		l = nullptr;
		r = nullptr;
		rank = 1;
	}
};

typedef unique_ptr<LeftNode> leftPtr;
 
class LeftHeap: public IHeap
{
private:
    leftPtr root;
public:
	int rk(leftPtr &v)
	{
		if (v == nullptr)
			return 0;
		return v->rank;
	}
	
    void balance(leftPtr &v)
    {
		assert(v != nullptr);
		if (rk(v->l) < rk(v->r))
			std::swap(v->l, v->r);
		v->rank = rk(v->l) + 1;
	}
 
    LeftHeap()
    {
		root = nullptr;
    }
    
    LeftHeap(int val)
    {
		root.reset(new LeftNode(val));
	}
    
    bool isEmpty()
    {
		return (root == nullptr);
	}
	
	int getMin()
	{
		return root->key;
	}
	
	void merge(leftPtr &a, leftPtr &b)
	{
		if (a == nullptr)
		{
			a = std::move(b);
			return;
		}
		if (b == nullptr)
			return;
		if (a->key > b->key)
			std::swap(a, b);
		if (a->r == nullptr)
			std::swap(a->l, a->r);
		balance(a);
		if (a->r == nullptr)
			a->r = move(b);
		else
			merge(a->r, b);
	}
	
	void insert(int x)
	{
		leftPtr newNode(new LeftNode(x));
		merge(root, newNode);
	}
	
	int depth()
	{
		return root->rank;
	}
 
    int extractMin()
    {
		assert(root != nullptr);
		int res = root->key;
		merge(root->l, root->r);
		root = move(root->l);
		return res;
    }
    
    void Meld(IHeap* h)
    {
		LeftHeap* lh = dynamic_cast<LeftHeap*>(h);
		if (lh)
		{
			merge(root, lh->root);
			delete lh;
		}
		else
			stupidMeld(h);
	}
};
