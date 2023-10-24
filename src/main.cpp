/**
 *  Copyright (C) 2023 Leyi Zhou
 *  Sample code for my CSTB presentation
 */

#include "Proc.h"

#undef main

int main(int argc, char *argv[])
{
    Proc proc(argc, argv);

    proc.loop();
    proc.shutdown();

    return 0;
}
