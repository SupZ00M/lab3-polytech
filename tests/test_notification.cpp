#include "Notification.hpp"
#include "NotificationQueue.hpp"
#include "NotificationPriorityQueue.hpp"
#include <iostream>

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::cerr << "FAIL: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++failures; } } while (0)

int main() {
    
    {
        Notification n1 = Notification::make_system (10, "msg",  Severity::NORMAL);
        Notification n2 = Notification::make_message(20, "A", "hi");
        Notification n3 = Notification::make_app    (30, "app", "t", "x");
        Notification n4 = Notification::make_system (40, "urgent", Severity::URGENT);

        Notification arr[4] = { n1, n2, n3, n4 };
        CHECK(count_by_type(arr, 4, NotificationType::SYSTEM)  == 2);
        CHECK(count_by_type(arr, 4, NotificationType::MESSAGE) == 1);
        CHECK(count_by_type(arr, 4, NotificationType::APP)     == 1);

        // Копирование
        Notification copy = n1;
        CHECK(copy.type == NotificationType::SYSTEM);
        CHECK(copy.data.system.message == "msg");
    }

   
    {
        NotificationQueue q;
        Notification a = Notification::make_system (10, "a", Severity::NORMAL);
        Notification b = Notification::make_system (20, "b", Severity::NORMAL);
        Notification c = Notification::make_system (30, "c", Severity::NORMAL);
        q.push(a); q.push(b); q.push(c);
        CHECK(q.size() == 3);

        Notification x = q.pop();
        CHECK(x.timestamp == 10);
        Notification y = q.pop();
        CHECK(y.timestamp == 20);
        CHECK(q.size() == 1);
    }

  
    {
        NotificationPriorityQueue pq;
        Notification n1 = Notification::make_system (10, "low",   Severity::NORMAL);
        Notification n2 = Notification::make_message(20, "A", "hi");
        Notification n3 = Notification::make_app    (30, "app", "t", "x");
        Notification n4 = Notification::make_system (40, "fire",  Severity::URGENT);
        pq.push(n1); pq.push(n2); pq.push(n3); pq.push(n4);

        // Правило 1: срочное системное — первое
        CHECK(pq.pop().timestamp == 40);
        // Правило 2: раньше = важнее
        CHECK(pq.pop().timestamp == 10);
        CHECK(pq.pop().timestamp == 20);
        CHECK(pq.pop().timestamp == 30);
        CHECK(pq.size() == 0);
    }

   
    {
        NotificationPriorityQueue pq;
        Notification s = Notification::make_system (100, "sys", Severity::NORMAL);
        Notification m = Notification::make_message(100, "A", "msg");
        Notification a = Notification::make_app    (100, "app", "t", "x");
        pq.push(a); pq.push(s); pq.push(m);
   
        CHECK(pq.pop().type == NotificationType::MESSAGE);
        CHECK(pq.pop().type == NotificationType::SYSTEM);
        CHECK(pq.pop().type == NotificationType::APP);
    }

    if (failures) { std::cerr << failures << " check(s) failed\n"; return 1; }
    std::cout << "test_notification: OK\n";
    return 0;
}