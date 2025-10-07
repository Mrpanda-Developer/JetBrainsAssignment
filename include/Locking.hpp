#pragma once
#include <shared_mutex>
//Kinda funny since I just heard about Mutex and Futex in OS lectures :D
//But I guess this is a good use case for it

class ReadLock {
    std::shared_mutex& m_;
public:
    explicit ReadLock(std::shared_mutex& m) : m_(m) { m_.lock_shared(); }
    ~ReadLock() { m_.unlock_shared(); }
};

class WriteLock {
    std::shared_mutex& m_;
public:
    explicit WriteLock(std::shared_mutex& m) : m_(m) { m_.lock(); }
    ~WriteLock() { m_.unlock(); }
};
