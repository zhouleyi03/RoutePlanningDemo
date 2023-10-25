#include "Algo.h"

#include <functional>

Algo::Algo(int *frame_var, bool *end_flag)
    : m_frame_var(frame_var), m_end_flag(end_flag)
{
}

Algo::~Algo()
{
    m_thread->join();
}

void Algo::operator()(std::shared_ptr<Map> map)
{
    m_map = map;

    m_thread = std::make_unique<std::thread>(&Algo::exec, this);
}
