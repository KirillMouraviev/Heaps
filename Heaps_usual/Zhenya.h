#include <bits/stdc++.h>

using namespace std;

class IHeap
{
};

const int INF = 2e9;

class LeftistSkewBaseHeap:public IHeap {
protected:
    int val_;
    LeftistSkewBaseHeap *left_, *right_;
public:
    virtual void recalc() = 0;
    virtual int getRank() = 0;
 
    LeftistSkewBaseHeap():val_(INF), left_(NULL), right_(NULL) {
       
    }
 
    LeftistSkewBaseHeap(int val, LeftistSkewBaseHeap *left, LeftistSkewBaseHeap *right): val_(val), left_(left), right_(right) {
 
    }
 
    void copy(const LeftistSkewBaseHeap *a) {
        this->val_ = a->val_;
        this->left_ = a->left_;
        this->right_ = a->right_;
    }
 
    int extractMin() {
        int res = val_;
        if (res == INF) {
            return INF;
        }
        if (this->left_ != NULL)
            copy(this->left_->mergeHeap(this->right_));
        else
            copy(this->right_->mergeHeap(this->left_));
        return res;
    }
 
    void swap(LeftistSkewBaseHeap *&a, LeftistSkewBaseHeap *&b) {
        LeftistSkewBaseHeap *buf = a;
        a = b;
        b = buf;
    }
 
    LeftistSkewBaseHeap* mergeHeap(LeftistSkewBaseHeap *b) {
        if (this == NULL) {
            return b;
        } else if (b == NULL) {
            return this;
        } else if (this->val_ == INF && b->val_ == INF) {
            return b;
        } else if (this->val_ <= b->val_) {
            this->right_ = this->right_->mergeHeap(b);
           recalc();
 
            return this;
        } else {
            b->left_ = this->mergeHeap(b->left_);
            b->recalc();
 
            return b;
        }
    }
 
    void erase() {
        if (this == NULL) {
            return;
        }
 
        left_->erase();
        right_->erase();
        delete (this);
    }
};
 
class SkewHeap: public LeftistSkewBaseHeap {
public:
    SkewHeap():LeftistSkewBaseHeap()
    {}
    SkewHeap(int val, LeftistSkewBaseHeap *left, LeftistSkewBaseHeap *right):LeftistSkewBaseHeap(val, left, right)
    {}
 
    int getRank() {
        return 0;
    }
 
    void recalc() {
        swap(left_, right_);
    }
 
    void insert(int key) {
        SkewHeap *now = new SkewHeap(key, NULL, NULL);
        meld(now);
    }
 
    void meld(SkewHeap *&a) {
        SkewHeap *now = new SkewHeap();
        now->copy(this);
        copy(now->mergeHeap(a));
    }
};
 
class LeftistHeap:public LeftistSkewBaseHeap {
private:
    int rank_;
public:
    LeftistHeap():rank_(0), LeftistSkewBaseHeap()
    {}
 
    LeftistHeap(int rank, int val, LeftistSkewBaseHeap *left, LeftistSkewBaseHeap *right):rank_(rank), LeftistSkewBaseHeap(val, left, right)
    {}
 
    int getRank() {
        if (this == NULL) {
            return 0;
        }
 
        return rank_;
    }
 
    void recalc() {
        if (left_->getRank() < right_->getRank()) {
            swap(left_, right_);
        }
 
        rank_ = right_->getRank() + 1;
    }
 
    void insert(int key) {
        LeftistHeap *now = new LeftistHeap(1, key, NULL, NULL);
        meld(now);
    }
 
    void meld(LeftistHeap *&a) {
        LeftistHeap *now = new LeftistHeap();
        now->copy(this);
        copy(now->mergeHeap(a));
    }
};

