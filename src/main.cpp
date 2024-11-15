#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <memory>
#include <tuple>
#include <vector>
#include <string>
#include <sstream>
#include <mutex>
#include <queue>
#include <cctype>
#include <iostream>

#include "bvh.hpp"
#include "camera.hpp"
#include "hittable.hpp"
#include "ppm_window.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "random_generator.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "obj_loader.hpp"
#include "basic_types.hpp"
#include "photo_map.hpp"

// 全局变量用于事件处理
std::mutex event_mutex;
std::queue<char> key_events;
bool is_running = true;

void handle_events();

char get_key_event();

bool spheres_overlap(const std::shared_ptr<Sphere>& sphere1, const std::shared_ptr<Sphere>& sphere2);

std::vector<std::shared_ptr<Hittable>> generate_random_scene();

std::tuple<std::vector<std::shared_ptr<Hittable>>, std::vector<std::shared_ptr<Hittable>>> generate_test_scene();

int main(int argc, char* argv[]) {

    //handle command line arguments
    std::vector<std::string> args(argv, argv + argc);

    //initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);

    //generate test scene
    auto [objects, lights] = generate_test_scene();
    auto world = std::make_shared<HittableList>(objects);

    //set up camera
    Camera camera(16.0 / 9.0, 800, 30, 5);
    camera.set_world(world, lights);
    camera.set_algorithm(Algorithm::PathTracingPDF);

    //generate the first image
    camera.render();
    std::stringstream ss;
    camera.write_image(ss);

    //display the image
    PPMWindow window;
    window.display_image(ss.str());

    //main loop
    while (is_running && window.running()) {

        handle_events();
        char key = get_key_event();
        bool camera_moved = false;

        switch (toupper(key)) {
            case 'W':
                camera.translate(0.3, 0, 0);
                camera_moved = true;
                break;
            case 'S':
                camera.translate(-0.3, 0, 0);
                camera_moved = true;
                break;
            case 'A':
                camera.translate(0, -0.3, 0);
                camera_moved = true;
                break;
            case 'D':
                camera.translate(0, 0.3, 0);
                camera_moved = true;
                break;
            case 'Q':
                camera.translate(0, 0, 0.3);
                camera_moved = true;
                break;
            case 'E':
                camera.translate(0, 0, -0.3);
                camera_moved = true;
                break;
            case 'I':
                camera.rotate(0, 0.1, 0);
                camera_moved = true;
                break;
            case 'K':
                camera.rotate(0, -0.1, 0);
                camera_moved = true;
                break;
            case 'J':
                camera.rotate(0.1, 0, 0);
                camera_moved = true;
                break;
            case 'L':
                camera.rotate(-0.1, 0, 0);
                camera_moved = true;
                break;
            case 'U':
                camera.rotate(0, 0, -0.1);
                camera_moved = true;
                break;
            case 'O':
                camera.rotate(0, 0, 0.1);
                camera_moved = true;
                break;
            default:
                break;
        }

        if (camera_moved) {
            camera.render_parallel();
            ss.str("");
            ss.clear();
            camera.write_image(ss);
            window.display_image(ss.str());
        }
        SDL_Delay(10);
    }
    return 0;
}

void handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            is_running = false;
        }
        else if (e.type == SDL_KEYDOWN) {
            char key_char = 0;
            switch (e.key.keysym.sym) {
                case SDLK_w:
                    key_char = 'w';
                    break;
                case SDLK_a:
                    key_char = 'a';
                    break;
                case SDLK_s:
                    key_char = 's';
                    break;
                case SDLK_d:
                    key_char = 'd';
                    break;
                case SDLK_q:
                    key_char = 'q';
                    break;
                case SDLK_e:
                    key_char = 'e';
                    break;
                case SDLK_u:
                    key_char = 'u';
                    break;
                case SDLK_i:
                    key_char = 'i';
                    break;
                case SDLK_o:
                    key_char = 'o';
                    break;
                case SDLK_j:
                    key_char = 'j';
                    break;
                case SDLK_k:
                    key_char = 'k';
                    break;
                case SDLK_l:
                    key_char = 'l';
                    break;
                default:
                    break;
            }
            if (key_char != 0) {
                std::lock_guard<std::mutex> lock(event_mutex);
                key_events.push(key_char);
                // 记录按键到日志
                std::clog << static_cast<char>(toupper(key_char)) << " 键被按下\n";
            }
        }
    }
}

char get_key_event() {
    std::lock_guard<std::mutex> lock(event_mutex);
    if (!key_events.empty()) {
        char key = key_events.front();
        key_events.pop();
        return key;
    }
    return 0;
}

