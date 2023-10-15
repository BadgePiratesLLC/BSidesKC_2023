#pragma once

#include <functional>

struct MenuItem
{
    const char *label;
    MenuItem *submenu;
    std::function<void()> action; // This will hold our lambda or any callable
};
