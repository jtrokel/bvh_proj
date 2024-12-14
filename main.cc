#include "rtcentral.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "tri.h"
#include "BVHNode.h"

#include <random>

void book_cover() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    BVH_Node bvh = BVH_Node(world, 0, world.objects.size());
    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = 500;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    //cam.render(world);
}

void simple() {
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_center2 = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_center3 = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_center4 = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_center5 = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50);
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -0.2),   0.5, material_center2));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -2.2),   0.5, material_center3));
    world.add(make_shared<sphere>(point3( 0.0,    1.0, -1.2),   0.5, material_center4));
    world.add(make_shared<sphere>(point3( 0.0,    1.0, -2.2),   0.5, material_center5));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 250;
    cam.max_depth         = 25;

    cam.vfov     = 60;
    cam.lookfrom = point3(-1,1,1);
    cam.lookat   = point3(0,0,-1);
    cam.vup      = vec3(0,1,0);

    //cam.render(world);
}

void quads() {
    hittable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    // world.add(make_shared<triangle>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<tri>(point3(-3,-2, 5), point3(-3,-2, 1), point3(-3, 2, 5), left_red));
    // world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    //cam.render(world);
}

void wowie(int argc, char* argv[]) {
    if (argc != 4) {
        std::clog << "Usage: " << argv[0] << " <no. of triangles> <'b' for bvh or 'n' for naive><max bvh tree depth>\n";
        return;
    }
    char* end;
    long val = std::strtol(argv[1], &end, 10);
    int bvh_tree_depth = std::atoi(argv[3]);
    std::clog << bvh_tree_depth << std::endl;
    bool good_args = (!end[0] && val >= 0 && bvh_tree_depth >= 0 && (argv[2][0] == 'b' || argv[2][0] == 'n'));
    if (!good_args) {
        std::clog << "Usage: " << argv[0] << " <no. of triangles> <'b' for bvh or 'n' for naive><max bvh tree depth>\n";
        return;
    }

    hittable_list world;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    // Triangles
    for (int i = 0; i < val; i++) {
        point3 v1 = point3(dis(gen) * 10 - 5, dis(gen) * 10 - 5, dis(gen) * 10 - 5);
        point3 v2 = v1 - point3(dis(gen) * 2 - 1, dis(gen) * 2 - 1, dis(gen) * 2 - 1);
        point3 v3 = v1 - point3(dis(gen) * 2 - 1, dis(gen) * 2 - 1, dis(gen) * 2 - 1);
        world.add(make_shared<tri>(v1, v2, v3, make_shared<lambertian>(color(dis(gen), dis(gen), dis(gen)))));
    }

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,10);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    BVH_Node bvh(world, 0, world.objects.size());
    bvh.subdivide(world, bvh_tree_depth);
    // bvh.print();

    cam.render(world, bvh, argv[2][0]);
}

void tri_test() {
    hittable_list world;

    // Materials
    auto red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto black = make_shared<lambertian>(color(0.0, 0.0, 0.0));

    auto t = tri(point3(0, 0, 0), point3(1, 0, 0), point3(0, 1, 0), red);
    auto triangle = make_shared<tri>(point3(0, 0, 0), point3(1, 0, 0), point3(0, 1, 0), red);
    world.add(triangle);
    world.add(make_shared<sphere>(t.cent(), 0.1, black));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1600;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,6);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    //cam.render(world);
}

int main(int argc, char* argv[]) {
    switch (4) {
        case 1: book_cover(); break;
        case 2: simple(); break;
        case 3: quads(); break;
        case 4: wowie(argc, argv); break;
        case 5: tri_test(); break;
    }
}