#include "TextDB.h"

#include <filesystem>
#include <iostream>

#include "rapidjson/document.h"

#include "Engine.h"
#include "TextureDB.h"

auto TextDB::LoadFont(const std::string &font_name, const int font_size) -> TTF_Font * {
    if (const auto font_it = loaded_fonts.find(font_name); font_it != loaded_fonts.end()) {
        if (const auto font_size_it = font_it->second.find(font_size); font_size_it != font_it->second.end()) {
            return font_size_it->second;
        }
    }
    auto font_file = "core/fonts/" + font_name + ".ttf";
    if (!std::filesystem::exists(font_file)) {
        font_file = "resources/fonts/" + font_name + ".ttf";
        if (!std::filesystem::exists(font_file)) {
            std::cout << "error: font " << font_name << " missing";
            exit(0);
        }
    }
    auto font = TTF_OpenFont(font_file.c_str(), font_size);
    loaded_fonts[font_name][font_size] = font;
    return font;
}

auto TextDB::DrawText(const char *text, float x, float y, const char *font_name, float font_size, float r, float g, float b, float a) -> void {
    if (text == nullptr || font_name == nullptr) {
        return;
    }
    draw_calls.push_back({
        text,
        static_cast<int>(x),
        static_cast<int>(y),
        font_name,
        static_cast<int>(font_size),
        static_cast<int>(r),
        static_cast<int>(g),
        static_cast<int>(b),
        static_cast<int>(a),
    });
}

auto TextDB::GenerateText(TTF_Font *font, const std::string &text, int r, int g, int b, int a) -> SDL_Texture * {
    const auto color = SDL_Color{
        static_cast<unsigned char>(r),
        static_cast<unsigned char>(g),
        static_cast<unsigned char>(b),
        static_cast<unsigned char>(a),
    };
    /*if (const auto font_it = generated_text.find(font); font_it != generated_text.end()) {
        if (const auto color_it = font_it->second.find(color); color_it != font_it->second.end()) {
            if (const auto texture_it = color_it->second.find(text); texture_it != color_it->second.end()) {
                return texture_it->second;
            }
        }
    }*/
    const auto surface = TTF_RenderText_Solid(font, text.c_str(), color);
    const auto texture = SDL_CreateTextureFromSurface(Engine::renderer, surface);
    // generated_text[font][color][text] = texture;
    return texture;
}

auto TextDB::RenderText() -> void {
    for (const auto &draw_call : draw_calls) {
        const auto font = LoadFont(draw_call.font_name, draw_call.font_size);
        auto texture = GenerateText(font, draw_call.text, draw_call.r, draw_call.g, draw_call.b, draw_call.a);
        auto destRect = SDL_Rect();
        destRect.x = draw_call.x;
        destRect.y = draw_call.y;
        TTF_SizeText(font, draw_call.text.c_str(), &destRect.w, &destRect.h);
        SDL_RenderCopy(Engine::renderer, texture, nullptr, &destRect);
    }
    draw_calls.clear();
}