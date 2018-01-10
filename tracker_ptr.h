#pragma once

#include <memory>

// this class is used to track a pointer that it managed elsewhere
// the tracker just wraps the pointer
// and make sure that it could be set to null if deleted by its real owner
template<class T>
class tracker_ptr
{
private:
    // the pointer, owned elsewhere
    T* m_ptr;

public:
    // ctor
    explicit tracker_ptr(T* ptr) : m_ptr(ptr)
    {
        if (m_ptr)
        {
            m_ptr->register_tracker(this);
        }
    }

    // std::unique_ptr ctor
    explicit tracker_ptr(const std::unique_ptr<T>& ptr) : m_ptr(ptr.get())
    {
        if (m_ptr)
        {
            m_ptr->register_tracker(this);
        }
    }
    // non copyable
    tracker_ptr(const tracker_ptr<T>& other) = delete;
    tracker_ptr<T>& operator=(const tracker_ptr<T>& other) = delete;

    // reset to another pointer or to nullptr
    void reset(T* ptr)
    {
        if (m_ptr)
        {
            m_ptr->unregister_tracker(this);
        }
        m_ptr = ptr;
        if (m_ptr)
        {
            m_ptr->register_tracker(this);
        }
    }

    T* operator->() const
    {
        return m_ptr;
    }

    T& operator*() const
    {
        return *m_ptr;
    }

    T* get() const
    {
        return m_ptr;
    }

    // check whether pointer is null
    explicit operator bool() const
    {
        return m_ptr != nullptr;
    }

    // dtor
    ~tracker_ptr()
    {
        if (m_ptr)
        {
            m_ptr->unregister_tracker(this);
        }
    }
};

// a class that want its pointers to be trackedby the tracker_ptr
// may inherit from the trackable_ptr_concept class.
// but do not have to: any class that implements register_tracker() and unregister_tracker()
// and make sure to call reset() on the tracker before actual deletion would work
template<class T>
class trackable_ptr_concept
{
private:
    // the tracker that tracks this pointer
    tracker_ptr<T>* m_tracker;

public:
    // ctor
    trackable_ptr_concept() : m_tracker(nullptr) {}

    // register a tracker that would be notified upon deletion
    void register_tracker(tracker_ptr<T>* tracker)
    {
        // no check if th tracker was already set
        // we implicitly unregister
        m_tracker = tracker;
    }
    
    // unregister a tracker
    void unregister_tracker(const tracker_ptr<T>* tracker)
    {
        if (tracker == m_tracker)
        {
            m_tracker = nullptr;
        }
        // else - this may happen if a tracker was
        // registered without unregistering first
    }

    // dtor
    ~trackable_ptr_concept()
    {
        if (m_tracker)
        {
            m_tracker->reset(nullptr);
        }
    }
};

