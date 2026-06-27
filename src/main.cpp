/**
 * @file main.cpp
 * @brief Small bootstrap that constructs the `Shrodyne` application
 * and runs the main loop.
 */

#include "program/Shrodyne.h"

int main(int argc, char** argv) {
    Shrodyne program;
    program.run();
    return 0;
}