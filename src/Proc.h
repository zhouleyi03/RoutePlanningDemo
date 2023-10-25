/**
 *  Copyright (C) 2023 Leyi Zhou
 *  Sample code for my CSTB presentation
 */

#pragma once

#include "Map.h"
#include "BlockGen.h"
#include "BFS.h"

#include <SDL.h>

#include <memory>
#include <utility>
#include <atomic>

class Proc
{
public:
    Proc(int argc, char **argv);

    template <typename TGenerator>
    void setGenerator()
    {
        m_generator = std::make_unique<TGenerator>();
    }

    template <typename TAlgo>
    void setAlgo()
    {
        m_algo = std::make_unique<TAlgo>();
    }

    void init();
    void initMap();
    void shutdown();
    void loop();

    void clearScreen();
    void updateScreen();

    void fillRect(int x1, int y1, int w, int h);
    void fillRect(int x1, int y1, int w, int h, int r, int g, int b);
    void fillGrid(int x, int y);
    void fillGrid(int x, int y, int r, int g, int b);
    void drawGrids();

    void renderMap();

    void test();

private:
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    bool m_window_should_close = false;

    int m_side_length = 0; // 窗口边长
    int m_grid_size = 16;  // 方格边长，单位为像素
    int m_side_grid_num;

    std::shared_ptr<Map> m_map;
    std::unique_ptr<MapGenerator> m_generator;
    std::unique_ptr<Algo> m_algo;

    std::atomic_int frame_var;
};
