#include "Map.h"

#include <cassert>

Map::Map(int w, int h) : m_width(w), m_height(h)
{
    m_data.resize(w * h);
}

GridState &Map::getGridState(int x, int y)
{
    assert(x + m_width * y < m_data.size());
    return m_data[x + m_width * y];
}

GridState &Map::getGridState(int i)
{
    assert(i < m_data.size());
    return m_data[i];
}

void Map::setGridState(int x, int y, GridState state)
{
    assert(x + m_width * y < m_data.size());
    m_data[x + m_width * y] = state;
}

void Map::setGridState(int i, GridState state)
{
    assert(i < m_data.size());
    m_data[i] = state;
}

void Map::visit(int x, int y)
{
    assert(x + m_width * y < m_data.size());
    m_data[x + m_width * y] = GridState::VISITED;
}

void Map::pend(int x, int y)
{
    assert(x + m_width * y < m_data.size());
    m_data[x + m_width * y] = GridState::PENDING;
}

std::vector<GridState> &Map::getData()
{
    return m_data;
}

int Map::getX(int i) const
{
    return i % m_width;
}

int Map::getY(int i) const
{
    return i / m_width;
}

int Map::getIndex(int x, int y) const
{
    return x + m_width * y;
}

bool Map::isPosValid(int x, int y) const
{
    return x < m_width && y < m_height && x >= 0 && y >= 0;
}
