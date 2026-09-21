#include "NotificationQueue.hpp"
#include <stdexcept>
#include <utility>

void NotificationQueue::ensure_capacity(int needed) {
    if (needed <= capacity) return;
    int new_cap = (capacity == 0) ? 4 : capacity * 2;
    while (new_cap < needed) new_cap *= 2;
    Notification* nd = new Notification[new_cap];
    for (int i = 0; i < count; ++i)
        nd[i] = std::move(data[i]);
    delete[] data;
    data = nd;
    capacity = new_cap;
}

NotificationQueue::NotificationQueue() : data(nullptr), count(0), capacity(0) {}

NotificationQueue::NotificationQueue(const NotificationQueue& other)
    : data(nullptr), count(0), capacity(0) {
    if (other.count == 0) return;
    data = new Notification[other.capacity];
    capacity = other.capacity;
    for (int i = 0; i < other.count; ++i)
        data[i] = other.data[i];
    count = other.count;
}

NotificationQueue::NotificationQueue(NotificationQueue&& other) noexcept
    : data(other.data), count(other.count), capacity(other.capacity) {
    other.data = nullptr; other.count = 0; other.capacity = 0;
}

NotificationQueue::~NotificationQueue() { delete[] data; }

NotificationQueue& NotificationQueue::operator=(const NotificationQueue& other) {
    if (this == &other) return *this;
    delete[] data; data = nullptr; count = 0; capacity = 0;
    if (other.count == 0) return *this;
    data = new Notification[other.capacity];
    capacity = other.capacity;
    for (int i = 0; i < other.count; ++i) data[i] = other.data[i];
    count = other.count;
    return *this;
}

NotificationQueue& NotificationQueue::operator=(NotificationQueue&& other) noexcept {
    if (this == &other) return *this;
    delete[] data;
    data = other.data; count = other.count; capacity = other.capacity;
    other.data = nullptr; other.count = 0; other.capacity = 0;
    return *this;
}

void NotificationQueue::push(const Notification& n) {
    ensure_capacity(count + 1);
    data[count] = n;
    ++count;
}

Notification NotificationQueue::pop() {
    if (count == 0)
        throw std::runtime_error("pop: queue is empty");
    Notification result = std::move(data[0]);
    // сдвигаем всё влево
    for (int i = 1; i < count; ++i)
        data[i - 1] = std::move(data[i]);
    --count;
    return result;
}