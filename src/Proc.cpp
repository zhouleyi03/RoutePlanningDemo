/**
 *  Copyright (C) 2023 Leyi Zhou
 *  Sample code for my CSTB presentation
 */

#include "Proc.h"

#include <cassert>
#include <ctime>
#include <cmath>
#include <utility>
#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <string>
#include <functional>
#include <mutex>

Proc::Proc(int argc, char **argv)
{
    std::system("chcp 65001");

    std::unordered_map<std::string, std::function<void(const std::string &)>> argmap;
    argmap["--gridsize"] = [this](const std::string &operand)
    {
        m_grid_size = std::stoi(operand);
    };
    argmap["--algo"] = [this](const std::string &operand)
    {
        if (operand == "BFS")
            setAlgo<BFS>();
        else if (operand == "DFS")
            setAlgo<DFS>();
        else if (operand == "A*")
            setAlgo<Astar>();
        else
        {
            std::cout << "warning: 未知的算法！将使用默认算法。\n";
            setAlgo<BFS>();
        }
    };
    argmap["-s"] = [this](const std::string &operand)
    {
        m_side_length = std::stoi(operand);
    };
    argmap["-g"] = [this](const std::string &operand)
    {
        if (operand == "BlockGen")
            setGenerator<BlockGen>();
        else
        {
            std::cout << "warning: 未知的地图生成器！将使用默认生成器。\n";
            setGenerator<BlockGen>();
        }
    };

    for (int i = 1; i < argc; i += 2)
    {
        std::string arg1(argv[i]);
        std::string arg2(argv[i + 1]);

        auto iter = argmap.find(arg1);
        if (iter == argmap.end())
        {
            std::cout << "fatal error: 未知的参数！\n";
            std::exit(-1);
        }
        iter->second(arg2);
    }

    if (!m_side_length)
    {
        std::cout << "fatal error: 没有给定窗口大小！\n";
        std::exit(-1);
    }
    if (!m_generator)
    {
        std::cout << "warning: 没有指定地图生成器！将使用默认生成器。\n";
        setGenerator<BlockGen>();
    }
    if (!m_algo)
    {
        std::cout << "warning: 未知的算法！将使用默认算法。\n";
        setAlgo<BFS>();
    }

    init();
}

void Proc::init()
{
    assert(m_side_length > 32);

    assert(!SDL_Init(SDL_INIT_VIDEO));

    m_window =
        SDL_CreateWindow("Route Planning Demo v1.0 @zhouleyi",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         m_side_length, m_side_length,
                         SDL_WINDOW_SHOWN);

    assert(m_window);

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    assert(m_renderer);

    m_side_grid_num = m_side_length / m_grid_size;

    m_map = std::make_shared<Map>(m_side_grid_num, m_side_grid_num);

    initMap();
}

void Proc::initMap()
{
    (*m_generator)(*m_map);
}

void Proc::shutdown()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Proc::loop()
{
    SDL_Event ev;
    while (!m_window_should_close)
    {
        if (m_start_flag)
        {
            if (!m_frame_var)
            {
                m_algo->m_cv.notify_one();
            }
            else
            {
                --m_frame_var;
            }
        }

        clearScreen();
        renderMap();
        updateScreen();

        while (SDL_PollEvent(&ev) != 0)
        {
            if (ev.type == SDL_QUIT)
                m_window_should_close = true;
            else if (ev.type == SDL_KEYDOWN)
            {
                switch (ev.key.keysym.sym)
                {
                case SDLK_ESCAPE: // 按 esc 退出
                    m_window_should_close = true;
                    m_end_flag = true;
                    m_algo->m_cv.notify_one();
                    return;
                    break;
                case SDLK_1: // 按 1 启动
                    if (!m_start_flag)
                    {
                        m_start_flag = true;
                        (*m_algo)(m_map);
                    }
                    break;
                default:
                    break;
                }
            }
        }
        SDL_Delay(8); // ~ 120 fps
    }
}

void Proc::clearScreen()
{
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_renderer);
}

void Proc::updateScreen()
{
    SDL_RenderPresent(m_renderer);
}

void Proc::fillRect(int x1, int y1, int w, int h)
{
    SDL_Rect rect;
    rect.x = x1;
    rect.y = y1;
    rect.w = w;
    rect.h = h;
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(m_renderer, &rect);
}

void Proc::fillRect(int x1, int y1, int w, int h, int r, int g, int b)
{
    SDL_Rect rect;
    rect.x = x1;
    rect.y = y1;
    rect.w = w;
    rect.h = h;
    SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
    SDL_RenderFillRect(m_renderer, &rect);
}

void Proc::fillGrid(int x, int y)
{
    fillRect(x * m_grid_size,
             y * m_grid_size,
             m_grid_size,
             m_grid_size);
}

void Proc::fillGrid(int x, int y, int r, int g, int b)
{
    fillRect(x * m_grid_size,
             y * m_grid_size,
             m_grid_size,
             m_grid_size,
             r, g, b);
}

void Proc::drawGrids()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    for (int i = 0; i < m_side_length; i += m_grid_size)
    {
        SDL_RenderDrawLine(m_renderer, i, 0, i, m_side_length);
        SDL_RenderDrawLine(m_renderer, 0, i, m_side_length, i);
    }
}

void Proc::renderMap()
{
    drawGrids();
    for (int i = 0; i < m_side_grid_num; i++)
    {
        for (int j = 0; j < m_side_grid_num; j++)
        {
            switch (m_map->getGridState(i, j))
            {
            case GridState::BARRIER:
                fillGrid(i, j, 0x4b, 0x00, 0x82);
                break;
            case GridState::PENDING:
                fillGrid(i, j, 0xa9, 0xa9, 0xa9);
                break;
            case GridState::STARTPOINT:
                fillGrid(i, j, 0xff, 0x45, 0x00);
                break;
            case GridState::ENDPOINT:
                fillGrid(i, j, 0xff, 0x8c, 0x00);
                break;
            case GridState::VISITED:
                fillGrid(i, j, 0x7c, 0xfc, 0x00);
                break;
            case GridState::TARGET:
                fillGrid(i, j, 0xff, 0x14, 0x93);
                break;
            default:
                break;
            }
        }
    }
}
