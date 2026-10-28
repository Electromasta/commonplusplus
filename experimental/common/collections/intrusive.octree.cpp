#include "intrusive.octree.h"
#include "common/logging.types.h"

Octree::Octree(glm::vec3 center, glm::vec3 halfsize, u32 depth)    {
    this->center = center;
    this->halfsize = halfsize;
    this->DEPTH = depth;
    this->nodes.resize(2);
    this->free = NIL;
}

u32 Octree::insert(glm::vec3 point, u64 data)    {
    u32 index = traverse(point, true);
    if (index == NIL) return NIL;
    nodes[index].data = data;
    return index;
}

bool Octree::remove(glm::vec3 point)    {
    u32 index = traverse(point);
    if (index == NIL) return false;
    nodes[index].data = 0;
    prune(nodes[index].parent);
    return true;
}

u64 Octree::get(glm::vec3 point)    {
    u32 index = traverse(point);
    if (index == NIL) return 0;
    return nodes[index].data;
}

u32 Octree::allocate(u32 parent)  {
    u32 index;
    if (free == NIL) {
        index = (u32)nodes.size();
        nodes.resize(nodes.size() + 8);
    }   else    {
        index = free;
        free = nodes[free].parent;
        for (u32 i = 0; i < 8; i++) nodes[index + i] = Node{};
    }
    for (u32 i = 0; i < 8; i++) nodes[index + i].parent = parent;
    return index;
}

// Shape must be deallocate(nodes[n].child) or it will corrupt memory as index is the key to a block of 8
void Octree::deallocate(u32 index)    {
    for (u32 i = 0; i < 8; i++)
        if (nodes[index + i].child != NIL) deallocate(nodes[index + i].child);
    u32 parent = nodes[index].parent;
    for (u32 i = 0; i < 8; i++) 
        nodes[index + i] = Node{};
    if (parent != NIL) nodes[parent].child = NIL;
    nodes[index].parent = free;
    free = index;
}

void Octree::clear()    {
    nodes.clear();
    nodes.resize(2);
    free = NIL;
}

bool Octree::isLeaf(u32 index)   {
    return nodes[index].child == NIL;
}

void Octree::split(u32 parent)  {
    if (!isLeaf(parent)) return;
    nodes[parent].child = allocate(parent);
}

void Octree::prune(u32 index)   {
    while (index != NIL && index != ROOT) {
        u32 children = nodes[index].child;
        if (children == NIL) return;
        for (u32 i = 0; i < 8; i++)
            if (nodes[children + i].data != 0 || nodes[children + i].child != NIL) return;
        deallocate(children);
        index = nodes[index].parent;
    }
}

u32 Octree::traverse(glm::vec3 point, bool create)   {
    if (glm::any(glm::lessThan(point, center - halfsize)) || glm::any(glm::greaterThanEqual(point, center + halfsize))) return NIL;
    u32 index = ROOT;
    glm::vec3 centroid = center;
    glm::vec3 half = halfsize;
    for (int i = 0; i < DEPTH; i++) {
        if (isLeaf(index))  {
            if (!create) return NIL;
            split(index);
        }
        Octant octant = seek(point, centroid);
        half *= 0.5f;
        centroid += glm::vec3(
            octant & Bit::RIGHT ? half.x : -half.x,
            octant & Bit::SOUTH ? -half.y : half.y,
            octant & Bit::BOTTOM ? -half.z : half.z
        );
        index = nodes[index].child + octant;
    }
    return index;
}

Octree::Octant Octree::seek(glm::vec3 point, glm::vec3 mid) {
    u32 o = 0;
    if (point.x >= mid.x) o |= Bit::RIGHT;
    if (point.y <  mid.y) o |= Bit::SOUTH;
    if (point.z <  mid.z) o |= Bit::BOTTOM;
    return (Octant)o;
}
