#pragma once

namespace   common    {
    struct timer    {
        float interval;
        float accumulated = 0.0f;
    
        bool tick(float deltaTime) {
            accumulated += deltaTime;
            if (accumulated >= interval) {
                accumulated -= interval;
                return true;
            }
            return false;
        }
    };
};

using Timer = common::timer;
