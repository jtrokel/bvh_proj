#ifndef BVHNODE_H
#define BVHNODE_H
#include "aabb.h"
#include "hittable_list.h"
#include "hittable.h"
#include "ray.h"

class BVH_Node {
  public:
    aabb box;
    BVH_Node* left = nullptr; // Left child
    BVH_Node* right = nullptr;// right child
    bool isLeaf = false;
    int first_hittable, last_hittable; // Indices of the first and last triangles in the node

    // Constructs BVH Node with proper bounds
    BVH_Node(hittable_list& world, int start, int total) {
        first_hittable = start;
        last_hittable = start + total - 1;
        for (int i = start; i < start + total; i++) {
            // Expand the box to include the bounds of the objects
            for (int j = 0; j < 3; j++) {
                box.bounds[j].min = std::min(box.bounds[j].min, world.objects[i]->mins()[j]);
                box.bounds[j].max = std::max(box.bounds[j].max, world.objects[i]->maxs()[j]);
            }
        }
    }

    // Destructor
    ~BVH_Node() {
        if (left != nullptr) {
            delete left;
        }
        if (right != nullptr) {
            delete right;
        }
    }

    // Divide the BVH node
    void subdivide(hittable_list& world) {
        std::vector<int> axes = box.max_axis();
        int i, totalLeft, totalRight = 0;

        // Try the longest axis split first. If it wouldn't split any primitives,
        // try the next longest axis.
        for (int axis : axes) {
            interval max_interval = box.bounds[axis];
            double center = max_interval.min + max_interval.size()/2;

            i = first_hittable;
            int j = last_hittable;
            while (i <= j) {
                if (world.objects[i]->cent()[axis] < center) {
                    i++;
                } else {
                    std::swap(world.objects[i], world.objects[j--]);
                }
            }

            totalLeft =  i - first_hittable;
            totalRight = last_hittable - j;
            if (totalLeft != 0 && totalRight != 0) {
                break;
            }
        }

        // If we couldn't split the primitives
        if (totalLeft == 0 || totalRight == 0) {
            isLeaf = true;
            return;
        }
        
        left = new BVH_Node(world, first_hittable, totalLeft);
        right = new BVH_Node(world, i, totalRight);

        if (totalLeft > 2) {
            left->subdivide(world);
        } else {
            left->isLeaf = true;
        }
        if (totalRight > 2) {
            right->subdivide(world);
        } else {
            right->isLeaf = true;
        }
    }

    // Initialize stuff for BVH traversal
    bool traverse(const ray& r, interval& ray_t, hit_record& rec, const hittable_list& world) {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        if (box.hit(r, ray_t, rec)) {
            return std::get<0>(traverse_helper(r, ray_t, rec, world, closest_so_far));
        }
        return false;
    }

    // Traverse the BVH for ray hits
    std::tuple<bool, double> traverse_helper(const ray& r, interval& ray_t, hit_record& rec,
                                            const hittable_list& world, double closest_so_far) {
        bool hit_anything = false;
        hit_record temp_rec;


        if (isLeaf == true) {
            std::clog << "checking " << last_hittable - first_hittable + 1 << " objects" << std::endl;
            for (int i = first_hittable; i < last_hittable + 1; i++) {
                if (world.objects[i]->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return std::make_tuple(hit_anything, closest_so_far);
        }

        auto left_result = left->traverse_helper(r, ray_t, rec, world, closest_so_far);
        hit_anything = std::get<0>(left_result);
        closest_so_far = std::get<1>(left_result);

        auto right_result = right->traverse_helper(r, ray_t, rec, world, closest_so_far);
        hit_anything |= std::get<0>(right_result);
        closest_so_far = std::get<1>(right_result);

        return std::make_tuple(hit_anything, closest_so_far);
    } 

    // Print the BVH tree
    void print() {
        if (left != nullptr) {
            left->print();
        }
        if (right != nullptr) {
            right->print();
        }
        if (isLeaf) {
            std::clog << "Leaf: " << first_hittable << " " << last_hittable << std::endl;
        }
    }
};

#endif
