#include "Algo.h"

#include <functional>

Algo::~Algo()
{
    m_thread->join();
}

void Algo::operator()(Map &map, std::atomic_int &frame_var)
{
    m_thread = std::make_unique<std::thread>(
        &Algo::exec, this,
        std::ref(map), std::ref(frame_var));
}
