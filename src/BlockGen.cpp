/**
 *  Copyright (C) 2023 Leyi Zhou
 *  Sample code for my CSTB presentation
 */

#include "BlockGen.h"

#include <random>
#include <iostream>
#include <ctime>
#include <set>

void BlockGen::operator()(Map &map)
{
    std::vector<int> pivots;
    pivots.resize(BlockGen_block_num);
    std::set<int> pivots_aux;
    std::vector<std::tuple<int, int, int, int>> quad;
    std::vector<unsigned char> mapdata;
    mapdata.resize(map.getData().size());

    std::default_random_engine engine;
    std::uniform_int_distribution<int> dist(0, map.getData().size());
    engine.seed(static_cast<unsigned>(std::time(nullptr)));

    // 对于更详细的算法描述，请参考我的报告
    // 1. 生成若干个不重复的 “基点”
    for (int i = 0; i < BlockGen_block_num; i++)
    {
        pivots[i] = dist(engine);
        if (pivots_aux.count(pivots[i]))
            --i;
    }

    // 2. 对于每个基点，向四周 “扩展” 产生随机大小的矩形，保证矩形之间两两不相交
    for (int i = 0; i < BlockGen_block_num; i++)
    {
        quad.emplace_back();

        int x = map.getX(pivots[i]),
            y = map.getY(pivots[i]);

        int borders[4];
        // 左边界
        int tx = x;
        while (tx >= 0 && !mapdata[map.getIndex(tx, y)])
            --tx;
        borders[0] = x - tx;

        // 右边界
        tx = x;
        while (map.isPosValid(tx, y) && !mapdata[map.getIndex(tx, y)])
            ++tx;
        borders[1] = tx - x;

        // 上边界
        int ty = y;
        while (ty >= 0 && !mapdata[map.getIndex(x, ty)])
            --ty;
        borders[2] = y - ty;

        // 下边界
        ty = y;
        while (map.isPosValid(x, ty) && !mapdata[map.getIndex(x, ty)])
            ++ty;
        borders[3] = ty - y;

        for (int j = 0; j < 4; j++)
        {
            std::uniform_int_distribution<int> dist(0, std::max(0, borders[j] - 1));
            borders[j] = dist(engine);
        }

        std::get<0>(quad[i]) = borders[0];
        std::get<1>(quad[i]) = borders[1];
        std::get<2>(quad[i]) = borders[2];
        std::get<3>(quad[i]) = borders[3];

        std::cout << "[BlockGen] Select pivot (" << x << ", " << y << ")\n";
        std::cout << "[BlockGen] --> Border (" << borders[0] << ' ' << borders[1]
                  << ' ' << borders[2] << ' ' << borders[3] << ")\n";

        for (int u = x - borders[0]; u <= x + borders[1]; u++)
        {
            for (int v = y - borders[2]; v <= y + borders[3]; v++)
                mapdata[map.getIndex(u, v)] = 1;
        }
    }

    // 3. 将每个矩形向内 “收缩” 随机宽度
    for (int i = 0; i < BlockGen_block_num; i++)
    {
        int x = map.getX(pivots[i]),
            y = map.getY(pivots[i]);

        int *t[4] = {&std::get<0>(quad[i]), &std::get<1>(quad[i]),
                     &std::get<2>(quad[i]), &std::get<3>(quad[i])};

        for (int j = 0; j < 4; j++)
        {
            int dmax = std::max(0, *t[j] - 1),
                dmin = std::min(3, dmax);
            std::uniform_int_distribution<int> dist(dmin, dmax);
            *t[j] = dist(engine);
        }

        for (int u = x - *t[0]; u < x + *t[1]; u++)
        {
            for (int v = y - *t[2]; v < y + *t[3]; v++)
                map.setGridState(u, v, GridState::BARRIER);
        }
    }

    genSEPoint(map);
}
