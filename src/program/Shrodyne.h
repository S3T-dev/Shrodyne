#pragma once

/**
 * @file Shrodyne.h
 * @brief High-level application shell for the Shrodyne simulation.
 */
class Shrodyne {
public:
    /** Construct application and perform lightweight setup. */
    Shrodyne();

    /** Run the application main loop. */
    void run();

private:
    /** Initialize subsystems (window, Vulkan, etc.). */
    void initialize();

    /** Advance simulation / update state. */
    void update();

    /** Issue draw calls / present frame. */
    void render();
};