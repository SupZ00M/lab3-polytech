#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP

#include "My_string.hpp"
#include <cstdint>

// ---------- Тип уведомления ----------
enum class NotificationType {
    SYSTEM,       // системное
    MESSAGE,      // мгновенное сообщение
    APP           // от приложения
};

// ---------- Уровень серьёзности (для системного) ----------
enum class Severity {
    NORMAL,       // обычное
    URGENT        // срочное
};

// ---------- Данные по типам ----------
struct SystemData {
    MyString message;
    Severity severity;

    SystemData() : message(""), severity(Severity::NORMAL) {}
    SystemData(const MyString& m, Severity s) : message(m), severity(s) {}
};

struct MessageData {
    MyString contact;
    MyString text;

    MessageData() : contact(""), text("") {}
    MessageData(const MyString& c, const MyString& t) : contact(c), text(t) {}
};

struct AppData {
    MyString app_name;
    MyString title;
    MyString text;

    AppData() : app_name(""), title(""), text("") {}
    AppData(const MyString& a, const MyString& t, const MyString& x)
        : app_name(a), title(t), text(x) {}
};

// ---------- Уведомление ----------
struct Notification {
    // Именованный union — обязательное условие в C++
    union Data {
        SystemData  system;
        MessageData message;
        AppData     app;

        // Объявляем конструктор/деструктор/копирование,
        // но НЕ реализуем их — они нужны только чтобы
        // компилятор не удалял их автоматически.
        Data() {}
        ~Data() {}
        Data(const Data&) {}
        Data& operator=(const Data&) { return *this; }
    };

    std::uint64_t    timestamp;
    NotificationType type;
    Data             data;

    Notification();
    Notification(const Notification& other);
    Notification(Notification&& other) noexcept;
    ~Notification();

    Notification& operator=(const Notification& other);
    Notification& operator=(Notification&& other) noexcept;

    // Фабрики
    static Notification make_system(std::uint64_t ts, const MyString& msg, Severity sev);
    static Notification make_message(std::uint64_t ts, const MyString& contact, const MyString& text);
    static Notification make_app(std::uint64_t ts, const MyString& app, const MyString& title, const MyString& text);

    void print(std::ostream& os = std::cout) const;
};

// Свободные функции
void print_notification(const Notification& n);
int  count_by_type(const Notification* arr, int n, NotificationType t);

#endif