#pragma once

class AstarGrid
{
public:
    AstarGrid() = default;
    AstarGrid(int x, int y, int pre_x, int pre_y, int g, int h);

    bool operator<(const AstarGrid &rhs) const;
    bool operator==(const AstarGrid &rhs) const;

    int m_x;
    int m_y;
    int m_pre_x;
    int m_pre_y;
    int m_f;
    int m_g;
    int m_h;
};

class AstarGridHash
{
public:
    unsigned long long operator()(const AstarGrid &grid) const;
};
