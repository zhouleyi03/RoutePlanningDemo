#pragma once

#include "Map.h"

#include <thread>
#include <memory>
#include <atomic>
#include <condition_variable>

class Algo
{
public:
    virtual ~Algo() {}
    virtual void operator()(Map &map, std::atomic_int &frame) = 0;

protected:
    std::shared_ptr<std::thread> m_thread;
    std::condition_variable m_cv;
};
