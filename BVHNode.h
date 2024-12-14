#ifndef BVHNODE_H
#define BVHNODE_H
#include "aabb.h"
#include "hittable_list.h"
#include "hittable.h"

class BVH_Node {
  public:
    aabb box;
    BVH_Node* left; // Left child
    BVH_Node* right;// right child
    int first_hittable, last_hittable; // Indices of the first and last triangles in the node

   // Constructs BVH Node with proper bounds
    BVH_Node(hittable_list& world, int start, int total) {
        first_hittable = start;
        last_hittable = start + total - 1;
        for (int i = start; i < total; i++) {
            // Expand the box to include the bounds of the objects
            for (int j = 0; j < 3; j++) {
                box.bounds[j].min = std::min(box.bounds[j].min, world.objects[i]->mins()[j]);
                // std::clog << "min: " << world.objects[i]->mins()[j] << std::endl;
                box.bounds[j].max = std::max(box.bounds[j].max, world.objects[i]->maxs()[j]);
            }
        }
    }

    void subdivide(hittable_list world) {
        int axis = box.max_axis();
        interval max_interval = box.bounds[axis];
        double center = max_interval.min + max_interval.size()/2;
        std::clog << "first hittable: " << first_hittable << " last hittable: " << last_hittable << std::endl;
        int i = first_hittable;
        int j = last_hittable;
        while (i <= j) {
            if (world.objects[i]->cent()[axis] < center) {
                i++;
            } else {
                std::swap(world.objects[i], world.objects[j--]);
            }
        }
        int totalLeft =  i - first_hittable;
        int totalRight = last_hittable - j;
        BVH_Node leftNode(world, first_hittable, totalLeft);
        left = &leftNode;
        BVH_Node rightNode(world, i+totalLeft, totalRight);
        right = &rightNode;
        if (totalLeft > 2) leftNode.subdivide(world);
        if (totalRight > 2) rightNode.subdivide(world);
    }

    void traverse() {
        std::clog << "First: " << first_hittable << " Last: " << last_hittable << std::endl;
        if (left != nullptr) {
            left->traverse();
        }
        if (right != nullptr) {
            right->traverse();
        }
    }
};

#endif
