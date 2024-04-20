#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "SDL_image.h"
#include "SDL_ttf.h"

class TextDB {
  public:
    static auto LoadFont(const std::string &, const int) -> TTF_Font *;

    static auto DrawText(const char *, float, float, const char *, float, float, float, float, float) -> void;

    static auto GenerateText(TTF_Font *, const std::string &, int, int, int, int) -> SDL_Texture *;

    static auto RenderText() -> void;

  private:
    static inline std::unordered_map<std::string, std::unordered_map<int, TTF_Font *>> loaded_fonts;
    // static inline std::unordered_map<TTF_Font *, std::unordered_map<SDL_Color, std::unordered_map<std::string, SDL_Texture *>>> generated_text;

    class TextDrawCall {
      public:
        std::string text;
        int x;
        int y;
        std::string font_name;
        int font_size;
        int r;
        int g;
        int b;
        int a;
    };

    static inline std::vector<TextDrawCall> draw_calls;
};
