#include "Astar.h"

#include <queue>
#include <unordered_set>
#include <iostream>

Astar::Astar(int *frame_var, bool *end_flag)
    : Algo(frame_var, end_flag)
{
}

void Astar::exec()
{
    std::priority_queue<AstarGrid> open_list;
    std::unordered_set<AstarGrid, AstarGridHash> open_list_check;
    std::unordered_set<AstarGrid, AstarGridHash> closed_list_check;

    AstarGrid ep_grid;

    auto sp = m_map->getStartPoint();
    auto ep = m_map->getEndPoint();

    auto fastAbs = [](int x)
    {
        return (x ^ (x >> 31)) - (x >> 31);
    };

    auto calcH = [&](int x, int y) // 启发函数 h：曼哈顿距离
    {
        return fastAbs(x - ep.first) + fastAbs(y - ep.second);
    };

    auto attemptVisit = [&](const AstarGrid &cur, int dx, int dy)
    {
        int x = cur.m_x + dx, y = cur.m_y + dy;
        if (m_map->isPosValid(x, y))
        {
            AstarGrid t(x, y, cur.m_x, cur.m_y,
                        cur.m_g + fastAbs(dx) + fastAbs(dy), calcH(x, y));
            auto state = m_map->getGridState(x, y);
            if (state == GridState::ENDPOINT)
            {
                m_algo_end_flag = true;
                ep_grid = t;
                return;
            }
            else if (state == GridState::EMPTY)
            {
                if (!closed_list_check.count(t))
                {
                    auto iter = open_list_check.find(t);
                    if (iter == open_list_check.end()) // 加入 open list
                    {
                        open_list.push(t);
                        open_list_check.insert(t);
                        m_map->pend(t.m_x, t.m_y);
                    }
                    else // 已存在于 open list 中
                    {
                        if (t.m_g < iter->m_g)
                        {
                            open_list_check.erase(iter);
                            open_list_check.insert(t);
                        }
                    }
                }
            }
        }
    };

    open_list.emplace(sp.first, sp.second, -1, -1, 0, calcH(sp.first, sp.second));
    open_list_check.insert(open_list.top());

    while (!open_list.empty())
    {
        auto grid = open_list.top();
        auto iter = open_list_check.find(grid);
        if (iter->m_g != grid.m_g) // 检查当前 grid 的 g 值是否更新过
        {
            grid.m_g = iter->m_g;
            open_list.pop();
            open_list.push(grid); // 若已更新过，重新压入优先队列
            continue;
        }
        open_list.pop();
        open_list_check.erase(grid);
        closed_list_check.insert(grid);
        m_map->visit(grid.m_x, grid.m_y);

        std::unique_lock<std::mutex> lock(m_mutex);
        *m_frame_var = Astar_frame_interval;

        attemptVisit(grid, 0, -1);
        attemptVisit(grid, 0, 1);
        attemptVisit(grid, -1, 0);
        attemptVisit(grid, 1, 0);

        m_cv.wait(lock);

        if (*m_end_flag)
            return;

        if (m_algo_end_flag)
            break;
    }

    while (1)
    {
        ep_grid.m_x = ep_grid.m_pre_x;
        ep_grid.m_y = ep_grid.m_pre_y;
        auto iter = closed_list_check.find(ep_grid);
        if (iter == closed_list_check.end())
            break;
        m_map->target(ep_grid.m_x, ep_grid.m_y);
        ep_grid = *iter;
    }
}
