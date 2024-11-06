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
    // 构造函数：初始化 SDL，创建窗口和渲染器
    PPMWindow(const std::string& title = "PPM Image (P3)") :
        window(nullptr), renderer(nullptr), texture(nullptr), is_running(true)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL 初始化失败: " << SDL_GetError() << "\n";
            throw std::runtime_error("SDL_Init failed");
        }

        // 创建窗口，初始大小为 640x480，可根据需要调整
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

        // 创建渲染器
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

    // 显示 PPM 图像（P3 格式）
    bool display_image(const std::string& ppm_data) {
        std::vector<unsigned char> pixels;
        int width, height;

        // 解析 PPM 数据
        if (!load_PPM(ppm_data, pixels, width, height)) {
            return false;
        }

        // 如果已有纹理，销毁它
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }

        // 创建新的纹理
        texture = SDL_CreateTexture(renderer,
                                    SDL_PIXELFORMAT_RGB24,
                                    SDL_TEXTUREACCESS_STATIC,
                                    width,
                                    height);
        if (!texture) {
            std::cerr << "创建纹理失败: " << SDL_GetError() << "\n";
            return false;
        }

        // 更新纹理像素数据
        if (SDL_UpdateTexture(texture, NULL, pixels.data(), width * 3) != 0) {
            std::cerr << "更新纹理失败: " << SDL_GetError() << "\n";
            return false;
        }

        // 调整窗口大小以匹配图像大小（可选）
        SDL_SetWindowSize(window, width, height);

        // 渲染纹理到窗口
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        return true;
    }

    // 检查窗口是否仍在运行
    bool running() const {
        return is_running;
    }

private:
    // 跳过注释行
    void skip_comments(std::istringstream& ss) {
        while (ss >> std::ws && ss.peek() == '#') {
            std::string comment;
            std::getline(ss, comment);
        }
    }

    // 读取下一个整数，忽略逗号和其他非数字字符
    bool read_next_int(std::istringstream& ss, int& value) {
        char ch;
        // 跳过任何非数字字符（如逗号和空白）
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

    // 解析 PPM 数据
    bool load_PPM(const std::string& ppm_data, std::vector<unsigned char>& pixels, int& width, int& height) {
        std::istringstream ss(ppm_data);
        std::string format;
        int max_val;

        // 读取格式标识符（应为 P3）
        ss >> format;
        if (format != "P3") {
            std::cerr << "仅支持 P3 ASCII 格式的 PPM 文件。\n";
            return false;
        }

        // 读取宽度和高度
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

        // 读取最大像素值
        skip_comments(ss);
        if (!(ss >> max_val)) {
            std::cerr << "无法读取最大像素值。\n";
            return false;
        }

        if (max_val <= 0) {
            std::cerr << "无效的最大像素值。\n";
            return false;
        }

        // 读取像素数据
        size_t pixel_count = width * height;
        pixels.resize(pixel_count * 3);
        int r, g, b;

        for (size_t i = 0; i < pixel_count; ++i) {
            // 读取红色分量
            if (!read_next_int(ss, r)) {
                std::cerr << "像素数据不完整或格式错误（红色分量）。\n";
                return false;
            }

            // 读取绿色分量
            if (!read_next_int(ss, g)) {
                std::cerr << "像素数据不完整或格式错误（绿色分量）。\n";
                return false;
            }

            // 读取蓝色分量
            if (!read_next_int(ss, b)) {
                std::cerr << "像素数据不完整或格式错误（蓝色分量）。\n";
                return false;
            }

            // 缩放像素值到 0-255
            unsigned char red = static_cast<unsigned char>((r * 255) / max_val);
            unsigned char green = static_cast<unsigned char>((g * 255) / max_val);
            unsigned char blue = static_cast<unsigned char>((b * 255) / max_val);

            pixels[i * 3]     = red;
            pixels[i * 3 + 1] = green;
            pixels[i * 3 + 2] = blue;
        }

        return true;
    }

    // 成员变量
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    bool is_running;

    // 按键事件队列
    std::queue<char> key_events;
    std::mutex event_mutex;
};

#endif // PPMWINDOW_H
