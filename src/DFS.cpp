#include "DFS.h"

#include <stack>
#include <iostream>

DFS::DFS(int *frame_var, bool *end_flag)
    : Algo(frame_var, end_flag)
{
}

void DFS::exec()
{
    std::stack<Map::Pos> s;
    s.push(m_map->getStartPoint());

    auto attemptVisit = [&](const Map::Pos &cur, int dx, int dy)
    {
        Map::Pos t(cur.first + dx, cur.second + dy);
        if (m_map->isPosValid(t.first, t.second))
        {
            auto state = m_map->getGridState(t.first, t.second);
            if (state == GridState::ENDPOINT)
            {
                *m_end_flag = true;
                return;
            }
            else if (state == GridState::EMPTY)
            {
                s.push(t);
                m_map->visit(t.first, t.second);
                std::cout << "[DFS] visit " << cur.first << ' ' << cur.second << '\n';
            }
        }
    };

    while (!s.empty())
    {
        auto cur = s.top();
        s.pop();

        std::unique_lock<std::mutex> lock(m_mutex);
        *m_frame_var = DFS_frame_interval;

        attemptVisit(cur, -1, 0);
        attemptVisit(cur, 1, 0);
        attemptVisit(cur, 0, -1);
        attemptVisit(cur, 0, 1);

        m_cv.wait(lock);

        if (*m_end_flag)
            return;
    }

    std::cout << "[DFS] traversal ended." << std::endl;
}
