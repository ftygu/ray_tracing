#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <sstream>
#include <mutex>
#include <queue>
#include <cctype>
#include <iostream>

#include "camera.hpp"
#include "ppm_window.hpp"

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

// 获取按键事件的函数
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

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init 错误: " << SDL_GetError() << std::endl;
        return 1;
    }


    atexit(SDL_Quit);

    std::vector<std::string> args(argv, argv + argc);


    Camera camera(16.0 / 9.0, 600);
    camera.render();


    std::stringstream ss;
    camera.write_image(ss);


    PPMWindow window;
    if (!window.display_image(ss.str())) {
        std::cerr << "无法显示图像。" << std::endl;
        return 1;
    }

    // 主循环
    while (is_running && window.running()) {

        handle_events();


        char key = get_key_event();
        bool camera_moved = false;


        switch (toupper(key)) {
            case 'W':
                camera.translate(Direction(0, 0, 0.3));
                camera_moved = true;
                break;
            case 'S':
                camera.translate(Direction(0, 0, -0.3));
                camera_moved = true;
                break;
            case 'A':
                camera.translate(Direction(-0.3, 0, 0));
                camera_moved = true;
                break;
            case 'D':
                camera.translate(Direction(0.3, 0, 0));
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

            if (!window.display_image(ss.str())) {
                std::cerr << "无法更新图像。" << std::endl;
                is_running = false;
            }
        }


        SDL_Delay(10);
    }

    return 0;
}
