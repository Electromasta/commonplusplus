#pragma once
#include <cstdint>

namespace common  {
    struct counter {
        std::uint32_t count;
        
        counter() : count(0)  {}

        std::uint32_t next()    {
            return count++;
        }  
    };
};

using Counter = common::counter;
