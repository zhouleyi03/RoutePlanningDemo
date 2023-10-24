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

Proc::Proc(int argc, char **argv)
{
    std::system("chcp 65001");

    std::unordered_map<std::string, std::function<void(const std::string &)>> argmap;
    argmap["--gridsize"] = [this](const std::string &operand)
    {
        m_grid_size = std::stoi(operand);
    };
    argmap["-algo"] = [this](const std::string &operand)
    {
        
    };
    argmap["-s"] = [this](const std::string &operand)
    {
        init(std::stoi(operand));
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

    if (!m_window)
    {
        std::cout << "fatal error: 没有给定窗口大小！\n";
        std::exit(-1);
    }
    if (!m_generator)
    {
        std::cout << "warning: 没有指定地图生成器！将使用默认生成器。\n";
        setGenerator<BlockGen>();
    }
}

void Proc::init(int side_length_pow)
{
    assert(side_length_pow > 5);
    m_side_length = 1 << side_length_pow;

    assert(!SDL_Init(SDL_INIT_VIDEO));

    m_window =
        SDL_CreateWindow("CSTB Pre demo v1.0",
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
        clearScreen();
        renderMap();
        updateScreen();

        while (SDL_PollEvent(&ev) != 0)
        {
            if (ev.type == SDL_QUIT)
                m_window_should_close = true;
        }
        SDL_Delay(16); // ~ 60 fps (16.67ms per frame)
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
                fillGrid(i, j);
                break;
            case GridState::STARTPOINT:
                fillGrid(i, j, 0xff, 0x45, 0x00);
                break;
            case GridState::ENDPOINT:
                fillGrid(i, j, 0x8a, 0x2b, 0xe2);
                break;
            case GridState::VISITED:
                fillGrid(i, j, 0, 0xff, 0);
                break;
            default:
                break;
            }
        }
    }
}
