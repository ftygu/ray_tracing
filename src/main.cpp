#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <memory>
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
#include "point.hpp"
#include "ppm_window.hpp"
#include "hittable_list.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "triangle.hpp"
#include "obj_loader.hpp"

// 全局变量用于事件处理
std::mutex event_mutex;
std::queue<char> key_events;
bool is_running = true;

// 处理 SDL 事件的函数
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

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv, argv + argc);

    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);

    std::vector<std::shared_ptr<Hittable>> objects;

    auto floor_material = std::make_shared<Lambertian>(Color(50, 125, 60));
    auto earth = std::make_shared<Sphere>(Point(0, -100000, 0),99999, floor_material);
    objects.push_back(earth);

    auto red_material = std::make_shared<Lambertian>(Color(255, 0, 0));
    auto red_sphere = std::make_shared<Sphere>(Point(0, 0, -2), 1, red_material);
    objects.push_back(red_sphere);

    auto metal_material = std::make_shared<Metal>(Color(200, 200, 200), 0.1);

    objl::Loader loader;
    loader.LoadFile("../models/bunny/bunny.obj");
    auto mesh = loader.LoadedMeshes[0];
    for (size_t i = 0; i < mesh.Indices.size(); i += 3) {
        auto v0 = Point(mesh.Vertices[mesh.Indices[i]].Position.X, mesh.Vertices[mesh.Indices[i]].Position.Y, mesh.Vertices[mesh.Indices[i]].Position.Z);
        auto v1 = Point(mesh.Vertices[mesh.Indices[i + 1]].Position.X, mesh.Vertices[mesh.Indices[i + 1]].Position.Y, mesh.Vertices[mesh.Indices[i + 1]].Position.Z);
        auto v2 = Point(mesh.Vertices[mesh.Indices[i + 2]].Position.X, mesh.Vertices[mesh.Indices[i + 2]].Position.Y, mesh.Vertices[mesh.Indices[i + 2]].Position.Z);
        auto triangle = std::make_shared<Triangle>(v0, v1, v2, metal_material);
        objects.push_back(triangle);
    }

    auto world = std::make_shared<BVH>(objects, 0, objects.size());


    Camera camera(16.0 / 9.0, 360);
    camera.set_world(world);
    camera.render();

    std::stringstream ss;
    camera.write_image(ss);

    PPMWindow window;
    window.display_image(ss.str());

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
            camera.render();
            ss.str("");
            ss.clear();
            camera.write_image(ss);
            window.display_image(ss.str());
        }
        SDL_Delay(10);
    }

    return 0;
}
