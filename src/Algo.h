#pragma once

#include "Map.h"

#include <thread>
#include <memory>
#include <atomic>
#include <condition_variable>
#include <mutex>

class Algo
{
public:
    virtual ~Algo();
    void operator()(Map &map, std::atomic_int &frame_var);
    virtual void exec(Map &map, std::atomic_int &frame_var) = 0;

    std::condition_variable m_cv;

protected:
    std::unique_ptr<std::thread> m_thread;
    std::mutex m_mutex;
};
