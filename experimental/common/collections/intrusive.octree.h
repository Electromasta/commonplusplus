#pragma once
#include "common/core.types.h"
#include "common/math.types.h"

class Octree    {
    public:
        static constexpr u32 NIL = 0, ROOT = 1;
        enum Bit { RIGHT = 1, SOUTH = 2, BOTTOM = 4 };
        enum Octant {
            TOP_NORTH_LEFT = 0,
            TOP_NORTH_RIGHT = 1,
            TOP_SOUTH_LEFT = 2,
            TOP_SOUTH_RIGHT = 3,
            BOTTOM_NORTH_LEFT = 4,
            BOTTOM_NORTH_RIGHT = 5,
            BOTTOM_SOUTH_LEFT = 6,
            BOTTOM_SOUTH_RIGHT = 7
        };
        struct Node { 
            u32 parent = NIL; 
            u32 child = NIL;
            u64 data; 
        };
        Octree(glm::vec3 center, glm::vec3 halfsize, u32 depth = 5);

        u32 insert(glm::vec3 point, u64 data);
        bool remove(glm::vec3 point);
        u64 get(glm::vec3 point);
        void clear();
    private:
        ArrayList<Node> nodes;
        glm::vec3 center;
        glm::vec3 halfsize;
        u32 free;
        u32 DEPTH;

        u32 allocate(u32 parent);
        void deallocate(u32 index);

        bool isLeaf(u32 index);
        void split(u32 parent);
        void prune(u32 index);
        u32  traverse(glm::vec3 point, bool create = false);
        Octant seek(glm::vec3 point, glm::vec3 mid);
};
