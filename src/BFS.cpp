#include "BFS.h"

#include <queue>
#include <iostream>

BFS::BFS(int *frame_var, bool *end_flag)
    : Algo(frame_var, end_flag)
{
}

void BFS::exec()
{
    Map::Pos sp_pos = m_map->getStartPoint(), ep_pos;

    std::vector<Map::Pos> pre;
    pre.resize(m_map->getData().size());

    std::queue<Map::Pos> q;
    q.push(m_map->getStartPoint());

    auto attemptVisit = [&](const Map::Pos &cur, int dx, int dy)
    {
        Map::Pos t(cur.first + dx, cur.second + dy);
        if (m_map->isPosValid(t.first, t.second))
        {
            auto state = m_map->getGridState(t.first, t.second);
            if (state == GridState::ENDPOINT)
            {
                pre[m_map->getIndex(t.first, t.second)] = {cur.first, cur.second};
                ep_pos = t;
                *m_end_flag = true;
                return;
            }
            else if (state == GridState::EMPTY)
            {
                pre[m_map->getIndex(t.first, t.second)] = {cur.first, cur.second};
                q.push(t);
                m_map->pend(t.first, t.second);
            }
        }
    };

    while (!q.empty())
    {
        auto cur = q.front();
        m_map->visit(cur.first, cur.second);
        q.pop();

        std::unique_lock<std::mutex> lock(m_mutex);
        *m_frame_var = BFS_frame_interval;

        attemptVisit(cur, -1, 0);
        attemptVisit(cur, 1, 0);
        attemptVisit(cur, 0, -1);
        attemptVisit(cur, 0, 1);

        m_cv.wait(lock);

        if (*m_end_flag)
            break;
    }

    while (1) // 回溯，输出路径
    {
        auto t = pre[m_map->getIndex(ep_pos.first, ep_pos.second)];
        if (t == sp_pos)
            break;
        m_map->target(ep_pos.first, ep_pos.second);
        ep_pos = t;
    }
}
