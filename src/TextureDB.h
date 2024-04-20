#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "SDL_image.h"

class TextureDB {
  public:
    static auto LoadTexture(const std::string &) -> SDL_Texture *;

    static auto DrawUI(const char *, float, float) -> void;

    static auto DrawUIEx(const char *, float, float, float, float, float, float, float, float, float, float, float, float) -> void;

    static auto DrawImage(const char *, float, float) -> void;

    static auto DrawImageEx(const char *, float, float, float, float, float, float, float, float, float, float, float, float) -> void;

    static auto DrawTile(const char *, float, float, float, float, float, float) -> void;

    static auto DrawTileEx(const char *, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float) -> void;

    static auto DrawPixel(float, float, float, float, float, float) -> void;

    static auto RenderScene() -> void;

    static auto RenderTiles() -> void;

    static auto RenderUI() -> void;

    static auto RenderPixels() -> void;

  private:
    static inline std::unordered_map<std::string, SDL_Texture *> loaded_textures;

    class ImageDrawCall {
      public:
        std::string image_name;
        float x;
        float y;
        int rotation_degrees = 0;
        float scale_x = 1.f;
        float scale_y = 1.f;
        float pivot_x = 0.5f;
        float pivot_y = 0.5f;
        int r = 255;
        int g = 255;
        int b = 255;
        int a = 255;
        int sorting_order = 0;

        auto operator<(const ImageDrawCall &other) const -> bool {
            return sorting_order < other.sorting_order;
        }
    };

    class UIDrawCall {
      public:
        std::string image_name;
        int x;
        int y;
        int rotation_degrees = 0;
        float scale_x = 1.f;
        float scale_y = 1.f;
        float pivot_x = 0.5f;
        float pivot_y = 0.5f;
        int r = 255;
        int g = 255;
        int b = 255;
        int a = 255;
        int sorting_order = 0;

        auto operator<(const UIDrawCall &other) const -> bool {
            return sorting_order < other.sorting_order;
        }
    };

    class PixelDrawCall {
      public:
        int x;
        int y;
        int r;
        int g;
        int b;
        int a;
    };

    class TileDrawCall {
      public:
        std::string tileset_name;
        int tile_x;
        int tile_y;
        int tile_w;
        int tile_h;
        float x;
        float y;
        int w;
        int h;
        int rotation_degrees = 0;
        float scale_x = 1.f;
        float scale_y = 1.f;
        float pivot_x = 0.5f;
        float pivot_y = 0.5f;
        int r = 255;
        int g = 255;
        int b = 255;
        int a = 255;
        int sorting_order = 0;

        auto operator<(const TileDrawCall &other) const -> bool {
            return sorting_order < other.sorting_order;
        }
    };

    static inline std::vector<ImageDrawCall> image_draw_calls;
    static inline std::vector<UIDrawCall> ui_draw_calls;
    static inline std::vector<PixelDrawCall> pixel_draw_calls;
    static inline std::vector<TileDrawCall> tile_draw_calls;
};
