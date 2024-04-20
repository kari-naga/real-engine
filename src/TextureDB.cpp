#include "TextureDB.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>

#include "rapidjson/document.h"
#include "SDL_image.h"
#include "SDL.h"

#include "Engine.h"
#include "Physics.h"

auto TextureDB::LoadTexture(const std::string &texture_name) -> SDL_Texture * {
    auto texture_name_lower = texture_name;
    std::transform(texture_name_lower.begin(), texture_name_lower.end(), texture_name_lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    if (const auto texture_it = loaded_textures.find(texture_name_lower); texture_it != loaded_textures.end()) {
        return texture_it->second;
    }
    auto texture_file = "core/images/" + texture_name_lower + ".png";
    if (!std::filesystem::exists(texture_file)) {
        texture_file = "resources/images/" + texture_name_lower + ".png";
        if (!std::filesystem::exists(texture_file)) {
            std::cout << "error: missing image " << texture_name_lower;
            exit(0);
        }
    }
    auto texture = IMG_LoadTexture(Engine::renderer, texture_file.c_str());
    loaded_textures.insert({texture_name_lower, texture});
    return texture;
}

auto TextureDB::DrawUI(const char *image_name, float x, float y) -> void {
    if (image_name == nullptr) {
        return;
    }
    auto draw_call = UIDrawCall{};
    draw_call.image_name = image_name;
    draw_call.x = static_cast<int>(x);
    draw_call.y = static_cast<int>(y);
    ui_draw_calls.push_back(draw_call);
}

auto TextureDB::DrawUIEx(const char *image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, float sorting_order) -> void {
    if (image_name == nullptr) {
        return;
    }
    auto draw_call = UIDrawCall{};
    draw_call.image_name = image_name;
    draw_call.x = static_cast<int>(x);
    draw_call.y = static_cast<int>(y);
    draw_call.rotation_degrees = static_cast<int>(rotation_degrees);
    draw_call.scale_x = scale_x;
    draw_call.scale_y = scale_y;
    draw_call.pivot_x = pivot_x;
    draw_call.pivot_y = pivot_y;
    draw_call.r = static_cast<int>(r);
    draw_call.g = static_cast<int>(g);
    draw_call.b = static_cast<int>(b);
    draw_call.a = static_cast<int>(a);
    draw_call.sorting_order = static_cast<int>(sorting_order);
    ui_draw_calls.push_back(draw_call);
}

auto TextureDB::DrawImage(const char *image_name, float x, float y) -> void {
    if (image_name == nullptr) {
        return;
    }
    auto draw_call = ImageDrawCall{};
    draw_call.image_name = image_name;
    draw_call.x = x;
    draw_call.y = y;
    image_draw_calls.push_back(draw_call);
}

auto TextureDB::DrawImageEx(const char *image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, float sorting_order) -> void {
    if (image_name == nullptr) {
        return;
    }
    auto draw_call = ImageDrawCall{};
    draw_call.image_name = image_name;
    draw_call.x = x;
    draw_call.y = y;
    draw_call.rotation_degrees = static_cast<int>(rotation_degrees);
    draw_call.scale_x = scale_x;
    draw_call.scale_y = scale_y;
    draw_call.pivot_x = pivot_x;
    draw_call.pivot_y = pivot_y;
    draw_call.r = static_cast<int>(r);
    draw_call.g = static_cast<int>(g);
    draw_call.b = static_cast<int>(b);
    draw_call.a = static_cast<int>(a);
    draw_call.sorting_order = static_cast<int>(sorting_order);
    image_draw_calls.push_back(draw_call);
}

auto TextureDB::DrawTile(const char *tileset_name, float tile_x, float tile_y, float tile_w, float tile_h, float x, float y) -> void {
    if (tileset_name == nullptr) {
        return;
    }
    auto draw_call = TileDrawCall{};
    draw_call.tileset_name = tileset_name;
    draw_call.tile_x = static_cast<int>(tile_x);
    draw_call.tile_y = static_cast<int>(tile_y);
    draw_call.tile_w = static_cast<int>(tile_w);
    draw_call.tile_h = static_cast<int>(tile_h);
    draw_call.x = x;
    draw_call.y = y;
    draw_call.w = draw_call.tile_w;
    draw_call.h = draw_call.tile_h;
    tile_draw_calls.push_back(draw_call);
}

auto TextureDB::DrawTileEx(const char *tileset_name, float tile_x, float tile_y, float tile_w, float tile_h, float x, float y, float w, float h, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, float sorting_order) -> void {
    if (tileset_name == nullptr) {
        return;
    }
    auto draw_call = TileDrawCall{};
    draw_call.tileset_name = tileset_name;
    draw_call.tile_x = static_cast<int>(tile_x);
    draw_call.tile_y = static_cast<int>(tile_y);
    draw_call.tile_w = static_cast<int>(tile_w);
    draw_call.tile_h = static_cast<int>(tile_h);
    draw_call.x = x;
    draw_call.y = y;
    draw_call.w = static_cast<int>(w);
    draw_call.h = static_cast<int>(h);
    draw_call.rotation_degrees = static_cast<int>(rotation_degrees);
    draw_call.scale_x = scale_x;
    draw_call.scale_y = scale_y;
    draw_call.pivot_x = pivot_x;
    draw_call.pivot_y = pivot_y;
    draw_call.r = static_cast<int>(r);
    draw_call.g = static_cast<int>(g);
    draw_call.b = static_cast<int>(b);
    draw_call.a = static_cast<int>(a);
    draw_call.sorting_order = static_cast<int>(sorting_order);
    tile_draw_calls.push_back(draw_call);
}

auto TextureDB::DrawPixel(float x, float y, float r, float g, float b, float a) -> void {
    auto draw_call = PixelDrawCall{};
    draw_call.x = static_cast<int>(x);
    draw_call.y = static_cast<int>(y);
    draw_call.r = static_cast<int>(r);
    draw_call.g = static_cast<int>(g);
    draw_call.b = static_cast<int>(b);
    draw_call.a = static_cast<int>(a);
    pixel_draw_calls.push_back(draw_call);
}

auto TextureDB::RenderScene() -> void {
    std::stable_sort(image_draw_calls.begin(), image_draw_calls.end());
    const auto zoom_factor = Engine::config.zoom_factor;
    SDL_RenderSetScale(Engine::renderer, zoom_factor, zoom_factor);
    for (const auto &draw_call : image_draw_calls) {
        const auto final_rendering_position = glm::vec2(draw_call.x, draw_call.y) - Engine::camera_position;
        const auto texture = LoadTexture(draw_call.image_name);
        auto dest_rect = SDL_Rect();
        SDL_QueryTexture(texture, nullptr, nullptr, &dest_rect.w, &dest_rect.h);
        auto flip_mode = static_cast<int>(SDL_FLIP_NONE);
        if (draw_call.scale_x < 0) {
            flip_mode |= SDL_FLIP_HORIZONTAL;
        }
        if (draw_call.scale_y < 0) {
            flip_mode |= SDL_FLIP_VERTICAL;
        }
        const auto x_scale = std::abs(draw_call.scale_x);
        const auto y_scale = std::abs(draw_call.scale_y);
        dest_rect.w = static_cast<int>(dest_rect.w * x_scale);
        dest_rect.h = static_cast<int>(dest_rect.h * y_scale);
        const auto pivot_point = SDL_Point{
            static_cast<int>(draw_call.pivot_x * dest_rect.w),
            static_cast<int>(draw_call.pivot_y * dest_rect.h),
        };
        const auto cam_dimensions = glm::ivec2(Engine::config.window_width, Engine::config.window_height);
        dest_rect.x = static_cast<int>(final_rendering_position.x * Engine::config.pixels_per_meter + cam_dimensions.x * 0.5f / zoom_factor - pivot_point.x);
        dest_rect.y = static_cast<int>(final_rendering_position.y * Engine::config.pixels_per_meter + cam_dimensions.y * 0.5f / zoom_factor - pivot_point.y);
        if (dest_rect.x > cam_dimensions.x / zoom_factor + 50 || dest_rect.y > cam_dimensions.y / zoom_factor + 50 || dest_rect.x + dest_rect.w < -50 || dest_rect.y + dest_rect.h < -50) {
            continue;
        }
        SDL_SetTextureColorMod(texture, draw_call.r, draw_call.g, draw_call.b);
        SDL_SetTextureAlphaMod(texture, draw_call.a);
        SDL_RenderCopyEx(Engine::renderer, texture, nullptr, &dest_rect, draw_call.rotation_degrees, &pivot_point, static_cast<SDL_RendererFlip>(flip_mode));
        SDL_SetTextureColorMod(texture, 255, 255, 255);
        SDL_SetTextureAlphaMod(texture, 255);
    }
    image_draw_calls.clear();
}

auto TextureDB::RenderTiles() -> void {
    std::stable_sort(tile_draw_calls.begin(), tile_draw_calls.end());
    const auto zoom_factor = Engine::config.zoom_factor;
    SDL_RenderSetScale(Engine::renderer, zoom_factor, zoom_factor);
    for (const auto &draw_call : tile_draw_calls) {
        const auto final_rendering_position = glm::vec2(draw_call.x, draw_call.y) - Engine::camera_position;
        const auto texture = LoadTexture(draw_call.tileset_name);
        auto dest_rect = SDL_Rect();
        dest_rect.w = draw_call.w;
        dest_rect.h = draw_call.h;
        auto flip_mode = static_cast<int>(SDL_FLIP_NONE);
        if (draw_call.scale_x < 0) {
            flip_mode |= SDL_FLIP_HORIZONTAL;
        }
        if (draw_call.scale_y < 0) {
            flip_mode |= SDL_FLIP_VERTICAL;
        }
        const auto x_scale = std::abs(draw_call.scale_x);
        const auto y_scale = std::abs(draw_call.scale_y);
        dest_rect.w = static_cast<int>(dest_rect.w * x_scale);
        dest_rect.h = static_cast<int>(dest_rect.h * y_scale);
        const auto pivot_point = SDL_Point{
            static_cast<int>(draw_call.pivot_x * dest_rect.w),
            static_cast<int>(draw_call.pivot_y * dest_rect.h),
        };
        const auto cam_dimensions = glm::ivec2(Engine::config.window_width, Engine::config.window_height);
        dest_rect.x = static_cast<int>(final_rendering_position.x * Engine::config.pixels_per_meter + cam_dimensions.x * 0.5f / zoom_factor - pivot_point.x);
        dest_rect.y = static_cast<int>(final_rendering_position.y * Engine::config.pixels_per_meter + cam_dimensions.y * 0.5f / zoom_factor - pivot_point.y);
        if (dest_rect.x > cam_dimensions.x / zoom_factor + 50 || dest_rect.y > cam_dimensions.y / zoom_factor + 50 || dest_rect.x + dest_rect.w < -50 || dest_rect.y + dest_rect.h < -50) {
            continue;
        }
        auto src_rect = SDL_Rect();
        src_rect.x = draw_call.tile_x;
        src_rect.y = draw_call.tile_y;
        src_rect.w = draw_call.tile_w;
        src_rect.h = draw_call.tile_h;
        SDL_SetTextureColorMod(texture, draw_call.r, draw_call.g, draw_call.b);
        SDL_SetTextureAlphaMod(texture, draw_call.a);
        SDL_RenderCopyEx(Engine::renderer, texture, &src_rect, &dest_rect, draw_call.rotation_degrees, &pivot_point, static_cast<SDL_RendererFlip>(flip_mode));
        SDL_SetTextureColorMod(texture, 255, 255, 255);
        SDL_SetTextureAlphaMod(texture, 255);
    }
    tile_draw_calls.clear();
}

auto TextureDB::RenderUI() -> void {
    std::stable_sort(ui_draw_calls.begin(), ui_draw_calls.end());
    SDL_RenderSetScale(Engine::renderer, 1, 1);
    for (const auto &draw_call : ui_draw_calls) {
        const auto texture = LoadTexture(draw_call.image_name);
        auto dest_rect = SDL_Rect();
        SDL_QueryTexture(texture, nullptr, nullptr, &dest_rect.w, &dest_rect.h);
        const auto x_scale = std::abs(draw_call.scale_x);
        const auto y_scale = std::abs(draw_call.scale_y);
        dest_rect.w = static_cast<int>(dest_rect.w * x_scale);
        dest_rect.h = static_cast<int>(dest_rect.h * y_scale);
        const auto pivot_point = SDL_Point{
            static_cast<int>(draw_call.pivot_x * dest_rect.w),
            static_cast<int>(draw_call.pivot_y * dest_rect.h),
        };
        dest_rect.x = draw_call.x;
        dest_rect.y = draw_call.y;
        SDL_SetTextureColorMod(texture, draw_call.r, draw_call.g, draw_call.b);
        SDL_SetTextureAlphaMod(texture, draw_call.a);
        SDL_RenderCopyEx(Engine::renderer, texture, nullptr, &dest_rect, draw_call.rotation_degrees, &pivot_point, SDL_FLIP_NONE);
        SDL_SetTextureColorMod(texture, 255, 255, 255);
        SDL_SetTextureAlphaMod(texture, 255);
    }
    ui_draw_calls.clear();
}

auto TextureDB::RenderPixels() -> void {
    SDL_SetRenderDrawBlendMode(Engine::renderer, SDL_BLENDMODE_BLEND);
    for (const auto &draw_call : pixel_draw_calls) {
        SDL_SetRenderDrawColor(Engine::renderer, draw_call.r, draw_call.g, draw_call.b, draw_call.a);
        SDL_RenderDrawPoint(Engine::renderer, draw_call.x, draw_call.y);
    }
    SDL_SetRenderDrawBlendMode(Engine::renderer, SDL_BLENDMODE_NONE);
    pixel_draw_calls.clear();
}