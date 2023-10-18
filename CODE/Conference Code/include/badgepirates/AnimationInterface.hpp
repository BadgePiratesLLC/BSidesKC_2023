// AnimationInterface.hpp
#pragma once

class AnimationInterface
{
public:
    virtual void setup() = 0;
    virtual void loop(unsigned long currentMillis) = 0;
};