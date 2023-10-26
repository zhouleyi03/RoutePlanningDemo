#include "AstarGrid.h"

#include <functional>

AstarGrid::AstarGrid(int x, int y, int pre_x, int pre_y, int g, int h)
    : m_x(x), m_y(y), m_pre_x(pre_x), m_pre_y(pre_y), m_g(g), m_h(h)
{
    m_f = g + h;
}

bool AstarGrid::operator<(const AstarGrid &rhs) const
{
    return m_f > rhs.m_f; // 小根堆
}

bool AstarGrid::operator==(const AstarGrid &rhs) const
{
    return m_x == rhs.m_x && m_y == rhs.m_y;
}

unsigned long long AstarGridHash::operator()(const AstarGrid &grid) const
{
    return std::hash<int>()(grid.m_x) ^ std::hash<int>()(grid.m_y);
}
