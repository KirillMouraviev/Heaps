#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cassert>

const int inf = 1e9;

using std::cout;
using std::endl;

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

class LightHeap:public IHeap 
{
protected:
    int key;
    LightHeap *l;
    LightHeap *r;
public:
    virtual void balance() = 0;
    virtual int getRank() = 0;
 
    LightHeap()
    {
		key = -inf;
		l = nullptr;
		r = nullptr;
    }
    
    LightHeap(int val)
    {
		key = val;
		l = nullptr;
		r = nullptr;
	}
 
    LightHeap(int val, LightHeap *left, LightHeap *right)
    {
		key = val;
		l = left;
		r = right;
    }
 
    virtual void copy(const LightHeap *a)
    {
		//unique_ptr<LightHeap> p;
		if (a == nullptr)
		{
			key = -inf;
			l = nullptr;
			r = nullptr;
			return;
		}
		assert(a->l != this);
		assert(a->r != this);
        key = a->key;
        LightHeap* al = a->l;
        LightHeap* ar = a->r;
        l = al;
        r = ar;
        assert(l != this);
        assert(r != this);
        delete a;
    }
    
    bool isEmpty()
    {
		return (key == -inf);
	}
	
	int getMin()
	{
		return key;
	}
 
    int extractMin()
    {
        int res = key;
        if (res == -inf)
            return -inf;
        if ((l == nullptr) && (r == nullptr))
		{
			copy(l);
			return res;
		}
        if (l != nullptr)
            copy(l->Merge(r));
        else if (r != nullptr)
            copy(r->Merge(l));
        return res;
    }
 
    LightHeap* Merge(LightHeap *h)
    {
        if ((this == nullptr) || isEmpty())
			return h;
		if ((h == nullptr) || (h->isEmpty()))
			return this;
        if (key <= h->key)
        {
			if (r == nullptr)
				std::swap(l, r);
			if (r == nullptr)
				r = h;
			else
				r = r->Merge(h);
			balance();
			return this;
		}
        else 
        {
            h->l = Merge(h->l);
            h->balance();
            return h;
        }
    }
    
    void Meld(IHeap* h)
    {
		LightHeap* lh = dynamic_cast<LightHeap*>(h);
		if (lh)
		{
			LightHeap* newHeap = Merge(lh);
			copy(newHeap);
		}
		else
			stupidMeld(h);
	}
};
 
class SkewHeap: public LightHeap 
{
public:
    SkewHeap():LightHeap()
    {
	}
	
	SkewHeap(int val):LightHeap(val)
	{
	}
    
    SkewHeap(int val, LightHeap *left, LightHeap *right):LightHeap(val, left, right)
    {
	}
 
    int getRank() 
    {
        return 0;
    }
 
    void balance() 
    {
        std::swap(l, r);
    }
 
    void Meld(SkewHeap* &a) 
    {
        SkewHeap *newHeap = new SkewHeap;
        newHeap->copy(this);
        copy(newHeap->Merge(a));
    }
 
    void insert(int key) 
    {
        SkewHeap *newHeap = new SkewHeap(key);
        Meld(newHeap);
    }
};
 
class LeftHeap:public LightHeap 
{
private:
    int rank;
public:
    LeftHeap():LightHeap()
    {
		rank = 0;
	}
	
	LeftHeap(int val)
	{
		rank = 1;
		key = val;
		l = nullptr;
		r = nullptr;
	}
 
    LeftHeap(int rk, int val, LightHeap *left, LightHeap *right):LightHeap(val, left, right)
    {
		rank = rk;
	}
	
	void copy(LeftHeap* h)
	{
		if (h == nullptr)
			rank = 0;
		else
			rank = h->rank;
	}
	
	int getRank()
	{
		return rank;
	}
 
    int rk(LightHeap* h)
    {
		if (h == nullptr)
			return 0;
		return h->getRank();
	}
 
    void balance() 
    {
        if (rk(r) < rk(l))
            std::swap(l, r);
        rank = rk(l) + 1;
    }
    
    void Meld(LeftHeap* &a)
    {
        LeftHeap *newHeap = new LeftHeap;
        newHeap->copy(this);
        copy(newHeap->Merge(a));
    }
 
    void insert(int key) 
    {
        LeftHeap *newHeap = new LeftHeap(key);
        Meld(newHeap);
    }
};
