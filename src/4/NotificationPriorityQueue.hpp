#ifndef NOTIFICATION_PRIORITY_QUEUE_HPP
#define NOTIFICATION_PRIORITY_QUEUE_HPP

#include "Notification.hpp"

class NotificationPriorityQueue {
private:
    Notification* data;
    int count;
    int capacity;

    void ensure_capacity(int needed);
    int  best_index() const; 

public:
    NotificationPriorityQueue();
    NotificationPriorityQueue(const NotificationPriorityQueue& other);
    NotificationPriorityQueue(NotificationPriorityQueue&& other) noexcept;
    ~NotificationPriorityQueue();

    NotificationPriorityQueue& operator=(const NotificationPriorityQueue& other);
    NotificationPriorityQueue& operator=(NotificationPriorityQueue&& other) noexcept;

    void push(const Notification& n);
    int  size() const { return count; }
    Notification pop();  

    class Iterator {
    private:
        Notification* ptr;
    public:
        Iterator() : ptr(nullptr) {}
        explicit Iterator(Notification* p) : ptr(p) {}
        Notification& operator*()  const { return *ptr; }
        Notification* operator->() const { return  ptr; }
        Iterator& operator++()    { ++ptr; return *this; }
        Iterator  operator++(int) { Iterator t(*this); ++ptr; return t; }
        bool operator==(const Iterator& o) const { return ptr == o.ptr; }
        bool operator!=(const Iterator& o) const { return ptr != o.ptr; }
    };

    Iterator begin() { return Iterator(data); }
    Iterator end()   { return Iterator(data + count); }
};

#endif