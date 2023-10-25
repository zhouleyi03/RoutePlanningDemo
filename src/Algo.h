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
    Algo(int *frame_var, bool *end_flag);
    virtual ~Algo();

    void operator()(std::shared_ptr<Map> map);

    virtual void exec() = 0;

    std::condition_variable m_cv;

protected:
    std::mutex m_mutex;

    std::shared_ptr<Map> m_map;
    int *m_frame_var;
    bool *m_end_flag;

private:
    std::unique_ptr<std::thread> m_thread;
};
