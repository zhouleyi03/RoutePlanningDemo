#pragma once

#include "Algo.h"

constexpr int BFS_frame_interval = 1;

class BFS : public Algo
{
public:
    BFS(int *frame_var, bool *end_flag);
    
    virtual void exec() override;
};
