#ifndef NOTIFICATION_QUEUE_HPP
#define NOTIFICATION_QUEUE_HPP

#include "Notification.hpp"

class NotificationQueue {
private:
    Notification* data;
    int count;
    int capacity;

    void ensure_capacity(int needed);

public:
    NotificationQueue();
    NotificationQueue(const NotificationQueue& other);
    NotificationQueue(NotificationQueue&& other) noexcept;
    ~NotificationQueue();

    NotificationQueue& operator=(const NotificationQueue& other);
    NotificationQueue& operator=(NotificationQueue&& other) noexcept;

    // ---- Задание 4.3 ----
    void push(const Notification& n);
    int  size() const { return count; }
    Notification pop();   // FIFO + удаление

    // ---- Итератор ----
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