#pragma once
#include<atomic>
class Spinlock{
    public:
        Spinlock() = default;
        ~Spinlock(){
            this->unlock();
        }
        Spinlock(const Spinlock&) = delete;
        Spinlock(Spinlock&&) = delete;
        Spinlock& operator=(const Spinlock&) = delete;

        void lock(){
            while(flag.test_and_set(std::memory_order_acquire)){}
        }

        void unlock(){
            flag.clear(std::memory_order_release); //释放锁
        }

    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
};