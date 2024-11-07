#ifndef PPMWINDOW_H
#define PPMWINDOW_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <queue>
#include <mutex>

class PPMWindow {
public:
    PPMWindow(const std::string& title = "PPM Image (P3)") :
        window(nullptr), renderer(nullptr), texture(nullptr), is_running(true)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL 初始化失败: " << SDL_GetError() << "\n";
            throw std::runtime_error("SDL_Init failed");
        }

        window = SDL_CreateWindow(title.c_str(),
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  640, 480,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (!window) {
            std::cerr << "创建窗口失败: " << SDL_GetError() << "\n";
            SDL_Quit();
            throw std::runtime_error("SDL_CreateWindow failed");
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            std::cerr << "创建渲染器失败: " << SDL_GetError() << "\n";
            SDL_DestroyWindow(window);
            SDL_Quit();
            throw std::runtime_error("SDL_CreateRenderer failed");
        }
    }

    // 析构函数：清理资源
    ~PPMWindow() {
        if (texture) SDL_DestroyTexture(texture);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }

    bool display_image(const std::string& ppm_data) {
        std::vector<unsigned char> pixels;
        int width, height;

        if (!load_PPM(ppm_data, pixels, width, height)) {
            return false;
        }

        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        texture = SDL_CreateTexture(renderer,
                                    SDL_PIXELFORMAT_RGB24,
                                    SDL_TEXTUREACCESS_STATIC,
                                    width,
                                    height);
        if (!texture) {
            std::cerr << "创建纹理失败: " << SDL_GetError() << "\n";
            return false;
        }

        if (SDL_UpdateTexture(texture, NULL, pixels.data(), width * 3) != 0) {
            std::cerr << "更新纹理失败: " << SDL_GetError() << "\n";
            return false;
        }

        SDL_SetWindowSize(window, width, height);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        return true;
    }

    bool running() const {
        return is_running;
    }

private:
    void skip_comments(std::istringstream& ss) {
        while (ss >> std::ws && ss.peek() == '#') {
            std::string comment;
            std::getline(ss, comment);
        }
    }

    bool read_next_int(std::istringstream& ss, int& value) {
        char ch;
        while (ss >> ch) {
            if (isdigit(ch)) {
                ss.putback(ch);
                break;
            }
        }
        if (!(ss >> value)) {
            return false;
        }
        return true;
    }

    bool load_PPM(const std::string& ppm_data, std::vector<unsigned char>& pixels, int& width, int& height) {
        std::istringstream ss(ppm_data);
        std::string format;
        int max_val;

        ss >> format;
        if (format != "P3") {
            std::cerr << "仅支持 P3 ASCII 格式的 PPM 文件。\n";
            return false;
        }

        skip_comments(ss);
        if (!(ss >> width)) {
            std::cerr << "无法读取图像宽度。\n";
            return false;
        }

        skip_comments(ss);
        if (!(ss >> height)) {
            std::cerr << "无法读取图像高度。\n";
            return false;
        }

        skip_comments(ss);
        if (!(ss >> max_val)) {
            std::cerr << "无法读取最大像素值。\n";
            return false;
        }

        if (max_val <= 0) {
            std::cerr << "无效的最大像素值。\n";
            return false;
        }

        size_t pixel_count = width * height;
        pixels.resize(pixel_count * 3);
        int r, g, b;

        for (size_t i = 0; i < pixel_count; ++i) {
            if (!read_next_int(ss, r)) {
                std::cerr << "像素数据不完整或格式错误（红色分量）。\n";
                return false;
            }

            if (!read_next_int(ss, g)) {
                std::cerr << "像素数据不完整或格式错误（绿色分量）。\n";
                return false;
            }

            if (!read_next_int(ss, b)) {
                std::cerr << "像素数据不完整或格式错误（蓝色分量）。\n";
                return false;
            }

            unsigned char red = static_cast<unsigned char>((r * 255) / max_val);
            unsigned char green = static_cast<unsigned char>((g * 255) / max_val);
            unsigned char blue = static_cast<unsigned char>((b * 255) / max_val);

            pixels[i * 3]     = red;
            pixels[i * 3 + 1] = green;
            pixels[i * 3 + 2] = blue;
        }

        return true;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    bool is_running;

    std::queue<char> key_events;
    std::mutex event_mutex;
};

#endif
