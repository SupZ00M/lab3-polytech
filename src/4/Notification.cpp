#include "Notification.hpp"
#include <iostream>
#include <stdexcept>
#include <new>          // placement new

// ---------- Уничтожение активного члена ----------
static void destroy_union(Notification& n) {
    switch (n.type) {
        case NotificationType::SYSTEM:  n.data.system.~SystemData();  break;
        case NotificationType::MESSAGE: n.data.message.~MessageData(); break;
        case NotificationType::APP:     n.data.app.~AppData();         break;
    }
}

// ---------- Копирование активного члена ----------
static void copy_union(Notification& dst, const Notification& src) {
    switch (src.type) {
        case NotificationType::SYSTEM:
            new (&dst.data.system) SystemData(src.data.system);  break;
        case NotificationType::MESSAGE:
            new (&dst.data.message) MessageData(src.data.message); break;
        case NotificationType::APP:
            new (&dst.data.app) AppData(src.data.app);           break;
    }
}

// ---------- Перемещение активного члена ----------
static void move_union(Notification& dst, Notification& src) {
    switch (src.type) {
        case NotificationType::SYSTEM:
            new (&dst.data.system) SystemData(std::move(src.data.system));  break;
        case NotificationType::MESSAGE:
            new (&dst.data.message) MessageData(std::move(src.data.message)); break;
        case NotificationType::APP:
            new (&dst.data.app) AppData(std::move(src.data.app));           break;
    }
}

// ---------- Конструкторы / деструктор ----------
Notification::Notification()
    : timestamp(0), type(NotificationType::SYSTEM) {
    new (&data.system) SystemData();
}

Notification::Notification(const Notification& other)
    : timestamp(other.timestamp), type(other.type) {
    copy_union(*this, other);
}

Notification::Notification(Notification&& other) noexcept
    : timestamp(other.timestamp), type(other.type) {
    move_union(*this, other);
}

Notification::~Notification() {
    destroy_union(*this);
}

Notification& Notification::operator=(const Notification& other) {
    if (this == &other) return *this;
    destroy_union(*this);
    timestamp = other.timestamp;
    type      = other.type;
    copy_union(*this, other);
    return *this;
}

Notification& Notification::operator=(Notification&& other) noexcept {
    if (this == &other) return *this;
    destroy_union(*this);
    timestamp = other.timestamp;
    type      = other.type;
    move_union(*this, other);
    return *this;
}

// ---------- Фабрики ----------
Notification Notification::make_system(std::uint64_t ts, const MyString& msg, Severity sev) {
    Notification n;
    destroy_union(n);                    // уничтожаем SystemData() по умолчанию
    n.timestamp = ts;
    n.type = NotificationType::SYSTEM;
    new (&n.data.system) SystemData(msg, sev);
    return n;
}

Notification Notification::make_message(std::uint64_t ts, const MyString& contact, const MyString& text) {
    Notification n;
    destroy_union(n);
    n.timestamp = ts;
    n.type = NotificationType::MESSAGE;
    new (&n.data.message) MessageData(contact, text);
    return n;
}

Notification Notification::make_app(std::uint64_t ts, const MyString& app, const MyString& title, const MyString& text) {
    Notification n;
    destroy_union(n);
    n.timestamp = ts;
    n.type = NotificationType::APP;
    new (&n.data.app) AppData(app, title, text);
    return n;
}

// ---------- Вывод ----------
void Notification::print(std::ostream& os) const {
    os << "[ts=" << timestamp << "] ";
    switch (type) {
        case NotificationType::SYSTEM:
            os << "SYSTEM "
               << (data.system.severity == Severity::URGENT ? "(URGENT) " : "(normal) ")
               << data.system.message;
            break;
        case NotificationType::MESSAGE:
            os << "MESSAGE from " << data.message.contact
               << ": " << data.message.text;
            break;
        case NotificationType::APP:
            os << "APP " << data.app.app_name
               << " / " << data.app.title
               << ": " << data.app.text;
            break;
    }
    os << "\n";
}

// ---------- Свободные функции ----------
void print_notification(const Notification& n) {
    n.print();
}

int count_by_type(const Notification* arr, int n, NotificationType t) {
    int cnt = 0;
    for (int i = 0; i < n; ++i)
        if (arr[i].type == t) ++cnt;
    return cnt;
}