bool spheres_overlap(const std::shared_ptr<Sphere>& sphere1, const std::shared_ptr<Sphere>& sphere2) {
    double distance_squared = (sphere1->get_center().x() - sphere2->get_center().x()) * (sphere1->get_center().x() - sphere2->get_center().x()) +
                              (sphere1->get_center().y() - sphere2->get_center().y()) * (sphere1->get_center().y() - sphere2->get_center().y()) +
                              (sphere1->get_center().z() - sphere2->get_center().z()) * (sphere1->get_center().z() - sphere2->get_center().z());
    double radius_sum = sphere1->get_radius() + sphere2->get_radius();
    return distance_squared < (radius_sum * radius_sum);
}

std::vector<std::shared_ptr<Hittable>> generate_random_scene() {
    RandomGenerator random_generator;
    std::vector<std::shared_ptr<Hittable>> objects;
    objects.reserve(10001); // 100随机球 + 地板

    const int max_attempts = 1000; // 每个球的最大尝试次数
    const int desired_sphere_count = 1000;
    int sphere_count = 0;

    while (sphere_count < desired_sphere_count) {
        bool overlap = false;
        int attempts = 0;

        double random_r = random_generator.get_random_double(0, 255);
        double random_g = random_generator.get_random_double(0, 255);
        double random_b = random_generator.get_random_double(0, 255);

        auto random_double = random_generator.get_random_double(0, 1);

        std::shared_ptr<Material> random_material;

        switch (static_cast<int>(random_double * 3)) {
            case 0:
                random_material = std::make_shared<Lambertian>(Color(random_r, random_g, random_b));
                break;
            case 1:
                random_material = std::make_shared<Metal>(Color(random_r, random_g, random_b), random_generator.get_random_double(0, 0.3));
                break;
            case 2:
                random_material = std::make_shared<Dielectric>(random_generator.get_random_double(1.1, 2.5));
                break;
            default:
                break;
        }

        double random_x, random_y, random_z, random_radius;
        std::shared_ptr<Sphere> random_sphere;

        do {
            if (attempts >= max_attempts) {
                break;
            }

            random_x = random_generator.get_random_double(-10, 10);
            random_radius = random_generator.get_random_double(0.1, 1);
            double y = random_radius;
            random_z = random_generator.get_random_double(-10, 10);

            random_sphere = std::make_shared<Sphere>(Point(random_x, y, random_z), random_radius, random_material);

            overlap = false;
            for (const auto& obj : objects) {
                auto existing_sphere = std::dynamic_pointer_cast<Sphere>(obj);
                if (existing_sphere) {
                    if (spheres_overlap(random_sphere, existing_sphere)) {
                        overlap = true;
                        break;
                    }
                }
            }

            attempts++;
        } while (overlap);

        if (!overlap) {
            objects.push_back(random_sphere);
            sphere_count++;
        }
    }

    // 添加地板
    auto floor_material = std::make_shared<Lambertian>(Color(125, 125, 125));
    auto floor = std::make_shared<Sphere>(Point(0, -1000, 0), 1000, floor_material);
    objects.push_back(floor);
    return objects;
}

std::tuple<std::vector<std::shared_ptr<Hittable>>, std::vector<std::shared_ptr<Hittable>>> generate_test_scene() {
    auto floor_material = std::make_shared<Lambertian>(Color(125, 125, 125));
    auto light_material = std::make_shared<Lambertian>(Color(255, 255, 255));
    light_material->set_light_color(Color(10000, 10000, 10000));

    auto floor = std::make_shared<Sphere>(Point(0, -1000, 0), 1000, floor_material);
    auto sphere1 = std::make_shared<Sphere>(Point(1, 1, -2), 1, std::make_shared<Lambertian>(Color(200, 0, 0)));
    auto sphere2 = std::make_shared<Sphere>(Point(-1, 1, -2), 1, std::make_shared<Lambertian>(Color(0, 200, 0)));
    auto sphere3 = std::make_shared<Sphere>(Point(0, 3, -2), 0.1, light_material);
    auto point1 = Point(1.75, 2.25, -3);
    auto point2 = Point(1.75, 2, -3);
    auto point3 = Point(2, 2, -3);
    auto triangle = std::make_shared<Triangle>(point1, point2, point3, light_material);

    std::vector<std::shared_ptr<Hittable>> objects;
    objects.push_back(floor);
    objects.push_back(sphere1);
    objects.push_back(sphere2);
    objects.push_back(sphere3);
    objects.push_back(triangle);

    std::vector<std::shared_ptr<Hittable>> lights;
    lights.push_back(sphere3);
    lights.push_back(triangle);

    return {objects, lights};
}