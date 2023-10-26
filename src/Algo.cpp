#include "Algo.h"

#include <functional>

Algo::Algo(int *frame_var, bool *end_flag)
    : m_frame_var(frame_var), m_end_flag(end_flag)
{
}

Algo::~Algo()
{
    if (m_thread) // 防止用户在启动算法之前就按下 esc 退出程序，此时 thread 对象还未构造，造成段错误
        m_thread->join();
}

void Algo::operator()(std::shared_ptr<Map> map)
{
    m_map = map;

    m_thread = std::make_unique<std::thread>(&Algo::exec, this);
}
