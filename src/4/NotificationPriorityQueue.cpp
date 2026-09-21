#include "NotificationPriorityQueue.hpp"
#include <stdexcept>
#include <utility>

void NotificationPriorityQueue::ensure_capacity(int needed) {
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

// ---- Ранг актуальности: чем больше, тем важнее ----
static int priority_rank(const Notification& n) {
    // Правило 1: срочное системное — максимальный приоритет.
    if (n.type == NotificationType::SYSTEM && n.data.system.severity == Severity::URGENT)
        return 100;

    // Правило 3: тип при одинаковом времени.
    switch (n.type) {
        case NotificationType::MESSAGE: return 3;
        case NotificationType::SYSTEM:  return 2;   // обычное
        case NotificationType::APP:     return 1;
    }
    return 0;
}
static int type_rank(NotificationType t) {
    switch (t) {
        case NotificationType::MESSAGE: return 3;
        case NotificationType::SYSTEM:  return 2;
        case NotificationType::APP:     return 1;
    }
    return 0;
}

static bool is_urgent_system(const Notification& n) {
    return n.type == NotificationType::SYSTEM &&
           n.data.system.severity == Severity::URGENT;
}

static bool more_urgent(const Notification& a, const Notification& b) {
    bool a_urgent = is_urgent_system(a);
    bool b_urgent = is_urgent_system(b);

    // Правило 1: срочное системное важнее любого другого
    if (a_urgent != b_urgent) return a_urgent;

    // Если оба срочные системные — раньше важнее
    if (a_urgent && b_urgent) return a.timestamp < b.timestamp;

    // Правило 2: раньше = важнее (основной критерий для остальных)
    if (a.timestamp != b.timestamp) return a.timestamp < b.timestamp;

    // Правило 3: при равном времени — по типу
    int ra = type_rank(a.type);
    int rb = type_rank(b.type);
    if (ra != rb) return ra > rb;

    // Правило 4: при равных времени и типе — раньше добавленный
    return false;
}

int NotificationPriorityQueue::best_index() const {
    if (count == 0) return -1;
    int best = 0;
    for (int i = 1; i < count; ++i) {
        if (more_urgent(data[i], data[best]))
            best = i;
    }
    return best;
}

NotificationPriorityQueue::NotificationPriorityQueue()
    : data(nullptr), count(0), capacity(0) {}

NotificationPriorityQueue::NotificationPriorityQueue(const NotificationPriorityQueue& other)
    : data(nullptr), count(0), capacity(0) {
    if (other.count == 0) return;
    data = new Notification[other.capacity];
    capacity = other.capacity;
    for (int i = 0; i < other.count; ++i) data[i] = other.data[i];
    count = other.count;
}

NotificationPriorityQueue::NotificationPriorityQueue(NotificationPriorityQueue&& other) noexcept
    : data(other.data), count(other.count), capacity(other.capacity) {
    other.data = nullptr; other.count = 0; other.capacity = 0;
}

NotificationPriorityQueue::~NotificationPriorityQueue() { delete[] data; }

NotificationPriorityQueue& NotificationPriorityQueue::operator=(const NotificationPriorityQueue& other) {
    if (this == &other) return *this;
    delete[] data; data = nullptr; count = 0; capacity = 0;
    if (other.count == 0) return *this;
    data = new Notification[other.capacity];
    capacity = other.capacity;
    for (int i = 0; i < other.count; ++i) data[i] = other.data[i];
    count = other.count;
    return *this;
}

NotificationPriorityQueue& NotificationPriorityQueue::operator=(NotificationPriorityQueue&& other) noexcept {
    if (this == &other) return *this;
    delete[] data;
    data = other.data; count = other.count; capacity = other.capacity;
    other.data = nullptr; other.count = 0; other.capacity = 0;
    return *this;
}

void NotificationPriorityQueue::push(const Notification& n) {
    ensure_capacity(count + 1);
    data[count] = n;
    ++count;
}

Notification NotificationPriorityQueue::pop() {
    if (count == 0)
        throw std::runtime_error("pop: queue is empty");
    int idx = best_index();
    Notification result = std::move(data[idx]);
    // Сдвигаем хвост влево, начиная с idx
    for (int i = idx + 1; i < count; ++i)
        data[i - 1] = std::move(data[i]);
    --count;
    return result;
}