#ifndef __TICKETLOCK_HPP__
#define __TICKETLOCK_HPP__

#include <atomic>
#include <thread>
using namespace std;

class TicketLock {
private:
    atomic<unsigned int> next_ticket{0};
    atomic<unsigned int> now_serving{0};

public:
    void Lock() {
        unsigned int my_ticket = next_ticket.fetch_add(1, memory_order_relaxed);

        while(now_serving.load(memory_order_acquire) != my_ticket){
            // pular sua vez na CPU enquanto espera o now_serving
            this_thread::yield();
        }
    }

    void Unlock() {
        unsigned current = now_serving.load(memory_order_relaxed);
        now_serving.store(current + 1, memory_order_release);
    }
};

#endif