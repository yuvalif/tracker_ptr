#include "tracker_ptr.h"
#include <assert.h>
#include <iostream>

class A
{
private:
    int m_a;
public:
    A() : m_a(0) {}

    void set(int a)
    {
        m_a = a;
    }

    int get() const
    {
        return m_a;
    }
};

// class B is a trackable version of class A
class B : public A, public trackable_ptr_concept<B>
{
};

// class C just implements the interface of the trackable_ptr_concept
class C : public A
{
private:
    bool m_is_tracked;
public:
    C() : A(), m_is_tracked(false) {}

    void register_tracker(tracker_ptr<C>* tracker)
    {
        m_is_tracked = true;
    }

    void unregister_tracker(const tracker_ptr<C>* tracker)
    {
        m_is_tracked = false;
    }

    bool is_tracked() const {return m_is_tracked;}
};

void TestBasic()
{
    B* b = new B;
    tracker_ptr<B> p(b);
    assert(p);
    p->set(5);
    assert(p->get() == b->get());
    assert((*p).get() == 5);
    delete b;
    // now b is already deleted
    assert(!p);
    // calling p->set(5); at this point should crash
}

void TestStaticAlloc()
{
    tracker_ptr<B> p(nullptr);
    {
        B b;
        p.reset(&b);
        // a is still valid here
        assert(p);
        (*p).set(5);
        assert(p->get() == b.get());
        assert((*p).get() == b.get());
    }
    // now b is already deleted
    assert(!p);
}

void TestScope()
{
    B* b = new B;
    {
        tracker_ptr<B> p(b);
        assert(p);
        p->set(5);
        assert(p->get() == b->get());
        // tracking stop here
    }
    delete b;
}

void TestChange()
{
    B* b1 = new B;
    B* b2 = new B;
    tracker_ptr<B> p(b1);
    assert(p);
    p->set(4);
    assert(p->get() == b1->get());
    p.reset(b2);
    assert(p);
    p->set(5);
    assert(p->get() != b1->get());
    assert(p->get() == b2->get());
    delete b1;
    // b2 is still valid
    assert(p);
    assert(p->get() == b2->get());
    delete b2;
    // now b2 also deleted
    assert(!p);
}

void TestWithoutConcept()
{
    C* c = new C;
    assert(!c->is_tracked());
    tracker_ptr<C> p(c);
    p->set(5);
    assert(p->get() == c->get());
    assert(c->is_tracked());
    // we must rest before deletion
    p.reset(nullptr);
    delete c;
}

int main()
{
    TestBasic();
    TestStaticAlloc();
    TestScope();
    TestChange();
    TestWithoutConcept();
    std::cout << "all tests passed" << std::endl;
    return 0;
}

