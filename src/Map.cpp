#include "Map.h"

#include <cassert>
#include <thread>
#include <iostream>

Map::Map(int w, int h) : m_width(w), m_height(h)
{
    m_data.resize(w * h);
}

void Map::setSEPoint(int sp_x, int sp_y, int ep_x, int ep_y)
{
    m_sp_x = sp_x;
    m_sp_y = sp_y;
    m_ep_x = ep_x;
    m_ep_y = ep_y;
}

void Map::setSEPoint(int sp_i, int ep_i)
{
    m_sp_x = sp_i % m_width;
    m_sp_y = sp_i / m_width;
    m_ep_x = ep_i % m_width;
    m_ep_y = ep_i / m_width;
}

Map::Pos Map::getStartPoint() const
{
    return {m_sp_x, m_sp_y};
}

Map::Pos Map::getEndPoint() const
{
    return {m_ep_x, m_ep_y};
}

GridState Map::getGridState(int x, int y)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    assert(x + m_width * y < m_data.size());
    return m_data[x + m_width * y];
}

GridState Map::getGridState(int i)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    assert(i < m_data.size());
    return m_data[i];
}

void Map::setGridState(int x, int y, GridState state)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    assert(x + m_width * y < m_data.size());
    m_data[x + m_width * y] = state;
}

void Map::setGridState(int i, GridState state)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    assert(i < m_data.size());
    m_data[i] = state;
}

void Map::visit(int x, int y)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    assert(x + m_width * y < m_data.size());
    m_data[x + m_width * y] = GridState::VISITED;
}

void Map::pend(int x, int y)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    assert(x + m_width * y < m_data.size());
    m_data[x + m_width * y] = GridState::PENDING;
}

void Map::printData()
{
    for (int i = 0; i < m_width; i++)
    {
        for (int j = 0; j < m_height; j++)
        {
            std::cout << static_cast<int>(m_data[i + j * m_width]) << ' ';
        }
        std::cout << '\n';
    }
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
