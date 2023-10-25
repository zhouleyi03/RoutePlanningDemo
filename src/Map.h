#pragma once

#include "GridState.h"

#include <vector>
#include <utility>
#include <mutex>

class Map
{
    friend class BlockGen;

public:
    using Pos = std::pair<int, int>;

    Map(int w, int h);

    void setSEPoint(int sp_x, int sp_y, int ep_x, int ep_y);
    void setSEPoint(int sp_i, int ep_i);

    Pos getStartPoint() const;
    Pos getEndPoint() const;

    GridState getGridState(int x, int y);
    GridState getGridState(int i);

    void setGridState(int x, int y, GridState state);
    void setGridState(int i, GridState state);

    void visit(int x, int y);
    void pend(int x, int y);

    void printData();

    std::vector<GridState> &getData();
    int getX(int i) const;
    int getY(int i) const;
    int getIndex(int x, int y) const;
    bool isPosValid(int x, int y) const;

private:
    int m_width, m_height;
    int m_sp_x, m_sp_y;
    int m_ep_x, m_ep_y;
    std::vector<GridState> m_data;
    std::mutex m_mutex;
};
