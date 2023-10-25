#include "BFS.h"

void BFS::exec(Map &map, std::atomic_int &frame_var)
{
    GridState toggle = GridState::EMPTY;
    while (1)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        Map::Pos x = map.getStartPoint();
        map.setGridState(x.first, x.second, toggle);
        toggle = (toggle == GridState::EMPTY) ? GridState::BARRIER : GridState::EMPTY;
        frame_var = BFS_frame_interval;
        m_cv.wait(lock);
    }
}
