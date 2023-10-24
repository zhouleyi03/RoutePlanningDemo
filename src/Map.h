#pragma once

#include "GridState.h"

#include <vector>

class Map
{
    friend class BlockGen;

public:
    Map(int w, int h);

    GridState &getGridState(int x, int y);
    GridState &getGridState(int i);

    void setGridState(int x, int y, GridState state);
    void setGridState(int i, GridState state);

    void visit(int x, int y);
    void pend(int x, int y);

    std::vector<GridState> &getData();
    int getX(int i) const;
    int getY(int i) const;
    int getIndex(int x, int y) const;
    bool isPosValid(int x, int y) const;

private:
    int m_width, m_height;
    std::vector<GridState> m_data;
};
